#include "query.h"
#include "column.h"

#include "util/util.h"

Table* table_new()
{
        Table* new_table = NULL;
        malloc_(new_table, sizeof(*new_table));

        return table_construct(new_table);
}

Table* table_construct(Table* table)
{
        *table = (Table) {
                 reader_new() /* reader */
                ,schema_new() /* schema */
                ,{ 0 }        /* name */
        };

        string_construct(&table->name);

        return table;
}

void table_free(Table* table)
{
        if (table == NULL)
                return;

        string_destroy(&table->name);
        reader_free(table->reader);
        schema_free(table->schema);
        free_(table);
}



Source* source_new(Table* table,
                   const char* alias,
                   size_t idx,
                   enum source_type source_type,
                   enum join_type join_type)
{
        Source* new_source = NULL;
        malloc_(new_source, sizeof(*new_source));

        return source_construct(new_source, table, alias, idx, source_type, join_type);
}

Source* source_construct(Source* src,
                         Table* table,
                         const char* alias,
                         size_t idx,
                         enum source_type source_type,
                         enum join_type join_type)
{
        *src = (Source) {
                 table                  /* table */
                ,NULL                   /* condition */
                ,vec_new_(Column*)      /* validation_list */
                ,{ 0 }                  /* alias */
                ,idx
                ,source_type            /* source_type */
                ,join_type              /* join_type */

        };

        if (alias[0] == '\0') {
                string_construct_from_string(&src->alias, &table->name);
        } else {
                string_construct_from_char_ptr(&src->alias, alias);
        }

        return src;
}

void source_free(Source* source)
{
        source_destroy(source);
        free_(source);
}

void source_destroy(Source* source)
{
        Column** it = vec_begin(source->validation_list);
        for (; it != vec_end(source->validation_list); ++it) {
                column_free(*it);
        }
        string_destroy(&source->alias);
        vec_free(source->validation_list);
        table_free(source->table);
}
