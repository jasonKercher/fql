#include "query.h"

#include "util/util.h"

table_t* table_new()
{
        table_t* new_table = NULL;
        malloc_(new_table, sizeof(*new_table));

        *new_table = (table_t) {
                 NULL   /* source */
                ,NULL   /* column queue */
                ,""     /* name */
                ,""     /* alias */
        };

        return new_table;
}

void table_add_column(table_t* table, expression_t* expr)
{
        queue_enqueue(&table->columns, expr);
}



