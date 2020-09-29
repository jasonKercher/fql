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

/* Everything below is involved in building logic trees */

struct logic_builder {
        struct logic_tree* tree;
        struct logic* back_one;
        struct logic* current;
        struct logic* next_and;
        struct logic* next_not;
};

struct logic_builder* logic_builder_new()
{
        struct logic_builder* builder = NULL;
        malloc_(builder, sizeof(*builder));

        *builder = (struct logic_builder) {
                 logic_tree_new()       /* tree */
                ,NULL                   /* back_one */
                ,NULL                   /* current */
                ,NULL                   /* next_and */
                ,logic_new()            /* next_not */
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

void enter_search(struct query* query)
{
        query->mode = MODE_SEARCH;

        struct logic_builder* upper = logic_builder_new();
        stack_push(&query->logic_stack, upper);

        upper->tree->begin = upper->next_not;

        if (query->logic_stack->next == NULL) {
                return;
        }
        struct logic_builder* lower = query->logic_stack->next->data;

        logic_free(lower->next_not);
        //lower->next_not = lower->current;
        lower->current = lower->back_one;

        /* TODO: Is this better in exit_search? */
        /* IF: a && (...) ELSE: a || (...) */

        /* THIS WHOLE SECTION IS FUCKED */

        //if (lower->current->out[true] == NULL) {
        ////if (lower->in_and) {
        //        lower->next_not = upper->tree->begin;
        //        lower->current->out[true] = lower->next_not;
        //} else {
        //        lower->next_and = upper->tree->begin;
        //        lower->current->out[false] = lower->next_and;
        //}

        //if (builder->current == NULL) {
        //        query->logic_tree = logic_tree_new();
        //} else {
        //        struct logic* current = query->current_logic->data;
        //        if (current->out[true] == NULL) {
        //                current->out[true] = query->not_stack->data;
        //        } else {
        //                current->out[false] = query->not_stack->data;
        //        }
        //}

}

void exit_search(struct query* query)
{
        //struct logic* upper_not = stack_pop(&query->not_stack);
        //struct logic* upper_and = stack_pop(&query->and_stack);
        //struct logic* upper_current = stack_pop(&query->current_logic);

        struct logic_builder* upper = stack_pop(&query->logic_stack);

        //if (query->logic_stack == NULL) {
                //upper_current->out[true] = upper_not;
        //upper->current->out[false] = upper->next_and;


        //upper->next_not->out[false] = upper->next_and;
        if (query->logic_stack != NULL) {
                struct logic_builder* lower = query->logic_stack->data;
                //upper->next_not->out[true] = lower->tree->end_true;
                upper->current->out[false] = lower->next_and;
                lower->current = upper->tree->begin;
                return;
        }

        upper->current->out[false] = upper->next_and;
        upper->next_and->comp_type = COMP_FALSE;
        upper->tree->end_false = upper->next_and;

        //query->logic_tree->end_true = upper_not;
        //query->logic_tree->end_false = upper_and;

        //upper_not->comp_type = COMP_TRUE;

        switch (query->logic_mode) {
        case LOGIC_JOIN: {
                struct source* src = vector_end(query->sources);
                src->condition = upper->tree;
                break;
        }
        case LOGIC_WHERE:
                query->where = upper->tree;
                break;
        case LOGIC_UNDEFINED:
                /* TODO - report error */;
        }
        return;
        //}

        //query->current_logic->data = query->not_stack->data;

        //logic_free(upper_not);
        //logic_free(upper_and);
}

void enter_search_and(struct query* query)
{
        //if (query->and_stack->data != NULL) {
        //        query->current_logic->data = query->and_stack->data;
        //}
        struct logic_builder* builder = query->logic_stack->data;
        builder->next_and = logic_new();
        //struct logic* next_and = query->and_stack->data;
        //struct logic* next_not = query->not_stack->data;
        //struct logic* current = query->current_logic->data;
        //if (builder->current != NULL) {
        //        builder->current->out[false] = builder->next_not;
        //}
        builder->next_not->out[false] = builder->next_and;
}

void exit_search_and(struct query* query)
{
        struct logic_builder* builder = query->logic_stack->data;
        builder->current->out[false] = builder->next_not;
        if (query->logic_stack->next == NULL) {
                builder->current->out[true] = builder->tree->end_true;
                return;
        }
        struct logic_builder* lower = query->logic_stack->next->data;
        builder->current->out[true] = lower->next_not;
}

void enter_search_not(struct query* query)
{
        struct logic_builder* builder = query->logic_stack->data;
        ///* If our stack count == 1 and data not set yet */
        //if (query->logic_stack->next == NULL &&
        //    builder->current == NULL) {
        //        builder->tree->begin = builder->next_not;
        //}

        builder->back_one = builder->current;
        builder->current = builder->next_not;
        builder->next_not = logic_new();
}

void exit_search_not(struct query* query)
{
        struct logic_builder* builder = query->logic_stack->data;
        if (builder->current->out[true] != NULL) {
                return;
        }
        builder->current->out[true] = builder->next_not;
        builder->current->out[false] = builder->next_and;
}
