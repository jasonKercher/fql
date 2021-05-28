#include "logic.h"

#include "column.h"
#include "field.h"
#include "util/util.h"
#include <string.h>

logic* logic_construct(logic* self)
{
	*self = (logic) {
	        {NULL, NULL},    /* col */
	        NULL,            /* proc */
	        FIELD_UNDEFINED, /* data_type */
	        COMP_NOT_SET     /* comp_type */
	};

	return self;
}

void logic_destroy(logic* self)
{
	if (self == NULL) {
		return;
	}
	delete_(column, self->col[0]);
	delete_(column, self->col[1]);
}

void logic_assign_process(logic* self, process* proc)
{
	self->data_type = field_determine_type(self->col[0]->field_type,
	                                       self->col[1]->field_type);
	self->logic__ = logic_matrix[self->comp_type][self->data_type];

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
	case COMP_LIKE:
		string_strcat(proc->action_msg, " LIKE ");
		break;
	case COMP_NOT_LIKE:
		string_strcat(proc->action_msg, " NOT LIKE ");
		break;
	case COMP_NULL:
		string_strcat(proc->action_msg, " NULL ");
		break;
	case COMP_NOT_NULL:
		string_strcat(proc->action_msg, " NOT NULL ");
		break;
	case COMP_NOT_SET:
		string_strcat(proc->action_msg, " <no comparison> ");
		break;
	default:
		break;
	}
	column_cat_description(self->col[1], proc->action_msg);
}

void logic_add_column(logic* self, struct column* col)
{
	if (self->col[0] == NULL) {
		self->col[0] = col;
		return;
	}
	self->col[1] = col;
}

void logic_set_comparison(logic* self, const char* op)
{
	if (string_eq(op, "="))
		self->comp_type = COMP_EQ;
	else if (string_eq(op, "<>") || string_eq(op, "!="))
		self->comp_type = COMP_NE;
	else if (string_eq(op, ">"))
		self->comp_type = COMP_GT;
	else if (string_eq(op, ">="))
		self->comp_type = COMP_GE;
	else if (string_eq(op, "<"))
		self->comp_type = COMP_LT;
	else if (string_eq(op, "<="))
		self->comp_type = COMP_LE;
	else if (istring_eq(op, "LIKE"))
		self->comp_type = COMP_LIKE;
	else if (istring_eq(op, "NOT_LIKE"))
		self->comp_type = COMP_NOT_LIKE;
	else if (istring_eq(op, "NULL"))
		self->comp_type = COMP_NULL;
	else if (istring_eq(op, "NOT_NULL"))
		self->comp_type = COMP_NOT_NULL;
}

logicgroup* logicgroup_construct(logicgroup* lg, enum logicgroup_type type)
{
	*lg = (logicgroup) {
	        type /* type */
	        ,
	        {0} /* items */
	        ,
	        NULL /* joinable */
	        ,
	        NULL /* join_logic */
	        ,
	        NULL /* condition */
	};

	vec_construct_(&lg->items, logicgroup*);

	return lg;
}

void logicgroup_destroy(logicgroup* lg)
{
	unsigned i = 0;
	for (; i < lg->items.size; ++i) {
		logicgroup** lg_item = vec_at(&lg->items, i);
		delete_(logic, (*lg_item)->condition);
		delete_(logicgroup, *lg_item);
	}
	if (lg->joinable) {
		delete_(vec, lg->joinable);
	}
	vec_destroy(&lg->items);
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
 * every self is true except the one provided.
 * the point is to determine if that self MUST be
 * true for the group to evaluate to true.
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
 * the skip argument is for self that can
 * be assumed true because it was evaluated
 * prior to calling this function.
 */
int logicgroup_eval(logicgroup* lg, vec* recs, logic* skip)
{
	logicgroup** it = vec_begin(&lg->items);
	if (lg->type == LG_NOT && lg->condition != NULL) {
		if (lg->condition == skip) {
			return 1;
		}
		return lg->condition->logic__(lg->condition, recs);
	}

	int ret = 0;
	for (; it != vec_end(&lg->items); ++it) {
		ret = logicgroup_eval(*it, recs, skip);
		if (ret == 0 && lg->type == LG_AND) {
			return 0;
		}
		if (ret == 1 && (*it)->type == LG_AND) {
			return 1;
		}
		fail_if_(ret == FQL_FAIL);
	}
	return ret;
}
