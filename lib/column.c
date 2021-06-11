#include "column.h"
#include "fql.h"
#include "misc.h"
#include "query.h"
#include "reader.h"
#include "record.h"
#include "process.h"
#include "aggregate.h"
#include "function.h"
#include "util/util.h"

column* column_construct(column* col,
                         enum expr_type expr,
                         void* data,
                         const char* table_name)
{
	*col = (column) {
	        expr,            /* expr */
	        NULL,            /* table */
	        NULL,            /* data_source */
	        NULL,            /* subquery */
	        {0},             /* name */
	        {0},             /* alias */
	        {0},             /* table_name */
	        {0},             /* buf */
	        FIELD_UNDEFINED, /* field_type */
	        {0},             /* field */
	        0,               /* location */
	        0,               /* width */
	        0,               /* src_idx */
	        false            /* descending */
	};

	string_construct(&col->buf);
	string_construct_from_char_ptr(&col->table_name, table_name);

	switch (expr) {
	case EXPR_COLUMN_NAME:
		string_construct_from_char_ptr(&col->alias, data);
		string_construct_from_char_ptr(&col->name, data);
		//string_copy(&col->buf, &col->alias);
		//col->field.s = data;
		return col; /* skip alias construct */
	case EXPR_FUNCTION:
		col->field.fn = data;
		break;
	case EXPR_AGGREGATE:
		col->field.agg = data;
		break;
	case EXPR_SUBQUERY:
		col->subquery = data;
	default:;
	}

	string_construct(&col->alias);
	return col;
}

void column_destroy(void* generic_col)
{
	column* col = generic_col;

	switch (col->expr) {
	case EXPR_FUNCTION:
		delete_(function, col->field.fn);
		break;
	case EXPR_AGGREGATE:
		/* TODO - this will double delete ?? */
		delete_(aggregate, col->field.agg);
		break;
	default:;
	}

	string_destroy(&col->name);
	string_destroy(&col->alias);
	string_destroy(&col->table_name);
	string_destroy(&col->buf);
}

void column_link(struct column* dest, struct column* src)
{
	dest->data_source = src;
	//dest->expr = src->expr;
	dest->field_type = src->field_type;

	if (src->expr == EXPR_AGGREGATE) {
		aggregate* agg = src->field.agg;
		agg->linked_column = dest;
	}

	table* src_table = src->table;
	if (src_table == NULL) {
		return;
	}
	if (src->location > src_table->reader->max_col_idx) {
		src_table->reader->max_col_idx = src->location;
	}
}

