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

/* TODO: The silly use of strncat_ and maxlen here
 *       should be replaced by vec(char). We can
 *       easily overflow here in any strcat. Fixing
 *       this without vec would be impractical.
 */
void column_cat_description(Column* col, char* msg)
{
        int maxlen = ACTION_MAX - strlen(msg);
        if (maxlen <= 2) {
                return;
        }
        switch (col->expr->type) {
        case EXPR_COLUMN_NAME:
                strncat_(msg, col->expr->data, maxlen);
                break;
        case EXPR_FUNCTION:
        {
                Function* func = col->expr->data;
                strncat_(msg, func->name, maxlen);
                strcat(msg, "(");

                Column** it = vec_begin(func->args);
                for (; it != vec_end(func->args); ++it) {
                        if (it != vec_begin(func->args)) {
                                strcat(msg, ",");
                        }
                        column_cat_description(*it, msg);
                        if (strlen(msg) - ACTION_MAX <= 2) {
                                return;
                        }
                }
                strcat(msg, ")");
                break;
        }
        case EXPR_CONST:
                switch (col->type) {
                case COL_STRING:
                        strncat_(msg, col->expr->data, maxlen);
                        break;
                case COL_INT:
                {
                        char buf[20];
                        sprintf(buf, "%ld", *(long*)col->expr->data);
                        strncat_(msg, buf, maxlen);
                        break;
                }
                case COL_FLOAT:
                {
                        char buf[30];
                        sprintf(buf, "%lf", *(double*)col->expr->data);
                        strncat_(msg, buf, maxlen);
                        break;
                }
                default:
                        strncat_(msg, "<<const>>", maxlen);

                }
                break;
        case EXPR_SOURCE:
                strncat_(msg, "TABLE SOURCE", maxlen);
                break;
        case EXPR_SUBQUERY:
                strncat_(msg, "SUBQUERY", maxlen);
                break;
        case EXPR_SUBQUERY_CONST:
                strncat_(msg, "SUBQUERY CONST", maxlen);
                break;
        case EXPR_NONE:
                strncat_(msg, "No expression", maxlen);
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

