#include "order.h"

#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "misc.h"
#include "field.h"
#include "record.h"
#include "expression.h"
#include "fqlselect.h"
#include "process.h"
#include "util/util.h"

struct _entry {
	size_t idx;
	size_t offset;
	unsigned len;
};

int _order_select_api(order* self, process*);
int _order_select(order* self, process*);

order* order_construct(order* self)
{
	memset(self, 0, sizeof(*self));
	self->select__ = _order_select;
	self->out_file = stdout;

	self->top_count = -1;

	vec_construct_(&self->expressions, expression*);
	vec_construct_(&self->entries, struct _entry);
	flex_construct(&self->order_data);

	return self;
}

void order_destroy(order* self)
{
	expression** it = vec_begin(&self->expressions);
	for (; it != vec_end(&self->expressions); ++it) {
		delete_(expression, *it);
	}
	vec_destroy(&self->expressions);
	vec_destroy(&self->entries);
	flex_destroy(&self->order_data);
	if (self->mmap != NULL) {
		munmap(self->mmap, self->file_size);
		close(self->fd);
	}
	if (self->out_file != NULL && self->out_file != stdout
	    && self->out_file != stderr) {
		fclose(self->out_file);
	}
}

int order_init_io(order* self, const char* in_name, const char* out_name)
{
	self->in_filename = in_name;
	if (out_name != NULL) {
		self->out_file = fopen(out_name, "w");
		if (self->out_file == NULL) {
			perror(out_name);
			return FQL_FAIL;
		}
	}
	return FQL_GOOD;
}

int order_preresolve_expressions(order* self, fqlselect* select, const vec* sources)
{
	/* First, we want to try and resolve by ordinal:
	 * SELECT foo
	 * FROM T1
	 * ORDER BY 1
	 */

	/* We also want to try and match fields by SELECT alias:
	 * SELECT foo bar
	 * FROM T1
	 * ORDER BY bar
	 *
	 * These were never mapped. Preflight does this.
	 */

	vec* select_exprs = select->schema->expressions;
	schema_preflight(select->schema);
	multimap* select_map = select->schema->expr_map;

	expression** it = vec_begin(&self->expressions);
	for (; it != vec_end(&self->expressions); ++it) {
		expression** result = NULL;
		switch ((*it)->expr) {
		case EXPR_REFERENCE:
		case EXPR_FULL_RECORD:
		case EXPR_COLUMN_NAME: {
			vec* exprs = multimap_get(select_map, (*it)->alias.data);
			if (exprs == NULL) {
				break;
			}
			if (exprs->size > 1) {
				fprintf(stderr,
				        "ORDER BY expression `%s' ambiguous\n",
				        (const char*)(*it)->alias.data);
				return FQL_FAIL;
			}
			result = vec_begin(exprs);
			break;
		}
		case EXPR_CONST: {
			long ordinal = 0;
			try_(expression_get_int(&ordinal, *it, NULL));

			if (ordinal <= 0 || ordinal > (int)select_exprs->size) {
				fprintf(stderr, "Ordinal `%ld' out of range\n", ordinal);
				return FQL_FAIL;
			}
			result = vec_at(select_exprs, ordinal - 1);
			break;
		}
		default:;
			// fprintf(stderr, "unexpected expression type");
			// return FQL_FAIL;
		}

		if (result == NULL) {
			continue;
		}
		if ((*result)->expr == EXPR_CONST) {
			table* result_table = vec_at(sources, (*result)->src_idx);
			expression_link(*it, *result, result_table);
			continue;
		}
		(*it)->index = (*result)->index;
		(*it)->expr = EXPR_REFERENCE;
	}

	return FQL_GOOD;
}

void order_connect_api(query* query, vec* api_vec)
{
	order* self = query->orderby;
	self->select__ = &_order_select_api;
	self->api = api_vec;
}

int order_add_expression(order* self, expression* expr)
{
	if (self == NULL) {
		delete_(expression, expr);
		return FQL_GOOD;
	}
	expr->index = self->expressions.size;
	vec_push_back(&self->expressions, &expr);
	return FQL_GOOD;
}

void order_cat_description(order* self, process* proc)
{
	expression** it = vec_begin(&self->expressions);
	for (; it != vec_end(&self->expressions); ++it) {
		if (it != vec_begin(&self->expressions)) {
			string_strcat(proc->plan_msg, ",");
		}
		expression_cat_description(*it, proc->plan_msg);
	}
}

