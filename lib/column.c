#include "column.h"
#include "fql.h"
#include "query.h"
#include "process.h"
#include "reader.h"
#include "record.h"
#include "util/util.h"

Column* column_new(enum expr_type expr, void* data, const char* table_name)
{
	Column* new_column = NULL;
	malloc_(new_column, sizeof(*new_column));

	return column_construct(new_column, expr, data, table_name);
}

Column* column_construct(Column* col, enum expr_type expr, void* data, const char* table_name)
{
	*col = (Column) {
		 expr                   /* expr */
		,NULL                   /* table */
		,NULL                   /* data_source */
		,{ 0 }                  /* name */
		,{ 0 }                  /* alias */
		,{ 0 }                  /* table_name */
		,{ 0 }                  /* buf */
		,FIELD_UNDEFINED        /* field_type */
		,NULL                   /* field */
		,0                      /* location */
		,0                      /* width */
		,0                      /* src_idx */
	};

	string_construct(&col->buf);
	string_construct_from_char_ptr(&col->table_name, table_name);

	switch (expr) {
	case EXPR_COLUMN_NAME:
		string_construct_from_char_ptr(&col->alias, data);
		string_construct_from_char_ptr(&col->name, data);
		//string_copy(&col->buf, &col->alias);
		//col->field.s = data;
		break;
	case EXPR_FUNCTION:
		col->field.fn = data;
	default:
		string_construct(&col->alias);
	}

	return col;
}

void column_free(void* generic_col)
{
	Column* col = generic_col;

	switch(col->expr) {
	case EXPR_FUNCTION:
		function_free(col->field.fn);
		break;
	default:
		;
	}

	string_destroy(&col->name);
	string_destroy(&col->alias);
	string_destroy(&col->table_name);
	string_destroy(&col->buf);
	free_(col);
}

void column_cat_description(Column* col, String* msg)
{
	switch (col->expr) {
	case EXPR_COLUMN_NAME:
		string_append(msg, &col->alias);
		break;
	case EXPR_FUNCTION:
	{
		Function* func = col->field.fn;
		string_strcat(msg, func->name);
		string_push_back(msg, '(');

		Column** it = vec_begin(func->args);
		for (; it != vec_end(func->args); ++it) {
			if (it != vec_begin(func->args)) {
			        string_push_back(msg, ',');
			}
			column_cat_description(*it, msg);
		}
		string_push_back(msg, ')');
		break;
	}
	case EXPR_CONST:
		switch (col->field_type) {
		case FIELD_STRING:
			string_push_back(msg, '\'');
			string_append(msg, col->field.s);
			string_push_back(msg, '\'');
			break;
		case FIELD_INT:
		{
			char buf[20];
			sprintf(buf, "%ld", col->field.i);
			string_strcat(msg, buf);
			break;
		}
		case FIELD_FLOAT:
		{
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
	case EXPR_SUBQUERY_CONST:
		string_strcat(msg, "SUBQUERY CONST");
		break;
	case EXPR_NONE:
		string_strcat(msg, "No expression");
		break;
	}
}

int column_try_assign_source(Column* col, Table* table, int idx)
{
	Column** src_col = hashmap_get(table->schema->col_map, col->name.data);
	if (src_col != NULL) {
		col->data_source = *src_col;
		col->src_idx = idx;
		if (col->data_source->location > table->reader->max_col_idx) {
			table->reader->max_col_idx = col->data_source->location;
		}
		return 1;
	}

	return 0;
}

/* TODO: field_to_xx functions should only be called once.
 *       After which we should be able to assume the correct
 *       type. The best way to handle this would be to set
 *       the type during parsing.
 */
int column_get_int(long* ret, Column* col, Vec* recs)
{
	switch (col->expr) {
	case EXPR_COLUMN_NAME:
	{
		Record** rec = vec_at(recs, col->src_idx);
		StringView* sv = vec_at((*rec)->fields, col->data_source->location);
		string_copy_from_stringview(&col->buf, sv);
		if (str2long(ret, col->buf.data)) {
			return FQL_FAIL;
		}
		return FQL_GOOD;
	}
	case EXPR_FUNCTION:
	{
		Function* func = col->field.fn;
		union field new_field;
		if (col->field_type == FIELD_STRING) {
			new_field.s = &col->buf;
			string_clear(new_field.s);
		}
		func->caller(func, &new_field, recs);
		if (field_to_int(ret, &new_field, &col->field_type)) {
			return FQL_FAIL;
		}
		break;
	}
	case EXPR_CONST:
		if (field_to_int(ret, &col->field, &col->field_type)) {
			return FQL_FAIL;
		}
		break;
	default:
		fprintf(stderr, "col_get_int: Unexpected expression\n");
		return FQL_FAIL;
	}

	return FQL_GOOD;
}

int column_get_float(double* ret, Column* col, Vec* recs)
{
	switch (col->expr) {
	case EXPR_COLUMN_NAME:
	{
		Record** rec = vec_at(recs, col->src_idx);
		StringView* sv = vec_at((*rec)->fields, col->data_source->location);
		string_copy_from_stringview(&col->buf, sv);
		if (str2double(ret, col->buf.data)) {
			return FQL_FAIL;
		}
		return FQL_GOOD;
	}
	case EXPR_FUNCTION:
	{
		Function* func = col->field.fn;
		union field new_field;
		if (col->field_type == FIELD_STRING) {
			new_field.s = &col->buf;
			string_clear(new_field.s);
		}
		func->caller(func, &new_field, recs);
		if (field_to_float(ret, &new_field, &col->field_type)) {
			return FQL_FAIL;
		}
		break;
	}
	case EXPR_CONST:
		if (field_to_float(ret, &col->field, &col->field_type)) {
			return FQL_FAIL;
		}
		break;
	default:
		return FQL_FAIL;
	}

	return FQL_GOOD;
}

int column_get_stringview(StringView* ret, Column* col, Vec* recs)
{
	switch (col->expr) {
	case EXPR_COLUMN_NAME:
	{
		Record** rec = vec_at(recs, col->src_idx);
		StringView* sv = vec_at((*rec)->fields, col->data_source->location);
		ret->data = sv->data;
		ret->len = sv->len;
		return FQL_GOOD;
	}
	case EXPR_FUNCTION:
	{
		Function* func = col->field.fn;
		union field new_field;
		if (col->field_type == FIELD_STRING) {
			new_field.s = &col->buf;
			string_clear(new_field.s);
		}
		func->caller(func, &new_field, recs);
		if (field_to_stringview(ret, &new_field, &col->field_type)) {
			return FQL_FAIL;
		}
		break;
	}
	case EXPR_CONST:
		if (field_to_stringview(ret, &col->field, &col->field_type)) {
			return FQL_FAIL;
		}
		break;
	default:
		return FQL_FAIL;
	}

	return FQL_GOOD;
}

