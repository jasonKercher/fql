#include "query.h"

#include <stdbool.h>

#include "fql.h"
#include "fqlselect.h"
#include "logic.h"
#include "group.h"
#include "aggregate.h"
#include "field.h"
#include "function.h"
#include "util/dgraph.h"
#include "util/util.h"

query* query_new(int id)
{
	return new_(query, id);
}

query* query_construct(query* self, int id)
{
	*self = (query) {
		 NULL                   /* plan */
		,new_t_(vec, table)     /* sources */
		,NULL                   /* where */
		,new_t_(vec, column*)   /* validation_list */
		,new_(group)            /* groupby */
		,NULL                   /* distinct */
		//,NULL                   /* having */
		//,NULL                   /* limit */
		,NULL                   /* orderby */
		,NULL                   /* operation */
		,id                     /* query_id */
		,0                      /* query_total */

		,NULL                   /* logic_stack */
		,NULL                   /* joinable */
		,NULL                   /* function_stack */

		//,NULL                   /* expr */
		,MODE_UNDEFINED         /* mode */
		,LOGIC_UNDEFINED        /* logic_mode */
		,JOIN_FROM              /* join */
	};

	return self;
}

void query_destroy(query* self)
{
	delete_(plan, self->plan);
	table* it = vec_begin(self->sources);
	for (; it != vec_end(self->sources); ++it) {
		table_destroy(it);
	}
	delete_(op, self->op);
	delete_(vec, self->sources);
	delete_(logicgroup, self->where);
	delete_(vec, self->validation_list);
	delete_(group, self->groupby);
	delete_(group, self->distinct);
	//queue_free_data(&self->having);
	//free_(self->limit);
	//free_(self->expr);
}

/* only here for address */
void query_free(void* data)
{
	delete_(query, data);
}

void _add_validation_column(query* self, column* col)
{
	/* if sources are empty we arrived here by way of
	 * some retarded query like:
	 * SELECT 'hello'
	 * WHERE 1 = 0
	 */
	if (vec_empty(self->sources)) {
		vec_push_back(self->validation_list, &col);
		return;
	}

	table* table = vec_back(self->sources);
	vec_push_back(table->validation_list, &col);
}

void _add_logic_column(query* self, column* col)
{
	logicgroup* lg = self->logic_stack->data;
	if (lg->condition == NULL) {
		lg->condition = new_(logic);
	}
	logic_add_column(lg->condition, col);
}

int _distribute_column(query* self, column* col)
{
	if (self->function_stack) {
		column* fn_col = self->function_stack->data;
		function_add_column(fn_col->field.fn, col);
		return FQL_GOOD;
	}
	switch(self->mode) {
	case MODE_SELECT:
		fqlselect_add_column(self->op, col);

		/* TODO */
		if (col->expr == EXPR_AGGREGATE && self->distinct) {
			fputs("currently unsafe to mix DISTINCT and GROUP BY\n", stderr);
		}
		if (self->distinct) {
			group_add_column(self->distinct, col);
		}
		break;
	case MODE_SEARCH:
		_add_logic_column(self, col);
		_add_validation_column(self, col);
		break;
	case MODE_GROUPBY:
		group_add_column(self->groupby, col);
		break;
	case MODE_AGGREGATE:
	 {
		aggregate** back = vec_back(&self->groupby->aggregates);
		aggregate_add_column(*back, col);
		break;
	 }
	case MODE_ORDERBY:
	default:
		return FQL_FAIL;
	}
	return FQL_GOOD;
}

void query_add_column(query* self, char* col_name, const char* table_id)
{
	column* col = new_(column, EXPR_COLUMN_NAME, col_name, table_id);
	if (_distribute_column(self, col)) {
		fprintf(stderr, "unhandled COLUMN_NAME: %s\n", col_name);
		exit(EXIT_FAILURE);
	}
}

void query_add_asterisk(query* self, const char* table_id)
{
	column* col = new_(column, EXPR_ASTERISK, NULL, table_id);
	if (_distribute_column(self, col)) {
		fprintf(stderr, "unhandled asterisk\n");
		exit(EXIT_FAILURE);
	}
}

