#include "switchcase.h"
#include "expression.h"
#include "record.h"
#include "logic.h"
#include "misc.h"

switchcase* switchcase_construct(switchcase* self)
{
	vec_construct_(&self->values, expression*);
	self->static_expr = NULL;
	self->state = SWITCH_STATIC;

	vec_construct_(&self->tests, logicgroup*);
	return self;
}

void switchcase_destroy(switchcase* self)
{
	vec_destroy(&self->tests);
	vec_destroy(&self->values);
}

void switchcase_add_value(switchcase* self, const expression* expr)
{
	vec_push_back(&self->values, &expr);
	self->state = SWITCH_LOGIC_GROUP;
}

void switchcase_add_logicgroup(switchcase* self, logicgroup* lg)
{
	vec_push_back(&self->tests, &lg);
	self->state = SWITCH_VALUE;
}

int switchcase_resolve_type(switchcase* self, expression* expr)
{
	expression** first_val = vec_begin(&self->values);
	expr->field_type = (*first_val)->field_type;
	return FQL_GOOD;
}

int _logic_eval_idx(switchcase* self, recgroup* rg)
{
	unsigned i = 0;
	logicgroup** it = vec_begin(&self->tests);

	for (; it != vec_end(&self->tests); ++it, ++i) {
		switch (logicgroup_eval(*it, rg, NULL)) {
		case 1:
			return i;
			break;
		case FQL_FAIL:
			return FQL_FAIL;
		default:;
		}
	}

	/* If there is an extra value, it is the ELSE branch */
	if (self->values.size > self->tests.size) {
		return self->values.size - 1;
	}

	return -1;
}

int switchcase_eval_to_int(switchcase* self, long* ret, recgroup* rg)
{
	int val_idx = try_(_logic_eval_idx(self, rg));

	/* NO MATCH */
	if (val_idx == -1) {
		*ret = 0;
		return FQL_GOOD;
	}

	expression** val = vec_at(&self->values, val_idx);

	return expression_get_int(ret, *val, rg);
}

int switchcase_eval_to_float(switchcase* self, double* ret, recgroup* rg)
{
	int val_idx = try_(_logic_eval_idx(self, rg));

	/* ELSE */
	if (val_idx == -1) {
		*ret = 0;
		return FQL_GOOD;
	}

	expression** val = vec_at(&self->values, val_idx);

	return expression_get_float(ret, *val, rg);
}

int switchcase_eval_to_stringview(switchcase* self, stringview* ret, recgroup* rg)
{
	int val_idx = try_(_logic_eval_idx(self, rg));

	static const char* null = "";

	/* ELSE */
	if (val_idx == -1) {
		ret->data = null;
		ret->len = 0;
		return FQL_GOOD;
	}

	expression** val = vec_at(&self->values, val_idx);

	return expression_get_stringview(ret, *val, rg);
}
