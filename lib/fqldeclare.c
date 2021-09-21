#include "fqldeclare.h"
#include "fqlplan.h"
#include "process.h"
#include "expression.h"
#include "util/util.h"

fqldeclare* fqldeclare_construct(fqldeclare* self)
{
	*self = (fqldeclare) {
	        OP_DECLARE, /* oper_type */
	        NULL,       /* init_expr */
	        0,          /* variable_idx */
	};

	return self;
}

void fqldeclare_destroy(fqldeclare* self)
{
	delete_if_exists_(expression, self->init_expr);
}

void fqldeclare_set_init_expression(fqldeclare* self, expression* expr)
{
	self->init_expr = expr;
}

void fqldeclare_preop(fqldeclare* self, query* query) { }

int fqldeclare_apply_process(query* query, plan* plan)
{
	fqldeclare* self = query->op;
	process* declare_proc = plan->op_true->data;

	declare_proc->action__ = &fql_declare;
	declare_proc->proc_data = self;

	process* false_proc = plan->op_false->data;
	false_proc->is_passive = true;

	return FQL_GOOD;
}
