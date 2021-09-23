#include "fqlset.h"
#include "fqlplan.h"
#include "process.h"
#include "expression.h"
#include "util/util.h"

fqlset* fqlset_construct(fqlset* self)
{
	*self = (fqlset) {
	        .oper_type = FQL_SET,
	        .init_expr = NULL,
	        .variable_idx = 0,
	};

	vec_construct_(&self->_expr_vec, expression*);

	return self;
}

void fqlset_destroy(fqlset* self)
{
	delete_if_exists_(expression, self->init_expr);
	vec_destroy(&self->_expr_vec);
}

void fqlset_set_init_expression(fqlset* self, expression* expr)
{
	self->init_expr = expr;
	vec_push_back(&self->_expr_vec, &expr);
}

void fqlset_preop(fqlset* self, query* query) { }

int fqlset_apply_process(query* query, plan* plan)
{
	fqlset* self = query->op;
	process* declare_proc = plan->op_true->data;

	declare_proc->action__ = &fql_set;
	declare_proc->proc_data = self;

	process* false_proc = plan->op_false->data;
	false_proc->is_passive = true;

	return FQL_GOOD;
}
