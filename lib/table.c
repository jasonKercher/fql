#include "query.h"
#include "column.h"

#include "util/util.h"

Table* table_new()
{
        Table* new_table = NULL;
        malloc_(new_table, sizeof(*new_table));

        return table_init(new_table);
}

Table* table_init(Table* table)
{
        *table = (Table) {
                 reader_new() /* reader */
                ,schema_new() /* schema */
                ,""           /* name */
        };

        return table;
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

        return source_init(new_source, table, alias, source_type, join_type);
}

Source* source_init(Source* src,
                    Table* table,
                    const char* alias,
                    enum source_type source_type,
                    enum join_type join_type)
{
        *src = (Source) {
                 table                  /* table */
                ,NULL                   /* condition */
                ,vec_new_(Column*)      /* validation_list */
                ,""                     /* alias */
                ,source_type            /* source_type */
                ,join_type              /* join_type */
        };

        if (alias[0] == '\0') {
                strncpy_(src->alias, table->name, TABLE_NAME_MAX);
        } else {
                strncpy_(src->alias, alias, TABLE_NAME_MAX);
        }

        return src;
}

void source_free(Source* source)
{
        if (source == NULL)
                return;

        vec_free(source->validation_list);
        table_free(source->table);
        free_(source);
}
