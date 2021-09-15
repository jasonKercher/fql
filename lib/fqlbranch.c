#include "fqlbranch.h"
#include "fql.h"
#include "misc.h"
#include "logic.h"
#include "fqlplan.h"
#include "fqlhandle.h"
#include "util/util.h"
#include <stdio.h>
#include <string.h>

fqlbranch* fqlbranch_construct(fqlbranch* self, query* query)
{
	*self = (fqlbranch) {
	        OP_IF,               /* oper_type */
	        {0},                 /* conditions */
	        &query->next_idx,    /* next_query_idx_ref */
	        query->next_idx + 1, /* next_idx */
	};

	vec_construct_(&self->conditions, logicgroup*);

	return self;
}

void fqlbranch_destroy(fqlbranch* self)
{
	logicgroup** it = vec_begin(&self->conditions);
	for (; it != vec_end(&self->conditions); ++it) {
		delete_(logicgroup, *it);
	}
	vec_destroy(&self->conditions);
}

void fqlbranch_add_logicgroup(fqlbranch* self, logicgroup* lg)
{
	vec_push_back(&self->conditions, &lg);
}

void fqlbranch_preop(fqlbranch* self, query* query) { }

/* fql_if is a single step process, op_true and 
 * op_false can both be passive
 */
int fqlbranch_apply_process(query* query, plan* plan)
{
	fqlbranch* self = query->op;
	process* logic_proc = plan->op_true->data;

	logicgroup** it = vec_begin(&self->conditions);
	for (; it != vec_end(&self->conditions); ++it) {
		try_(plan_logic_to_process(plan, logic_proc, *it));
	}
	logic_proc->action__ = fql_if;
	logic_proc->proc_data = self;

	process* false_proc = plan->op_false->data;
	false_proc->is_passive = true;

	return FQL_GOOD;
}
