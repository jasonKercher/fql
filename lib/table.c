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



struct source* source_new(struct table* table,
                          const char* alias,
                          enum source_type source_type,
                          enum join_type join_type)
{
        struct source* new_source = NULL;
        malloc_(new_source, sizeof(*new_source));

        *new_source = (struct source) {
                 table          /* table */
                ,NULL           /* condition */
                ,""             /* alias */
                ,source_type    /* source_type */
                ,join_type      /* join_type */
        };

        if (alias[0] == '\0') {
                strncpy_(new_source->alias, table->name, TABLE_NAME_MAX);
        } else {
                strncpy_(new_source->alias, alias, TABLE_NAME_MAX);
        }

        return new_source;
}

void source_free(struct source* source)
{
        if (source == NULL)
                return;

        table_free(source->table);
        free_(source);
}



