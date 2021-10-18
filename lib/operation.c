#include "operation.h"

#include "fql.h"
#include "misc.h"
#include "table.h"
#include "group.h"
#include "order.h"
#include "fqlbranch.h"
#include "reader.h"
#include "fqlhandle.h"
#include "fqlselect.h"
#include "fqldelete.h"
#include "fqlupdate.h"
#include "fqlset.h"
#include "util/util.h"

int _expand_asterisk(vec* expr_vec, table* table, unsigned src_idx, unsigned* expr_idx);

void op_destroy(enum fql_operation* self)
{
	switch (*self) {
	case FQL_SET:
		fqlset_destroy((fqlset*)self);
		break;
	case FQL_WHILE:
	case FQL_IF:
		fqlbranch_destroy((fqlbranch*)self);
		break;
	case FQL_SELECT:
		fqlselect_destroy((fqlselect*)self);
		break;
	case FQL_DELETE:
		fqldelete_destroy((fqldelete*)self);
		break;
	case FQL_UPDATE:
		fqlupdate_destroy((fqlupdate*)self);
		break;
	case FQL_NONE:;
	}
}


vec* op_get_expressions(enum fql_operation* self)
{
	/* fqlset has no schema, but it does
	 * have a useless vector of expressions
	 */
	if (*self == FQL_SET) {
		fqlset* setstmt = (fqlset*)self;
		return &setstmt->_expr_vec;
	}
	schema* op_schema = op_get_schema(self);
	if (op_schema == NULL) {
		return NULL;
	}
	return op_schema->expressions;
}

vec* op_get_additional_exprs(enum fql_operation* self)
{
	switch (*self) {
	case FQL_UPDATE: {
		fqlupdate* update = (fqlupdate*)self;
		return &update->value_expressions;
	}
	case FQL_SELECT:
	case FQL_DELETE:
	case FQL_SET:
	case FQL_WHILE:
	case FQL_IF:
	case FQL_NONE:
		return NULL;
	}
}

schema* op_get_schema(enum fql_operation* self)
{
	switch (*self) {
	case FQL_SELECT: {
		fqlselect* select = (fqlselect*)self;
		return select->schema;
	}
	case FQL_DELETE: {
		fqldelete* delete = (fqldelete*)self;
		return delete->schema;
	}
	case FQL_UPDATE: {
		fqlupdate* update = (fqlupdate*)self;
		return update->schema;
	}
	case FQL_SET:
	case FQL_WHILE:
	case FQL_IF:
	case FQL_NONE:
		return NULL;
	}
}

const char* op_get_table_name(enum fql_operation* self)
{
	switch (*self) {
	case FQL_DELETE: {
		fqldelete* delete = (fqldelete*)self;
		expression** fake_asterisk = vec_begin(delete->schema->expressions);
		return string_c_str(&(*fake_asterisk)->table_name);
	}
	case FQL_UPDATE: {
		fqlupdate* update = (fqlupdate*)self;
		expression** fake_asterisk = vec_begin(update->schema->expressions);
		return string_c_str(&(*fake_asterisk)->table_name);
	}
	case FQL_SELECT:
	case FQL_SET:
	case FQL_WHILE:
	case FQL_IF:
	case FQL_NONE:
		return NULL;
	}
}

