#include "operation.h"

#include "fql.h"
#include "misc.h"
#include "table.h"
#include "group.h"
#include "order.h"
#include "reader.h"
#include "fqlhandle.h"
#include "fqlselect.h"
#include "fqldelete.h"
#include "fqlupdate.h"
#include "util/util.h"

void op_destroy(enum op* self)
{
	switch (*self) {
	case OP_SELECT:
		fqlselect_destroy((fqlselect*)self);
		break;
	case OP_DELETE:
		fqldelete_destroy((fqldelete*)self);
		break;
	case OP_UPDATE:
		fqlupdate_destroy((fqlupdate*)self);
		break;
	default:;
	}
}


vec* op_get_expressions(enum op* self)
{
	return op_get_schema(self)->expressions;
}

vec* op_get_additional_exprs(enum op* self)
{
	switch (*self) {
	case OP_UPDATE: {
		fqlupdate* update = (fqlupdate*)self;
		return &update->value_expressions;
	}
	case OP_SELECT:
	case OP_DELETE:
	default:
		return NULL;
	}
}

schema* op_get_schema(enum op* self)
{
	switch (*self) {
	case OP_SELECT: {
		fqlselect* select = (fqlselect*)self;
		return select->schema;
	}
	case OP_DELETE: {
		fqldelete* delete = (fqldelete*)self;
		return delete->schema;
	}
	case OP_UPDATE: {
		fqlupdate* update = (fqlupdate*)self;
		return update->schema;
	}
	default:
		return NULL;
	}
}

const char* op_get_table_name(enum op* self)
{
	switch (*self) {
	case OP_DELETE: {
		fqldelete* delete = (fqldelete*)self;
		expression** fake_asterisk = vec_begin(delete->schema->expressions);
		return string_c_str(&(*fake_asterisk)->table_name);
	}
	case OP_UPDATE: {
		fqlupdate* update = (fqlupdate*)self;
		expression** fake_asterisk = vec_begin(update->schema->expressions);
		return string_c_str(&(*fake_asterisk)->table_name);
	}
	case OP_SELECT:
	default:
		return NULL;
	}
}

void op_match_table_alias(enum op* self, table* check_table)
{
	bool* has_matched_alias = NULL;
	const char* op_table_name = NULL;
	unsigned* table_idx = NULL;

	switch (*self) {
	case OP_DELETE: {
		fqldelete* delete = (fqldelete*)self;
		has_matched_alias = &delete->has_matched_alias;
		if (*has_matched_alias) {
			return;
		}
		table_idx = &delete->table_idx;
		expression** fake_asterisk = vec_begin(delete->schema->expressions);
		op_table_name = string_c_str(&(*fake_asterisk)->table_name);
		break;
	}
	case OP_UPDATE: {
		fqlupdate* update = (fqlupdate*)self;
		has_matched_alias = &update->has_matched_alias;
		if (*has_matched_alias) {
			return;
		}
		table_idx = &update->table_idx;
		expression** fake_asterisk = vec_begin(update->schema->expressions);
		op_table_name = string_c_str(&(*fake_asterisk)->table_name);
		break;
	}
	case OP_SELECT:
	default:
		return;
	}

	if (!strcasecmp(op_table_name, string_c_str(&check_table->alias))) {
		*has_matched_alias = true;
		*table_idx = check_table->idx;
		return;
	}

	/* Well, we didn't match on alias or by any default
	 * rules. No biggie, because this is still legal:
	 *
	 * DELETE [file.txt]  -- Note: Still legal to refer to the table
	 * FROM [file.txt] t1 --       name and not just the alias...
	 * JOIN [file2.txt] t2
	 * 	ON t2.foo = t1.foo
	 */
	if (!strcasecmp(op_table_name, string_c_str(&check_table->name))) {
		*table_idx = check_table->idx;
	}
}

void op_set_table_name(enum op* self, const char* table_name)
{
	schema* op_schema = op_get_schema(self);
	expression** fake_asterisk = vec_begin(op_schema->expressions);
	string_strcpy(&(*fake_asterisk)->table_name, table_name);
}

