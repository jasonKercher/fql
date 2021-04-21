#include "select.h"
#include "fql.h"
#include "reader.h"
#include "column.h"
#include "schema.h"
#include "util/util.h"

Select* select_new()
{
	Select* new_select = NULL;
	malloc_(new_select, sizeof(*new_select));

	return select_construct(new_select);
}

Select* select_construct(Select* select)
{
	*select = (Select) {
		 OP_SELECT              /* oper_type */
		,NULL                   /* api */
		,schema_new()           /* schema */
		,writer_new()           /* writer */
		,&select_record         /* select_record__ */
	};

	return select;
}

void select_free(Select* select)
{
	if (select == NULL) {
		return;
	}
	writer_free(select->writer);
	schema_free(select->schema);
	free_(select);
}

_Bool select_has_delim(Select* select)
{
	return select->schema->delimiter[0];
}

void select_set_delim(Select* select, const char* delim)
{
	strncpy_(select->schema->delimiter, delim, DELIM_LEN_MAX);
}

void select_add_column(Select* select, Column* col)
{
	schema_add_column(select->schema, col);
}

void _resize_raw_rec(Vec* raw_rec, unsigned size)
{
	unsigned org_size = raw_rec->size;
	vec_resize(raw_rec, size);
	String* s = vec_at(raw_rec, org_size);
	for (; s != vec_end(raw_rec); ++s) {
		string_construct(s);
	}
}

int _expand_asterisk(Vec* col_vec, Table* table, unsigned src_idx, unsigned* col_idx)
{
	Vec* src_col_vec = table->schema->columns;

	table->reader->max_col_idx = src_col_vec->size - 1;

	Column** it = vec_begin(src_col_vec);
	for (; it != vec_end(src_col_vec); ++it) {
		//String* col_name = string_from_string(&(*it)->alias);
		Column* new_col = column_new(EXPR_COLUMN_NAME, (*it)->alias.data, "");
		new_col->data_source = *it;
		new_col->src_idx = src_idx;
		new_col->field_type = (*it)->field_type;
		vec_insert(col_vec, *col_idx, &new_col);
		++(*col_idx);
	}

	return table->schema->columns->size;
}

void _expand_asterisks(Query* query, _Bool force_expansion)
{
	Select* select = query->op;
	Vec* col_vec = select->schema->columns;
	unsigned i = 0;

	for (; i < col_vec->size; ++i) {
		Column** col = vec_at(col_vec, i);
		if ((*col)->expr != EXPR_ASTERISK) {
			continue;
		}

		Table* table = vec_at(query->sources, (*col)->src_idx);

		if (table->subquery == NULL  /* Is a subquery source */
		    && !force_expansion
		    && query->query_id == 0  /* Is in main query */
		    && string_eq(table->schema->delimiter, select->schema->delimiter)) {
			continue;
		}

		unsigned asterisk_index = i++;
		_expand_asterisk(col_vec, table, (*col)->src_idx, &i);

		Column** asterisk_col = vec_at(col_vec, asterisk_index);
		column_free(*asterisk_col);
		vec_remove(col_vec, asterisk_index);
	}

	_resize_raw_rec(select->writer->raw_rec, col_vec->size);
}

void select_connect_api(Query* query, Vec* api)
{
	Select* select = query->op;
	select->select__ = &select_record_api;
	_expand_asterisks(query, true);
	vec_resize(api, select->schema->columns->size);
	select->api = api;
}

void select_apply_process(Query* query, Plan* plan)
{
	Select* select = query->op;
	Process* proc = plan->op_true->data;
	proc->action__ = &fql_select;
	proc->proc_data = select;
	string_strcpy(proc->action_msg, "SELECT ");

	writer_set_delimiter(select->writer, select->schema->delimiter);

	Vec* col_vec = select->schema->columns;
	Column** col = vec_begin(col_vec);
	for (; col != vec_end(col_vec); ++col) {
		if (col != vec_begin(col_vec)) {
			string_strcat(proc->action_msg, ",");
		}
		column_cat_description(*col, proc->action_msg);
	}

	proc = plan->op_false->data;
	proc->is_passive = true;
}

