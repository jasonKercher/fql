#include "query.h"

#include "util/util.h"

table_t* table_new()
{
        table_t* new_table = NULL;
        malloc_(new_table, sizeof(*new_table));

        *new_table = (table_t) {
                 NULL   /* source */
                ,NULL   /* column list */
                ,""     /* name */
                ,""     /* alias */
        };

        return new_table;
}



