#include "column.h"
#include "query.h"
#include "util/util.h"

Column* column_new(Expression* expr, const char* table_name)
{
        Column* new_column = NULL;
        malloc_(new_column, sizeof(*new_column));

        *new_column = (Column) {
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
        Column* col = generic_col;
        expression_free(col->expr);
        free_(col);
}

void column_cat_description(Column* col, char* msg)
{
        switch (col->expr->type) {
        case EXPR_COLUMN_NAME:
                strcat(msg, col->expr->expr);
                break;
        case EXPR_CONST:
                strcat(msg, "CONST EXPRESSION");
                break;
        case EXPR_SOURCE:
                strcat(msg, "TABLE SOURCE");
                break;
        case EXPR_SUBQUERY:
                strcat(msg, "SUBQUERY");
                break;
        case EXPR_SUBQUERY_CONST:
                strcat(msg, "SUBQUERY CONST");
                break;
        case EXPR_NONE:
                strcat(msg, "No expression");
                break;
        }
}

int column_try_assign_source(Column* col, Source* src)
{
        col->data_source = hmap_get(src->table->schema->col_map, col->alias);
        if (col->data_source) {
                return 1;
        }

        return 0;
}

