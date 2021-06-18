#include "logic.h"

#include <string.h>

#include "column.h"
#include "field.h"
#include "misc.h"
#include "util/util.h"

static logic_fn _logic_matrix[COMP_COUNT][FIELD_TYPE_COUNT] = {
        {&fql_logic_eq_i, &fql_logic_eq_f, &fql_logic_eq_s},
        {&fql_logic_ne_i, &fql_logic_ne_f, &fql_logic_ne_s},
        {&fql_logic_gt_i, &fql_logic_gt_f, &fql_logic_gt_s},
        {&fql_logic_ge_i, &fql_logic_ge_f, &fql_logic_ge_s},
        {&fql_logic_lt_i, &fql_logic_lt_f, &fql_logic_lt_s},
        {&fql_logic_le_i, &fql_logic_le_f, &fql_logic_le_s},
        {&fql_logic_in_i, &fql_logic_in_f, &fql_logic_in_s},
        {&fql_logic_subin_i, &fql_logic_subin_f, &fql_logic_subin_s},
        {NULL, NULL, &fql_logic_like},
        {&fql_logic_is_null, &fql_logic_is_null, &fql_logic_is_null},
};

logic* logic_construct(logic* self)
{
	*self = (logic) {
	        {NULL, NULL},    /* col */
	        NULL,            /* like_data */
	        NULL,            /* in_data */
	        NULL,            /* logic__ */
	        FIELD_UNDEFINED, /* data_type */
	        COMP_NOT_SET,    /* comp_type */
	};

	return self;
}

void logic_destroy(logic* self)
{
	if (self == NULL) {
		return;
	}
	delete_if_exists_(column, self->col[0]);
	delete_if_exists_(column, self->col[1]);
	delete_if_exists_(inlist, self->in_data);
}

int _precompile_like(logic* self)
{
	self->like_data = new_(like);
	if (self->col[1]->expr != EXPR_CONST) {
		return FQL_GOOD;
	}
	stringview sv = {0};
	try_(column_get_stringview(&sv, self->col[1], NULL));
	try_(like_to_regex(self->like_data, sv));

	pcre2_jit_compile(self->like_data->regex, PCRE2_JIT_COMPLETE);

	return FQL_GOOD;
}

int logic_assign_process(logic* self, process* proc)
{
	if (self->in_data != NULL) {
		self->data_type =
		        inlist_determine_type(self->in_data, self->col[0]);
	} else {
		self->data_type =
		        field_determine_type(self->col[0]->field_type,
		                             self->col[1]->field_type);
	}
	self->logic__ = _logic_matrix[self->comp_type][self->data_type];

	column_cat_description(self->col[0], proc->action_msg);
	switch (self->comp_type) {
	case COMP_EQ:
		string_strcat(proc->action_msg, " = ");
		break;
	case COMP_NE:
		string_strcat(proc->action_msg, " != ");
		break;
	case COMP_GT:
		string_strcat(proc->action_msg, " > ");
		break;
	case COMP_GE:
		string_strcat(proc->action_msg, " >= ");
		break;
	case COMP_LT:
		string_strcat(proc->action_msg, " < ");
		break;
	case COMP_LE:
		string_strcat(proc->action_msg, " <= ");
		break;
	case COMP_IN:
		string_strcat(proc->action_msg, " IN ");
		break;
	case COMP_LIKE:
		try_(_precompile_like(self));
		string_strcat(proc->action_msg, " LIKE ");
		break;
	case COMP_NULL:
		string_strcat(proc->action_msg, " NULL ");
		break;
	case COMP_NOT_SET:
		string_strcat(proc->action_msg, " <no comparison> ");
		break;
	default:
		break;
	}
	if (self->in_data != NULL) {
		inlist_cat_description(self->in_data, proc->action_msg);
	} else {
		column_cat_description(self->col[1], proc->action_msg);
	}

	return FQL_GOOD;
}

void logic_add_column(logic* self, struct column* col)
{
	if (self->col[0] == NULL) {
		self->col[0] = col;
		return;
	}
	if (self->in_data != NULL) {
		inlist_add_column(self->in_data, col);
		return;
	}
	self->col[1] = col;
}