int order_add_record(order* self, node* rg)
{
	record* top = rg->data;
	struct _entry entry = {
	        flex_size(&self->order_data), /* idx */
	        top->offset,                  /* offset */
	        top->select_len,              /* len */
	};

	expression** exprs = vec_begin(&self->expressions);

	unsigned i = 0;
	for (; i < self->expressions.size; ++i) {
		switch (exprs[i]->field_type) {
		case FIELD_INT: {
			/* NOTE: storing numbers in binary */
			long num_i = 0;
			try_(expression_get_int(&num_i, exprs[i], rg));
			flex_push_back_(&self->order_data, &num_i, long);
			break;
		}
		case FIELD_FLOAT: {
			double num_f = 0;
			try_(expression_get_float(&num_f, exprs[i], rg));
			flex_push_back_(&self->order_data, &num_f, double);
			break;
		}
		case FIELD_STRING: {
			stringview sv;
			try_(expression_get_stringview(&sv, exprs[i], rg));
			flex_push_back(&self->order_data, (void*)sv.data, sv.len);
			break;
		}
		default:
			fputs("Undefined field type\n", stderr);
			return FQL_FAIL;
		}
	}

	vec_push_back(&self->entries, &entry);

	return FQL_GOOD;
}

int _compare(const void* a, const void* b, void* data)
{
	order* orderby = data;
	const struct _entry* p0 = a;
	const struct _entry* p1 = b;

	size_t idx0 = p0->idx;
	size_t idx1 = p1->idx;

	int ret = 0;
	expression** it = vec_begin(&orderby->expressions);
	for (; ret == 0 && it != vec_end(&orderby->expressions); ++it) {
		switch ((*it)->field_type) {
		case FIELD_INT: {
			long* num0 = flex_at(&orderby->order_data, idx0);
			long* num1 = flex_at(&orderby->order_data, idx1);
			ret = num_compare_(*num0, *num1);
			break;
		}
		case FIELD_FLOAT: {
			double* num0 = flex_at(&orderby->order_data, idx0);
			double* num1 = flex_at(&orderby->order_data, idx1);
			ret = num_compare_(*num0, *num1);
			break;
		}
		case FIELD_STRING: {
			stringview sv0 = flex_pair_at(&orderby->order_data, idx0);
			stringview sv1 = flex_pair_at(&orderby->order_data, idx1);
			ret = stringview_compare_nocase_rtrim(&sv0, &sv1);
		}
		default:;
		}

		if ((*it)->descending)
			ret *= -1;

		++idx0;
		++idx1;
	}
	return ret;
}

int _order_select_api(order* self, process* proc)
{
	if (self->entry_iter == vec_end(&self->entries)) {
		return 0;
	}
	size_t offset = self->entry_iter->offset;
	struct fql_field* it = vec_begin(self->api);
	for (; it != vec_end(self->api); ++it) {
		switch (it->type) {
		case FQL_INT:
			it->data.i = *(long*)(&self->mmap[offset]);
			offset += sizeof(long);
			break;
		case FQL_FLOAT:
			it->data.f = *(double*)(&self->mmap[offset]);
			offset += sizeof(double);
			break;
		case FQL_STRING: {
			unsigned len = *(unsigned*)(&self->mmap[offset]);
			offset += sizeof(unsigned);
			stringview sv = {&self->mmap[offset], len};
			offset += len;
			string_copy_from_stringview(it->_in, &sv);
			it->data.s = string_c_str(it->_in);
			break;
		}
		default:
			fputs("Unexpected type in order_select_api\n", stderr);
			return FQL_FAIL;
		}
	}

	++proc->rows_affected;
	++self->entry_iter;
	return 1;
}

int _order_select(order* self, process* proc)
{
	struct _entry* it = vec_begin(&self->entries);
	for (; it != vec_end(&self->entries) && proc->rows_affected < self->top_count;
	     ++it) {
		fprintf(self->out_file, "%.*s", it->len, &self->mmap[it->offset]);
		++proc->rows_affected;
	}

	return 0;
}

int order_sort(order* self)
{
	vec_sort_r(&self->entries, &_compare, self);
	self->sorted = true;

	self->fd = open(self->in_filename, O_RDONLY);
	if (self->fd == -1) {
		perror(self->in_filename);
		return FQL_FAIL;
	}

	struct stat sb;
	if (fstat(self->fd, &sb) == -1) {
		perror(self->in_filename);
		return FQL_FAIL;
	}

	if (sb.st_size == 0) {
		return FQL_GOOD;
	}

	self->file_size = sb.st_size;

	self->mmap = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, self->fd, 0);

	if (self->mmap == MAP_FAILED) {
		perror(self->in_filename);
		return FQL_FAIL;
	}
	madvise(self->mmap, sb.st_size, MADV_RANDOM);

	self->entry_iter = vec_begin(&self->entries);

	return FQL_GOOD;
}
