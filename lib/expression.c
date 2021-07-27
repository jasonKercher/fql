#include "expression.h"
#include "fql.h"
#include "misc.h"
#include "query.h"
#include "reader.h"
#include "record.h"
#include "process.h"
#include "function.h"
#include "aggregate.h"
#include "switchcase.h"
#include "util/util.h"
#include "util/stringy.h"

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
	case EXPR_SWITCH_CASE:
		self->field.sc = data;
		break;
	case EXPR_FUNCTION:
		self->field.fn = data;
		break;
	case EXPR_AGGREGATE:
		self->field.agg = data;
		break;
	case EXPR_SUBQUERY:
		self->subquery = data;
		break;
	case EXPR_NULL:
		self->field_type = FIELD_STRING;
		string_construct(&self->alias);
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
		delete_(aggregate, self->field.agg);
		break;
	case EXPR_SWITCH_CASE:
		delete_(switchcase, self->field.sc);
		break;
	default:;
	}

	string_destroy(&self->name);
	string_destroy(&self->alias);
	string_destroy(&self->table_name);
	string_destroy(&self->buf);
}

/* This is used for static case expressions */
expression* expression_copy(const struct expression* src)
{
	expression* dest = malloc_(sizeof(*dest));
	memset(dest, 0, sizeof(*dest));

	string_construct_from_string(&dest->name, &src->name);
	string_construct_from_string(&dest->alias, &src->alias);
	string_construct_from_string(&dest->table_name, &src->table_name);
	string_construct_from_string(&dest->buf, &src->buf);

	dest->expr = src->expr;
	dest->field = src->field;
	dest->field_type = src->field_type;

	return dest;
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
	case EXPR_NULL:
		string_strcat(msg, "NULL");
		break;
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

int expression_type_check(expression* self, node* rg)
{
	switch (self->field_type) {
	case FIELD_INT: {
		long num = 0;
		try_(expression_get_int(&num, self, rg));
		break;
	}
	case FIELD_FLOAT: {
		double num = 0;
		try_(expression_get_float(&num, self, rg));
		break;
	}
	case FIELD_STRING: /* Not sure how this could fail */
		break;
	default:
		return FQL_FAIL;
	}
	return FQL_GOOD;
}

int expression_cast(expression* self, enum field_type new_type)
{
	if (self->field_type == new_type) {
		return FQL_GOOD;
	}

	if (self->expr != EXPR_CONST) {
		/* lol - this is probably wrong */
		self->field_type = new_type;
		return FQL_GOOD;
	}

	switch (new_type) {
	case FIELD_INT: {
		long num = 0;
		try_(expression_get_int(&num, self, NULL));
		self->field_type = new_type;
		self->field.i = num;
		break;
	}
	case FIELD_FLOAT: {
		double num = 0;
		try_(expression_get_float(&num, self, NULL));
		self->field_type = new_type;
		self->field.f = num;
		break;
	}
	case FIELD_STRING: {
		stringview sv;
		try_(expression_get_stringview(&sv, self, NULL));

		/* This is redundant because of the current behavior
		 * of expression_get_stringview, but in case that ever
		 * changes... We do the redundant thing...
		 */
		string_strncpy(&self->buf, sv.data, sv.len);
		self->field.s = &self->buf;

		return FQL_FAIL;
	}
	default:
		return FQL_FAIL;
	}

	return FQL_GOOD;
}

void expression_update_indicies(vec* expr_vec)
{
	unsigned i = 0;
	expression** exprs = vec_begin(expr_vec);
	for (; i < expr_vec->size; ++i) {
		exprs[i]->index = i;
	}
}


/* TODO: field_to_xx functions should only be called once.
 *       After which we should be able to assume the correct
 *       type. The best way to handle this would be to set
 *       the type during parsing.
 */
int expression_get_int(long* ret, expression* self, node* rg)
{
	switch (self->expr) {
	case EXPR_NULL:
		*ret = 0;
		return FQL_NULL;
	case EXPR_SWITCH_CASE:
		try_(switchcase_eval_to_int(self->field.sc, ret, rg));
		return FQL_GOOD;
	case EXPR_ROW_NUMBER:
		*ret = *self->rownum_ref;
		return FQL_GOOD;
	case EXPR_FULL_RECORD: {
		record* rec = node_data_at(rg, self->src_idx);
		if (rec == NULL) {
			*ret = 0;
			return FQL_NULL;
		}
		stringview* sv = &rec->rec_ref;
		string_copy_from_stringview(&self->buf, sv);
		fail_if_(str2long(ret, self->buf.data));
		return FQL_GOOD;
	}
	case EXPR_GROUPING:
	case EXPR_AGGREGATE:
	case EXPR_COLUMN_NAME: {
		expression* src_expr = self->data_source;
		record* rec = node_data_at(rg, src_expr->src_idx);
		if (rec == NULL) {
			*ret = 0;
			return FQL_NULL;
		}
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
		try_deref_(func->call__(func, &new_field, rg));
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

int expression_get_float(double* ret, expression* self, node* rg)
{
	switch (self->expr) {
	case EXPR_NULL:
		*ret = -0;
		return FQL_NULL;
	case EXPR_SWITCH_CASE:
		try_(switchcase_eval_to_float(self->field.sc, ret, rg));
		return FQL_GOOD;
	case EXPR_ROW_NUMBER:
		*ret = (double)*self->rownum_ref;
		return FQL_GOOD;
	case EXPR_FULL_RECORD: {
		record* rec = node_data_at(rg, self->src_idx);
		if (rec == NULL) {
			*ret = -0;
			return FQL_NULL;
		}
		stringview* sv = &rec->rec_ref;
		string_copy_from_stringview(&self->buf, sv);
		fail_if_(str2double(ret, self->buf.data));
		return FQL_GOOD;
	}
	case EXPR_GROUPING:
	case EXPR_AGGREGATE:
	case EXPR_COLUMN_NAME: {
		expression* src_expr = self->data_source;
		record* rec = node_data_at(rg, src_expr->src_idx);
		if (rec == NULL) {
			return -0;
			return FQL_NULL;
		}
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
		try_deref_(func->call__(func, &new_field, rg));
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

int expression_get_stringview(stringview* ret, expression* self, node* rg)
{
	switch (self->expr) {
	case EXPR_NULL:
		ret->len = 0;
		return FQL_NULL;
	case EXPR_SWITCH_CASE:
		try_(switchcase_eval_to_stringview(self->field.sc, ret, rg));
		return FQL_GOOD;
	case EXPR_FULL_RECORD: {
		record* rec = node_data_at(rg, self->src_idx);
		if (rec == NULL) {
			ret->len = 0;
			return FQL_NULL;
		}
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
		record* rec = node_data_at(rg, src_expr->src_idx);
		if (rec == NULL) {
			ret->len = 0;
			return FQL_NULL;
		}
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
		try_deref_(func->call__(func, &new_field, rg));
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
