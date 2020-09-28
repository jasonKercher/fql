#include "search.h"

#include "column.h"
#include "util/util.h"

struct search* search_new()
{
        struct search* new_search = NULL;
        malloc_(new_search, sizeof(*new_search));

        *new_search = (struct search) {
                 {NULL, NULL}
                ,{NULL, NULL}
                ,0
                ,COMP_NOT_SET
        };

        return new_search;
}

void search_free(struct search* search)
{
        free_(search);
}

void search_get_description(struct search* search, char* msg)
{
        if (search->col[0] == NULL) {
                return;
        }

        column_cat_description(search->col[0], msg);
        switch (search->comp_type) {
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
        column_cat_description(search->col[1], msg);
}

void search_add_column(struct search* search,
                       struct expression* expr,
                       const char* table_name)
{
        struct column* new_col = column_new(expr, table_name);
        if (search->col[0] == NULL) {
                search->col[0] = new_col;
        } else {
                search->col[1] = new_col;
        }
}

void search_set_comparison(struct search* search, const char* op)
{
        if(string_eq(op, "="))
                search->comp_type = COMP_EQ;
        else if(string_eq(op, "<>") || string_eq(op, "!="))
                search->comp_type = COMP_NE;
        else if(string_eq(op, ">"))
                search->comp_type = COMP_GT;
        else if(string_eq(op, ">="))
                search->comp_type = COMP_GE;
        else if(string_eq(op, "<"))
                search->comp_type = COMP_LT;
        else if(string_eq(op, "<="))
                search->comp_type = COMP_LE;
        else if(istring_eq(op, "LIKE"))
                search->comp_type = COMP_LIKE;
        else if(istring_eq(op, "NOT_LIKE"))
                search->comp_type = COMP_NOT_LIKE;
        else if(istring_eq(op, "NULL"))
                search->comp_type = COMP_NULL;
        else if(istring_eq(op, "NOT_NULL"))
                search->comp_type = COMP_NOT_NULL;
}

struct search_tree* search_tree_new()
{
        struct search_tree* new_tree = NULL;
        malloc_(new_tree, sizeof(*new_tree));

        *new_tree = (struct search_tree) {
                 NULL
                ,search_new()
                ,NULL
        };

        new_tree->end_true->comp_type = COMP_TRUE;

        return new_tree;
}

void search_free_tree(struct search_tree* tree)
{
        /* TODO - recursively free search tree */

        free_(tree);
}

