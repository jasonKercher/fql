#include "column.h"
#include "query.h"
#include "util/util.h"

struct column* column_new(struct expression* expr, const char* table_name)
{
        struct column* new_column = NULL;
        malloc_(new_column, sizeof(*new_column));

        *new_column = (struct column) {
                 NULL   /* table */
                ,NULL   /* data_source */
                ,expr   /* expression */
                ,""     /* alias */
                ,""     /* table_name */
                ,0      /* location */
                ,0      /* width */
        };

        strncpy_(new_column->table_name, table_name, TABLE_NAME_MAX);
        if (expr->type == EXPR_COLUMN_NAME) {
                strncpy_(new_column->alias, expr->expr, COLUMN_NAME_MAX);
        }

        return new_column;
}

void column_free(void* generic_col)
{
        struct column* col = generic_col;       
        expression_free(col->expr);
        free_(col);
}

int column_try_assign_source(struct column* col, struct source* src)
{
        col->data_source = hmap_get(src->table->schema->col_map, col->alias);
        if (col->data_source) {
                return 1;
        }

        return 0;
}

