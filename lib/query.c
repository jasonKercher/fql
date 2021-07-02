#include "query.h"

#include <stdbool.h>
#include <unistd.h>

#include "fql.h"
#include "fqlselect.h"
#include "logic.h"
#include "group.h"
#include "order.h"
#include "aggregate.h"
#include "field.h"
#include "function.h"
#include "misc.h"
#include "util/dgraph.h"
#include "util/util.h"

query* query_new(int id)
{
	return new_(query, id);
}

query* query_construct(query* self, int id)
{
	*self = (query) {
	        NULL,                /* plan */
	        new_t_(vec, table),  /* sources */
	        NULL,                /* where */
	        NULL,                /* groupby */
	        NULL,                /* distinct */
	        NULL,                /* having */
	        NULL,                /* orderby */
	        NULL,                /* op */
	        new_t_(vec, query*), /* subquery_const_vec */
	        NULL,                /* top_expr */
	        -1,                  /* top_count */
	        id,                  /* query_id */
	        0,                   /* query_total */
	        0,                   /* expect_where */

	        NULL, /* logic_stack */
	        NULL, /* joinable */
	        NULL, /* function_stack */

	        0, /* in_aggregate */
	        0, /* in_bracket_expression */

	        MODE_UNDEFINED,  /* mode */
	        MODE_UNDEFINED,  /* mode_store */
	        LOGIC_UNDEFINED, /* logic_mode */
	        JOIN_FROM        /* join */
	};

	return self;
}

void query_destroy(query* self)
{
	delete_if_exists_(plan, self->plan);
	table* t_it = vec_begin(self->sources);
	for (; t_it != vec_end(self->sources); ++t_it) {
		table_destroy(t_it);
	}
	delete_(vec, self->sources);
	delete_if_exists_(op, self->op);
	delete_if_exists_(logicgroup, self->where);
	delete_if_exists_(group, self->groupby);
	delete_if_exists_(distinct, self->distinct);
	delete_if_exists_(logicgroup, self->having);
	delete_if_exists_(order, self->orderby);
	delete_if_exists_(column, self->top_expr);

	query** q_it = vec_begin(self->subquery_const_vec);
	for (; q_it != vec_end(self->subquery_const_vec); ++q_it) {
		delete_(query, *q_it);
	}
	delete_(vec, self->subquery_const_vec);
}

/* only here for address */
void query_free(void* data)
{
	delete_if_exists_(query, data);
}

int _add_logic_column(query* self, column* col)
{
	if (self->logic_mode != LOGIC_HAVING && col->expr == EXPR_AGGREGATE) {
		fputs("Cannot have aggregate logic outside of HAVING\n",
		      stderr);
		return FQL_FAIL;
	}
	logicgroup* lg = stack_bottom(self->logic_stack)->data;
	vec_push_back(lg->columns, &col);

	lg = self->logic_stack->data;
	if (lg->condition == NULL) {
		lg->condition = new_(logic);
	}
	logic_add_column(lg->condition, col);

	return FQL_GOOD;
}

int _distribute_column(query* self, column* col)
{
	if (self->function_stack) {
		column* fn_col = self->function_stack->data;
		function_add_column(fn_col->field.fn, col);
		return FQL_GOOD;
	}
	if (self->in_aggregate) {
		aggregate** back = vec_back(&self->groupby->aggregates);
		aggregate_add_column(*back, col);
		return FQL_GOOD;
	}
	switch (self->mode) {
	case MODE_SELECT:
		fqlselect_add_column(self->op, col);
		if (self->distinct) {
			group_add_column(self->distinct, col);
		}
		break;
	case MODE_TOP:
		self->top_expr = col;
		break;
	case MODE_IN:
	case MODE_SEARCH:
		try_(_add_logic_column(self, col));
		break;
	case MODE_GROUPBY:
		group_add_column(self->groupby, col);
		break;
	case MODE_ORDERBY:
		order_add_column(self->orderby, col);
		break;
	default:
		return FQL_FAIL;
	}
	return FQL_GOOD;
}

