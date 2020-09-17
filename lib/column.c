#include "column.h"
#include "query.h"
#include "util/util.h"

column_t* column_new(expression_t* expr, const char* table_name)
{
        column_t* new_column = NULL;
        malloc_(new_column, sizeof(*new_column));

        *new_column = (column_t) {
                 NULL   /* table */
                ,NULL   /* data_source */
                ,expr   /* expression */
                ,""     /* alias */
                ,""     /* table_name */
                ,0      /* location */
                ,0      /* width */
        };

        strncpy_(new_column->table_name, table_name, TABLE_NAME_MAX);

        return new_column;
}

void column_free(void* generic_col)
{
        column_t* col = generic_col;       
        expression_free(col->expr);
        free_(col);
}

int column_try_assign_source(column_t* col, source_t* src)
{
        col->data_source = hmap_get(src->table->schema->col_map, col->alias);
        if (col->data_source) {
                return 1;
        }

        return 0;
}

