#include "fqlselect.h"
#include "csv.h"
#include "fql.h"
#include "misc.h"
#include "group.h"
#include "order.h"
#include "logic.h"
#include "query.h"
#include "reader.h"
#include "column.h"
#include "schema.h"
#include "process.h"
#include "util/stringy.h"
#include "util/util.h"

int _select_record(fqlselect*, struct vec* rec);
int _select_record_api(fqlselect*, struct vec* rec);
int _select_record_order_api(fqlselect*, struct vec* rec);
int _select_record_to_list(fqlselect*, struct vec* rec);
int _select_record_to_const(fqlselect*, struct vec* rec);
int _select_subquery(fqlselect*, struct vec* rec);

fqlselect* fqlselect_construct(fqlselect* self)
{
	*self = (fqlselect) {
	        OP_SELECT,       /* oper_type */
	        NULL,            /* api */
	        new_(schema),    /* schema */
	        NULL,            /* writer */
	        NULL,            /* list_data */
	        NULL,            /* const_dest */
	        NULL,            /* union_column_vecs */
	        NULL,            /* _selection_cols */
	        &_select_record, /* select__ */
	        0                /* offset */
	};

	self->_selection_cols = self->schema->columns;

	return self;
}

void fqlselect_destroy(fqlselect* self)
{
	if (self == NULL) {
		return;
	}
	//queue_free(&self->union_column_vecs);
	delete_if_exists_(writer, self->writer);
	delete_(schema, self->schema);
}

unsigned fqlselect_get_field_count(fqlselect* self)
{
	unsigned field_count = 0;
	column** it = vec_begin(self->schema->columns);
	for (; it != vec_end(self->schema->columns); ++it) {
		switch ((*it)->expr) {
		case EXPR_ASTERISK:
			field_count += (*it)->table->schema->columns->size;
			break;
		default:
			++field_count;
		}
	}

	return field_count;
}

void fqlselect_set_delim(fqlselect* self, const char* delim)
{
	strncpy_(self->schema->delimiter, delim, DELIM_LEN_MAX);
}

void fqlselect_set_rec_terminator(fqlselect* self, const char* term)
{
	strncpy_(self->schema->rec_terminator, term, DELIM_LEN_MAX);
}

void fqlselect_set_schema(fqlselect* self, const schema* src_schema)
{
	if (src_schema == NULL) {
		fqlselect_set_delim(self, ",");
		self->schema->io_type = IO_LIBCSV;
		self->schema->write_io_type = IO_LIBCSV;
		self->schema->is_default = true;
		return;
	}
	if (!self->schema->delimiter[0]) {
		fqlselect_set_delim(self, src_schema->delimiter);
	}
	self->schema->io_type = src_schema->io_type;
	self->schema->write_io_type = src_schema->write_io_type;
	self->schema->is_default = src_schema->is_default;
}

void fqlselect_add_column(fqlselect* self, column* col)
{
	schema_add_column(self->schema, col, 0);
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
		vec_insert_at(col_vec, *col_idx, &new_col, 1);
	}

	return table->schema->columns->size;
}

void _expand_asterisks(query* query, bool force_expansion)
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

		if (table->subquery == NULL /* is not a subquery source */
		    && !force_expansion && query->query_id == 0 /* is in main query */
		    && schema_eq(table->schema, self->schema)) {
			continue;
		}

		unsigned asterisk_index = i;
		_expand_asterisk(col_vec, table, (*col)->src_idx, &i);

		column** asterisk_col = vec_at(col_vec, asterisk_index);
		delete_(column, *asterisk_col);
		vec_erase_at(col_vec, asterisk_index, 1);
		--i;
	}

	if (self->writer == NULL) {
		return;
	}
	_resize_raw_rec(self->writer->raw_rec, col_vec->size);
}

int fqlselect_resolve_type_from_subquery(column* col)
{
	query* subquery = col->subquery;
	fqlselect* subselect = subquery->op;
	if (subselect->schema->columns->size > 1) {
		fputs("Only one column can be supplied if subquery is an "
		      "expression\n",
		      stderr);
		return FQL_FAIL;
	}
	column** subcol = vec_begin(subselect->schema->columns);
	if ((*subcol)->subquery == NULL) {
		col->field_type = (*subcol)->field_type;
		return FQL_GOOD;
	}
	try_(fqlselect_resolve_type_from_subquery(*subcol));
	col->field_type = (*subcol)->field_type;

	return FQL_GOOD;
}

