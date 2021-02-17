#include "logic.h"

#include "column.h"
#include "field.h"
#include "util/util.h"
#include <string.h>

Logic* logic_new()
{
        Logic* new_logic = NULL;
        malloc_(new_logic, sizeof(*new_logic));

        return logic_construct(new_logic);
}

Logic* logic_construct(Logic* logic)
{
        *logic = (Logic) {
                 {NULL, NULL}    /* col */
                ,NULL            /* proc */
                ,FIELD_UNDEFINED /* data_type */
                ,COMP_NOT_SET    /* comp_type */
        };

        return logic;
}

void logic_free(Logic* logic)
{
        free_(logic);
}

void logic_assign_process(Logic* logic, Process* proc)
{
        logic->data_type = field_determine_type(logic->col[0]->field_type,
                                                logic->col[1]->field_type);
        logic->logic_fn = logic_matrix[logic->comp_type][logic->data_type];

        column_cat_description(logic->col[0], proc->action_msg);
        switch (logic->comp_type) {
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
        column_cat_description(logic->col[1], proc->action_msg);
}

void logic_add_column(Logic* logic, struct column* col)
{
        if (logic->col[0] == NULL) {
                logic->col[0] = col;
                return;
        }
        logic->col[1] = col;
}

void logic_set_comparison(Logic* logic, const char* op)
{
        if(string_eq(op, "="))
                logic->comp_type = COMP_EQ;
        else if(string_eq(op, "<>") || string_eq(op, "!="))
                logic->comp_type = COMP_NE;
        else if(string_eq(op, ">"))
                logic->comp_type = COMP_GT;
        else if(string_eq(op, ">="))
                logic->comp_type = COMP_GE;
        else if(string_eq(op, "<"))
                logic->comp_type = COMP_LT;
        else if(string_eq(op, "<="))
                logic->comp_type = COMP_LE;
        else if(istring_eq(op, "LIKE"))
                logic->comp_type = COMP_LIKE;
        else if(istring_eq(op, "NOT_LIKE"))
                logic->comp_type = COMP_NOT_LIKE;
        else if(istring_eq(op, "NULL"))
                logic->comp_type = COMP_NULL;
        else if(istring_eq(op, "NOT_NULL"))
                logic->comp_type = COMP_NOT_NULL;
}

LogicGroup* logicgroup_new(enum logicgroup_type type)
{
        LogicGroup* new_lg = NULL;
        malloc_(new_lg, sizeof(*new_lg));

        return logicgroup_construct(new_lg, type);
}

LogicGroup* logicgroup_construct(LogicGroup* lg, enum logicgroup_type type)
{
        *lg = (LogicGroup) {
                 type   /* type */
                ,{ 0 }  /* items */
                ,NULL   /* joinable */
                ,NULL   /* join_logic */
                ,NULL   /* condition */
        };

        vec_construct_(&lg->items, LogicGroup*);

        return lg;
}

void logicgroup_free(LogicGroup* lg)
{
        unsigned i = 0;
        for (; i < lg->items.size; ++i) {
                LogicGroup** lg_item = vec_at(&lg->items, i);
                logicgroup_free(*lg_item);
        }
        vec_destroy(&lg->items);
        free_(lg);
}

int logicgroup_eval(LogicGroup* lg, Vec* recs)
{
        LogicGroup** it = vec_begin(&lg->items);
        if (lg->type == LG_NOT && lg->condition != NULL) {
                return lg->condition->logic_fn(lg->condition, recs);
        }

        int ret = 0;
        for (; it != vec_end(&lg->items); ++it) {
                ret = logicgroup_eval(*it, recs);
                if (ret == 0 && lg->type == LG_AND) {
                        return 0;
                }
                if (ret == 1 && (*it)->type == LG_AND) {
                        return 1;
                }
                if (ret == FQL_FAIL) {
                        return FQL_FAIL;
                }
        }
        return ret;
}
