#include "query.h"

#include <stdbool.h>

#include "util/util.h"

struct query* query_new()
{
        struct query* new_query = NULL;
        malloc_(new_query, sizeof(*new_query));

        *new_query = (struct query) {
                 table_new()            /* table */
                ,vector_new()           /* sources */
                ,NULL                   /* where */
                ,NULL                   /* groups */
                ,NULL                   /* having */
                ,NULL                   /* limit */
                ,OP_NONE                /* operation */

                ,NULL                   /* begin search */
                ,NULL                   /* current_search */
                ,NULL                   /* not_stack */
                ,NULL                   /* and_stack */

                ,NULL                   /* expr */
                ,MODE_UNDEFINED         /* mode */
                ,SEARCH_UNDEFINED       /* search_mode */
                ,JOIN_FROM              /* join */
        };

        return new_query;
}

void query_free(void* generic_query)
{
        struct query* query = generic_query;
        int i = 0;
        for (; i < query->sources->size; ++i) {
                source_free(query->sources->data_vec[i]);
        }
        free_(query->sources);

        search_free_tree(query->where);

        queue_free_data(&query->groups);
        queue_free_data(&query->having);

        table_free(query->table);
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
void query_add_source(struct query* query,
                      struct stack** source_stack,
                      const char* alias)
{
        struct table* new_table = table_new();
        enum source_type type = SOURCE_SUBQUERY;

        char* table_name = stack_pop(source_stack);
        if (table_name != NULL) {
                type = SOURCE_TABLE;
                strncpy_(new_table->name, table_name, TABLE_NAME_MAX);
                free_(table_name);

                char* schema_name = stack_pop(source_stack);
                if (schema_name != NULL) {
                        strncpy_(new_table->schema->name,
                                 schema_name,
                                 TABLE_NAME_MAX);
                        free_(schema_name);
                }
        }

        stack_free_data(source_stack);

        vector_push_back(query->sources, source_new(new_table,
                                                    alias,
                                                    type,
                                                    query->join));
}

void query_apply_table_alias(struct query* query, const char* alias)
{
        struct source* source = vector_end(query->sources);
        strncpy_(source->alias, alias, TABLE_NAME_MAX);
}

void query_add_search_column(struct query* query,
                             struct expression* expr,
                             const char* table_name)
{
        search_add_column(query->current_search->data, expr, table_name);
}

void query_set_search_comparison(struct query* query, const char* op)
{
        search_set_comparison(query->current_search->data, op);
}

/** Search building **/
void enter_search(struct query* query)
{
        query->mode = MODE_SEARCH;
        stack_push(&query->and_stack, search_new());
        stack_push(&query->not_stack, search_new());

        if (query->current_search == NULL) {
                query->search_tree = search_tree_new();
        }

        stack_push(&query->current_search, NULL);
}

void exit_search(struct query* query)
{
        struct search* next_not = stack_pop(&query->not_stack);
        struct search* next_and = stack_pop(&query->and_stack);
        struct search* current = stack_pop(&query->current_search);

        if (query->not_stack == NULL) {
                /* redundant? */
                current->out[true] = next_not;
                current->out[false] = next_and;

                query->search_tree->end_true = next_not;
                query->search_tree->end_false = next_and;

                next_not->comp_type = COMP_TRUE;
                next_and->comp_type = COMP_FALSE;

                switch (query->search_mode) {
                case SEARCH_JOIN: {
                        struct source* src = vector_end(query->sources);
                        src->condition = query->search_tree;
                        break;
                }
                case SEARCH_WHERE:
                        query->where = query->search_tree;
                        break;
                case SEARCH_UNDEFINED:
                        /* TODO - report error */;
                }
                return;
        }
        search_free(next_not);
        search_free(next_and);
}

void enter_search_and(struct query* query)
{
        struct search* next_and = query->and_stack->data;
        struct search* next_not = query->not_stack->data;
        next_not->out[false] = next_and;
}

void exit_search_and(struct query* query)
{
        struct search* current = query->current_search->data;
        if (query->not_stack->next == NULL) {
                return;
        }
        current->out[true] = query->not_stack->next->data;
}

void enter_search_not(struct query* query)
{
        /* If our stack count == 1 and data not set yet */
        if (query->current_search->next == NULL && 
            query->current_search->data == NULL) {
                query->search_tree->begin = query->not_stack->data;
        }

        query->current_search->data = query->not_stack->data;
        query->not_stack->data = search_new();
}

void exit_search_not(struct query* query)
{
        struct search* current = query->current_search->data;
        current->out[true] = query->not_stack->data;
}
