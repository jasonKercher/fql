#include "expression.h"
#include "fql.h"
#include "misc.h"
#include "query.h"
#include "reader.h"
#include "record.h"
#include "process.h"
#include "aggregate.h"
#include "function.h"
#include "stringy.h"
#include "util/util.h"

expression* expression_construct(expression* self,
                                 enum expr_type expr,
                                 void* data,
                                 const char* table_name)
{
	*self = (expression) {
	        expr,            /* expr */
	        NULL,            /* table */
	        NULL,            /* data_source */
	        NULL,            /* subquery */
	        NULL,            /* rownum_ref */
	        {0},             /* name */
	        {0},             /* alias */
	        {0},             /* table_name */
	        {0},             /* buf */
	        FIELD_UNDEFINED, /* field_type */
	        {0},             /* field */
	        0,               /* index */
	        0,               /* location */
	        0,               /* width */
	        0,               /* src_idx */
	        false            /* descending */
	};

	string_construct(&self->buf);
	string_construct_from_char_ptr(&self->table_name, table_name);

	switch (expr) {
	case EXPR_GROUPING:
	case EXPR_ROW_NUMBER:
	case EXPR_FULL_RECORD:
	case EXPR_COLUMN_NAME:
		string_construct_from_char_ptr(&self->alias, data);
		string_construct_from_char_ptr(&self->name, data);
		return self; /* skip alias construct */
	case EXPR_FUNCTION:
		self->field.fn = data;
		break;
	case EXPR_AGGREGATE:
		self->field.agg = data;
		break;
	case EXPR_SUBQUERY:
		self->subquery = data;
	default:;
	}

	string_construct(&self->alias);
	return self;
}

void expression_destroy(void* generic_expr)
{
	expression* self = generic_expr;

	switch (self->expr) {
	case EXPR_FUNCTION:
		delete_(function, self->field.fn);
		break;
	case EXPR_AGGREGATE:
		/* TODO - this will double delete ?? */
		delete_(aggregate, self->field.agg);
		break;
	default:;
	}

	string_destroy(&self->name);
	string_destroy(&self->alias);
	string_destroy(&self->table_name);
	string_destroy(&self->buf);
}

void expression_link(struct expression* dest, struct expression* src)
{
	dest->data_source = src;
	dest->field_type = src->field_type;

	if (src->expr == EXPR_AGGREGATE) {
		aggregate* agg = src->field.agg;
		agg->linked_expression = dest;
	}

	table* src_table = src->table;
	if (src_table == NULL) {
		return;
	}
	if (src->index > src_table->reader->max_idx) {
		src_table->reader->max_idx = src->index;
	}
}

void expression_cat_description(expression* self, string* msg)
{
	switch (self->expr) {
	case EXPR_GROUPING:
	case EXPR_ROW_NUMBER:
	case EXPR_FULL_RECORD:
	case EXPR_COLUMN_NAME:
		string_append(msg, &self->alias);
		break;
	case EXPR_FUNCTION: {
		function* func = self->field.fn;
		string_strcat(msg, function_get_name(func));
		string_push_back(msg, '(');

		expression** it = vec_begin(func->args);
		for (; it != vec_end(func->args); ++it) {
			if (it != vec_begin(func->args)) {
				string_push_back(msg, ',');
			}
			expression_cat_description(*it, msg);
		}
		string_push_back(msg, ')');
		break;
	}
	case EXPR_AGGREGATE:
		string_strcat(msg, aggregate_get_name(self->field.agg));
		break;
	case EXPR_CONST:
		switch (self->field_type) {
		case FIELD_STRING:
			string_push_back(msg, '\'');
			string_append(msg, self->field.s);
			string_push_back(msg, '\'');
			break;
		case FIELD_INT: {
			char buf[20];
			sprintf(buf, "%ld", self->field.i);
			string_strcat(msg, buf);
			break;
		}
		case FIELD_FLOAT: {
			char buf[30];
			sprintf(buf, "%lf", self->field.f);
			string_strcat(msg, buf);
			break;
		}
		default:
			string_strcat(msg, "<<const>>");
		}
		break;
	case EXPR_ASTERISK:
		string_strcat(msg, "*");
		break;
	case EXPR_SOURCE:
		string_strcat(msg, "TABLE SOURCE");
		break;
	case EXPR_SUBQUERY:
		string_strcat(msg, "SUBQUERY");
		break;
	default:
		string_strcat(msg, "no expression");
	}
}

int expression_try_assign_source(expression* self, table* table)
{
	if (self->expr == EXPR_FULL_RECORD || self->expr == EXPR_ROW_NUMBER) {
		self->src_idx = table->idx;
		return 1;
	}
	vec* exprs = multimap_get(table->schema->expr_map, self->name.data);
	if (exprs == NULL) {
		return 0;
	}

	expression** first_match = vec_begin(exprs);
	expression_link(self, *first_match);
	return exprs->size;
}

/* TODO: field_to_xx functions should only be called once.
 *       After which we should be able to assume the correct
 *       type. The best way to handle this would be to set
 *       the type during parsing.
 */
