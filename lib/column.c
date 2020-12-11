#include "column.h"
#include "query.h"
#include "process.h"
#include "util/util.h"

Column* column_new(Expression* expr, const char* table_name)
{
        Column* new_column = NULL;
        malloc_(new_column, sizeof(*new_column));

        return column_init(new_column, expr, table_name);
}

Column* column_init(Column* col, Expression* expr, const char* table_name)
{
        *col = (Column) {
                 COL_UNDEFINED  /* type */
                ,NULL           /* table */
                ,NULL           /* data_source */
                ,expr           /* expression */
                ,""             /* alias */
                ,""             /* table_name */
                ,0              /* location */
                ,0              /* width */
        };

        strncpy_(col->table_name, table_name, TABLE_NAME_MAX);

        /* hmmmm... */
        if (expr->type == EXPR_COLUMN_NAME) {
                strncpy_(col->alias, expr->data, COLUMN_NAME_MAX);
        }

        return col;
}

void column_free(void* generic_col)
{
        Column* col = generic_col;
        expression_free(col->expr);
        free_(col);
}

void column_cat_description(Column* col, String* msg)
{
        switch (col->expr->type) {
        case EXPR_COLUMN_NAME:
                string_cat(msg, col->expr->data);
                break;
        case EXPR_FUNCTION:
        {
                Function* func = col->expr->data;
                string_cat(msg, func->name);
                string_push_back(msg, '(');

                Column** it = vec_begin(func->args);
                for (; it != vec_end(func->args); ++it) {
                        if (it != vec_begin(func->args)) {
                                string_push_back(msg, ',');
                        }
                        column_cat_description(*it, msg);
                }
                string_push_back(msg, ')');
                break;
        }
        case EXPR_CONST:
                switch (col->type) {
                case COL_STRING:
                        string_cat(msg, col->expr->data);
                        break;
                case COL_INT:
                {
                        char buf[20];
                        sprintf(buf, "%ld", *(long*)col->expr->data);
                        string_cat(msg, buf);
                        break;
                }
                case COL_FLOAT:
                {
                        char buf[30];
                        sprintf(buf, "%lf", *(double*)col->expr->data);
                        string_cat(msg, buf);
                        break;
                }
                default:
                        string_cat(msg, "<<const>>");

                }
                break;
        case EXPR_SOURCE:
                string_cat(msg, "TABLE SOURCE");
                break;
        case EXPR_SUBQUERY:
                string_cat(msg, "SUBQUERY");
                break;
        case EXPR_SUBQUERY_CONST:
                string_cat(msg, "SUBQUERY CONST");
                break;
        case EXPR_NONE:
                string_cat(msg, "No expression");
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

