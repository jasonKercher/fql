#include "query.h"

#include <stdbool.h>
#include <unistd.h>

#include "fql.h"
#include "misc.h"
#include "logic.h"
#include "group.h"
#include "order.h"
#include "field.h"
#include "fqlbranch.h"
#include "function.h"
#include "fqlselect.h"
#include "fqldelete.h"
#include "fqlupdate.h"
#include "aggregate.h"
#include "expression.h"
#include "switchcase.h"
#include "util/util.h"
#include "util/dgraph.h"

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
	        new_t_(vec, query*), /* unions */
	        NULL,                /* into_table_name */
	        -1,                  /* top_count */
	        0,                   /* idx */
	        0,                   /* next_idx */
	        0,                   /* union_id */
	        id,                  /* query_id */
	        0,                   /* query_total */
	        0,                   /* expect_where */

	        NULL, /* joinable */
	        NULL, /* logic_stack */
	        NULL, /* function_stack */
	        NULL, /* switchcase_stack */

	        MODE_UNDEFINED,  /* mode */
	        MODE_UNDEFINED,  /* mode_store */
	        LOGIC_UNDEFINED, /* logic_mode */
	        JOIN_FROM,       /* join */
	        false,           /* in_bracket_expression */
	};

	return self;
}

void query_destroy(query* self)
{
	query_release_sources(self);
	delete_if_exists_(plan, self->plan);
	delete_(vec, self->sources);
	delete_if_exists_(op, self->op);
	delete_if_exists_(logicgroup, self->where);
	delete_if_exists_(group, self->groupby);
	delete_if_exists_(distinct, self->distinct);
	delete_if_exists_(logicgroup, self->having);
	delete_if_exists_(order, self->orderby);
	delete_if_exists_(expression, self->top_expr);
	delete_if_exists_(vec, self->joinable);

	query** it = vec_begin(self->subquery_const_vec);
	for (; it != vec_end(self->subquery_const_vec); ++it) {
		delete_(query, *it);
	}

	it = vec_begin(self->unions);
	for (; it != vec_end(self->unions); ++it) {
		delete_(query, *it);
	}
	delete_(vec, self->unions);
	delete_(vec, self->subquery_const_vec);
	free_if_exists_(self->into_table_name);
}

void query_release_sources(query* self)
{
	table* it = vec_begin(self->sources);
	for (; it != vec_end(self->sources); ++it) {
		table_destroy(it);
	}
	vec_resize(self->sources, 0);
}

query* query_add_query(struct fqlhandle* fql)
{
	query* newquery = new_(query, 0);
	newquery->idx = fql->query_vec->size;
	newquery->next_idx = fql->query_vec->size + 1;
	vec_push_back(fql->query_vec, &newquery);
	return newquery;
}

int _add_logic_expression(query* self, expression* expr)
{
	if (self->logic_mode != LOGIC_HAVING && expr->expr == EXPR_AGGREGATE) {
		fputs("Cannot have aggregate logic outside of HAVING\n",
		      stderr);
		return FQL_FAIL;
	}
	logicgroup* lg = node_bottom(self->logic_stack)->data;
	vec_push_back(lg->expressions, &expr);

	lg = self->logic_stack->data;
	if (lg->condition == NULL) {
		lg->condition = new_(logic);
	}
	logic_add_expression(lg->condition, expr);

	return FQL_GOOD;
}

int _add_switchcase_expression(query* self, expression* expr)
{
	switchcase* sc = self->switchcase_stack->data;
	switch (sc->state) {
	case SWITCH_STATIC:
		sc->static_expr = expr;
		break;
	case SWITCH_VALUE:
		switchcase_add_value(sc, expr);
		break;
	case SWITCH_STATIC_CMP:
		sc->state = SWITCH_VALUE;
		try_(_add_logic_expression(self, expr));
		break;
	case SWITCH_LOGIC_GROUP:
		try_(_add_logic_expression(self, expr));
	}
	return FQL_GOOD;
}