int query_add_column(query* self, char* col_name, const char* table_id)
{
	column* col = NULL;
	if (istring_eq(col_name, "__rec")) {
		col = new_(column, EXPR_FULL_RECORD, col_name, table_id);
	} else {
		col = new_(column, EXPR_COLUMN_NAME, col_name, table_id);
	}

	/* TODO: remove when this is impossible */
	if (_distribute_column(self, col)) {
		fprintf(stderr, "unhandled COLUMN_NAME: %s\n", col_name);
		return FQL_FAIL;
	}
	return FQL_GOOD;
}

int query_add_asterisk(query* self, const char* table_id)
{
	column* col = new_(column, EXPR_ASTERISK, NULL, table_id);

	/* TODO: remove when this is impossible */
	if (_distribute_column(self, col)) {
		fprintf(stderr, "unhandled asterisk\n");
		return FQL_FAIL;
	}
	return FQL_GOOD;
}

int query_add_constant(query* self, const char* s, int len)
{
	column* col = new_(column, EXPR_CONST, NULL, "");

	enum field_type type = FIELD_UNDEFINED;
	if (s[0] == '\'') {
		type = FIELD_STRING;
		string_strcpy(&col->buf, s + 1);
		((char*)col->buf.data)[len - 2] = '\0';
		--col->buf.size;
		col->field.s = &col->buf;
	} else {
		if (strhaschar(s, '.')) {
			type = FIELD_FLOAT;
			fail_if_(str2double(&col->field.f, s));
		} else {
			type = FIELD_INT;
			fail_if_(str2long(&col->field.i, s));
		}
	}

	col->field_type = type;
	if (_distribute_column(self, col)) {
		fprintf(stderr,
		        "unhandled constant expression: %d\n",
		        self->mode);
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
void query_add_source(query* self, stack** source_stack, const char* alias)
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
		new_table->schema->name = string_take(schema_name);
	}
}

void query_add_subquery_source(query* self, query* subquery, const char* alias)
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

void query_apply_column_alias(query* self, const char* alias)
{
	fqlselect_apply_column_alias(self->op, alias);
}

void query_set_distinct(query* self)
{
	self->distinct = new_(group);
}

int query_set_top_count(query* self, const char* count_str)
{
	long top_count;
	fail_if_(str2long(&top_count, count_str));
	fail_if_(top_count < 0);
	self->top_count = top_count;
	return FQL_GOOD;
}

int query_set_into_table(query* self, const char* table_name)
{
	if (access(table_name, F_OK) == 0) {
		fprintf(stderr,
		        "Cannot SELECT INTO: file `%s' already exists\n",
		        table_name);
		return FQL_FAIL;
	}
	fqlselect* select = self->op;
	return writer_open(select->writer, table_name);
}

int query_add_aggregate(query* self, enum aggregate_function agg_type)
{
	if (self->groupby == NULL) {
		self->groupby = new_(group);
	}
	aggregate* agg = new_(aggregate, agg_type);
	vec_push_back(&self->groupby->aggregates, &agg);

	column* group_col = new_(column, EXPR_AGGREGATE, agg, "");
	group_add_column(self->groupby, group_col);

	column* linked_col = new_(column, EXPR_AGGREGATE, agg, "");
	column_link(linked_col, group_col);
	agg->linked_column = linked_col;
	try_(_distribute_column(self, linked_col));

	return FQL_GOOD;
}

int _add_function(query* self, function* func, enum field_type type)
{
	column* col = new_(column, EXPR_FUNCTION, func, "");
	col->field_type = type;

	if (_distribute_column(self, col)) {
		fprintf(stderr,
		        "unhandled function: %s\n",
		        function_get_name(func));
		return FQL_FAIL;
	}
	stack_push(&self->function_stack, col);
	return FQL_GOOD;
}

int query_init_op(query* self)
{
	switch (self->mode) {
	case MODE_SELECT:
		self->op = new_(fqlselect);
		break;
	default:
		fprintf(stderr, "unexpected operation mode `%d'\n", self->mode);
		return FQL_FAIL;
	}
	return FQL_GOOD;
}

void query_init_groupby(query* self)
{
	if (self->groupby == NULL) {
		self->groupby = new_(group);
	}
	self->mode = MODE_GROUPBY;
}

