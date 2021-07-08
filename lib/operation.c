#include "operation.h"

#include "fql.h"
#include "fqlselect.h"
#include "util/util.h"

vec* op_get_columns(void* self)
{
	enum op* type = self;

	switch (*type) {
	case OP_SELECT:
		return ((fqlselect*)self)->schema->columns;
	default:
		return NULL;
	}

	return NULL;
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
	default:
		return NULL;
	}
}

void op_set_rec_terminator(enum op* self, const char* terminator)
{
	switch (*self) {
	case OP_SELECT:
		fqlselect_set_rec_terminator((fqlselect*)self, terminator);
		break;
	default:;
	}
}

void op_set_delim(enum op* self, const char* delim)
{
	switch (*self) {
	case OP_SELECT:
		fqlselect_set_delim((fqlselect*)self, delim);
		break;
	default:;
	}
}

void op_set_schema(enum op* self, const schema* src_schema)
{
	switch (*self) {
	case OP_SELECT:
		fqlselect_set_schema((fqlselect*)self, src_schema);
		break;
	default:;
	}
}

int op_writer_init(query* query)
{
	enum op* self = query->op;

	switch (*self) {
	case OP_SELECT:
		return fqlselect_writer_init(query->op, query);
	default:
		return FQL_FAIL;
	}
}

void op_apply_process(query* query, plan* plan, bool is_subquery)
{
	enum op* self = query->op;

	switch (*self) {
	case OP_SELECT:
		fqlselect_apply_process(query, plan, is_subquery);
		break;
	default:;
	}
}

void op_destroy(enum op* self)
{
	switch (*self) {
	case OP_SELECT:
		fqlselect_destroy((fqlselect*)self);
		break;
	default:;
	}
}
