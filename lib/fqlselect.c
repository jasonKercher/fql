#include "fqlselect.h"
#include "fql.h"
#include "reader.h"
#include "column.h"
#include "schema.h"
#include "process.h"
#include "util/util.h"

fqlselect* fqlselect_construct(fqlselect* self)
{
	*self = (fqlselect) {
		 OP_SELECT              /* oper_type */
		,NULL                   /* api */
		,new_(schema)           /* schema */
		,new_(writer)           /* writer */
		,&fqlselect_record         /* fqlselect_record__ */
	};

	return self;
}

void fqlselect_destroy(fqlselect* self)
{
	if (self == NULL) {
		return;
	}
	delete_(writer, self->writer);
	delete_(schema, self->schema);
}

_Bool fqlselect_has_delim(fqlselect* self)
{
	return self->schema->delimiter[0];
}

void fqlselect_set_delim(fqlselect* self, const char* delim)
{
	strncpy_(self->schema->delimiter, delim, DELIM_LEN_MAX);
}

void fqlselect_add_column(fqlselect* self, column* col)
{
	schema_add_column(self->schema, col);
}

void _resize_raw_rec(vec* raw_rec, unsigned size)
{
	unsigned org_size = raw_rec->size;
	vec_resize(raw_rec, size);
	string* s = vec_at(raw_rec, org_size);
	for (; s != vec_end(raw_rec); ++s) {
		string_construct(s);
	}
}

/* this should be in schema.c */
int _expand_asterisk(vec* col_vec, table* table, unsigned src_idx, unsigned* col_idx)
{
	vec* src_col_vec = table->schema->columns;

	table->reader->max_col_idx = src_col_vec->size - 1;

	column** it = vec_begin(src_col_vec);
	for (; it != vec_end(src_col_vec); ++it) {
		//string* col_name = string_from_string(&(*it)->alias);
		column* new_col = new_(column, EXPR_COLUMN_NAME, (*it)->alias.data, "");
		new_col->data_source = *it;
		new_col->src_idx = src_idx;
		new_col->field_type = (*it)->field_type;
		++(*col_idx);
		vec_insert(col_vec, *col_idx, &new_col);
	}

	return table->schema->columns->size;
}

void _expand_asterisks(query* query, _Bool force_expansion)
{
	fqlselect* self = query->op;
	vec* col_vec = self->schema->columns;
	unsigned i = 0;

	for (; i < col_vec->size; ++i) {
		column** col = vec_at(col_vec, i);
		if ((*col)->expr != EXPR_ASTERISK) {
			continue;
		}

		table* table = vec_at(query->sources, (*col)->src_idx);

		if (table->subquery == NULL  /* is not a subquery source */
		    && !force_expansion
		    && query->query_id == 0  /* is in main query */
		    && string_eq(table->schema->delimiter, self->schema->delimiter)) {
			continue;
		}

		unsigned asterisk_index = i;
		_expand_asterisk(col_vec, table, (*col)->src_idx, &i);

		column** asterisk_col = vec_at(col_vec, asterisk_index);
		delete_(column, *asterisk_col);
		vec_remove(col_vec, asterisk_index);
		--i;
	}

	_resize_raw_rec(self->writer->raw_rec, col_vec->size);
}

void fqlselect_connect_api(query* query, vec* api)
{
	fqlselect* self = query->op;
	self->select__ = &fqlselect_record_api;
	_expand_asterisks(query, true);
	vec_resize(api, self->schema->columns->size);
	self->api = api;
}

void fqlselect_apply_process(query* query, plan* plan)
{
	fqlselect* self = query->op;
	process* proc = plan->op_true->data;
	proc->action__ = &fql_select;
	proc->proc_data = self;
	string_strcpy(proc->action_msg, "SELECT ");

	writer_set_delimiter(self->writer, self->schema->delimiter);

	vec* col_vec = self->schema->columns;
	column** col = vec_begin(col_vec);
	for (; col != vec_end(col_vec); ++col) {
		if (col != vec_begin(col_vec)) {
			string_strcat(proc->action_msg, ",");
		}
		column_cat_description(*col, proc->action_msg);
	}

	proc = plan->op_false->data;
	proc->is_passive = true;
}

void fqlselect_apply_column_alias(fqlselect* self, const char* alias)
{
	schema_apply_column_alias(self->schema, alias);
}