void logic_set_comparison(logic* self, const char* op)
{
	if (string_eq(op, "=")) {
		self->comp_type = COMP_EQ;
	} else if (string_eq(op, "<>") || string_eq(op, "!=")) {
		self->comp_type = COMP_NE;
	} else if (string_eq(op, ">")) {
		self->comp_type = COMP_GT;
	} else if (string_eq(op, ">=")) {
		self->comp_type = COMP_GE;
	} else if (string_eq(op, "<")) {
		self->comp_type = COMP_LT;
	} else if (string_eq(op, "<=")) {
		self->comp_type = COMP_LE;
	} else if (istring_eq(op, "IN")) {
		self->comp_type = COMP_IN;
	} else if (istring_eq(op, "LIKE")) {
		self->comp_type = COMP_LIKE;
	} else if (istring_eq(op, "NULL")) {
		self->comp_type = COMP_NULL;
	}
}

/** LOGIC GROUP **/

logicgroup* logicgroup_construct(logicgroup* lg, enum logicgroup_type type)
{
	*lg = (logicgroup) {
	        type,  /* type */
	        {0},   /* items */
	        NULL,  /* columns */
	        NULL,  /* joinable */
	        NULL,  /* join_logic */
	        NULL,  /* condition */
	        false, /* negation */
	};

	vec_construct_(&lg->items, logicgroup*);

	return lg;
}

void logicgroup_destroy(logicgroup* lg)
{
	unsigned i = 0;
	for (; i < lg->items.size; ++i) {
		logicgroup** lg_item = vec_at(&lg->items, i);
		delete_if_exists_(logic, (*lg_item)->condition);
		delete_(logicgroup, *lg_item);
	}
	delete_if_exists_(vec, lg->joinable);
	vec_destroy(&lg->items);
	delete_if_exists_(vec, lg->columns);
}

void _get_condition_count(logicgroup* lg, unsigned* count)
{
	if (lg->type == LG_NOT) {
		++(*count);
	}
	logicgroup** it = vec_begin(&lg->items);
	for (; it != vec_end(&lg->items); ++it) {
		_get_condition_count(*it, count);
	}
}

unsigned logicgroup_get_condition_count(logicgroup* lg)
{
	unsigned count = 0;
	_get_condition_count(lg, &count);
	return count;
}

/* essentially the same as logicgroup_eval.
 * except all logic is true except the one provided.
 * the point is to determine if that self MUST be
 * true for the group to evaluate to true. I believe
 * ignoring negation here is correct.
 */
int logic_can_be_false(logicgroup* lg, logic* check_logic)
{
	logicgroup** it = vec_begin(&lg->items);
	if (lg->type == LG_NOT && lg->condition != NULL) {
		if (lg->condition == check_logic) {
			return 0;
		}
		return 1;
	}

	int ret = 0;
	for (; it != vec_end(&lg->items); ++it) {
		ret = logic_can_be_false(*it, check_logic);
		if (ret == 0 && lg->type == LG_AND) {
			return 0;
		}
		if (ret == 1 && (*it)->type == LG_AND) {
			return 1;
		}
	}
	return ret;
}

/* evaluate the self statement
 * the skip argument is for logic that can
 * be assumed true because it was evaluated
 * prior to calling this function.
 */
int logicgroup_eval(logicgroup* lg, vec* recs, logic* skip)
{
	if (lg->type == LG_NOT && lg->condition != NULL) {
		if (lg->condition == skip) {
			return 1;
		}
		int ret = try_(lg->condition->logic__(lg->condition, recs));
		if (lg->negation) {
			return !ret;
		}
		return ret;
	}

	int ret = 0;
	logicgroup** it = vec_begin(&lg->items);
	for (; it != vec_end(&lg->items); ++it) {
		ret = try_(logicgroup_eval(*it, recs, skip));
		if (lg->negation) {
			ret = !ret;
		}
		if (ret == 0 && lg->type == LG_AND) {
			return 0;
		}
		if (ret == 1 && (*it)->type == LG_AND) {
			return 1;
		}
	}
	return ret;
}
