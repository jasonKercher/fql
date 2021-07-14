#include "switchcase.h"
#include "expression.h"
#include "logic.h"

switchcase* switchcase_construct(switchcase* self)
{
	vec_construct_(&self->test_expr_refs, expression*);
	vec_construct_(&self->values, expression*);
	self->static_expr = NULL;
	self->state = SWITCH_STATIC;

	vec_construct_(&self->tests, logicgroup);
	logicgroup* new_logic = vec_add_one(&self->tests);
	logicgroup_construct(new_logic, LG_ROOT);

	return self;
}

void switchcase_destroy(switchcase* self)
{
	vec_destroy(&self->tests);
	vec_destroy(&self->test_expr_refs);
	vec_destroy(&self->values);
}

int switchcase_add_expression(switchcase* self, expression* expr)
{
	switch (self->state) {
	case SWITCH_STATIC:
		vec_push_back(&self->test_expr_refs, &expr);
		self->static_expr = expr;
		/* Add to logicgroup */
	}
	return FQL_GOOD;
}

int switchcase_eval(switchcase* self, union field* ret, recgroup* rg)
{
	return FQL_FAIL;
}
