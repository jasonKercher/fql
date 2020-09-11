#include "query.h"

#include "util/util.h"

column_t* column_new(expression_t* expr, const char* table_name)
{
        column_t* new_column = NULL;
        malloc_(new_column, sizeof(*new_column));

        *new_column = (column_t) {
                 NULL   /* table */
                ,expr   /* expression */
                ,""     /* alias */
                ,""     /* table_name */
                ,0      /* location */
                ,0      /* width */
        };

        strncpy_(new_column->table_name, table_name, TABLE_NAME_MAX);

        return new_column;
}


