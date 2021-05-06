#include "query.h"

#include <stdbool.h>

#include "fql.h"
#include "select.h"
#include "logic.h"
#include "group.h"
#include "aggregate.h"
#include "field.h"
#include "function.h"
#include "util/dgraph.h"
#include "util/util.h"

Query* query_new(int id)
{
	query* new_query = NULL;
	malloc_(new_query, sizeof(*new_query));

	return query_construct(new_query, id);
}

Query* query_construct(query* query, int id)
{
	*query = (query) {
		 NULL                   /* plan */
		,vec_new_(table)        /* sources */
		,NULL                   /* where */
		,vec_new_(column*)      /* validation_list */
		,group_new()            /* groupby */
		,NULL                   /* distinct */
		,NULL                   /* having */
		,NULL                   /* limit */
		,NULL                   /* operation */
		,id                     /* query_id */
		,0                      /* query_total */

		,NULL                   /* logic_stack */
		,NULL                   /* joinable */
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
	query* query = generic_query;

	plan_free(query->plan);
	table* it = vec_begin(query->sources);
	for (; it != vec_end(query->sources); ++it) {
		table_destroy(it);
	}
	op_free(query->op);
	vec_free(query->sources);
	logicgroup_free(query->where);
	vec_free(query->validation_list);
	group_free(query->groupby);
	group_free(query->distinct);
	queue_free_data(&query->having);
	free_(query->limit);
	free_(query->expr);
	free_(query);
}

void _add_validation_column(query* query, column* col)
{
	/* if sources are empty we arrived here by way of
	 * some retarded query like:
	 * SELECT 'hello'
	 * WHERE 1 = 0
	 */
	if (vec_empty(query->sources)) {
		vec_push_back(query->validation_list, &col);
		return;
	}

	table* table = vec_back(query->sources);
	vec_push_back(table->validation_list, &col);
}

void _add_logic_column(query* query, column* col)
{
	logicgroup* lg = query->logic_stack->data;
	if (lg->condition == NULL) {
		lg->condition = logic_new();
	}
	logic_add_column(lg->condition, col);
}

int _distribute_column(query* query, column* col)
{
	if (query->function_stack) {
		column* fn_col = query->function_stack->data;
		function_add_column(fn_col->field.fn, col);
		return FQL_GOOD;
	}
	switch(query->mode) {
	case MODE_SELECT:
		select_add_column(query->op, col);

		/* TODO */
		if (col->expr == EXPR_AGGREGATE && query->distinct) {
			fputs("currently unsafe to mix DISTINCT and GROUP BY\n", stderr);
		}
		if (query->distinct) {
			group_add_column(query->distinct, col);
		}
		break;
	case MODE_SEARCH:
		_add_logic_column(query, col);
		_add_validation_column(query, col);
		break;
	case MODE_GROUPBY:
		group_add_column(query->groupby, col);
		break;
	case MODE_AGGREGATE:
	 {
		aggregate** back = vec_back(&query->groupby->aggregates);
		aggregate_add_column(*back, col);
		break;
	 }
	default:
		return FQL_FAIL;
	}
	return FQL_GOOD;
}

void query_add_column(query* query, char* col_name, const char* table_id)
{
	column* col = column_new(EXPR_COLUMN_NAME, col_name, table_id);
	if (_distribute_column(query, col)) {
		fprintf(stderr, "unhandled COLUMN_NAME: %s\n", col_name);
		free_(col_name);
		return;
	}
}

void query_add_asterisk(query* query, const char* table_id)
{
	column* col = column_new(EXPR_ASTERISK, NULL, table_id);
	if (_distribute_column(query, col)) {
		fprintf(stderr, "unhandled asterisk\n");
	}
}

int query_add_constant(query* query, const char* s, int len)
{
	column* col = column_new(EXPR_CONST, NULL, "");

	enum field_type type = FIELD_UNDEFINED;
	if (s[0] == '\'') {
		type = FIELD_STRING;
		string_strcpy(&col->buf, s+1);
		((char*) col->buf.data)[len-2] = '\0';
		--col->buf.size;
		col->field.s = &col->buf;
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
		fprintf(stderr, "unhandled constant expression: %d\n", query->mode);
		return FQL_FAIL;
	}

	return FQL_GOOD;
}


/**
 * create new table and source object
 * assign name and schema if provided. source_stack
 * is a stack of allocated char* of the form:
 * object->schema->database->server
 * we ignore database and server for now.
 */
void query_add_source(query* query,
		      stack** source_stack,
		      const char* alias)
{
	char* table_name = stack_pop(source_stack);
	char* schema_name = stack_pop(source_stack);

	stack_free_data(source_stack);
	table* new_table = vec_add_one(query->sources);
	table_construct(new_table,
			table_name,
			alias,
			query->sources->size - 1,
			query->join);

	if (schema_name != NULL) {
		strncpy_(new_table->schema->name
			,schema_name
			,TABLE_NAME_MAX);
		free_(schema_name);
	}
}

void query_add_subquery_source(query* query,
			       query* subquery,
			       const char* alias)
{
	table* new_table = vec_add_one(query->sources);
	table_construct_subquery(new_table,
				 subquery,
				 alias,
				 query->sources->size - 1,
				 query->join);

}

void query_apply_table_alias(query* query, const char* alias)
{
	table* table = vec_back(query->sources);
	string_strcpy(&table->alias, alias);
}

void query_set_distinct(query* query)
{
	query->distinct = group_new();
}

int query_add_aggregate(query* query, enum aggregate_function agg_type)
{
	aggregate* agg = aggregate_new(agg_type);
	vec_push_back(&query->groupby->aggregates, &agg);

	column* group_col = column_new(EXPR_AGGREGATE, agg, "");
	group_add_column(query->groupby, group_col);

	column* op_col = column_new(EXPR_AGGREGATE, agg, "");
	op_col->data_source = group_col;
	if (_distribute_column(query, op_col)) {
		return FQL_FAIL;
	}

	return FQL_GOOD;
}

void _add_function(query* query, function* func, enum field_type type)
{
	column* col = column_new(EXPR_FUNCTION, func, "");
	col->field_type = type;

	if (_distribute_column(query, col)) {
		fprintf(stderr,
			"unhandled function: %s\n",
			function_get_name(func));
		return;
	}
	stack_push(&query->function_stack, col);
}

int query_enter_function(query* query, enum scalar_function scalar_type, int char_as_byte)
{
	enum field_type type = FIELD_UNDEFINED;
	function* func = function_new(scalar_type, &type, char_as_byte);
	if (func->call__ == NULL) {
		return FQL_FAIL;
	}
	_add_function(query, func, type);

	return FQL_GOOD;
}

void query_enter_operator(query* query, enum scalar_function op)
{
	enum field_type type = FIELD_UNDEFINED;
	function* func = function_new(op, &type, true);
	_add_function(query, func, FIELD_UNDEFINED);
}

void query_exit_function(query* query)
{
	column* col = stack_pop(&query->function_stack);
}

void query_set_logic_comparison(query* query, const char* op)
{
	logicgroup* lg = query->logic_stack->data;
	logic* logic = lg->condition;
	logic_set_comparison(logic, op);
}

void _assign_logic(query* query, logicgroup* lg)
{
	switch (query->logic_mode) {
	case LOGIC_JOIN: {
		table* table = vec_back(query->sources);
		table->condition = lg;
		break;
	}
	case LOGIC_WHERE:
		query->where = lg;
		break;
	case LOGIC_UNDEFINED:
		fprintf(stderr, "unexpected logic mode assigning group\n");
	}
}

void _add_item(query* query, enum logicgroup_type type)
{
	logicgroup* lg = logicgroup_new(type);
	logicgroup* parent = query->logic_stack->data;
	vec_push_back(&parent->items, &lg);
	stack_push(&query->logic_stack, lg);
}

void enter_search(query* query)
{
	if (query->logic_stack == NULL) {
		stack_push(&query->logic_stack, logicgroup_new(LG_ROOT));
		query->joinable = vec_new_(logic*);
		return;
	}

	_add_item(query, LG_ROOT);
}

void exit_search(query* query)
{
	logicgroup* lg = stack_pop(&query->logic_stack);
	if (query->logic_stack == NULL) {
		if (vec_empty(query->joinable)) {
			vec_free(query->joinable);
		} else {
			lg->joinable = query->joinable;
			query->joinable = NULL;
		}

		_assign_logic(query, lg);
	}
}

void enter_search_and(query* query)
{
	_add_item(query, LG_AND);
}

void exit_search_and(query* query)
{
	stack_pop(&query->logic_stack);
}

void enter_search_not(query* query)
{
	_add_item(query, LG_NOT);
}

void exit_search_not(query* query)
{
	logicgroup* top = stack_pop(&query->logic_stack);

	/* pre-requisite test for joinability
	 * A complete test for joinablity cannot occur
	 * until all logic columns have been resolved
	 * to a source.
	 */
	if (top->condition != NULL &&
	    top->condition->comp_type == COMP_EQ &&
	    top->condition->col[0]->expr != EXPR_CONST &&
	    top->condition->col[1]->expr != EXPR_CONST) {
		vec_push_back(query->joinable, &top->condition);
	}

}
