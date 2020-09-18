#include "query.h"
#include "column.h"

#include "util/util.h"

struct table* table_new()
{
        struct table* new_table = NULL;
        malloc_(new_table, sizeof(*new_table));

        *new_table = (struct table) {
                 reader_new() /* reader */
                ,schema_new() /* schema */
                ,""           /* name */
        };

        return new_table;
}

void table_free(struct table* table)
{
        if (table == NULL)
                return;

        reader_free(table->reader);
        schema_free(table->schema);
        free_(table);
}

void table_add_column(struct table* table,
                      struct expression* expr,
                      const char* table_name)
{
        struct column* new_col = column_new(expr, table_name);
        if (expr->type == EXPR_COLUMN_NAME) {
                strncpy_(new_col->alias, expr->expr, COLUMN_NAME_MAX);
        }
        stack_push(&table->schema->columns, new_col);
}

void table_apply_column_alias(struct table* table, const char* alias)
{
        struct column* col = table->schema->columns->data;
        strncpy_(col->alias, alias, COLUMN_NAME_MAX);
}




struct source* source_new(struct table* table, enum source_type type)
{
        struct source* new_source = NULL;
        malloc_(new_source, sizeof(*new_source));

        *new_source = (struct source) {
                 table          /* table */
                ,type           /* source_type */
                ,JOIN_FROM      /* join_type */
                ,""             /* alias */
        };

        return new_source;
}

void source_free(struct source* source)
{
        if (source == NULL)
                return;

        table_free(source->table);
        free_(source);
}