int query_add_constant(query* self, const char* s, int len)
{
	column* col = new_(column, EXPR_CONST, NULL, "");

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
			fail_if_ (str2double(&col->field.f, s));
		} else {
			type = FIELD_INT;
			fail_if_ (str2long(&col->field.i, s));
		}
	}

	col->field_type = type;
	if (_distribute_column(self, col)) {
		fprintf(stderr, "unhandled constant expression: %d\n", self->mode);
		exit(EXIT_FAILURE);
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
void query_add_source(query* self,
		      stack** source_stack,
		      const char* alias)
{
	char* table_name = stack_pop(source_stack);
	char* schema_name = stack_pop(source_stack);

	stack_free_data(source_stack);
	table* new_table = vec_add_one(self->sources);
	table_construct(new_table,
			table_name,
			alias,
			self->sources->size - 1,
			self->join);

	if (schema_name != NULL) {
		strncpy_(new_table->schema->name
			,schema_name
			,TABLE_NAME_MAX);
		free_(schema_name);
	}
}

void query_add_subquery_source(query* self,
			       query* subquery,
			       const char* alias)
{
	table* new_table = vec_add_one(self->sources);
	table_construct_subquery(new_table,
				 subquery,
				 alias,
				 self->sources->size - 1,
				 self->join);

}

void query_apply_table_alias(query* self, const char* alias)
{
	table* table = vec_back(self->sources);
	string_strcpy(&table->alias, alias);
}

void query_set_distinct(query* self)
{
	self->distinct = new_(group);
}

int query_add_aggregate(query* self, enum aggregate_function agg_type)
{
	aggregate* agg = new_(aggregate, agg_type);
	vec_push_back(&self->groupby->aggregates, &agg);

	column* group_col = new_(column, EXPR_AGGREGATE, agg, "");
	group_add_column(self->groupby, group_col);

	column* op_col = new_(column, EXPR_AGGREGATE, agg, "");
	op_col->data_source = group_col;
	try_ (_distribute_column(self, op_col));

	return FQL_GOOD;
}

void _add_function(query* self, function* func, enum field_type type)
{
	column* col = new_(column, EXPR_FUNCTION, func, "");
	col->field_type = type;

	if (_distribute_column(self, col)) {
		fprintf(stderr,
			"unhandled function: %s\n",
			function_get_name(func));
		exit(EXIT_FAILURE);
	}
	stack_push(&self->function_stack, col);
}

int query_init_op(query* self)
{
	switch (self->mode) {
	case MODE_SELECT:
		self->op = new_(fqlselect);
		break;
	default:
		fprintf(stderr, 
		        "unexpected operation mode `%d'\n",
			self->mode);
		return FQL_FAIL;
	}
	return FQL_GOOD;
}

int query_enter_function(query* self, enum scalar_function scalar_type, int char_as_byte)
{
	enum field_type type = FIELD_UNDEFINED;
	function* func = new_(function, scalar_type, &type, char_as_byte);
	fail_if_ (func->call__ == NULL);
	_add_function(self, func, type);

	return FQL_GOOD;
}

void query_enter_operator(query* self, enum scalar_function op)
{
	enum field_type type = FIELD_UNDEFINED;
	function* func = new_(function, op, &type, true);
	_add_function(self, func, FIELD_UNDEFINED);
}

void query_exit_function(query* self)
{
	column* col = stack_pop(&self->function_stack);
}

void query_set_logic_comparison(query* self, const char* op)
{
	logicgroup* lg = self->logic_stack->data;
	logic* logic = lg->condition;
	logic_set_comparison(logic, op);
}

void _assign_logic(query* self, logicgroup* lg)
{
	switch (self->logic_mode) {
	case LOGIC_JOIN: {
		table* table = vec_back(self->sources);
		table->condition = lg;
		break;
	}
	case LOGIC_WHERE:
		self->where = lg;
		break;
	case LOGIC_UNDEFINED:
		fprintf(stderr, "unexpected logic mode assigning group\n");
	}
}

void _add_item(query* self, enum logicgroup_type type)
{
	logicgroup* lg = new_(logicgroup, type);
	logicgroup* parent = self->logic_stack->data;
	vec_push_back(&parent->items, &lg);
	stack_push(&self->logic_stack, lg);
}

void enter_search(query* self)
{
	if (self->logic_stack == NULL) {
		stack_push(&self->logic_stack, new_(logicgroup, LG_ROOT));
		self->joinable = new_t_(vec, logic*);
		return;
	}

	_add_item(self, LG_ROOT);
}

void exit_search(query* self)
{
	logicgroup* lg = stack_pop(&self->logic_stack);
	if (self->logic_stack == NULL) {
		if (vec_empty(self->joinable)) {
			delete_(vec, self->joinable);
		} else {
			lg->joinable = self->joinable;
			self->joinable = NULL;
		}

		_assign_logic(self, lg);
	}
}

void enter_search_and(query* self)
{
	_add_item(self, LG_AND);
}

void exit_search_and(query* self)
{
	stack_pop(&self->logic_stack);
}

void enter_search_not(query* self)
{
	_add_item(self, LG_NOT);
}

void exit_search_not(query* self)
{
	logicgroup* top = stack_pop(&self->logic_stack);

	/* pre-requisite test for joinability
	 * A complete test for joinablity cannot occur
	 * until all logic columns have been resolved
	 * to a source.
	 */
	if (top->condition != NULL &&
	    top->condition->comp_type == COMP_EQ &&
	    top->condition->col[0]->expr != EXPR_CONST &&
	    top->condition->col[1]->expr != EXPR_CONST) {
		vec_push_back(self->joinable, &top->condition);
	}

}
