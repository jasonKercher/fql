#include "operation.h"

#include "fql.h"
#include "fqlselect.h"
#include "util/util.h"

vec* op_get_validation_list(void* op)
{
	enum op* type = op;

	switch (*type) {
	case OP_SELECT:
		return ((fqlselect*) op)->schema->columns;
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
	default:
		;
	}
}

_Bool op_has_delim(enum op* op)
{
	switch (*op) {
	case OP_SELECT:
		return fqlselect_has_delim((fqlselect*)op);
		break;
	default:
		return true;
	}
}

void op_set_delim(enum op* op, const char* delim)
{
	switch (*op) {
	case OP_SELECT:
		fqlselect_set_delim((fqlselect*)op, delim);
		break;
	default:
		;
	}
}

void op_finalize(query* query)
{
	enum op* type = query->op;

	switch (*type) {
	case OP_SELECT:
		fqlselect_finalize(query->op, query);
		break;
	default:
		;
	}
}
void op_apply_process(query* query, plan* plan)
{
	enum op* type = query->op;

	switch (*type) {
	case OP_SELECT:
		fqlselect_apply_process(query, plan);
		break;
	default:
		;
	}
}

void op_destroy(enum op* op)
{
	switch (*op) {
	case OP_SELECT:
		fqlselect_destroy((fqlselect*)op);
		break;
	default:
		;
	}
}
