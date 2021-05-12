#include "order.h"
#include <stdio.h>
#include "field.h"
#include "process.h"
#include "util/util.h"

struct indexpair {
	size_t idx;
	unsigned len;
};

order* order_construct(order* self)
{
	vec_construct_(&self->columns, column*);
	vec_construct_(&self->_index_pairs, struct indexpair);
	vec_construct_(&self->_raw, char);

	return self;
}

void order_destroy(order* self)
{
	vec_destroy(&self->columns);
	//vec_destroy(&self->_indicies);
	vec_destroy(&self->_raw);
	delete_ (vec, self->_views);
}

int order_add_column(order* self, column* col)
{
	vec_push_back(&self->columns, col);
	return FQL_GOOD;
}

void order_cat_description(order* self, process* proc)
{
	column** it = vec_begin(&self->columns);
	for (; it != vec_end(&self->columns); ++it) {
		if (it != vec_begin(&self->columns)) {
			string_strcat(proc->action_msg, ",");
		}
		column_cat_description(*it, proc->action_msg);
	}
}

int order_add_record(order* self, vec* recs)
{
	struct indexpair pair = {
		 self->_raw.size  /* idx */
		,0                /* len */
	};

	column** cols = vec_begin(&self->columns);

	int i = 0;
	for (; i < self->columns.size; ++i) {
		switch (cols[i]->field_type) {
		case FIELD_INT:
		 {
			/* NOTE: storing numbers in binary */
			long num_i = 0;
			try_ (column_get_int(&num_i, cols[i], recs));
			pair.len = sizeof(long);
			vec_append(&self->_raw, &num_i, pair.len);
			break;
		 }
		case FIELD_FLOAT:
		 {
			double num_f = 0;
			try_ (column_get_float(&num_f, cols[i], recs));
			pair.len = sizeof(double);
			vec_append(&self->_raw, &num_f, pair.len);
			break;
		 }
		case FIELD_STRING:
		 {
			stringview sv;
			try_ (column_get_stringview(&sv, cols[i], recs));
			vec_append(&self->_raw, sv.data, sv.len);
			pair.len = sv.len;
			break;
		 }
		default:
			fputs("Undefined field type\n", stderr);
			return FQL_FAIL;
		}

		vec_push_back(&self->_index_pairs, &pair);
		pair.idx += pair.len;
	}

	return FQL_GOOD;
}

int _compare(const void* a, const void* b, void* data)
{
	order* self = data;
	const struct indexpair* p0 = a;
	const struct indexpair* p1 = b;

	int ret = 0;
	column** it = vec_begin(&self->columns);
	for (; ret == 0 && it != vec_end(&self->columns); ++it) {
		switch ((*it)->field_type) {
		case FIELD_INT:
		 {
			long* num0 = vec_at(&self->_raw, p0->idx);
			long* num1 = vec_at(&self->_raw, p1->idx);
			ret = num_compare_(*num0, *num1);
			break;
		 }
		case FIELD_FLOAT:
		 {
			double* num0 = vec_at(&self->_raw, p0->idx);
			double* num1 = vec_at(&self->_raw, p1->idx);
			ret = num_compare_(*num0, *num1);
			break;
		 }
		case FIELD_STRING:
		 {
			stringview sv0 = {
				 vec_at(&self->_raw, p0->idx)
				,p0->len
			};
			stringview sv1 = {
				 vec_at(&self->_raw, p1->idx)
				,p1->len
			};
			ret = stringview_compare_nocase_rtrim(&sv0, &sv1);
		 }
		default:
			;
		}
	}
	return ret;
}

int order_sort(order* self)
{
	vec_sort_r(&self->_index_pairs, &_compare, self);
	return FQL_GOOD;
}