int query_init_orderby(query* self)
{
	/* If there are no sources, then we are trying to
	 * order a const expression... Don't bother...
	 */
	if (vec_empty(self->sources)) {
		return FQL_GOOD;
	}

	/* orderby allows us to assume SELECT */
	fqlselect* select = self->op;

	char* out_name = writer_take_filename(select->writer);
	const char* in_name = writer_get_tempname(select->writer);
	self->orderby = new_(order, in_name, out_name);
	/* take_filename takes ownership, must free */
	if (out_name) {
		free_(out_name);
	}
	return (self->orderby->out_file) ? FQL_GOOD : FQL_FAIL;
}

void query_init_in_statement(query* self)
{
	logicgroup* lg = self->logic_stack->data;
	if (lg->condition == NULL) {
		lg->condition = new_(logic);
	}
	lg->condition->in_data = new_(inlist);
}

void query_assign_in_subquery(query* self, query* subquery)
{
	logicgroup* lg = self->logic_stack->data;
	lg->condition->in_data->subquery = subquery;
	lg->condition->comp_type = COMP_SUBIN;
	vec_push_back(self->subquery_const_vec, &subquery);
}

void query_add_subquery_const(query* self, query* subquery)
{
	column* subquery_column = new_(column, EXPR_SUBQUERY, subquery, "");
	fqlselect* subselect = subquery->op;
	subselect->const_dest = subquery_column;
	vec_push_back(self->subquery_const_vec, &subquery);
	_distribute_column(self, subquery_column);
}

void query_set_order_desc(query* self)
{
	column** col = vec_back(&self->orderby->columns);
	(*col)->descending = true;
}

int query_enter_function(query* self,
                         enum scalar_function scalar_type,
                         int char_as_byte)
{
	enum field_type type = FIELD_UNDEFINED;
	function* func = new_(function, scalar_type, &type, char_as_byte);
	fail_if_(func->call__ == NULL);
	try_(_add_function(self, func, type));

	return FQL_GOOD;
}

int query_enter_operator(query* self, enum scalar_function op)
{
	enum field_type type = FIELD_UNDEFINED;
	function* func = new_(function, op, &type, true);
	try_(_add_function(self, func, FIELD_UNDEFINED));
	return FQL_GOOD;
}

void query_exit_function(query* self)
{
	//column* col = stack_pop(&self->function_stack);
	stack_pop(&self->function_stack);
}

void query_set_logic_comparison(query* self, const char* op, int negation)
{
	logicgroup* lg = self->logic_stack->data;
	/* Check for negation first because standard
	 * comparisons like = or > set the negation
	 * in the enter_search_not function.
	 */
	if (negation) {
		lg->negation = true;
	}
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
	case LOGIC_HAVING:
		self->having = lg;
		break;
	default:
		fprintf(stderr, "unexpected logic mode assigning group\n");
	}
	self->logic_mode = LOGIC_UNDEFINED;
}

logicgroup* _add_item(query* self, enum logicgroup_type type)
{
	logicgroup* lg = new_(logicgroup, type);
	logicgroup* parent = self->logic_stack->data;
	vec_push_back(&parent->items, &lg);
	stack_push(&self->logic_stack, lg);
	return lg;
}

void enter_search(query* self)
{
	if (self->logic_stack == NULL) {
		logicgroup* lg = new_(logicgroup, LG_ROOT);
		lg->columns = new_t_(vec, column*);
		stack_push(&self->logic_stack, lg);
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

void enter_search_not(query* self, int negation)
{
	logicgroup* lg = _add_item(self, LG_NOT);
	lg->negation = negation;
}

void exit_search_not(query* self)
{
	logicgroup* top = stack_pop(&self->logic_stack);

	/* pre-requisite test for joinability
	 * A complete test for joinablity cannot occur
	 * until all logic columns have been resolved
	 * to a source.
	 */
	if (top->condition != NULL && top->condition->comp_type == COMP_EQ
	    && top->condition->col[0]->expr != EXPR_CONST
	    && top->condition->col[1]->expr != EXPR_CONST) {
		vec_push_back(self->joinable, &top->condition);
	}
}
