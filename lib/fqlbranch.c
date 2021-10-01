#include "fqlbranch.h"
#include "fql.h"
#include "misc.h"
#include "logic.h"
#include "fqlplan.h"
#include "fqlhandle.h"
#include "util/util.h"
#include <stdio.h>
#include <string.h>

fqlbranch* fqlbranch_construct(fqlbranch* self,
                               fqlhandle* fql,
                               query* query,
                               enum fql_operation operation)
{
	*self = (fqlbranch) {
	        .oper_type = operation,
	        .next_query_idx_ref = &query->next_idx,
	        .scope = new_(scope),
	        .true_idx = query->idx + 1,
	        .false_idx = -1,
	};

	if (fql->branch_state == BRANCH_EXPECT_ELSE) {
		self->scope->parent_scope = fql->_scope->parent_scope;
	} else {
		fql->branch_state = BRANCH_EXPECT_EXPR;
		self->scope->parent_scope = fql->_scope;
	}
	fql->_scope = self->scope;

	return self;
}

void fqlbranch_destroy(fqlbranch* self)
{
	delete_if_exists_(logicgroup, self->condition);
}

void fqlbranch_add_logicgroup(fqlbranch* self, logicgroup* lg)
{
	self->condition = lg;
}

void fqlbranch_preop(fqlbranch* self, query* query) { }

/* fql_if is a single step process, op_true and 
 * op_false can both be passive
 */
int fqlbranch_apply_process(query* query, plan* plan)
{
	fqlbranch* self = query->op;
	process* logic_proc = plan->op_true->data;

	try_(plan_logic_to_process(plan, logic_proc, self->condition));
	logic_proc->action__ = fql_if;
	logic_proc->proc_data = self;

	process* false_proc = plan->op_false->data;
	false_proc->is_passive = true;

	return FQL_GOOD;
}
