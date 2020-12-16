#include "query.h"

#include <stdbool.h>

#include "fql.h"
#include "logic.h"
#include "select.h"
#include "util/dgraph.h"
#include "util/util.h"

struct logic_builder {
        LogicTree* ltree;
        Dnode* current;
        Dnode* next_not;
        Vec* truths;
        Vec* falses;
        _Bool sublogic_exit;
};


Query* query_new()
{
        Query* new_query = NULL;
        malloc_(new_query, sizeof(*new_query));

        return query_init(new_query);
}

Query* query_init(Query* query)
{
        *query = (Query) {
                 schema_new()           /* table */
                ,vec_new_(Source)       /* sources */
                ,NULL                   /* where */
                ,vec_new_(Column*)      /* groups */
                ,NULL                   /* having */
                ,NULL                   /* limit */
                ,NULL                   /* operation */

                ,NULL                   /* logic_stack */
                ,NULL                   /* function_stack */
                ,NULL                   /* expr */
                ,MODE_UNDEFINED         /* mode */
                ,LOGIC_UNDEFINED        /* logic_mode */
                ,JOIN_FROM              /* join */
        };

        return query;
}

void query_free(void* generic_query)
{
        Query* query = generic_query;

        /* TODO: leaking here */
        vec_free(query->sources);

        logic_tree_free(query->where);

        /* TODO: leaking here */
        vec_free(query->groups);
        queue_free_data(&query->having);

        //table_free(query->table);
        schema_free(query->schema);
        free_(query->limit);
        free_(query->expr);
        free_(query);
}

void _add_validation_column(Query* query, Column* col)
{
        Source* src = vec_back(query->sources);
        vec_push_back(src->validation_list, &col);
}

void _add_logic_column(Query* query, Column* col)
{
        struct logic_builder* builder = query->logic_stack->data;
        logic_add_column(builder->current->data, col);
}

int _distribute_column(Query* query, Column* col)
{
        if (query->function_stack) {
                function_add_column(query->function_stack->data, col);
                return FQL_GOOD;
        }
        switch(query->mode) {
        case MODE_SELECT:
                select_add_column(query->op, col);
                break;
        case MODE_SEARCH:
                _add_logic_column(query, col);
                _add_validation_column(query, col);
                break;
        case MODE_GROUPBY:
                vec_push_back(query->groups, &col);
                _add_validation_column(query, col);
                break;
        default:
                return FQL_FAIL;
        }
        return FQL_GOOD;
}

void query_add_column(Query* query, char* col_name, const char* table_id)
{
        String* col_name_str = string_take(col_name);
        Column* col = column_new(EXPR_COLUMN_NAME, col_name_str, table_id);
        if (_distribute_column(query, col)) {
                fprintf(stderr, "Unhandled COLUMN_NAME: %s\n", col_name);
                free_(col_name);
                return;
        }

        /* TODO: This is sort of misplaced. Only acting as a default, but
         *       it should be set when schema is applied.
         */
        col->field_type = FIELD_STRING;
}

void query_add_constant(Query* query, const char* s, int len)
{
        Column* col = column_new(EXPR_CONST, NULL, "");

        enum field_type type = FIELD_UNDEFINED;
        if (s[0] == '\'') {
                type = FIELD_STRING;
                String* literal = string_from_char_ptr(s + 1);
                ((char*) literal->data)[len-2] = '\0';
                --literal->size;
                col->field.s = literal;
        } else {
                if (strhaschar(s, '.')) {
                        type = FIELD_FLOAT;
                        col->field.f = str2double(s);
                } else {
                        type = FIELD_INT;
                        col->field.i = str2long(s);
                }
        }

        col->field_type = type;
        if (_distribute_column(query, col)) {
                fprintf(stderr, "Unhandled constant expression: %d\n", query->mode);
                return;
        }
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

        source_init(vec_add_one(query->sources),
                    new_table,
                    alias,
                    type,
                    query->join);
}

void query_apply_table_alias(Query* query, const char* alias)
{
        Source* source = vec_back(query->sources);
        strncpy_(source->alias, alias, TABLE_NAME_MAX);
}

void _add_function(Query* query, Function* func, enum field_type type)
{
        Column* col = column_new(EXPR_FUNCTION, func, "");
        col->field_type = type;

        if (_distribute_column(query, col)) {
                fprintf(stderr, "Unhandled function: %s\n", func->name);
                return;
        }
        stack_push(&query->function_stack, func);
}

void query_enter_function(Query* query, const char* func_name)
{
        enum field_type type = FIELD_UNDEFINED;
        Function* func = function_new(func_name, &type);
        _add_function(query, func, type);
}

void query_enter_operator(Query* query, enum expr_operator op)
{
        enum field_type type = FIELD_UNDEFINED;
        Function* func = function_new_op(op, &type);
        _add_function(query, func, type);
}

void query_exit_function(Query* query)
{
        stack_pop(&query->function_stack);
}

/*** Scroll below this comment at your own risk ***/

struct logic_builder* logic_builder_new()
{
        struct logic_builder* builder = NULL;
        malloc_(builder, sizeof(*builder));

        *builder = (struct logic_builder) {
                 logic_tree_new()       /* tree */
                ,NULL                   /* current */
                ,NULL                   /* next_not */
                ,vec_new_(Dnode*)       /* truths */
                ,vec_new_(Dnode*)       /* falses */
                ,false                  /* sublogic_exit */
        };

        return builder;
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
        Dnode** node = vec_begin(nodes);
        while ((void*)node != vec_end(nodes)) {
                Logic* logic = (*node)->data;
                if (logic->comp_type == COMP_NOT_SET) {
                        dnode_free(*node);
                        vec_erase(nodes, node);
                } else {
                        ++node;
                }
        }

        switch (query->logic_mode) {
        case LOGIC_JOIN: {
                Source* src = vec_back(query->sources);
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
                Dnode** it = vec_begin(upper->falses);
                for (; it != vec_end(upper->falses); ++it) {
                        (*it)->out[false] = upper->ltree->end_false;
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
        Dnode** it = vec_begin(builder->falses);
        for (; it != vec_end(builder->falses); ++it) {
                (*it)->out[false] = builder->next_not;
        }

        vec_resize(builder->falses, 0);
}

void exit_search_and(Query* query)
{
        struct logic_builder* builder = query->logic_stack->data;
        vec_push_back(builder->truths, &builder->current);

        if (query->logic_stack->next != NULL) {
                struct logic_builder* lower = query->logic_stack->next->data;
                vec_extend(lower->truths, builder->truths);
                vec_resize(builder->truths, 0);
                return;
        }

        Dnode** it = vec_begin(builder->truths);
        for (; it != vec_end(builder->truths); ++it) {
                (*it)->out[true] = builder->ltree->end_true;
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

        Dnode** it = vec_begin(builder->truths);
        for (; it != vec_end(builder->truths); ++it) {
                (*it)->out[true] = builder->next_not;
        }
        if (builder->sublogic_exit) {
                return;
        }
        builder->current->out[true] = builder->next_not;
        vec_push_back(builder->falses, &builder->current);
}
