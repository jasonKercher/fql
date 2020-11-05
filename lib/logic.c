#include "logic.h"

#include "column.h"
#include "util/util.h"

struct logic* logic_new()
{
        struct logic* new_logic = NULL;
        malloc_(new_logic, sizeof(*new_logic));

        *new_logic = (struct logic) {
                 {NULL, NULL}   /* col */
                ,0              /* data_type */
                ,COMP_NOT_SET   /* comp_type */
                ,NULL           /* proc */
        };

        return new_logic;
}

void logic_free(struct logic* logic)
{
        free_(logic);
}

void logic_get_description(struct logic* logic, char* msg)
{
        if (logic->col[0] == NULL) {
                return;
        }

        column_cat_description(logic->col[0], msg);
        switch (logic->comp_type) {
        case COMP_EQ:
                strcat(msg, " = ");
                break;
        case COMP_NE:
                strcat(msg, " != ");
                break;
        case COMP_GT:
                strcat(msg, " > ");
                break;
        case COMP_GE:
                strcat(msg, " >= ");
                break;
        case COMP_LT:
                strcat(msg, " < ");
                break;
        case COMP_LE:
                strcat(msg, " <= ");
                break;
        case COMP_LIKE:
                strcat(msg, " LIKE ");
                break;
        case COMP_NOT_LIKE:
                strcat(msg, " NOT LIKE ");
                break;
        case COMP_NULL:
                strcat(msg, " NULL ");
                break;
        case COMP_NOT_NULL:
                strcat(msg, " NOT NULL ");
                break;
        case COMP_NOT_SET:
                strcat(msg, " <no comparison> ");
                break;
        default:
                break;
        }
        column_cat_description(logic->col[1], msg);
}

void logic_add_column(struct logic* logic, struct column* col)
{
        if (logic->col[0] == NULL) {
                logic->col[0] = col;
        } else {
                logic->col[1] = col;
        }
}

void logic_set_comparison(struct logic* logic, const char* op)
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

struct logic_tree* logic_tree_new()
{
        struct logic_tree* new_tree = NULL;
        malloc_(new_tree, sizeof(*new_tree));

        *new_tree = (struct logic_tree) {
                 dtree_new()    /* tree */
                ,NULL           /* end_true */
                ,NULL           /* end_false */
        };

        //new_tree->end_true->comp_type = COMP_TRUE

        return new_tree;
}

void logic_tree_free(struct logic_tree* tree)
{
        /* TODO - recursively free logic tree */

        free_(tree);
}