void op_set_top_count(enum op* self, size_t top_count)
{
	switch (*self) {
	case OP_SELECT: {
		fqlselect* select = (fqlselect*)self;
		select->top_count = top_count;
		break;
	}
	case OP_DELETE: {
		fqldelete* delete = (fqldelete*)self;
		delete->top_count = top_count;
		break;
	}
	case OP_UPDATE: {
		fqlupdate* update = (fqlupdate*)self;
		update->top_count = top_count;
		break;
	}
	default:;
	}
}

void op_preop(struct fql_handle* fql)
{
	query* query = fql->query_list->data;
	enum op* type = query->op;

	switch (*type) {
	case OP_SELECT:
		if (fql->props.print_header) {
			fqlselect_preop(query->op, query);
		}
		break;
	case OP_DELETE:
		fqldelete_preop(query->op, query);
		break;
	case OP_UPDATE:
		fqlupdate_preop(query->op, query);
		break;
	default:;
	}
}

writer* op_get_writer(enum op* self)
{
	switch (*self) {
	case OP_SELECT: {
		fqlselect* select = (fqlselect*)self;
		return select->writer;
	}
	case OP_DELETE: {
		fqldelete* delete = (fqldelete*)self;
		return delete->writer;
	}
	case OP_UPDATE: {
		fqlupdate* update = (fqlupdate*)self;
		return update->writer;
	}
	default:
		return NULL;
	}
}

void op_set_rec_terminator(enum op* self, const char* term)
{
	strncpy_(op_get_schema(self)->rec_terminator, term, DELIM_LEN_MAX);
}

void op_set_delim(enum op* self, const char* delim)
{
	schema_set_delim(op_get_schema(self), delim);
}

void op_set_writer(enum op* self, writer* src_writer)
{
	switch (*self) {
	case OP_SELECT: {
		fqlselect* select = (fqlselect*)self;
		select->writer = src_writer;
		break;
	}
	case OP_DELETE: {
		fqldelete* delete = (fqldelete*)self;
		delete->writer = src_writer;
		break;
	}
	case OP_UPDATE: {
		fqlupdate* update = (fqlupdate*)self;
		update->writer = src_writer;
		break;
	}
	default:;
	}
}

void op_set_schema(enum op* self, const schema* src_schema)
{
	schema* op_schema = op_get_schema(self);

	if (src_schema == NULL) {
		if (!op_schema->delim_is_set) {
			op_set_delim(self, ",");
		}
		op_schema->io_type = IO_LIBCSV;
		op_schema->write_io_type = IO_LIBCSV;
		op_schema->is_default = true;
		return;
	}
	if (!op_schema->delim_is_set) {
		op_set_delim(self, src_schema->delimiter);
	}
	op_schema->io_type = src_schema->io_type;
	op_schema->write_io_type = src_schema->write_io_type;
	op_schema->is_default = src_schema->is_default;
}

/* NOTE: do not allow any writer initialization if a union query */
int op_writer_init(query* query, struct fql_handle* fql)
{
	enum op* self = query->op;
	schema* op_schema = op_get_schema(query->op);
	writer* op_writer = NULL;
	if (!query->union_id) {
		op_writer = new_(writer, op_schema->write_io_type, fql);
		op_set_writer(self, op_writer);
	}
	op_get_writer(query->op);

	table* op_table = NULL;

	switch (*self) {
	case OP_DELETE: {
		fqldelete* delete = query->op;
		op_table = vec_at(query->sources, delete->table_idx);
		break;
	}
	case OP_UPDATE: {
		fqlupdate* update = query->op;
		op_table = vec_at(query->sources, update->table_idx);
		break;
	}
	case OP_SELECT:
	default:;
	}

	if (op_table != NULL) {
		/* query owns this so we dup it */
		query->into_table_name =
		        strdup(string_c_str(&op_table->reader->file_name));
	}

	/* If we are dealing specifically with
	 *
	 * SELECT foo
	 * INTO [new.txt]
	 * FROM t1
	 *
	 * verify new.txt does not already exist.
	 * NOTE: --overwrite nullifies this test.
	 */
	if (!query->union_id && query->into_table_name) {
		if (*self == OP_SELECT && !fql->props.overwrite
		    && access(query->into_table_name, F_OK) == 0) {
			fprintf(stderr,
			        "Cannot SELECT INTO: file `%s' already exists\n",
			        query->into_table_name);
			return FQL_FAIL;
		}
		try_(writer_open(op_writer, query->into_table_name));
	}

	if (!query->union_id && query->orderby != NULL) {
		char* out_name = writer_take_filename(op_writer);
		const char* in_name = writer_get_tempname(op_writer);

		try_(order_init_io(query->orderby, in_name, out_name));
		/* take_filename takes ownership, must free */
		free_(out_name);
	}

	if (*self != OP_SELECT) {
		return FQL_GOOD;
	}

	op_expand_asterisks(query, (!query->groupby || !query->distinct));

	/* TODO: verify this does not cause double free */
	if (query->distinct != NULL) {
		expression** it = vec_begin(op_schema->expressions);
		for (; it != vec_end(op_schema->expressions); ++it) {
			group_add_expression(query->distinct, *it);
		}
	}

	return FQL_GOOD;
}