void op_match_table_alias(enum fql_operation* self, table* check_table)
{
	bool* has_matched_alias = NULL;
	const char* op_table_name = NULL;
	unsigned* table_idx = NULL;

	switch (*self) {
	case FQL_DELETE: {
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
	case FQL_UPDATE: {
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
	case FQL_SELECT:
	case FQL_SET:
	case FQL_WHILE:
	case FQL_IF:
	case FQL_NONE:
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

void op_set_table_name(enum fql_operation* self, const char* table_name)
{
	schema* op_schema = op_get_schema(self);
	expression** fake_asterisk = vec_begin(op_schema->expressions);
	string_strcpy(&(*fake_asterisk)->table_name, table_name);
}

void op_set_top_count(enum fql_operation* self, size_t top_count)
{
	switch (*self) {
	case FQL_SELECT: {
		fqlselect* select = (fqlselect*)self;
		select->top_count = top_count;
		break;
	}
	case FQL_DELETE: {
		fqldelete* delete = (fqldelete*)self;
		delete->top_count = top_count;
		break;
	}
	case FQL_UPDATE: {
		fqlupdate* update = (fqlupdate*)self;
		update->top_count = top_count;
		break;
	}
	case FQL_SET:
	case FQL_WHILE:
	case FQL_IF:
	case FQL_NONE:;
	}
}

int op_preop(query* query)
{
	if (query->plan && query->plan->has_stepped) {
		return FQL_GOOD;
	}

	enum fql_operation* type = query->op;
	switch (*type) {
	case FQL_SET:
		fqlset_preop(query->op, query);
		break;
	case FQL_WHILE:
	case FQL_IF:
		fqlbranch_preop(query->op, query);
		break;
	case FQL_SELECT:
		fqlselect_preop(query->op, query);
		break;
	case FQL_DELETE:
		fqldelete_preop(query->op, query);
		break;
	case FQL_UPDATE:
		fqlupdate_preop(query->op, query);
		break;
	case FQL_NONE:;
	}

	return FQL_GOOD;
}

writer* op_get_writer(enum fql_operation* self)
{
	switch (*self) {
	case FQL_SELECT: {
		fqlselect* select = (fqlselect*)self;
		return select->writer;
	}
	case FQL_DELETE: {
		fqldelete* delete = (fqldelete*)self;
		return delete->writer;
	}
	case FQL_UPDATE: {
		fqlupdate* update = (fqlupdate*)self;
		return update->writer;
	}
	case FQL_SET:
	case FQL_WHILE:
	case FQL_IF:
	case FQL_NONE:
		return NULL;
	}
}

void op_set_rec_terminator(enum fql_operation* self, const char* term)
{
	strncpy_(op_get_schema(self)->rec_terminator, term, DELIM_LEN_MAX);
}

void op_set_delim(enum fql_operation* self, const char* delim)
{
	schema_set_delim(op_get_schema(self), delim);
}

void op_set_writer(enum fql_operation* self, writer* src_writer)
{
	switch (*self) {
	case FQL_SELECT: {
		fqlselect* select = (fqlselect*)self;
		select->writer = src_writer;
		break;
	}
	case FQL_DELETE: {
		fqldelete* delete = (fqldelete*)self;
		delete->writer = src_writer;
		break;
	}
	case FQL_UPDATE: {
		fqlupdate* update = (fqlupdate*)self;
		update->writer = src_writer;
		break;
	}
	case FQL_SET:
	case FQL_WHILE:
	case FQL_IF:
	case FQL_NONE:;
	}
}

void op_set_schema(enum fql_operation* self, const schema* src_schema)
{
	schema* op_schema = op_get_schema(self);
	schema_copy(op_schema, src_schema);
}

void op_assign_rownum_ref(enum fql_operation* self, expression* expr)
{
	switch (*self) {
	case FQL_SELECT: {
		fqlselect* select = (fqlselect*)self;
		expr->rownum_ref = &select->rownum;
		break;
	}
	case FQL_UPDATE: {
		fqlupdate* update = (fqlupdate*)self;
		expr->rownum_ref = &update->rownum;
		break;
	}
	case FQL_DELETE:
	case FQL_SET:
	case FQL_WHILE:
	case FQL_IF:
	case FQL_NONE:
		break;
	}
}

int _resolve_into_table_variable(query* query)
{
	return FQL_GOOD;
}

/* NOTE: do not allow any writer initialization if a union query */
int op_reset(query* query, bool has_executed)
{
	enum fql_operation* self = query->op;
	writer* op_writer = op_get_writer(query->op);

	/* Retrieve op_table */
	table* op_table = NULL;
	switch (*self) {
	case FQL_DELETE: {
		fqldelete* delete = query->op;
		op_table = vec_at(query->sources, delete->table_idx);
		try_(fqldelete_reset(delete));
		break;
	}
	case FQL_UPDATE: {
		fqlupdate* update = query->op;
		op_table = vec_at(query->sources, update->table_idx);
		try_(fqlupdate_reset(update));
		break;
	}
	case FQL_SELECT:
		fqlselect_reset((fqlselect*)self);
		break;
	case FQL_SET:
	case FQL_WHILE:
	case FQL_IF:
	case FQL_NONE:;
	}

	if (op_table != NULL && query->into_table_name == NULL) {
		/* query owns this so we dup it */
		query->into_table_name =
		        strdup(string_c_str(&op_table->reader->file_name));
	}

	/* If our into_table_name is a variable, set it now. */
	if (query->into_table_var_idx != -1) {
		variable* table_var =
		        vec_at(query->fqlref->variables, query->into_table_var_idx);
		query->into_table_name = string_c_str(table_var->value.s);
		_resolve_into_table_variable(query);
	}

	if (!query->union_id && query->into_table_name) {
		if (has_executed && *self == FQL_SELECT && !query->fqlref->props.overwrite
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
		const char* in_name = writer_export_temp(op_writer);
		try_(order_init_io(query->orderby, in_name, out_name));
		free_(in_name);
		free_(out_name);
	}

	return FQL_GOOD;
}

int op_writer_init(struct query* query)
{
	enum fql_operation* self = query->op;
	schema* op_schema = op_get_schema(query->op);

	if (op_schema != NULL && !query->union_id) {
		writer* op_writer = new_(writer, op_schema->write_io_type, query->fqlref);
		op_set_writer(query->op, op_writer);
	}

	if (*self == FQL_SELECT) {
		op_expand_asterisks(query, (!query->groupby || !query->distinct));
		if (query->distinct != NULL) {
			expression** it = vec_begin(op_schema->expressions);
			for (; it != vec_end(op_schema->expressions); ++it) {
				group_add_expression(query->distinct, *it);
			}
		}
		fqlselect_verify_must_run((fqlselect*)self);
		return FQL_GOOD;
	}

	if (*self == FQL_UPDATE) {
		fqlupdate* update = (fqlupdate*)self;
		table* update_table = vec_at(query->sources, update->table_idx);

		unsigned expr_idx = 0;
		_expand_asterisk(&update->update_expressions,
		                 update_table,
		                 update->table_idx,
		                 &expr_idx);
		try_(fqlupdate_resolve_additional((fqlupdate*)self, query));
	}

	return FQL_GOOD;
}

int op_apply_process(query* query, plan* plan, bool is_subquery)
{
	enum fql_operation* self = query->op;

	switch (*self) {
	case FQL_SET:
		return fqlset_apply_process(query, plan);
	case FQL_WHILE:
	case FQL_IF:
		return fqlbranch_apply_process(query, plan);
	case FQL_SELECT:
		fqlselect_apply_process(query, plan, is_subquery);
		return FQL_GOOD;
	case FQL_DELETE:
		return fqldelete_apply_process(query, plan);
	case FQL_UPDATE:
		return fqlupdate_apply_process(query, plan);
	case FQL_NONE:
		return FQL_GOOD;
	}
}

int op_resolve_final_types(enum fql_operation* self)
{
	switch (*self) {
	case FQL_SELECT:
		return fqlselect_resolve_final_types((fqlselect*)self);
	case FQL_UPDATE:
		return fqlupdate_resolve_final_types((fqlupdate*)self);
	case FQL_DELETE:
	case FQL_SET:
	case FQL_WHILE:
	case FQL_IF:
	case FQL_NONE:
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

	unsigned aster_idx = *expr_idx;

	expression** it = vec_begin(src_expr_vec);
	for (; it != vec_end(src_expr_vec); ++it) {
		//string* expr_name = string_from_string(&(*it)->alias);
		expression* new_expr =
		        new_(expression, EXPR_COLUMN_NAME, (*it)->alias.data, "");
		new_expr->data_source = *it;
		new_expr->width = (*it)->width;
		new_expr->location = (*it)->location;
		new_expr->field_type = (*it)->field_type;

		new_expr->src_idx = src_idx;
		if (table->subquery != NULL) {
			new_expr->subquery_src_idx = src_idx;
		}

		++(*expr_idx);
		vec_insert_at(expr_vec, *expr_idx, &new_expr, 1);
	}

	expression** asterisk_expr = vec_at(expr_vec, aster_idx);
	delete_(expression, *asterisk_expr);
	vec_erase_at(expr_vec, aster_idx, 1);
	--(*expr_idx);
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

		_expand_asterisk(expr_vec, table, (*expr)->src_idx, &i);
	}

	expression_update_indicies(expr_vec);

	writer* op_writer = op_get_writer(query->op);
	if (op_writer == NULL) {
		return;
	}
	_resize_raw_rec(op_writer->raw_rec, expr_vec->size);
}
