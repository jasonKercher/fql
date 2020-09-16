#include "query.h"

#include "util/util.h"

table_t* table_new()
{
        table_t* new_table = NULL;
        malloc_(new_table, sizeof(*new_table));

        *new_table = (table_t) {
                 NULL   /* source */
                ,NULL   /* column queue */
                ,""     /* schema */
                ,""     /* name */
        };

        return new_table;
}

void table_free(table_t* table)
{
        if (table == NULL)
                return;

        stack_free_func(&table->columns, &column_free);
        free_(table);
}

void table_add_column(table_t* table,
                      expression_t* expr,
                      const char* table_name)
{
        stack_push(&table->columns,
                   column_new(expr, table_name));
}

void table_apply_column_alias(table_t* table, const char* alias)
{
        column_t* col = table->columns->data;
        strncpy_(col->alias, alias, COLUMN_NAME_MAX);
}




source_t* source_new(table_t* table, enum source type)
{
        source_t* new_source = NULL;
        malloc_(new_source, sizeof(*new_source));

        *new_source = (source_t) {
                 table          /* table */
                ,type           /* source_type */
                ,JOIN_FROM      /* join_type */
                ,""             /* alias */
        };

        return new_source;
}

void source_free(source_t* source)
{
        if (source == NULL)
                return;

        table_free(source->table);
        free_(source);
}