int _distribute_expression(query* self, expression* expr)
{
	/* Order probably important here. This is
	 * the priority of ownership for the most
	 * recent expression.
	 */
	if (self->function_stack) {
		expression* fn_expr = self->function_stack->data;
		function_add_expression(fn_expr->field.fn, expr);
		return FQL_GOOD;
	}
	switch (self->mode) {
	case MODE_SELECTLIST:
		fqlselect_add_expression(self->op, expr);
		break;
	case MODE_UPDATELIST:
		try_(fqlupdate_add_expression(self->op, expr));
		break;
	case MODE_TOP:
		self->top_expr = expr;
		break;
	case MODE_CASE:
		try_(_add_switchcase_expression(self, expr));
		break;
	case MODE_IN:
	case MODE_SEARCH:
		try_(_add_logic_expression(self, expr));
		break;
	case MODE_GROUPBY:
		group_add_expression(self->groupby, expr);
		break;
	case MODE_ORDERBY:
		order_add_expression(self->orderby, expr);
		break;
	case MODE_AGGREGATE: {
		aggregate** back = vec_back(&self->groupby->aggregates);
		aggregate_add_expression(*back, expr);
		break;
	}

	default:
		return FQL_FAIL;
	}
	return FQL_GOOD;
}

int query_add_expression(query* self, char* expr_name, const char* table_id)
{
	/* TODO: should probably handle built-ins with
	 *       their own function triggered by "__.*"
	 */
	expression* expr = NULL;
	if (istring_eq(expr_name, "__lf")) {
		expr = new_(expression, EXPR_CONST, NULL, "");
		string_strcpy(&expr->buf, "\n");
		expr->field_type = FIELD_STRING;
		expr->field.s = &expr->buf;
	} else if (istring_eq(expr_name, "__crlf")) {
		expr = new_(expression, EXPR_CONST, NULL, "");
		string_strcpy(&expr->buf, "\r\n");
		expr->field_type = FIELD_STRING;
		expr->field.s = &expr->buf;
	} else if (istring_eq(expr_name, "__cr")) {
		expr = new_(expression, EXPR_CONST, NULL, "");
		string_strcpy(&expr->buf, "\r");
		expr->field_type = FIELD_STRING;
		expr->field.s = &expr->buf;
	} else if (istring_eq(expr_name, "__rec")) {
		expr = new_(expression, EXPR_FULL_RECORD, expr_name, table_id);
	} else if (istring_eq(expr_name,
	                      "__rownum")) { // && self->mode == MODE_SELECTLIST) {
		expr = new_(expression, EXPR_ROW_NUMBER, expr_name, table_id);
		expr->field_type = FIELD_INT;
		op_assign_rownum_ref(self->op, expr);
	} else {
		expr = new_(expression, EXPR_COLUMN_NAME, expr_name, table_id);
	}

	if (_distribute_expression(self, expr)) {
		fprintf(stderr, "unhandled COLUMN_NAME: %s\n", expr_name);
		return FQL_FAIL;
	}

	return FQL_GOOD;
}

int query_add_asterisk(query* self, const char* table_id)
{
	expression* expr = new_(expression, EXPR_ASTERISK, NULL, table_id);

	if (_distribute_expression(self, expr)) {
		fprintf(stderr, "unhandled asterisk\n");
		return FQL_FAIL;
	}
	return FQL_GOOD;
}

int query_add_constant(query* self, const char* s, int len)
{
	expression* expr = new_(expression, EXPR_CONST, NULL, "");

	enum field_type type = FIELD_UNDEFINED;
	if (s[0] == '\'') {
		type = FIELD_STRING;
		string_strcpy(&expr->buf, s + 1);
		((char*)expr->buf.data)[len - 2] = '\0';
		--expr->buf.size;
		expr->field.s = &expr->buf;
	} else {
		if (strhaschar(s, '.')) {
			type = FIELD_FLOAT;
			fail_if_(str2double(&expr->field.f, s));
		} else {
			type = FIELD_INT;
			fail_if_(str2long(&expr->field.i, s));
		}
	}

	expr->field_type = type;
	if (_distribute_expression(self, expr)) {
		fprintf(stderr,
		        "unhandled constant expression: %d\n",
		        self->mode);
		return FQL_FAIL;
	}

	return FQL_GOOD;
}

int query_add_null_expression(query* self)
{
	expression* expr = new_(expression, EXPR_NULL, NULL, "");
	try_(_distribute_expression(self, expr));
	return FQL_GOOD;
}

/* create new table and source object
 * assign name and schema if provided. source_stack
 * is a stack of allocated char* of the form:
 * object->schema->database->server
 * we ignore database and server for now.
 */
