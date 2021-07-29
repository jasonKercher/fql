#include "operation.h"

#include "fql.h"
#include "misc.h"
#include "group.h"
#include "order.h"
#include "fqlhandle.h"
#include "fqlselect.h"
#include "fqldelete.h"
#include "util/util.h"

vec* op_get_expressions(void* self)
{
	enum op* type = self;

	switch (*type) {
	case OP_SELECT:
		return ((fqlselect*)self)->schema->expressions;
	case OP_DELETE:
	default:
		return NULL;
	}

	return NULL;
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
	default:;
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
	default:
		return NULL;
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

	if (!query->union_id && query->into_table_name) {
		if (!fql->props.overwrite && access(query->into_table_name, F_OK) == 0) {
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

	fqlselect_expand_asterisks(query, (!query->groupby || !query->distinct));

	/* TODO: verify this does not cause double free */
	if (query->distinct != NULL) {
		expression** it = vec_begin(op_schema->expressions);
		for (; it != vec_end(op_schema->expressions); ++it) {
			group_add_expression(query->distinct, *it);
		}
	}

	return FQL_GOOD;
}

void op_apply_process(query* query, plan* plan, bool is_subquery)
{
	enum op* self = query->op;

	switch (*self) {
	case OP_SELECT:
		fqlselect_apply_process(query, plan, is_subquery);
		break;
	case OP_DELETE:
		fqldelete_apply_process(query, plan);
		break;
	default:;
	}
}

int op_resolve_final_types(enum op* self)
{
	switch (*self) {
	case OP_SELECT:
		return fqlselect_resolve_final_types((fqlselect*)self);
	case OP_DELETE:
	default:
		return FQL_GOOD;
	}
}

void op_destroy(enum op* self)
{
	switch (*self) {
	case OP_SELECT:
		fqlselect_destroy((fqlselect*)self);
		break;
	case OP_DELETE:
		fqldelete_destroy((fqldelete*)self);
		break;
	default:;
	}
}