int expression_get_int(long* ret, expression* self, recgroup* rg)
{
	switch (self->expr) {
	case EXPR_ROW_NUMBER:
		*ret = *self->rownum_ref;
		return FQL_GOOD;
	case EXPR_FULL_RECORD: {
		record* rec = recgroup_rec_at(rg, self->src_idx);
		stringview* sv = &rec->rec_ref;
		string_copy_from_stringview(&self->buf, sv);
		fail_if_(str2long(ret, self->buf.data));
		return FQL_GOOD;
	}
	case EXPR_GROUPING:
	case EXPR_AGGREGATE:
	case EXPR_COLUMN_NAME: {
		expression* src_expr = self->data_source;
		record* rec = recgroup_rec_at(rg, src_expr->src_idx);
		if (rec->fields.size <= src_expr->index) {
			string_clear(&self->buf);
			*ret = 0;
			return FQL_GOOD;
		}
		stringview* sv = vec_at(&rec->fields, src_expr->index);
		string_copy_from_stringview(&self->buf, sv);
		fail_if_(str2long(ret, self->buf.data));
		return FQL_GOOD;
	}
	case EXPR_FUNCTION: {
		function* func = self->field.fn;
		union field new_field;
		enum field_type new_field_type = self->field_type;
		if (self->field_type == FIELD_STRING) {
			new_field.s = &self->buf;
			string_clear(new_field.s);
		}
		try_(func->call__(func, &new_field, rg));
		try_(field_to_int(ret, &new_field, &new_field_type));
		break;
	}
	case EXPR_CONST:
		try_(field_to_int(ret, &self->field, &self->field_type));
		break;
	default:
		fprintf(stderr, "expr_get_int: Unexpected expression\n");
		return FQL_FAIL;
	}

	return FQL_GOOD;
}

int expression_get_float(double* ret, expression* self, recgroup* rg)
{
	switch (self->expr) {
	case EXPR_ROW_NUMBER:
		*ret = (double)*self->rownum_ref;
		return FQL_GOOD;
	case EXPR_FULL_RECORD: {
		record* rec = recgroup_rec_at(rg, self->src_idx);
		stringview* sv = &rec->rec_ref;
		string_copy_from_stringview(&self->buf, sv);
		fail_if_(str2double(ret, self->buf.data));
		return FQL_GOOD;
	}
	case EXPR_GROUPING:
	case EXPR_AGGREGATE:
	case EXPR_COLUMN_NAME: {
		expression* src_expr = self->data_source;
		record* rec = recgroup_rec_at(rg, src_expr->src_idx);
		if (rec->fields.size <= src_expr->index) {
			string_clear(&self->buf);
			*ret = 0;
			return FQL_GOOD;
		}
		stringview* sv = vec_at(&rec->fields, src_expr->index);
		string_copy_from_stringview(&self->buf, sv);
		fail_if_(str2double(ret, self->buf.data));
		return FQL_GOOD;
	}
	case EXPR_FUNCTION: {
		function* func = self->field.fn;
		union field new_field;
		enum field_type new_field_type = self->field_type;
		if (self->field_type == FIELD_STRING) {
			new_field.s = &self->buf;
			string_clear(new_field.s);
		}
		try_(func->call__(func, &new_field, rg));
		try_(field_to_float(ret, &new_field, &new_field_type));
		break;
	}
	case EXPR_CONST:
		try_(field_to_float(ret, &self->field, &self->field_type));
		break;
	default:
		fprintf(stderr, "expr_get_float: Unexpected expression\n");
		return FQL_FAIL;
	}

	return FQL_GOOD;
}

int expression_get_stringview(stringview* ret, expression* self, recgroup* rg)
{
	switch (self->expr) {
	case EXPR_FULL_RECORD: {
		record* rec = recgroup_rec_at(rg, self->src_idx);
		*ret = rec->rec_ref;
		return FQL_GOOD;
	}
	case EXPR_ROW_NUMBER:
		string_sprintf(&self->buf, "%u", *self->rownum_ref);
		ret->data = self->buf.data;
		ret->len = self->buf.size;
		return FQL_GOOD;
	case EXPR_GROUPING:
	case EXPR_AGGREGATE:
	case EXPR_COLUMN_NAME: {
		expression* src_expr = self->data_source;
		record* rec = recgroup_rec_at(rg, src_expr->src_idx);
		if (rec->fields.size <= src_expr->index) {
			string_clear(&self->buf);
			ret->data = self->buf.data;
			ret->len = 0;
			return FQL_GOOD;
		}
		stringview* sv = vec_at(&rec->fields, src_expr->index);
		ret->data = sv->data;
		ret->len = sv->len;
		return FQL_GOOD;
	}
	case EXPR_FUNCTION: {
		function* func = self->field.fn;
		union field new_field;
		enum field_type new_field_type = self->field_type;
		if (self->field_type == FIELD_STRING) {
			new_field.s = &self->buf;
			string_clear(new_field.s);
		}
		try_(func->call__(func, &new_field, rg));
		try_(field_to_stringview(ret, &self->buf, &new_field, &new_field_type));
		break;
	}
	case EXPR_CONST:
		try_(field_to_stringview(ret,
		                         &self->buf,
		                         &self->field,
		                         &self->field_type));
		break;
	default:
		fprintf(stderr, "expr_get_stringview: Unexpected expression\n");
		return FQL_FAIL;
	}

	return FQL_GOOD;
}