int op_apply_process(query* query, plan* plan, bool is_subquery)
{
	enum op* self = query->op;

	switch (*self) {
	case OP_SELECT:
		fqlselect_apply_process(query, plan, is_subquery);
		return FQL_GOOD;
	case OP_DELETE:
		return fqldelete_apply_process(query, plan);
	case OP_UPDATE:
		return fqlupdate_apply_process(query, plan);
	default:
		return FQL_GOOD;
	}
}

int op_resolve_additional(enum op* self, query* query)
{
	switch (*self) {
	case OP_UPDATE:
		return fqlupdate_resolve_additional((fqlupdate*)self, query);
	case OP_SELECT:
	case OP_DELETE:
	default:
		return FQL_GOOD;
	}
}

int op_resolve_final_types(enum op* self)
{
	switch (*self) {
	case OP_SELECT:
		return fqlselect_resolve_final_types((fqlselect*)self);
	case OP_UPDATE:
		return fqlupdate_resolve_final_types((fqlupdate*)self);
	case OP_DELETE:
	default:
		return FQL_GOOD;
	}
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

int _expand_asterisk(vec* expr_vec, table* table, unsigned src_idx, unsigned* expr_idx)
{
	vec* src_expr_vec = table->schema->expressions;

	table->reader->max_idx = src_expr_vec->size - 1;

	expression** it = vec_begin(src_expr_vec);
	for (; it != vec_end(src_expr_vec); ++it) {
		//string* expr_name = string_from_string(&(*it)->alias);
		expression* new_expr =
		        new_(expression, EXPR_COLUMN_NAME, (*it)->alias.data, "");
		new_expr->data_source = *it;
		new_expr->src_idx = src_idx;
		new_expr->field_type = (*it)->field_type;
		++(*expr_idx);
		vec_insert_at(expr_vec, *expr_idx, &new_expr, 1);
	}

	return table->schema->expressions->size;
}

void op_expand_asterisks(query* query, bool force_expansion)
{
	schema* op_schema = op_get_schema(query->op);
	vec* expr_vec = op_schema->expressions;
	unsigned i = 0;

	for (; i < expr_vec->size; ++i) {
		expression** expr = vec_at(expr_vec, i);
		if ((*expr)->expr != EXPR_ASTERISK) {
			continue;
		}

		table* table = vec_at(query->sources, (*expr)->src_idx);

		if (table->subquery == NULL /* is not a subquery source */
		    && !force_expansion && query->query_id == 0 /* is in main query */
		    && schema_eq(table->schema, op_schema)) {
			continue;
		}

		unsigned asterisk_index = i;
		_expand_asterisk(expr_vec, table, (*expr)->src_idx, &i);

		expression** asterisk_expr = vec_at(expr_vec, asterisk_index);
		delete_(expression, *asterisk_expr);
		vec_erase_at(expr_vec, asterisk_index, 1);
		--i;
	}

	expression_update_indicies(expr_vec);

	writer* op_writer = op_get_writer(query->op);
	if (op_writer == NULL) {
		return;
	}
	_resize_raw_rec(op_writer->raw_rec, expr_vec->size);
}