int fqlselect_writer_open(fqlselect* self, const char* file_name)
{
	/* TODO: csv_writer_open is assumed */
	int ret = csv_writer_open(self->writer->writer_data, file_name);
	if (ret == CSV_FAIL) {
		return FQL_FAIL;
	}
	return FQL_GOOD;
}

int fqlselect_finish(fqlselect* self)
{
	int ret = csv_writer_close(self->writer->writer_data);
	fail_if_ (ret == CSV_FAIL);
	return FQL_GOOD;
}

void fqlselect_preflight(fqlselect* self, query* query)
{
	_expand_asterisks(query, false);
	schema_preflight(self->schema);
}

void fqlselect_preop(fqlselect* self, query* query)
{
	vec header;
	vec_construct_(&header, column*);

	/* print header */
	column** it = vec_begin(self->schema->columns);
	for (; it != vec_end(self->schema->columns); ++it) {
		if ((*it)->expr == EXPR_ASTERISK) {
			table* aster_src = vec_at(query->sources, (*it)->src_idx);
			vec* aster_cols = aster_src->schema->columns;
			column** it2 = vec_begin(aster_cols);
			for (; it2 != vec_end(aster_cols); ++it2) {
				column* field_col = new_(column, EXPR_CONST, NULL, "");
				string* field_str = string_from_string(&(*it2)->alias);
				field_col->field.s = field_str;
				field_col->field_type = FIELD_STRING;
				vec_push_back(&header, &field_col);
			}
			continue;
		}
		column* field_col = new_(column, EXPR_CONST, NULL, "");
		string* field_str = string_from_string(&(*it)->alias);
		field_col->field.s = field_str;
		field_col->field_type = FIELD_STRING;
		vec_push_back(&header, &field_col);
	}

	writer* writer = self->writer;
	writer->write_record__(writer->writer_data, &header, NULL);

	it = vec_begin(&header);
	for (; it != vec_end(&header); ++it) {
		delete_(column, *it);
	}
	vec_destroy(&header);
}

int fqlselect_record_api(fqlselect* self, struct vec* recs)
{
	writer* writer = self->writer;

	vec* col_vec = self->schema->columns;

	column** cols = vec_begin(col_vec);
	int i = 0;
	for (; i < col_vec->size; ++i) {
		struct fql_field* field = vec_at(self->api, i);
		switch (cols[i]->field_type) {
		case FIELD_STRING:
		{
			stringview sv;
			try_ (column_get_stringview(&sv, cols[i], recs));
			/* even though the we are not using the
			 * writer directly, we use the data it
			 * owns for our string fields.
			 */
			string* s = vec_at(writer->raw_rec, i);
			string_copy_from_stringview(s, &sv);
			field->type = FQL_STRING;
			field->data.s = s->data;
			break;
		}
		case FIELD_INT:
			field->type = FQL_INT;
			try_ (column_get_int(&field->data.i, cols[i], recs));
			break;
		case FIELD_FLOAT:
			field->type = FQL_FLOAT;
			try_ (column_get_float(&field->data.f, cols[i], recs));
		default:
			;
		}
	}

	return 1;
}

int fqlselect_record(fqlselect* self, vec* recs)
{
	writer* writer = self->writer;
	vec* col_vec = self->schema->columns;

	return writer->write_record__(writer->writer_data, col_vec, recs);
}

/* this becomes a big copy operation because
 * I want to recycle the subquery's record.
 */
int fqlselect_subquery_record(reader* reader, record* rec)
{
	schema* sub_schema = reader->reader_data;
	vec* sub_col_vec = sub_schema->columns;
	vec* sub_recs = reader->subquery_recs;

	int i = 0;
	column** sub_cols = vec_begin(sub_col_vec);

	for (; i < sub_col_vec->size; ++i) {
		stringview sv;
		column_get_stringview(&sv, sub_cols[i], sub_recs);
		string* s = vec_at(rec->_field_data, i);
		string_copy_from_stringview(s, &sv);
		stringview* rec_sv = vec_at(rec->fields, i);
		rec_sv->data = s->data;
		rec_sv->len = s->size;
	}

	return FQL_GOOD;
}

/* TODO */
int fqlselect_subquery_reset(reader* reader)
{
	return FQL_FAIL;
}