int fqlselect_connect_api(query* query, vec* api)
{
	fqlselect* self = query->op;
	if (query->orderby == NULL) {
		self->select__ = &_select_record_api;
	} else {
		self->select__ = &_select_record_order_api;
	}
	_expand_asterisks(query, true);

	vec* cols = self->schema->columns;
	vec_resize(api, cols->size);

	column** it = vec_begin(cols);
	unsigned i = 0;
	for (; i < cols->size; ++i) {
		if ((*it)->subquery != NULL) {
			try_(fqlselect_resolve_type_from_subquery(*it));
		}
		struct fql_field* field = vec_at(api, i);
		switch (it[i]->field_type) {
		case FIELD_INT:
			field->type = FQL_INT;
			break;
		case FIELD_FLOAT:
			field->type = FQL_FLOAT;
			break;
		case FIELD_STRING:
			field->type = FQL_STRING;
			field->_in = new_(string);
			break;
		default:
			fputs("Undefined API type\n", stderr);
			return FQL_FAIL;
		}
	}

	self->api = api;
	return FQL_GOOD;
}

void fqlselect_apply_process(query* query, plan* plan, bool is_subquery_source)
{
	fqlselect* self = query->op;
	process* proc = plan->op_true->data;
	proc->action__ = &fql_select;
	proc->wait_for_in0_end = true;
	proc->proc_data = self;

	if (self->const_dest != NULL) {
		self->select__ = &_select_record_to_const;
	} else if (is_subquery_source) {
		self->select__ = &_select_subquery;
	}

	string_strcpy(proc->action_msg, "SELECT ");

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

	if (self->writer == NULL) {
		return;
	}
	writer_set_delimiter(self->writer, self->schema->delimiter);
	writer_set_rec_terminator(self->writer, self->schema->rec_terminator);
}

void fqlselect_apply_column_alias(fqlselect* self, const char* alias)
{
	schema_apply_column_alias(self->schema, alias);
}

int fqlselect_set_as_inlist(fqlselect* self, inlist* inlist)
{
	if (self->schema->columns->size == 1) {
		column** col = vec_begin(self->schema->columns);
		if ((*col)->expr == EXPR_ASTERISK) {
			_expand_asterisks(inlist->subquery, true);
		}
	}
	if (self->schema->columns->size != 1) {
		fputs("Only one expression can be specified in subquery\n", stderr);
		return FQL_FAIL;
	}
	inlist->list_data = new_(set, 16, HASHMAP_PROP_NOCASE | HASHMAP_PROP_RTRIM);
	self->list_data = inlist->list_data;
	self->select__ = &_select_record_to_list;
	return FQL_GOOD;
}

/* NOTE: do not allow any writer initialization if a union query */
int fqlselect_writer_init(fqlselect* self, query* query)
{
	if (!query->union_id) {
		self->writer = new_(writer, self->schema->write_io_type);
	}

	if (!query->union_id && query->into_table_name) {
		if (access(query->into_table_name, F_OK) == 0) {
			fprintf(stderr,
			        "Cannot SELECT INTO: file `%s' already exists\n",
			        query->into_table_name);
			return FQL_FAIL;
		}
		try_(writer_open(self->writer, query->into_table_name));
	}

	if (!query->union_id && query->orderby != NULL) {
		char* out_name = writer_take_filename(self->writer);
		const char* in_name = writer_get_tempname(self->writer);

		try_(order_init_io(query->orderby, in_name, out_name));
		/* take_filename takes ownership, must free */
		free_(out_name);
	}

	_expand_asterisks(query, (!query->groupby || !query->distinct));

	/* TODO: verify this does not cause double free */
	if (query->distinct != NULL) {
		column** it = vec_begin(self->schema->columns);
		for (; it != vec_end(self->schema->columns); ++it) {
			group_add_column(query->distinct, *it);
		}
	}

	schema_preflight(self->schema);

	return FQL_GOOD;
}

int fqlselect_next_union(fqlselect* self)
{
	if (self->union_column_vecs == NULL) {
		return 0;
	}

	self->_selection_cols = queue_dequeue(&self->union_column_vecs);
	return 1;
}

void fqlselect_preop(fqlselect* self, query* query)
{
	if (!self->schema->is_default) {
		return;
	}

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
				string_copy(&field_col->buf, &(*it2)->alias);
				field_col->field.s = &field_col->buf;
				field_col->field_type = FIELD_STRING;
				vec_push_back(&header, &field_col);
			}
			continue;
		}
		column* field_col = new_(column, EXPR_CONST, NULL, "");
		string_copy(&field_col->buf, &(*it)->alias);
		field_col->field.s = &field_col->buf;
		field_col->field_type = FIELD_STRING;
		vec_push_back(&header, &field_col);
	}

	writer* writer = self->writer;
	if (query->orderby != NULL) {
		writer->write_record__(writer->writer_data,
		                       &header,
		                       NULL,
		                       query->orderby->out_file);
	} else {
		writer->write_record__(writer->writer_data, &header, NULL, NULL);
	}

	it = vec_begin(&header);
	for (; it != vec_end(&header); ++it) {
		delete_(column, *it);
	}
	vec_destroy(&header);
}

