#include "logic.h"

#include "column.h"
#include "util/util.h"
#include <string.h>

Logic* logic_new()
{
        Logic* new_logic = NULL;
        malloc_(new_logic, sizeof(*new_logic));

        return logic_init(new_logic);
}

Logic* logic_init(Logic* logic)
{
        *logic = (Logic) {
                 {NULL, NULL}   /* col */
                ,0              /* data_type */
                ,COMP_NOT_SET   /* comp_type */
                ,NULL           /* proc */
        };

        return logic;
}

void logic_free(Logic* logic)
{
        free_(logic);
}

void logic_assign_process(Logic* logic, Process* proc)
{
        if (logic->comp_type == COMP_TRUE) {
                strcpy(proc->action_msg, "End Logic: TRUE");
                proc->is_passive = true;
                return;
        }
        if (logic->comp_type == COMP_FALSE) {
                strcpy(proc->action_msg, "End Logic: FALSE");
                proc->is_passive = true;
                return;
        }
        if (logic->col[0] == NULL) {
                return;
        }

        column_cat_description(logic->col[0], proc->action_msg);
        switch (logic->comp_type) {
        case COMP_EQ:
                strcat(proc->action_msg, " = ");
                break;
        case COMP_NE:
                strcat(proc->action_msg, " != ");
                break;
        case COMP_GT:
                strcat(proc->action_msg, " > ");
                break;
        case COMP_GE:
                strcat(proc->action_msg, " >= ");
                break;
        case COMP_LT:
                strcat(proc->action_msg, " < ");
                break;
        case COMP_LE:
                strcat(proc->action_msg, " <= ");
                break;
        case COMP_LIKE:
                strcat(proc->action_msg, " LIKE ");
                break;
        case COMP_NOT_LIKE:
                strcat(proc->action_msg, " NOT LIKE ");
                break;
        case COMP_NULL:
                strcat(proc->action_msg, " NULL ");
                break;
        case COMP_NOT_NULL:
                strcat(proc->action_msg, " NOT NULL ");
                break;
        case COMP_NOT_SET:
                strcat(proc->action_msg, " <no comparison> ");
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
        } else {
                logic->col[1] = col;
        }
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

LogicTree* logic_tree_new()
{
        LogicTree* new_tree = NULL;
        malloc_(new_tree, sizeof(*new_tree));

        *new_tree = (LogicTree) {
                 dgraph_new()    /* tree */
                ,NULL            /* end_true */
                ,NULL            /* end_false */
        };

        //new_tree->end_true->comp_type = COMP_TRUE

        return new_tree;
}

void logic_tree_free(LogicTree* tree)
{
        /* TODO - recursively free logic tree */

        free_(tree);
}

