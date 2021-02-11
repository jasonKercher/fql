#include "query.h"

#include <stdbool.h>

#include "field.h"
#include "fql.h"
#include "logic.h"
#include "select.h"
#include "util/dgraph.h"
#include "util/util.h"

Query* query_new()
{
        Query* new_query = NULL;
        malloc_(new_query, sizeof(*new_query));

        return query_construct(new_query);
}

Query* query_construct(Query* query)
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

        Source* it = vec_begin(query->sources);
        for (; it != vec_end(query->sources); ++it) {
                source_destroy(it);
        }
        op_free(query->op);
        vec_free(query->sources);
        //logic_tree_free(query->where);
        vec_free(query->groups);
        queue_free_data(&query->having);
        schema_free(query->schema);
        free_(query->limit);
        free_(query->expr);
        free_(query);
}

void _add_validation_column(Query* query, Column* col)
{
        /* If sources are empty we arrived here by way of
         * some retarded query like:
         * SELECT 'hello'
         * WHERE 1 = 0
         * There is no need to validate anything here
         * since columns are either variables or
         * constants that will optimize away.
         */
        if (vec_empty(query->sources)) {
                return;
        }

        Source* src = vec_back(query->sources);
        vec_push_back(src->validation_list, &col);
}

void _add_logic_column(Query* query, Column* col)
{
        LogicGroup* lg = query->logic_stack->data;
        if (lg->condition == NULL) {
                lg->condition = logic_new();
        }
        logic_add_column(lg->condition, col);
}

int _distribute_column(Query* query, Column* col)
{
        if (query->function_stack) {
                Column* fn_col = query->function_stack->data;
                function_add_column(fn_col->field.fn, col);
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
        Column* col = column_new(EXPR_COLUMN_NAME, col_name, table_id);
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

void query_add_asterisk(Query* query, const char* table_id)
{
        Column* col = column_new(EXPR_ASTERISK, NULL, table_id);
        if (_distribute_column(query, col)) {
                fprintf(stderr, "Unhandled asterisk\n");
        }
}

int query_add_constant(Query* query, const char* s, int len)
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
                        if (str2double(&col->field.f, s)) {
                                return FQL_FAIL;
                        }
                } else {
                        type = FIELD_INT;
                        if (str2long(&col->field.i, s)) {
                                return FQL_FAIL;
                        }
                }
        }

        col->field_type = type;
        if (_distribute_column(query, col)) {
                fprintf(stderr, "Unhandled constant expression: %d\n", query->mode);
                return FQL_FAIL;
        }

        return FQL_GOOD;
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
                string_cpy(&new_table->name, table_name);
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

        source_construct(vec_add_one(query->sources),
                         new_table,
                         alias,
                         type,
                         query->join,
                         query->sources->size - 1);
}

void query_apply_table_alias(Query* query, const char* alias)
{
        Source* source = vec_back(query->sources);
        string_cpy(&source->alias, alias);
}

void _add_function(Query* query, Function* func, enum field_type type)
{
        Column* col = column_new(EXPR_FUNCTION, func, "");
        col->field_type = type;

        if (_distribute_column(query, col)) {
                fprintf(stderr, "Unhandled function: %s\n", func->name);
                return;
        }
        stack_push(&query->function_stack, col);
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
        Function* func = function_new_op(op);
        _add_function(query, func, FIELD_UNDEFINED);
}

void query_exit_function(Query* query)
{
        Column* col = stack_pop(&query->function_stack);
        if (function_op_resolve(col->field.fn, &col->field_type)) {
                ;/* TODO: stop here */
        }
}

/*** Scroll below this comment at your own risk ***/

void query_set_logic_comparison(Query* query, const char* op)
{
        LogicGroup* lg = query->logic_stack->data;
        Logic* logic = lg->condition;
        logic_set_comparison(logic, op);
}

void _assign_logic(Query* query, LogicGroup* lg)
{
        switch (query->logic_mode) {
        case LOGIC_JOIN: {
                Source* src = vec_back(query->sources);
                src->condition = lg;
                break;
        }
        case LOGIC_WHERE:
                query->where = lg;
                break;
        case LOGIC_UNDEFINED:
                fprintf(stderr, "Unexpected logic mode assigning group\n");
        }
}

void _add_item(Query* query, enum logicgroup_type type)
{
        LogicGroup* lg = logicgroup_new(type);
        LogicGroup* parent = query->logic_stack->data;
        vec_push_back(&parent->items, &lg);
        stack_push(&query->logic_stack, lg);
}

void enter_search(Query* query)
{
        if (query->logic_stack == NULL) {
                stack_push(&query->logic_stack, logicgroup_new(LG_ROOT));
                return;
        }

        _add_item(query, LG_ROOT);
}

void exit_search(Query* query)
{
        LogicGroup* lg = stack_pop(&query->logic_stack);
        if (query->logic_stack == NULL) {
                _assign_logic(query, lg);
        }
}

void enter_search_and(Query* query)
{
        _add_item(query, LG_AND);
}

void exit_search_and(Query* query)
{
        stack_pop(&query->logic_stack);
}

void enter_search_not(Query* query)
{
        _add_item(query, LG_NOT);
}

void exit_search_not(Query* query)
{
        stack_pop(&query->logic_stack);
}
