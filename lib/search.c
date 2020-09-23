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

void search_add_column(struct search* search,
                       struct expression* expr,
                       const char* table_name)
{
        struct column* new_col = column_new(expr, table_name);
        if (search->expr[0] == NULL) {
                search->expr[0] = new_col;
        } else {
                search->expr[1] = new_col;
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