void column_cat_description(column* col, string* msg)
{
	switch (col->expr) {
	case EXPR_COLUMN_NAME:
		string_append(msg, &col->alias);
		break;
	case EXPR_FUNCTION: {
		function* func = col->field.fn;
		string_strcat(msg, function_get_name(func));
		string_push_back(msg, '(');

		column** it = vec_begin(func->args);
		for (; it != vec_end(func->args); ++it) {
			if (it != vec_begin(func->args)) {
				string_push_back(msg, ',');
			}
			column_cat_description(*it, msg);
		}
		string_push_back(msg, ')');
		break;
	}
	case EXPR_AGGREGATE:
		string_strcat(msg, aggregate_get_name(col->field.agg));
		break;
	case EXPR_CONST:
		switch (col->field_type) {
		case FIELD_STRING:
			string_push_back(msg, '\'');
			string_append(msg, col->field.s);
			string_push_back(msg, '\'');
			break;
		case FIELD_INT: {
			char buf[20];
			sprintf(buf, "%ld", col->field.i);
			string_strcat(msg, buf);
			break;
		}
		case FIELD_FLOAT: {
			char buf[30];
			sprintf(buf, "%lf", col->field.f);
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

int column_try_assign_source(column* col, table* table)
{
	vec* cols = multimap_get(table->schema->col_map, col->name.data);
	if (cols == NULL) {
		return 0;
	}

	column** first_match = vec_begin(cols);
	column_link(col, *first_match);
	return cols->size;
}

/* TODO: field_to_xx functions should only be called once.
 *       After which we should be able to assume the correct
 *       type. The best way to handle this would be to set
 *       the type during parsing.
 */
int column_get_int(long* ret, column* col, vec* recs)
{
	switch (col->expr) {
	case EXPR_AGGREGATE:
	case EXPR_COLUMN_NAME: {
		column* src_col = col->data_source;
		record** rec = vec_at(recs, src_col->src_idx);
		if ((*rec)->fields->size <= src_col->location) {
			string_clear(&col->buf);
			*ret = 0;
			return FQL_GOOD;
		}
		stringview* sv = vec_at((*rec)->fields, src_col->location);
		string_copy_from_stringview(&col->buf, sv);
		fail_if_(str2long(ret, col->buf.data));
		return FQL_GOOD;
	}
	case EXPR_FUNCTION: {
		function* func = col->field.fn;
		union field new_field;
		enum field_type new_field_type = col->field_type;
		if (col->field_type == FIELD_STRING) {
			new_field.s = &col->buf;
			string_clear(new_field.s);
		}
		try_(func->call__(func, &new_field, recs));
		try_(field_to_int(ret, &new_field, &new_field_type));
		break;
	}
	case EXPR_CONST:
		try_(field_to_int(ret, &col->field, &col->field_type));
		break;
	default:
		fprintf(stderr, "col_get_int: Unexpected expression\n");
		return FQL_FAIL;
	}

	return FQL_GOOD;
}

int column_get_float(double* ret, column* col, vec* recs)
{
	switch (col->expr) {
	case EXPR_AGGREGATE:
	case EXPR_COLUMN_NAME: {
		column* src_col = col->data_source;
		record** rec = vec_at(recs, src_col->src_idx);
		if ((*rec)->fields->size <= src_col->location) {
			string_clear(&col->buf);
			*ret = 0;
			return FQL_GOOD;
		}
		stringview* sv = vec_at((*rec)->fields, src_col->location);
		string_copy_from_stringview(&col->buf, sv);
		fail_if_(str2double(ret, col->buf.data));
		return FQL_GOOD;
	}
	case EXPR_FUNCTION: {
		function* func = col->field.fn;
		union field new_field;
		enum field_type new_field_type = col->field_type;
		if (col->field_type == FIELD_STRING) {
			new_field.s = &col->buf;
			string_clear(new_field.s);
		}
		try_(func->call__(func, &new_field, recs));
		try_(field_to_float(ret, &new_field, &new_field_type));
		break;
	}
	case EXPR_CONST:
		try_(field_to_float(ret, &col->field, &col->field_type));
		break;
	default:
		fprintf(stderr, "col_get_float: Unexpected expression\n");
		return FQL_FAIL;
	}

	return FQL_GOOD;
}

int column_get_stringview(stringview* ret, column* col, vec* recs)
{
	switch (col->expr) {
	case EXPR_AGGREGATE:
	case EXPR_COLUMN_NAME: {
		column* src_col = col->data_source;
		record** rec = vec_at(recs, src_col->src_idx);
		if ((*rec)->fields->size <= src_col->location) {
			string_clear(&col->buf);
			ret->data = col->buf.data;
			ret->len = 0;
			return FQL_GOOD;
		}
		stringview* sv = vec_at((*rec)->fields, src_col->location);
		ret->data = sv->data;
		ret->len = sv->len;
		return FQL_GOOD;
	}
	case EXPR_FUNCTION: {
		function* func = col->field.fn;
		union field new_field;
		enum field_type new_field_type = col->field_type;
		if (col->field_type == FIELD_STRING) {
			new_field.s = &col->buf;
			string_clear(new_field.s);
		}
		try_(func->call__(func, &new_field, recs));
		try_(field_to_stringview(ret, &new_field, &new_field_type));
		break;
	}
	case EXPR_CONST:
		try_(field_to_stringview(ret, &col->field, &col->field_type));
		break;
	default:
		fprintf(stderr, "col_get_stringview: Unexpected expression\n");
		return FQL_FAIL;
	}

	return FQL_GOOD;
}