int query_add_source(query* self,
                     struct fqlhandle* fql,
                     node** source_stack,
                     const char* alias)
{
	char* table_name = node_pop(source_stack);
	char* schema_name = node_pop(source_stack);

	node_free_data(source_stack);
	table* new_table = vec_add_one(self->sources);
	table_construct(new_table,
	                table_name,
	                alias,
	                self->sources->size - 1,
	                self->join);


	if (schema_name != NULL) {
		new_table->schema->name = string_take(schema_name);
	}

	char* unused = node_pop(source_stack);
	while (unused != NULL) {
		fprintf(stderr, "Ignored table qualification: `%s'\n", unused);
		free_(unused);
		unused = node_pop(source_stack);
	}

	if (istring_eq(table_name, "__stdin")) {
		if (fql->props.allow_stdin) {
			new_table->is_stdin = true;
			fql->props.allow_stdin = false; /* Only one allowed... */
			return FQL_GOOD;                /* avoid op_match_table_alias */
		}
		fputs("Unable to resolve table __STDIN\n", stderr);
		return FQL_FAIL;
	}

	/* Try to match the operation table.  For example:
	 *
	 * DELETE t1   --             ...And we want to try and match this
	 * FROM t1     -- You are here...
	 * WHERE FOO > 0
	 *
	 * We send the alias because:
	 *
	 * DELETE t2   -- Trying to match by alias here.
	 * FROM t1
	 * JOIN [dumb table name.txt] t2  -- you could also be here...
	 *
	 */
	op_match_table_alias(self->op, new_table);

	return FQL_GOOD;
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

void query_apply_expression_alias(query* self, const char* alias)
{
	fqlselect_apply_expression_alias(self->op, alias);
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
	self->into_table_name = table_name;
	return FQL_GOOD;
}

int query_enter_aggregate(query* self, enum aggregate_function agg_type)
{
	if (self->groupby == NULL) {
		self->groupby = new_(group);
	}
	aggregate* agg = new_(aggregate, agg_type);
	vec_push_back(&self->groupby->aggregates, &agg);

	expression* group_expr = new_(expression, EXPR_AGGREGATE, agg, "");
	group_add_expression(self->groupby, group_expr);

	expression* linked_expr = new_(expression, EXPR_AGGREGATE, agg, "");
	expression_link(linked_expr, group_expr, NULL);
	agg->linked_expression = linked_expr;
	try_(_distribute_expression(self, linked_expr));

	agg->return_mode = self->mode;
	self->mode = MODE_AGGREGATE;

	return FQL_GOOD;
}

int query_exit_aggregate(query* self)
{
	aggregate** agg = vec_back(&self->groupby->aggregates);
	self->mode = (*agg)->return_mode;
	return FQL_GOOD;
}

int query_init_op(query* self, struct fqlhandle* fql, enum op operation)
{
	switch (operation) {
	case OP_SELECT:
		self->op = new_(fqlselect);
		break;
	case OP_DELETE:
		self->op = new_(fqldelete);
		break;
	case OP_UPDATE:
		self->op = new_(fqlupdate);
		break;
	case OP_IF:
		self->op = new_(fqlbranch, self);
		//vec_push_back(fql->cfl_stack, &self->idx);
		break;
	default:
		fprintf(stderr, "unexpected operation mode `%d'\n", self->mode);
		return FQL_FAIL;
	}
	return FQL_GOOD;
}

void query_set_op_table(query* self, const char* op_table_name)
{
	op_set_table_name(self->op, op_table_name);
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

	self->orderby = new_(order);
	return FQL_GOOD;
}

void query_enter_in_statement(query* self)
{
	logicgroup* lg = self->logic_stack->data;
	if (lg->condition == NULL) {
		lg->condition = new_(logic);
	}
	lg->condition->in_data = new_(inlist);
	lg->condition->in_data->return_mode = self->mode;
	self->mode = MODE_IN;
}

//void query_exit_in_statement(query* self)
//{
//	logicgroup* lg = self->logic_stack->data;
//	self->mode = lg->condition->in_data->return_mode;
//}


void query_assign_in_subquery(query* self, query* subquery)
{
	logicgroup* lg = self->logic_stack->data;
	lg->condition->in_data->subquery = subquery;
	lg->condition->comp_type = COMP_SUBIN;
	vec_push_back(self->subquery_const_vec, &subquery);
}

void query_add_subquery_const(query* self, query* subquery)
{
	expression* subquery_expression = new_(expression, EXPR_SUBQUERY, subquery, "");
	fqlselect* subselect = subquery->op;
	subselect->const_dest = subquery_expression;
	vec_push_back(self->subquery_const_vec, &subquery);
	_distribute_expression(self, subquery_expression);
}

void query_set_order_desc(query* self)
{
	expression** expr = vec_back(&self->orderby->expressions);
	(*expr)->descending = true;
}

int query_enter_union(query* self, query* union_query)
{
	vec_push_back(self->unions, &union_query);
	union_query->op = new_(fqlselect);
	union_query->union_id = self->unions->size;
	return FQL_GOOD;
}

int query_exit_union(query* self, query* union_query)
{
	/* We need to track the expression vector
	 * from the union's select. Since we are
	 * tracking the vector itself, it will
	 * still be correct after all the schemas
	 * have resolved. select assumed...
	 */
	fqlselect* select = self->op;
	node_enqueue(&select->union_selects, union_query->op);

	return FQL_GOOD;
}

int _add_function(query* self, function* func, enum field_type type)
{
	expression* expr = new_(expression, EXPR_FUNCTION, func, "");
	expr->field_type = type;

	if (_distribute_expression(self, expr)) {
		fprintf(stderr, "unhandled function: %s\n", function_get_name(func));
		return FQL_FAIL;
	}
	node_push(&self->function_stack, expr);
	return FQL_GOOD;
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

int query_apply_data_type(query* self, const char* type_str)
{
	expression* cast_expr = self->function_stack->data;
	if (istring_eq(type_str, "int") || istring_eq(type_str, "bigint")
	    || istring_eq(type_str, "tinyint") || istring_eq(type_str, "smallint")) {
		cast_expr->field.fn->call__ = &fql_cast_int;
		cast_expr->field_type = FIELD_INT;
	} else if (istring_eq(type_str, "bit")) {
		cast_expr->field.fn->call__ = &fql_cast_bit;
		cast_expr->field_type = FIELD_INT;
	} else if (istring_eq(type_str, "float") || istring_eq(type_str, "real")) {
		cast_expr->field.fn->call__ = &fql_cast_float;
		cast_expr->field_type = FIELD_FLOAT;
	} else if (istring_eq(type_str, "varchar") || istring_eq(type_str, "nvarchar")
	           || istring_eq(type_str, "text") || istring_eq(type_str, "ntext")) {
		if (cast_expr->field.fn->args->size == 2) {
			cast_expr->field.fn->call__ = &fql_left;
		} else {
			cast_expr->field.fn->call__ = &fql_cast_string;
		}
		cast_expr->field_type = FIELD_STRING;
	} else if (istring_eq(type_str, "char") || istring_eq(type_str, "nchar")) {
		cast_expr->field.fn->call__ = &fql_cast_char;
		cast_expr->field_type = FIELD_STRING;
	} else {
		fprintf(stderr, "Type not implemented: `%s'\n", type_str);
		return FQL_FAIL;
	}

	return FQL_GOOD;
}

void query_exit_non_select_op(query* self, struct fqlhandle* fql)
{
	if (!vec_empty(self->sources)) {
		return;
	}

	const char* op_table_name = op_get_table_name(self->op);
	char* table_name_dup = strdup(op_table_name);

	node* fake_source_stack = NULL;
	node_push(&fake_source_stack, table_name_dup);
	query_add_source(self, fql, &fake_source_stack, table_name_dup);

	/* query_add_source will free the stack and its data */
}

void query_exit_function(query* self)
{
	node_pop(&self->function_stack);
}

int query_enter_case_expression(query* self)
{
	switchcase* new_sc = new_(switchcase);
	expression* expr = new_(expression, EXPR_SWITCH_CASE, new_sc, "");
	expr->field_type = FIELD_UNDEFINED;

	if (_distribute_expression(self, expr)) {
		fputs("unhandled switch case\n", stderr);
		return FQL_FAIL;
	}
	node_push(&self->switchcase_stack, new_sc);

	new_sc->return_mode = self->mode;
	new_sc->return_logic_mode = self->logic_mode;
	new_sc->return_logic_stack = self->logic_stack;
	new_sc->return_function_stack = self->function_stack;
	new_sc->return_joinable = self->joinable;
	self->mode = MODE_CASE;
	self->logic_mode = LOGIC_CASE;
	self->logic_stack = NULL;
	self->function_stack = NULL;
	self->joinable = NULL;
	return FQL_GOOD;
}

int query_exit_case_expression(query* self)
{
	switchcase* sc = node_pop(&self->switchcase_stack);
	self->mode = sc->return_mode;
	self->logic_mode = sc->return_logic_mode;
	self->function_stack = sc->return_function_stack;
	self->logic_stack = sc->return_logic_stack;
	self->joinable = sc->return_joinable;
	return FQL_GOOD;
}

int query_enter_switch_section(query* self)
{
	switchcase* sc = self->switchcase_stack->data;
	sc->state = SWITCH_STATIC_CMP;

	if (sc->static_expr == NULL) {
		fputs("reached WHEN branch without static case\n", stderr);
		return FQL_FAIL;
	}

	expression* static_expr_copy = expression_copy(sc->static_expr);
	//query_enter_search(self);
	//query_enter_search_and(self);
	query_enter_predicate(self, 0, false);
	_add_logic_expression(self, static_expr_copy);
	query_set_logic_comparison(self, "=");
	return FQL_GOOD;
}
int query_exit_switch_section(query* self)
{
	query_exit_predicate(self, 0, false);
	switchcase* sc = self->switchcase_stack->data;
	sc->state = SWITCH_VALUE;
	return FQL_GOOD;
}
int query_enter_switch_search(query* self)
{
	switchcase* sc = self->switchcase_stack->data;
	sc->state = SWITCH_LOGIC_GROUP;
	return FQL_GOOD;
}
int query_exit_switch_search(query* self)
{
	switchcase* sc = self->switchcase_stack->data;
	sc->state = SWITCH_VALUE;
	return FQL_GOOD;
}

void query_set_logic_comparison(query* self, const char* op)
{
	logicgroup* lg = self->logic_stack->data;
	logic* logic = lg->condition;
	logic_set_comparison(logic, op);
}

void query_set_logic_isnull(query* self, bool negation)
{
	logicgroup* lg = self->logic_stack->data;
	lg->negation = negation;
	logic* logic = lg->condition;
	logic_set_comparison(logic, "NULL");
}

void _assign_logic(query* self, logicgroup* lg)
{
	switch (self->logic_mode) {
	case LOGIC_IF:
		fqlbranch_add_logicgroup(self->op, lg);
		lg->jump_location = self->idx + 1;
		break;
	case LOGIC_JOIN: {
		table* table = vec_back(self->sources);
		table->condition = lg;
		break;
	}
	case LOGIC_WHERE:
		self->where = lg;
		break;
	case LOGIC_HAVING: {
		/* Having allows us to assume select */
		fqlselect* select = self->op;
		select->must_run_once = false;
		self->having = lg;
		break;
	}
	case LOGIC_CASE:
		switchcase_add_logicgroup(self->switchcase_stack->data, lg);
		break;
	default:
		fprintf(stderr, "unexpected logic mode assigning group\n");
	}
}

logicgroup* _add_logic_item(query* self, enum logicgroup_type type)
{
	logicgroup* lg = new_(logicgroup, type);

	if (self->logic_stack == NULL) {
		if (self->mode != MODE_CASE) {
			self->mode = MODE_SEARCH;
		}
		lg->expressions = new_t_(vec, expression*);
		self->joinable = new_t_(vec, logic*);
	} else {
		logicgroup* parent = self->logic_stack->data;
		if (parent->items[0] == NULL) {
			parent->items[0] = lg;
		} else {
			parent->items[1] = lg;
		}
	}
	node_push(&self->logic_stack, lg);
	return lg;
}

void _check_for_logic_exit(query* self, logicgroup* last)
{
	if (self->logic_stack == NULL) {
		if (self->joinable && vec_empty(self->joinable)) {
			delete_(vec, self->joinable);
		} else {
			last->joinable = self->joinable;
			self->joinable = NULL;
		}

		_assign_logic(self, last);
		if (self->logic_mode != LOGIC_CASE) {
			self->logic_mode = LOGIC_UNDEFINED;
		}
	}
}

void query_enter_search_or(query* self)
{
	_add_logic_item(self, LG_OR);
}

void query_enter_search_and(query* self)
{
	_add_logic_item(self, LG_AND);
}

void query_exit_search_item(query* self)
{
	_check_for_logic_exit(self, node_pop(&self->logic_stack));
}

void query_enter_predicate(query* self, unsigned negation, bool in)
{
	logicgroup* lg = _add_logic_item(self, LG_PREDICATE);
	lg->negation = (negation % 2);

	if (in) {
		query_enter_in_statement(self);
	}
}

void query_exit_predicate(query* self, bool in, bool like)
{
	logicgroup* lg = self->logic_stack->data;
	if (in) {
		query_set_logic_comparison(self, "IN");
		self->mode = lg->condition->in_data->return_mode;
	} else if (like) {
		query_set_logic_comparison(self, "LIKE");
	}

	node_pop(&self->logic_stack);

	/* pre-requisite test for joinability:
	 * A complete test for joinablity cannot occur
	 * until all logic expressions have been resolved
	 * to a source.
	 */
	if (self->logic_mode != LOGIC_HAVING && lg->condition != NULL
	    && lg->condition->comp_type == COMP_EQ
	    && lg->condition->expr[0]->expr != EXPR_CONST
	    && lg->condition->expr[1]->expr != EXPR_CONST) {
		vec_push_back(self->joinable, &lg->condition);
	}

	_check_for_logic_exit(self, lg);
}