void select_apply_column_alias(Select* select, const char* alias)
{
	schema_apply_column_alias(select->schema, alias);
}

void select_finalize(Select* select, Query* query)
{
	_expand_asterisks(query, false);
	schema_finalize(select->schema);
}

void select_preop(Select* select, Query* query)
{
	Vec header;
	vec_construct_(&header, Column*);

	/* Print header */
	Column** it = vec_begin(select->schema->columns);
	for (; it != vec_end(select->schema->columns); ++it) {
		if ((*it)->expr == EXPR_ASTERISK) {
			Table* aster_src = vec_at(query->sources, (*it)->src_idx);
			Vec* aster_cols = aster_src->schema->columns;
			Column** it2 = vec_begin(aster_cols);
			for (; it2 != vec_end(aster_cols); ++it2) {
				Column* field_col = column_new(EXPR_CONST, NULL, "");
				String* field_str = string_from_string(&(*it2)->alias);
				field_col->field.s = field_str;
				field_col->field_type = FIELD_STRING;
				vec_push_back(&header, &field_col);
			}
			continue;
		}
		Column* field_col = column_new(EXPR_CONST, NULL, "");
		String* field_str = string_from_string(&(*it)->alias);
		field_col->field.s = field_str;
		field_col->field_type = FIELD_STRING;
		vec_push_back(&header, &field_col);
	}

	Writer* writer = select->writer;
	writer->write_record__(writer->writer_data, &header, NULL);

	it = vec_begin(&header);
	for (; it != vec_end(&header); ++it) {
		column_free(*it);
	}
	vec_destroy(&header);
}

int select_record_api(Select* select, struct vec* recs)
{
	Writer* writer = select->writer;

	Vec* col_vec = select->schema->columns;

	Column** cols = vec_begin(col_vec);
	int i = 0;
	for (; i < col_vec->size; ++i) {
		struct fql_field* field = vec_at(select->api, i);
		switch (cols[i]->field_type) {
		case FIELD_STRING:
		{
			StringView sv;
			if (column_get_stringview(&sv, cols[i], recs)) {
				return FQL_FAIL;
			}
			/* Even though the we are not using the
			 * writer directly, we use the data it
			 * owns for our string fields.
			 */
			String* s = vec_at(writer->raw_rec, i);
			string_copy_from_stringview(s, &sv);
			field->type = FQL_STRING;
			field->data.s = s->data;
			break;
		}
		case FIELD_INT:
			field->type = FQL_INT;
			if (column_get_int(&field->data.i, cols[i], recs)) {
				return FQL_FAIL;
			}
			break;
		case FIELD_FLOAT:
			field->type = FQL_FLOAT;
			if (column_get_float(&field->data.f, cols[i], recs)) {
				return FQL_FAIL;
			}
		default:
			;
		}
	}

	return 1;
}

int select_record(Select* select, Vec* recs)
{
	Writer* writer = select->writer;
	Vec* col_vec = select->schema->columns;

	return writer->write_record__(writer->writer_data, col_vec, recs);
}

/* This becomes a big copy operation because
 * I want to recycle the subquery's record.
 */
int select_subquery_record(Reader* reader, Record* rec)
{
	Schema* sub_schema = reader->reader_data;
	Vec* sub_col_vec = sub_schema->columns;
	Vec* sub_recs = reader->subquery_recs;

	int i = 0;
	Column** sub_cols = vec_begin(sub_col_vec);

	for (; i < sub_col_vec->size; ++i) {
		StringView sv;
		column_get_stringview(&sv, sub_cols[i], sub_recs);
		String* s = vec_at(rec->_field_data, i);
		string_copy_from_stringview(s, &sv);
		StringView* rec_sv = vec_at(rec->fields, i);
		rec_sv->data = s->data;
		rec_sv->len = s->size;
	}

	return FQL_GOOD;
}

/* TODO */
int select_subquery_reset(Reader* reader)
{
	return FQL_FAIL;
}