int _select_record(fqlselect* self, vec* recs)
{
	writer* writer = self->writer;
	vec* col_vec = self->_selection_cols;

	int ret = writer->write_record__(writer->writer_data, col_vec, recs, NULL);

	if (ret == FQL_FAIL || recs == NULL) {
		return ret;
	}

	/* pass offset information along
	 * in the top record
	 */
	record** rec = vec_begin(recs);
	(*rec)->offset = self->offset;
	(*rec)->select_len = ret;

	self->offset += ret;

	return ret;
}

int _select_record_api(fqlselect* self, struct vec* recs)
{
	vec* col_vec = self->_selection_cols;

	column** cols = vec_begin(col_vec);
	unsigned i = 0;
	for (; i < col_vec->size; ++i) {
		struct fql_field* field = vec_at(self->api, i);
		switch (cols[i]->field_type) {
		case FIELD_INT:
			try_(column_get_int(&field->data.i, cols[i], recs));
			break;
		case FIELD_FLOAT:
			try_(column_get_float(&field->data.f, cols[i], recs));
			break;
		case FIELD_STRING: {
			stringview sv;
			try_(column_get_stringview(&sv, cols[i], recs));
			string* s = field->_in;
			string_copy_from_stringview(s, &sv);
			field->data.s = s->data;
			break;
		}
		default:;
		}
	}

	return 1;
}

int _select_record_order_api(fqlselect* self, struct vec* recs)
{
	FILE* order_input = writer_get_file(self->writer);

	vec* col_vec = self->_selection_cols;
	column** cols = vec_begin(col_vec);
	unsigned i = 0;
	unsigned len = 0;
	for (; i < col_vec->size; ++i) {
		switch (cols[i]->field_type) {
		case FIELD_INT: {
			long num = 0;
			try_(column_get_int(&num, cols[i], recs));
			fwrite(&num, sizeof(num), 1, order_input);
			len += sizeof(num);
			break;
		}
		case FIELD_FLOAT: {
			double num = 0;
			try_(column_get_float(&num, cols[i], recs));
			fwrite(&num, sizeof(num), 1, order_input);
			len += sizeof(num);
			break;
		}
		case FIELD_STRING: {
			stringview sv;
			try_(column_get_stringview(&sv, cols[i], recs));
			fwrite(&sv.len, sizeof(sv.len), 1, order_input);
			fwrite(sv.data, 1, sv.len, order_input);
			len += sizeof(sv.len) + sv.len;
			break;
		}
		default:;
		}
	}
	record** rec = vec_begin(recs);
	(*rec)->offset = self->offset;
	self->offset += len;

	return 1;
}

int _select_record_to_list(fqlselect* self, vec* recs)
{
	set* list = self->list_data;

	column* col = *(column**)vec_begin(self->_selection_cols);

	switch (col->field_type) {
	case FIELD_INT: {
		long num = 0;
		try_(column_get_int(&num, col, recs));
		set_nadd(list, (char*)&num, sizeof(num));
		return 1;
	}
	case FIELD_FLOAT: {
		double num = 0;
		try_(column_get_float(&num, col, recs));
		set_nadd(list, (char*)&num, sizeof(num));
		return 1;
	}
	case FIELD_STRING: {
		stringview sv;
		try_(column_get_stringview(&sv, col, recs));
		set_nadd(list, sv.data, sv.len);
		return 1;
	}
	default:
		fputs("Unexpected type in select list\n", stderr);
		return FQL_FAIL;
	}
}

int _select_record_to_const(fqlselect* self, vec* recs)
{
	//bool fail_if_on_result = (self->const_dest->expr == EXPR_CONST) {
	if (self->const_dest->expr == EXPR_CONST) {
		fputs("subquery returned more than 1 value as expression\n", stderr);
		return FQL_FAIL;
	}

	column* col = *(column**)vec_begin(self->_selection_cols);

	self->const_dest->field_type = col->field_type;
	self->const_dest->expr = EXPR_CONST;

	switch (col->field_type) {
	case FIELD_INT: {
		long num = 0;
		try_(column_get_int(&num, col, recs));
		self->const_dest->field.i = num;
		return 1;
	}
	case FIELD_FLOAT: {
		double num = 0;
		try_(column_get_float(&num, col, recs));
		self->const_dest->field.f = num;
		return 1;
	}
	case FIELD_STRING: {
		stringview sv;
		try_(column_get_stringview(&sv, col, recs));
		string_copy_from_stringview(&self->const_dest->buf, &sv);
		self->const_dest->field.s = &self->const_dest->buf;
		return 1;
	}
	default:
		fputs("Unexpected type in select list\n", stderr);
		return FQL_FAIL;
	}
}

/* This is correct.  The only reason this exists is
 * to take advantage of the fact that fql_select will
 * move through all unions via fqlselect_next_union.
 */
int _select_subquery(fqlselect* self, vec* recs)
{
	return FQL_GOOD;
}
