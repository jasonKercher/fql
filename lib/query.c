#include "query.h"

#include <stdbool.h>

#include "column.h"

#include "logic.h"
#include "util/dgraph.h"
#include "util/util.h"

Query* query_new()
{
        Query* new_query = NULL;
        malloc_(new_query, sizeof(*new_query));

        *new_query = (Query) {
                 schema_new()           /* table */
                ,vec_new()              /* sources */
                ,NULL                   /* where */
                ,NULL                   /* groups */
                ,NULL                   /* having */
                ,NULL                   /* limit */
                ,NULL                   /* operation */

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
        Query* query = generic_query;
        int i = 0;
        for (; i < query->sources->size; ++i) {
                source_free(query->sources->vector[i]);
        }
        free_(query->sources);

        logic_tree_free(query->where);

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
void query_add_source(Query* query,
                      Stack** source_stack,
                      const char* alias)
{
        Table* new_table = table_new();
        enum source_type type = SOURCE_SUBQUERY;

        char* table_name = stack_pop(source_stack);
        if (table_name != NULL) {
                type = SOURCE_TABLE;
                strncpy_(new_table->name, table_name, TABLE_NAME_MAX);
                free_(table_name);

                char* schema_name = stack_pop(source_stack);
                if (schema_name != NULL) {
                        strncpy_(new_table->schema->name
                                ,schema_name
                                ,TABLE_NAME_MAX);
                        free_(schema_name);
                }
        }

        stack_free_data(source_stack);

        vec_push_back(query->sources, source_new(new_table
                                                ,alias
                                                ,type
                                                ,query->join));
}

void query_apply_table_alias(Query* query, const char* alias)
{
        Source* source = vec_end(query->sources);
        strncpy_(source->alias, alias, TABLE_NAME_MAX);
}

/* Scroll below this comment at your own risk */

struct logic_builder {
        LogicTree* ltree;
        Dnode* current;
        Dnode* next_not;
        Vec* truths;
        Vec* falses;
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
                ,vec_new()              /* truths */
                ,vec_new()              /* falses */
                ,false                  /* sublogic_exit */
        };

        return builder;
}

void query_add_logic_column(Query* query,
                             Expression* expr,
                             const char* table_name)
{
        Column* new_col = column_new(expr, table_name);

        struct logic_builder* builder = query->logic_stack->data;
        logic_add_column(builder->current->data, new_col);

        Source* src = vec_end(query->sources);
        vec_push_back(src->logic_columns, new_col);
}

void query_set_logic_comparison(Query* query, const char* op)
{
        struct logic_builder* builder = query->logic_stack->data;
        logic_set_comparison(builder->current->data, op);
}

void _assign_logic(Query* query, struct logic_builder* builder)
{
        /* remove stranded nodes */
        Vec* nodes = builder->ltree->tree->nodes;
        int i = 0;
        for (; i < nodes->size; ++i) {
                Dnode* node = nodes->vector[i];
                Logic* logic = node->data;
                if (logic->comp_type == COMP_NOT_SET) {
                        vec_remove(nodes, i);
                        dnode_free(node);
                }
        }

        switch (query->logic_mode) {
        case LOGIC_JOIN: {
                Source* src = vec_end(query->sources);
                src->condition = builder->ltree;
                break;
        }
        case LOGIC_WHERE:
                query->where = builder->ltree;
                break;
        case LOGIC_UNDEFINED:
                /* TODO - report error */;
        }
}

void enter_search(Query* query)
{
        query->mode = MODE_SEARCH;

        struct logic_builder* upper = logic_builder_new();
        stack_push(&query->logic_stack, upper);

        if (query->logic_stack->next == NULL) {
                upper->next_not = dgraph_add_data(upper->ltree->tree, logic_new());

                Logic* new_false = logic_new();
                new_false->comp_type = COMP_FALSE;
                //upper->ltree->end_false = dnode_new(new_false);
                upper->ltree->end_false = dgraph_add_data(upper->ltree->tree, new_false);

                Logic* new_true = logic_new();
                new_true->comp_type = COMP_TRUE;
                upper->ltree->end_true = dgraph_add_data(upper->ltree->tree, new_true);
        } else {
                struct logic_builder* lower = query->logic_stack->next->data;
                dgraph_add_data(upper->ltree->tree, lower->current);
                upper->next_not = lower->current;
                upper->ltree->end_true = lower->next_not->data;
        }
}

void exit_search(Query* query)
{
        struct logic_builder* upper = stack_pop(&query->logic_stack);

        if (query->logic_stack == NULL) {
                int i = 0;
                for (i = 0; i < upper->falses->size; ++i) {
                        Dnode* node = upper->falses->vector[i];
                        node->out[false] = upper->ltree->end_false;
                }
                _assign_logic(query, upper);
                return;
        }

        struct logic_builder* lower = query->logic_stack->data;
        lower->current = upper->current;
        lower->sublogic_exit = true;

        vec_extend(lower->falses, upper->falses);
        vec_free(upper->falses);
}

void enter_search_and(Query* query)
{
        struct logic_builder* builder = query->logic_stack->data;
        int i = 0;
        for (; i < builder->falses->size; ++i) {
                Dnode* node = builder->falses->vector[i];
                node->out[false] = builder->next_not;
        }

        vec_resize(builder->falses, 0);
}

void exit_search_and(Query* query)
{
        struct logic_builder* builder = query->logic_stack->data;
        vec_push_back(builder->truths, builder->current);

        if (query->logic_stack->next != NULL) {
                struct logic_builder* lower = query->logic_stack->next->data;
                vec_extend(lower->truths, builder->truths);
                vec_resize(builder->truths, 0);
                return;
        }

        int i = 0;
        for (; i < builder->truths->size; ++i) {
                Dnode* truthy = builder->truths->vector[i];
                truthy->out[true] = builder->ltree->end_true;
        }
        vec_resize(builder->truths, 0);
}

void enter_search_not(Query* query)
{
        struct logic_builder* builder = query->logic_stack->data;
        if (builder->sublogic_exit) {
                builder->sublogic_exit = false;
                vec_resize(builder->truths, 0);
        }

        builder->current = builder->next_not;
        builder->next_not = dgraph_add_data(builder->ltree->tree, logic_new());
}

void exit_search_not(Query* query)
{
        struct logic_builder* builder = query->logic_stack->data;
        int i = 0;
        for (; i < builder->truths->size; ++i) {
                Dnode* truthy = builder->truths->vector[i];
                truthy->out[true] = builder->next_not;
        }
        if (builder->sublogic_exit) {
                return;
        }
        builder->current->out[true] = builder->next_not;
        vec_push_back(builder->falses, builder->current);
}
