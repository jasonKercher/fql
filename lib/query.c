/*** VERSION 1 ***/

#include "query.h"

#include <stdbool.h>

#include "util/util.h"

struct query* query_new()
{
        struct query* new_query = NULL;
        malloc_(new_query, sizeof(*new_query));

        *new_query = (struct query) {
                 schema_new()           /* table */
                ,vector_new()           /* sources */
                ,NULL                   /* where */
                ,NULL                   /* groups */
                ,NULL                   /* having */
                ,NULL                   /* limit */
                ,OP_NONE                /* operation */

                ,NULL                   /* logic_stack */
                ,NULL                   /* expr */
                ,MODE_UNDEFINED         /* mode */
                ,LOGIC_UNDEFINED        /* logic_mode */
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

        logic_free_tree(query->where);

        queue_free_data(&query->groups);
        queue_free_data(&query->having);

        //table_free(query->table);
        schema_free(query->schema);
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

/* Scroll below this comment at your own risk */

struct logic_builder {
        struct logic_tree* tree;
        struct logic* current;
        struct logic* next_not;
        struct vector* truths;
        struct vector* falses;
        _Bool sublogic_exit;
};

struct logic_builder* logic_builder_new()
{
        struct logic_builder* builder = NULL;
        malloc_(builder, sizeof(*builder));

        *builder = (struct logic_builder) {
                 logic_tree_new()       /* tree */
                ,NULL                   /* current */
                ,NULL                   /* next_not */
                ,vector_new()           /* truths */
                ,vector_new()           /* falses */
                ,false                  /* sublogic_exit */
        };

        return builder;
}

void query_add_search_column(struct query* query,
                             struct expression* expr,
                             const char* table_name)
{
        struct logic_builder* builder = query->logic_stack->data;
        logic_add_column(builder->current, expr, table_name);
}

void query_set_search_comparison(struct query* query, const char* op)
{
        struct logic_builder* builder = query->logic_stack->data;
        logic_set_comparison(builder->current, op);
}

void _assign_logic(struct query* query, struct logic_builder* builder)
{
        switch (query->logic_mode) {
        case LOGIC_JOIN: {
                struct source* src = vector_end(query->sources);
                src->condition = builder->tree;
                break;
        }
        case LOGIC_WHERE:
                query->where = builder->tree;
                break;
        case LOGIC_UNDEFINED:
                /* TODO - report error */;
        }
}

void enter_search(struct query* query)
{
        query->mode = MODE_SEARCH;

        struct logic_builder* upper = logic_builder_new();
        stack_push(&query->logic_stack, upper);

        if (query->logic_stack->next == NULL) {
                upper->next_not = logic_new();
                upper->tree->begin = upper->next_not;
                upper->tree->end_false = logic_new();
                upper->tree->end_false->comp_type = COMP_FALSE;
                upper->tree->end_true = logic_new();
                upper->tree->end_true->comp_type = COMP_TRUE;
        } else {
                struct logic_builder* lower = query->logic_stack->next->data;
                upper->tree->begin = lower->current;
                upper->next_not = lower->current;
                upper->tree->end_true = lower->next_not;
        }
}

void exit_search(struct query* query)
{
        struct logic_builder* upper = stack_pop(&query->logic_stack);

        if (query->logic_stack == NULL) {
                int i = 0;
                for (i = 0; i < upper->falses->size; ++i) {
                        struct logic* logic = upper->falses->data_vec[i];
                        logic->out[false] = upper->tree->end_false;
                }
                _assign_logic(query, upper);
                return;
        }
        
        struct logic_builder* lower = query->logic_stack->data;
        lower->current = upper->current;
        lower->sublogic_exit = true;

        vector_extend(lower->falses, upper->falses);
        vector_free(upper->falses);
}

void enter_search_and(struct query* query)
{
        struct logic_builder* builder = query->logic_stack->data;
        int i = 0;
        for (; i < builder->falses->size; ++i) {
                struct logic* logic = builder->falses->data_vec[i];
                logic->out[false] = builder->next_not;
        }

        vector_resize(builder->falses, 0);
}

void exit_search_and(struct query* query)
{
        struct logic_builder* builder = query->logic_stack->data;
        vector_push_back(builder->truths, builder->current);
        
        if (query->logic_stack->next != NULL) {
                struct logic_builder* lower = query->logic_stack->next->data;
                vector_extend(lower->truths, builder->truths);
                vector_resize(builder->truths, 0);
                return;
        }

        int i = 0;
        for (; i < builder->truths->size; ++i) {
                struct logic* truthy = builder->truths->data_vec[i];
                truthy->out[true] = builder->tree->end_true;
        }
        vector_resize(builder->truths, 0);
}

void enter_search_not(struct query* query)
{
        struct logic_builder* builder = query->logic_stack->data;
        if (builder->sublogic_exit) {
                builder->sublogic_exit = false;
                vector_resize(builder->truths, 0);
        }

        builder->current = builder->next_not;
        builder->next_not = logic_new();
}

void exit_search_not(struct query* query)
{
        struct logic_builder* builder = query->logic_stack->data;
        int i = 0;
        for (; i < builder->truths->size; ++i) {
                struct logic* truthy = builder->truths->data_vec[i];
                truthy->out[true] = builder->next_not;
        }
        if (builder->sublogic_exit) {
                return;
        }
        builder->current->out[true] = builder->next_not;
        vector_push_back(builder->falses, builder->current);
}
