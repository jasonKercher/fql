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
    //if(comp == "=")
    //    set_comparison(COMP_TYPE_EQ);
    //else if(comp == "<>" || comp == "!=")
    //    set_comparison(COMP_TYPE_NE);
    //else if(comp == ">")
    //    set_comparison(COMP_TYPE_GT);
    //else if(comp == ">=")
    //    set_comparison(COMP_TYPE_GE);
    //else if(comp == "<")
    //    set_comparison(COMP_TYPE_LT);
    //else if(comp == "<=")
    //    set_comparison(COMP_TYPE_LE);
    //else if(comp == "LIKE")
    //    set_comparison(COMP_TYPE_LIKE);
    //else if(comp == "NOT_LIKE")
    //    set_comparison(COMP_TYPE_NOT_LIKE);
    //else if(comp == "NULL")
    //    set_comparison(COMP_TYPE_NULL);
    //else if(comp == "NOT_NULL")
    //    set_comparison(COMP_TYPE_NOT_NULL);
}
