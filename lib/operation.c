#include "operation.h"

#include "fql.h"
#include "fqlhandle.h"
#include "fqlselect.h"
#include "util/util.h"

vec* op_get_expressions(void* self)
{
	enum op* type = self;

	switch (*type) {
	case OP_SELECT:
		return ((fqlselect*)self)->schema->expressions;
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

int op_writer_init(query* query, struct fql_handle* fql)
{
	enum op* self = query->op;

	switch (*self) {
	case OP_SELECT:
		return fqlselect_writer_init(query->op, query, fql);
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

int op_resolve_final_types(enum op* self)
{
	switch (*self) {
	case OP_SELECT:
		return fqlselect_resolve_final_types((fqlselect*)self);
	default:;
		return FQL_FAIL;
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
