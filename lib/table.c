#include "query.h"
#include "column.h"

#include "util/util.h"

Table* table_new()
{
        Table* new_table = NULL;
        malloc_(new_table, sizeof(*new_table));

        *new_table = (Table) {
                 reader_new() /* reader */
                ,schema_new() /* schema */
                ,""           /* name */
        };

        return new_table;
}

void table_free(Table* table)
{
        if (table == NULL)
                return;

        reader_free(table->reader);
        schema_free(table->schema);
        free_(table);
}



Source* source_new(Table* table,
                          const char* alias,
                          enum source_type source_type,
                          enum join_type join_type)
{
        Source* new_source = NULL;
        malloc_(new_source, sizeof(*new_source));

        *new_source = (Source) {
                 table          /* table */
                ,NULL           /* condition */
                ,vec_new()      /* logic_columns */
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

void source_free(Source* source)
{
        if (source == NULL)
                return;

        vec_free(source->logic_columns);
        table_free(source->table);
        free_(source);
}



