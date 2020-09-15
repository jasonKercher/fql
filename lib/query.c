#include "query.h"

#include "util/util.h"

query_t* query_new()
{
        query_t* new_query = NULL;
        malloc_(new_query, sizeof(*new_query));

        *new_query = (query_t) {
                 table_new()            /* table */
                ,NULL                   /* sources */
                ,NULL                   /* conditions */
                ,NULL                   /* groups */
                ,NULL                   /* having */
                ,NULL                   /* limit */
                ,OP_NONE                /* operation */

                ,NULL                   /* expr */
                ,MODE_UNDEFINED         /* mode */
                ,JOIN_FROM              /* join */
        };

        return new_query;
}

void query_free(query_t* query)
{
        stack_t* s_item = query->sources;
        for (; s_item; s_item = s_item->next) {
                source_free(s_item->data);
        }
        free_(query->sources);

        queue_free_data(&query->conditions);
        queue_free_data(&query->groups);
        queue_free_data(&query->having);

        free_(query->table);
        free_(query->limit);
        free_(query->expr);
        free_(query);
}

/**
 * Create new table and source object
 * Assign name and schema if provided. If not provided,
 * we are dealing with a subquery source. source_stack
 * is a stack of allocated char* of the form:
 * object->schema->database->server
 * We ignore database and server for now.
 */

void query_add_source(query_t* query, stack_t* source_stack)
{
        table_t* new_table = table_new();
        enum source type = SOURCE_SUBQUERY;

        char* source = stack_pop(&source_stack);
        if (source != NULL) {
                type = SOURCE_TABLE;
                strncpy_(new_table->name, source, TABLE_NAME_MAX);
                free_(source);

                source = stack_pop(&source_stack);
                if (source != NULL) {
                        strncpy_(new_table->schema, source, TABLE_NAME_MAX);
                        free_(source);
                }
        }

        stack_free_data(&source_stack);

        stack_push(&query->sources, source_new(new_table, type));
}

void query_apply_table_alias(query_t * query, const char * alias)
{
        source_t* source = query->sources->data;
        strncpy_(source->alias, alias, TABLE_NAME_MAX);
}
