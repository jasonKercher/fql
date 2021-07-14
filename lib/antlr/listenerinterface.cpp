#include "listenerinterface.h"

#include <cstring>

#include "fql.h"
#include "query.h"
//#include "expression.h"
#include "util/util.h"

/** Utility functions **/

void _trim_end_chars(std::string& s, char start, char end)
{
	if(s.length() <= 1) {
		return;
	}
	if(s[0] == start && s[s.length() - 1] == end) {
		s = s.substr(1, s.length() - 2);
	}
}

void _find_replace(std::string& s, const std::string& match, const std::string replacement)
{
	size_t pos = 0;
	while ((pos = s.find(match, pos)) != std::string::npos) {
	     s.replace(pos, match.length(), replacement);
	     pos += replacement.length();
	}
}

ListenerInterface::ListenerInterface(struct fql_handle* fql,
		                     TreeWalker* walker,
		                     const std::vector<std::string>& rules)
{
	_table_name[0] = '\0';
	_table_alias[0] = '\0';
	_rule_names = rules;
	_walker = walker;
	_fql = fql;

	_next_tok_type = TOK_UNDEFINED;
	_tok_type = TOK_UNDEFINED;
}

int ListenerInterface::get_return_code()
{
	return _return_code;
}

void ListenerInterface::enterSelect_list(TSqlParser::Select_listContext * ctx)
{
	_tok_type = TOK_COLUMN_NAME;
	_next_tok_type = TOK_TABLE_NAME;
}
void ListenerInterface::exitSelect_list(TSqlParser::Select_listContext * ctx)
{
	_query->mode = MODE_INTO;
}

void ListenerInterface::enterGroup_by_item(TSqlParser::Group_by_itemContext * ctx)
{
	query_init_groupby(_query);
}
void ListenerInterface::exitGroup_by_item(TSqlParser::Group_by_itemContext* ctx)
{
	_query->mode = MODE_UNDEFINED;
}

void ListenerInterface::enterAsterisk(TSqlParser::AsteriskContext * ctx)
{
	_on_asterisk = true;
}
void ListenerInterface::exitAsterisk(TSqlParser::AsteriskContext * ctx) { }

void ListenerInterface::enterColumn_elem(TSqlParser::Column_elemContext * ctx) { }
void ListenerInterface::exitColumn_elem(TSqlParser::Column_elemContext * ctx)
{
	_table_name[0] = '\0';
}

void ListenerInterface::enterExpression_elem(TSqlParser::Expression_elemContext * ctx) { }
void ListenerInterface::exitExpression_elem(TSqlParser::Expression_elemContext * ctx) { }

void ListenerInterface::enterSelect_list_elem(TSqlParser::Select_list_elemContext * ctx)
{
	_query->mode = MODE_SELECT;
	_tok_type = TOK_COLUMN_NAME;
}
void ListenerInterface::exitSelect_list_elem(TSqlParser::Select_list_elemContext * ctx)
{
	if (_on_asterisk) {
		if (query_add_asterisk(_query, _table_name)) {
			_return_code = FQL_FAIL;
			_walker->set_walking(false);
		}
		_on_asterisk = false;
	}
	_table_name[0] = '\0';
}

void ListenerInterface::enterTable_sources(TSqlParser::Table_sourcesContext * ctx)
{
	_query->mode = MODE_SOURCES;
}
void ListenerInterface::exitTable_sources(TSqlParser::Table_sourcesContext * ctx)
{
	_query->mode = MODE_UNDEFINED;
}

void ListenerInterface::enterTable_source(TSqlParser::Table_sourceContext * ctx) { }
void ListenerInterface::exitTable_source(TSqlParser::Table_sourceContext * ctx)
{
	/* Can this be combined with exittable_source_item? */
	//query_apply_table_alias(_query, _table_alias);
}

void ListenerInterface::enterTable_source_item_joined(TSqlParser::Table_source_item_joinedContext * ctx) { }
void ListenerInterface::exitTable_source_item_joined(TSqlParser::Table_source_item_joinedContext * ctx) { }

void ListenerInterface::enterTable_source_item(TSqlParser::Table_source_itemContext * ctx)
{
	_subquery = NULL;
}
void ListenerInterface::exitTable_source_item(TSqlParser::Table_source_itemContext * ctx)
{
	if (_subquery == NULL) {
		query_add_source(_query, &_source_stack, _table_alias);
	} else {
	        query_add_subquery_source(_query, _subquery, _table_alias);
	}

	_subquery = NULL;
	*_table_alias = '\0';
}

void ListenerInterface::enterJoin_part(TSqlParser::Join_partContext * ctx)
{
	std::string error_token = "";
	if (ctx->APPLY()) {
		error_token = "APPLY";
	} else if (ctx->MERGE()) {
		error_token = "MERGE";
	} else if (ctx->PIVOT()) {
		error_token = "PIVOT";
	} else if (ctx->UNPIVOT()) {
		error_token = "UNPIVOT";
	} else if (ctx->LOOP()) {
		error_token = "LOOP";
	} else if (ctx->REMOTE()) {
		error_token = "REMOTE";
	} else if (!ctx->JOIN()) {
		std::cerr << "Keyword `JOIN' expected but not found\n";
		_return_code = FQL_FAIL;
		_walker->set_walking(false);
	}

	if (!error_token.empty()) {
		std::cerr << "Keyword `" << error_token << "' currently not supported\n";
		_return_code = FQL_FAIL;
		_walker->set_walking(false);
	}

	if (ctx->LEFT()) {
		_query->join = JOIN_LEFT;
	} else if (ctx->RIGHT()) {
		_query->join = JOIN_RIGHT;
	} else if (ctx->FULL()) {
		_query->join = JOIN_FULL;
	} else if (ctx->CROSS()) {
		_query->join = JOIN_CROSS;
	} else {
		_query->join = JOIN_INNER;
	}

	_query->logic_mode = LOGIC_JOIN;
}
void ListenerInterface::exitJoin_part(TSqlParser::Join_partContext * ctx)
{
	_query->logic_mode = LOGIC_UNDEFINED;
}

void ListenerInterface::enterTable_name_with_hint(TSqlParser::Table_name_with_hintContext * ctx)
{
	_next_tok_type = TOK_TABLE_SOURCE;
}
void ListenerInterface::exitTable_name_with_hint(TSqlParser::Table_name_with_hintContext * ctx) { }

void ListenerInterface::enterDerived_table(TSqlParser::Derived_tableContext * ctx) { }
void ListenerInterface::exitDerived_table(TSqlParser::Derived_tableContext * ctx) { }

void ListenerInterface::enterAs_column_alias(TSqlParser::As_column_aliasContext * ctx) { }
void ListenerInterface::exitAs_column_alias(TSqlParser::As_column_aliasContext * ctx) { }

void ListenerInterface::enterColumn_alias(TSqlParser::Column_aliasContext * ctx)
{
	_tok_type = TOK_COLUMN_ALIAS;
}

void ListenerInterface::exitColumn_alias(TSqlParser::Column_aliasContext * ctx) { }

void ListenerInterface::enterAs_table_alias(TSqlParser::As_table_aliasContext * ctx) { }
void ListenerInterface::exitAs_table_alias(TSqlParser::As_table_aliasContext * ctx) { }

void ListenerInterface::enterTable_alias(TSqlParser::Table_aliasContext * ctx)
{
	_tok_type = TOK_TABLE_ALIAS;
}
void ListenerInterface::exitTable_alias(TSqlParser::Table_aliasContext * ctx) { }

void ListenerInterface::enterExpression_list(TSqlParser::Expression_listContext * ctx) { }
void ListenerInterface::exitExpression_list(TSqlParser::Expression_listContext * ctx) { }

void ListenerInterface::enterTable_name(TSqlParser::Table_nameContext * ctx)
{
	if (_query->mode == MODE_INTO) {
		_tok_type = TOK_INTO_TABLE;
		return;
	}
	_tok_type = _next_tok_type;
}
void ListenerInterface::exitTable_name(TSqlParser::Table_nameContext * ctx)
{
	if (_tok_type == TOK_TABLE_NAME) {
		_tok_type = TOK_COLUMN_NAME;
	}
}

void ListenerInterface::enterFunc_proc_name_schema(TSqlParser::Func_proc_name_schemaContext * ctx) { }
void ListenerInterface::exitFunc_proc_name_schema(TSqlParser::Func_proc_name_schemaContext * ctx) { }

void ListenerInterface::enterFunc_proc_name_database_schema(TSqlParser::Func_proc_name_database_schemaContext * ctx) { }
void ListenerInterface::exitFunc_proc_name_database_schema(TSqlParser::Func_proc_name_database_schemaContext * ctx) { }

void ListenerInterface::enterFunc_proc_name_server_database_schema(TSqlParser::Func_proc_name_server_database_schemaContext * ctx) { }
void ListenerInterface::exitFunc_proc_name_server_database_schema(TSqlParser::Func_proc_name_server_database_schemaContext * ctx) { }

void ListenerInterface::enterFull_column_name(TSqlParser::Full_column_nameContext * ctx)
{
	_tok_type = TOK_COLUMN_NAME;
	_next_tok_type = TOK_TABLE_NAME;
}
void ListenerInterface::exitFull_column_name(TSqlParser::Full_column_nameContext * ctx) { }

void ListenerInterface::enterNull_notnull(TSqlParser::Null_notnullContext * ctx) { }
void ListenerInterface::exitNull_notnull(TSqlParser::Null_notnullContext * ctx) { }

void ListenerInterface::enterTsql_file(TSqlParser::Tsql_fileContext * ctx) { }
void ListenerInterface::exitTsql_file(TSqlParser::Tsql_fileContext * ctx) { }

void ListenerInterface::enterBatch(TSqlParser::BatchContext * ctx) { }
void ListenerInterface::exitBatch(TSqlParser::BatchContext * ctx) { }

void ListenerInterface::enterSql_clauses(TSqlParser::Sql_clausesContext * ctx) { }
void ListenerInterface::exitSql_clauses(TSqlParser::Sql_clausesContext * ctx) { }

void ListenerInterface::enterSql_clause(TSqlParser::Sql_clauseContext * ctx)
{
	_query = query_new(0);
	queue_enqueue(&_fql->query_list, _query);
	stack_push(&_query_stack, _query);
}
void ListenerInterface::exitSql_clause(TSqlParser::Sql_clauseContext * ctx)
{
	_query->query_total = _query_id + 1;
	stack_free(&_query_stack);
}

void ListenerInterface::enterDml_clause(TSqlParser::Dml_clauseContext * ctx) { }
void ListenerInterface::exitDml_clause(TSqlParser::Dml_clauseContext * ctx) { }

void ListenerInterface::enterConstant(TSqlParser::ConstantContext * ctx)
{
	std::string new_str = ctx->getText();
	if (new_str[0] == '\'' && new_str.length() > 2) {
		_find_replace(new_str, "''", "'");
	}
	if (query_add_constant(_query, new_str.c_str(), new_str.length())) {
		_walker->set_walking(false);
		_return_code = FQL_FAIL;
	}
}
void ListenerInterface::exitConstant(TSqlParser::ConstantContext * ctx) { }

void ListenerInterface::enterSign(TSqlParser::SignContext * ctx) { }
void ListenerInterface::exitSign(TSqlParser::SignContext * ctx) { }

void ListenerInterface::enterUnary_operator_expression(TSqlParser::Unary_operator_expressionContext * ctx)
{
	if (ctx->MINUS()) {
		if (query_enter_operator(_query, SCALAR_OP_UNARY_MINUS)) {
			_return_code = FQL_FAIL;
			_walker->set_walking(false);
		}
		return;
	}

	if (ctx->BIT_NOT()) {
		if (query_enter_operator(_query, SCALAR_OP_UNARY_BIT_NOT)) {
			_return_code = FQL_FAIL;
			_walker->set_walking(false);
		}
	}
}
void ListenerInterface::exitUnary_operator_expression(TSqlParser::Unary_operator_expressionContext * ctx)
{
	if (ctx->MINUS()
	 || ctx->BIT_NOT()) {
		query_exit_function(_query);
	}
}

void ListenerInterface::enterId(TSqlParser::IdContext * ctx)
{
	std::string new_str = ctx->getText();

	if (new_str[0] == '\'') {
		_trim_end_chars(new_str, '\'', '\'');
		_find_replace(new_str, "''", "'");
	} else if (new_str[0] == '[') {
		_trim_end_chars(new_str, '[', ']');
	}

	char* token = strdup(new_str.c_str());

	int ret = 0;

	switch (_tok_type) {
	case TOK_COLUMN_NAME:
		if (query_add_expression(_query, token, _table_name)) {
			_walker->set_walking(false);
			_return_code = FQL_FAIL;
		}
		/* consume table designation */
		_table_name[0] = '\0';
		free_(token);
		break;
	case TOK_COLUMN_ALIAS:
		query_apply_expression_alias(_query, token);
		free_(token);
		break;
	case TOK_INTO_TABLE:
		ret = query_set_into_table(_query, token);
		if (ret) {
			_walker->set_walking(false);
			_return_code = FQL_FAIL;
		}
		break;
	case TOK_TABLE_NAME:
		strncpy_(_table_name, token, TABLE_NAME_MAX);
		free_(token);
		break;
	case TOK_TABLE_SOURCE:
		stack_push(&_source_stack, token);
		break;
	case TOK_TABLE_ALIAS:
		strncpy_(_table_alias, token, TABLE_NAME_MAX);
		free_(token);
		break;
	default:
		std::cerr << "Undefined list: " << _tok_type << '\n';
		free_(token);
	}
}
void ListenerInterface::exitId(TSqlParser::IdContext * ctx) { }

void ListenerInterface::enterSimple_id(TSqlParser::Simple_idContext * ctx) { }
void ListenerInterface::exitSimple_id(TSqlParser::Simple_idContext * ctx) { }

void ListenerInterface::enterComparison_operator(TSqlParser::Comparison_operatorContext * ctx)
{
	query_set_logic_comparison(_query, ctx->getText().c_str(), false);
}
void ListenerInterface::exitComparison_operator(TSqlParser::Comparison_operatorContext * ctx)
{
}

void ListenerInterface::enterSCALAR_FUNCTION(TSqlParser::SCALAR_FUNCTIONContext * ctx)
{
       /* Some functions may enter here only */
}
void ListenerInterface::exitSCALAR_FUNCTION(TSqlParser::SCALAR_FUNCTIONContext * ctx)
{
	query_exit_function(_query);
}

void ListenerInterface::enterScalar_function_name(TSqlParser::Scalar_function_nameContext * ctx)
{
	enum scalar_function fn = SCALAR_UNDEFINED;

	if (ctx->ABS())        fn = SCALAR_ABS;
	if (ctx->ASCII())      fn = SCALAR_ASCII;
	if (ctx->CEILING())    fn = SCALAR_CEILING;
	if (ctx->CHAR())       fn = SCALAR_CHAR;
	if (ctx->CHARINDEX())  fn = SCALAR_CHARINDEX;
	if (ctx->CHECKSUM())   fn = SCALAR_CHECKSUM;
	if (ctx->DATALENGTH()) fn = SCALAR_DATALENGTH;
	if (ctx->DAY())        fn = SCALAR_DAY;
	if (ctx->FLOOR())      fn = SCALAR_FLOOR;
	if (ctx->ISDATE())     fn = SCALAR_ISDATE;
	if (ctx->ISNUMERIC())  fn = SCALAR_ISNUMERIC;
	if (ctx->LEFT())       fn = SCALAR_LEFT;
	if (ctx->LEN())        fn = SCALAR_LEN;
	if (ctx->LOWER())      fn = SCALAR_LOWER;
	if (ctx->LTRIM())      fn = SCALAR_LTRIM;
	if (ctx->MONTH())      fn = SCALAR_MONTH;
	if (ctx->NCHAR())      fn = SCALAR_NCHAR;
	if (ctx->PATINDEX())   fn = SCALAR_PATINDEX;
	if (ctx->RAND())       fn = SCALAR_RAND;
	if (ctx->REPLACE())    fn = SCALAR_REPLACE;
	if (ctx->RIGHT())      fn = SCALAR_RIGHT;
	if (ctx->ROUND())      fn = SCALAR_ROUND;
	if (ctx->RTRIM())      fn = SCALAR_RTRIM;
	if (ctx->SIGN())       fn = SCALAR_SIGN;
	if (ctx->SPACE())      fn = SCALAR_SPACE;
	if (ctx->STR())        fn = SCALAR_STR;
	if (ctx->SUBSTRING())  fn = SCALAR_SUBSTRING;
	if (ctx->UPPER())      fn = SCALAR_UPPER;
	if (ctx->USER_NAME())  fn = SCALAR_USER_NAME;
	if (ctx->YEAR())       fn = SCALAR_YEAR;

	int ret = query_enter_function(_query, fn, _fql->props.char_as_byte);

	if (ret) {
		_walker->set_walking(false);
		_return_code = ret;
	}
}
void ListenerInterface::exitScalar_function_name(TSqlParser::Scalar_function_nameContext * ctx) { }

void ListenerInterface::enterSelect_statement(TSqlParser::Select_statementContext * ctx)
{
	_query->mode = MODE_SELECT;
	int ret = query_init_op(_query);
	if (ret) {
		_return_code = ret;
		_walker->set_walking(false);
	}
}

void ListenerInterface::exitSelect_statement(TSqlParser::Select_statementContext * ctx)
{

}

void ListenerInterface::enterExpression(TSqlParser::ExpressionContext * ctx)
{
	enum scalar_function op = SCALAR_UNDEFINED;
	if      (ctx->PLUS())    op = SCALAR_OP_PLUS;
	else if (ctx->MINUS())   op = SCALAR_OP_MINUS;
	else if (ctx->STAR())    op = SCALAR_OP_MULTIPY;
	else if (ctx->DIVIDE())  op = SCALAR_OP_DIVIDE;
	else if (ctx->MODULE())  op = SCALAR_OP_MODULE;
	else if (ctx->BIT_OR())  op = SCALAR_OP_BIT_OR;
	else if (ctx->BIT_AND()) op = SCALAR_OP_BIT_AND;
	else if (ctx->BIT_XOR()) op = SCALAR_OP_BIT_XOR;
	if (op == SCALAR_UNDEFINED) {
		//_return_code = FQL_FAIL;
		return;
	}

	query_enter_operator(_query, op);
}

void ListenerInterface::exitExpression(TSqlParser::ExpressionContext * ctx)
{
	if (ctx->PLUS()
	 || ctx->MINUS()
	 || ctx->STAR()
	 || ctx->DIVIDE()
	 || ctx->MODULE()
	 || ctx->BIT_OR()
	 || ctx->BIT_AND()
	 || ctx->BIT_XOR()) {
		query_exit_function(_query);
	}

}

void ListenerInterface::enterPrimitive_expression(TSqlParser::Primitive_expressionContext * ctx) { }
void ListenerInterface::exitPrimitive_expression(TSqlParser::Primitive_expressionContext * ctx) { }

void ListenerInterface::enterBracket_expression(TSqlParser::Bracket_expressionContext * ctx)
{
	_query->in_bracket_expression = true;
}
void ListenerInterface::exitBracket_expression(TSqlParser::Bracket_expressionContext * ctx)
{
	_query->in_bracket_expression = false;
}

void ListenerInterface::enterSubquery(TSqlParser::SubqueryContext * ctx)
{
	/* Check if an operation is already defined.
	 * If it is, this is a sub-query
	 */
	query* subquery = query_new(++_query_id);
	stack_push(&_query_stack, subquery);

	/* subquery as constant */
	if (_query->in_bracket_expression) {
		/* Handle this in subquery exit */
		_query = subquery;
		return;
	}

	switch(_query->mode) {
	case MODE_SOURCES: /* subquery as table */
		break;
	case MODE_IN:      /* subquery as list */
		query_assign_in_subquery(_query, subquery);
		break;
	default:
		std::cerr << "Unexpected mode for subquery\n";
		_return_code = FQL_FAIL;
		_walker->set_walking(false);
		return;
	}

	_query = subquery;

}
void ListenerInterface::exitSubquery(TSqlParser::SubqueryContext * ctx)
{
	_subquery = (struct query*) stack_pop(&_query_stack);
	_query = (struct query*) _query_stack->data;

	/* subquery as constant */
	if (_query->in_bracket_expression) {
		query_add_subquery_const(_query, _subquery);
	}
}

void ListenerInterface::enterSearch_condition(TSqlParser::Search_conditionContext * ctx)
{
	if (_query->logic_mode == LOGIC_UNDEFINED) {
		if (_query->expect_where && !_query->where) {
			_query->logic_mode = LOGIC_WHERE;
		} else {
			_query->logic_mode = LOGIC_HAVING;
			/* NOTE: SQL Server DOES support this. Until I see
			 *       a practical application for this, I will not.
			 */
			if (!_query->groupby) {
				std::cerr << "HAVING without GROUP BY\n";
				_walker->set_walking(false);
				_return_code = FQL_FAIL;
			}
		}
	}
	query_enter_search(_query);
}
void ListenerInterface::exitSearch_condition(TSqlParser::Search_conditionContext * ctx)
{
	query_exit_search(_query);
}

void ListenerInterface::enterSearch_condition_and(TSqlParser::Search_condition_andContext * ctx)
{
	query_enter_search_and(_query);
}
void ListenerInterface::exitSearch_condition_and(TSqlParser::Search_condition_andContext * ctx)
{
	query_exit_search_and(_query);
}

void ListenerInterface::enterSearch_condition_not(TSqlParser::Search_condition_notContext * ctx)
{
	bool negation = (ctx->NOT() != NULL);
	query_enter_search_not(_query, negation);
}
void ListenerInterface::exitSearch_condition_not(TSqlParser::Search_condition_notContext * ctx)
{
	query_exit_search_not(_query);
}

void ListenerInterface::enterPredicate(TSqlParser::PredicateContext * ctx)
{
	if (ctx->IN()) {
		query_enter_in_statement(_query);
	}
}

void ListenerInterface::exitPredicate(TSqlParser::PredicateContext * ctx)
{
	bool negation = (ctx->NOT() != NULL);
	if (ctx->IN()) {
		query_set_logic_comparison(_query, "IN", negation);
		query_exit_in_statement(_query);
	} else if (ctx->LIKE()) {
		query_set_logic_comparison(_query, "LIKE", negation);
	}
	//_query->mode = MODE_SEARCH;
}

void ListenerInterface::enterQuery_expression(TSqlParser::Query_expressionContext * ctx) { }
void ListenerInterface::exitQuery_expression(TSqlParser::Query_expressionContext * ctx) { }

void ListenerInterface::enterQuery_specification(TSqlParser::Query_specificationContext * ctx)
{
	if (ctx->DISTINCT()) {
		query_set_distinct(_query);
	}
	if (ctx->WHERE()) {
		_query->expect_where = true;
	}
}
void ListenerInterface::exitQuery_specification(TSqlParser::Query_specificationContext * ctx) { }

void ListenerInterface::enterAggregate_windowed_function(TSqlParser::Aggregate_windowed_functionContext * ctx)
{
	if (_fql->props.loose_groups) {
		std::cerr << "Cannot use aggregates with loose groups\n";
		_return_code = FQL_FAIL;
		_walker->set_walking(false);
		return;
	}

	enum aggregate_function fn = AGG_UNDEFINED;
	if (ctx->AVG())               fn = AGG_AVG;
	else if (ctx->CHECKSUM_AGG()) fn = AGG_CHECKSUM_AGG;
	else if (ctx->COUNT())        fn = AGG_COUNT;
	else if (ctx->COUNT_BIG())    fn = AGG_COUNT_BIG;
	else if (ctx->GROUPING())     fn = AGG_GROUPING;
	else if (ctx->GROUPING_ID())  fn = AGG_GROUPING_ID;
	//else if (ctx->LR_BRACKET())   fn = AGG_LR_BRACKET;
	else if (ctx->MAX())          fn = AGG_MAX;
	else if (ctx->MIN())          fn = AGG_MIN;
	//else if (ctx->RR_BRACKET())   fn = AGG_RR_BRACKET;
	//else if (ctx->STAR())         fn = AGG_STAR;
	else if (ctx->STDEV())        fn = AGG_STDEV;
	else if (ctx->STDEVP())       fn = AGG_STDEVP;
	else if (ctx->SUM())          fn = AGG_SUM;
	else if (ctx->VAR())          fn = AGG_VAR;
	else if (ctx->VARP())         fn = AGG_VARP;

	if (query_add_aggregate(_query, fn)) {
		_return_code = FQL_FAIL;
		_walker->set_walking(false);
	}
	_query->in_aggregate = true;
}
void ListenerInterface::exitAggregate_windowed_function(TSqlParser::Aggregate_windowed_functionContext * ctx)
{
	_query->in_aggregate = false;
}

void ListenerInterface::enterAGGREGATE_WINDOWED_FUNC(TSqlParser::AGGREGATE_WINDOWED_FUNCContext * ctx) { }
void ListenerInterface::exitAGGREGATE_WINDOWED_FUNC(TSqlParser::AGGREGATE_WINDOWED_FUNCContext * ctx) { }

void ListenerInterface::enterAll_distinct_expression(TSqlParser::All_distinct_expressionContext * ctx) { }
void ListenerInterface::exitAll_distinct_expression(TSqlParser::All_distinct_expressionContext * ctx) { }

void ListenerInterface::enterOrder_by_clause(TSqlParser::Order_by_clauseContext * ctx)
{
	_query->mode = MODE_ORDERBY;
	query_init_orderby(_query);
}
void ListenerInterface::exitOrder_by_clause(TSqlParser::Order_by_clauseContext * ctx) { }

void ListenerInterface::enterOrder_by_expression(TSqlParser::Order_by_expressionContext * ctx)
{
}
void ListenerInterface::exitOrder_by_expression(TSqlParser::Order_by_expressionContext * ctx)
{
	if (ctx->DESC()) {
		query_set_order_desc(_query);
	}
}

void ListenerInterface::enterTop_clause(TSqlParser::Top_clauseContext * ctx)
{
	_query->mode_store = _query->mode;
	_query->mode = MODE_TOP;
}
void ListenerInterface::exitTop_clause(TSqlParser::Top_clauseContext * ctx)
{
	_query->mode = _query->mode_store;
}

void ListenerInterface::enterTop_count(TSqlParser::Top_countContext * ctx)
{
	if (ctx->DECIMAL()) {
		query_set_top_count(_query, ctx->getText().c_str());
	}
}
void ListenerInterface::exitTop_count(TSqlParser::Top_countContext * ctx) { }

void ListenerInterface::enterCase_expression(TSqlParser::Case_expressionContext* ctx)
{
	query_enter_case_expression(_query);
}
void ListenerInterface::exitCase_expression(TSqlParser::Case_expressionContext* ctx)
{
	query_exit_case_expression(_query);
}

void ListenerInterface::enterSwitch_section(TSqlParser::Switch_sectionContext* ctx)
{
	query_enter_switch_section(_query);
}
void ListenerInterface::exitSwitch_section(TSqlParser::Switch_sectionContext* ctx)
{
	query_exit_switch_section(_query);
}

void ListenerInterface::enterSwitch_search_condition_section(
        TSqlParser::Switch_search_condition_sectionContext* ctx)
{
	query_enter_switch_search(_query);
}
void ListenerInterface::exitSwitch_search_condition_section(
        TSqlParser::Switch_search_condition_sectionContext* ctx)
{
	query_exit_switch_search(_query);
}

void ListenerInterface::enterSql_union(TSqlParser::Sql_unionContext * ctx)
{
	if (!ctx->ALL()) {
		std::cerr << "UNION without ALL is currently not supported\n";
		_return_code = FQL_FAIL;
		_walker->set_walking(false);
	}

	query* union_query = query_new(++_query_id);
	int ret = query_enter_union(_query, union_query);
	if (ret) {
		_return_code = ret;
		_walker->set_walking(false);
	}

	_query = union_query;
	stack_push(&_query_stack, _query);
	_query->mode = MODE_SELECT;
}

void ListenerInterface::exitSql_union(TSqlParser::Sql_unionContext * ctx)
{
	query* union_query = (query*)stack_pop(&_query_stack);
	_query = (query*)_query_stack->data;
	query_exit_union(_query, union_query);
}

/* Every Rule Operations */

void ListenerInterface::enterEveryRule(antlr4::ParserRuleContext * ctx)
{
	if(_fql->props.verbose)
		std::cerr << "ENTER: " << _rule_names[ctx->getRuleIndex()] << " : " << ctx->getText() << std::endl;

	if(_error_tokens.size() > 0)
	{
		std::cerr << "The following keyword is currently not supported:\n";
		std::cerr << _error_tokens[0];
		_error_tokens.pop_back();

		if(_fql->props.override_warnings) {
			std::cerr << "\nCAUTION: Overriding the above warnings! Results may be incorrect!\n";
		} else {
			std::cerr << "\nTerminated: Use -O to Override at your own risk.\n";
			_return_code = FQL_FAIL;
			_walker->set_walking(false);
		}
	}
}

void ListenerInterface::exitEveryRule(antlr4::ParserRuleContext * ctx)
{
	if(_fql->props.verbose)
		std::cerr << "EXIT:  " << _rule_names[ctx->getRuleIndex()] << " : " << ctx->getText() << std::endl;
}

/**
 * Not Implemented
 */

void ListenerInterface::_no_impl(const std::string& text, int rule_idx)
{
	_error_tokens.push_back(text + " ( " + _rule_names[rule_idx] + " )");
}

void ListenerInterface::enterDdl_clause(TSqlParser::Ddl_clauseContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDdl_clause(TSqlParser::Ddl_clauseContext * ctx) { }

void ListenerInterface::enterCfl_statement(TSqlParser::Cfl_statementContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCfl_statement(TSqlParser::Cfl_statementContext * ctx) { }

void ListenerInterface::enterBlock_statement(TSqlParser::Block_statementContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitBlock_statement(TSqlParser::Block_statementContext * ctx) { }

void ListenerInterface::enterBreak_statement(TSqlParser::Break_statementContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitBreak_statement(TSqlParser::Break_statementContext * ctx) { }

void ListenerInterface::enterContinue_statement(TSqlParser::Continue_statementContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitContinue_statement(TSqlParser::Continue_statementContext * ctx) { }

void ListenerInterface::enterGoto_statement(TSqlParser::Goto_statementContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitGoto_statement(TSqlParser::Goto_statementContext * ctx) { }

void ListenerInterface::enterReturn_statement(TSqlParser::Return_statementContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitReturn_statement(TSqlParser::Return_statementContext * ctx) { }

void ListenerInterface::enterIf_statement(TSqlParser::If_statementContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitIf_statement(TSqlParser::If_statementContext * ctx) { }

void ListenerInterface::enterThrow_statement(TSqlParser::Throw_statementContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitThrow_statement(TSqlParser::Throw_statementContext * ctx) { }

void ListenerInterface::enterThrow_error_number(TSqlParser::Throw_error_numberContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitThrow_error_number(TSqlParser::Throw_error_numberContext * ctx) { }

void ListenerInterface::enterThrow_message(TSqlParser::Throw_messageContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitThrow_message(TSqlParser::Throw_messageContext * ctx) { }

void ListenerInterface::enterThrow_state(TSqlParser::Throw_stateContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitThrow_state(TSqlParser::Throw_stateContext * ctx) { }

void ListenerInterface::enterTry_catch_statement(TSqlParser::Try_catch_statementContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitTry_catch_statement(TSqlParser::Try_catch_statementContext * ctx) { }

void ListenerInterface::enterWaitfor_statement(TSqlParser::Waitfor_statementContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitWaitfor_statement(TSqlParser::Waitfor_statementContext * ctx) { }

void ListenerInterface::enterWhile_statement(TSqlParser::While_statementContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitWhile_statement(TSqlParser::While_statementContext * ctx) { }

void ListenerInterface::enterPrint_statement(TSqlParser::Print_statementContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitPrint_statement(TSqlParser::Print_statementContext * ctx) { }

void ListenerInterface::enterRaiseerror_statement(TSqlParser::Raiseerror_statementContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitRaiseerror_statement(TSqlParser::Raiseerror_statementContext * ctx) { }

void ListenerInterface::enterEmpty_statement(TSqlParser::Empty_statementContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitEmpty_statement(TSqlParser::Empty_statementContext * ctx) { }

void ListenerInterface::enterAnother_statement(TSqlParser::Another_statementContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAnother_statement(TSqlParser::Another_statementContext * ctx) { }

void ListenerInterface::enterDrop_aggregate(TSqlParser::Drop_aggregateContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_aggregate(TSqlParser::Drop_aggregateContext * ctx) { }

void ListenerInterface::enterEntity_to(TSqlParser::Entity_toContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitEntity_to(TSqlParser::Entity_toContext * ctx) { }

void ListenerInterface::enterColon_colon(TSqlParser::Colon_colonContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitColon_colon(TSqlParser::Colon_colonContext * ctx) { }

void ListenerInterface::enterClass_type(TSqlParser::Class_typeContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitClass_type(TSqlParser::Class_typeContext * ctx) { }

void ListenerInterface::enterClass_type_for_sql_database(TSqlParser::Class_type_for_sql_databaseContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitClass_type_for_sql_database(TSqlParser::Class_type_for_sql_databaseContext * ctx) { }

void ListenerInterface::enterClass_type_for_parallel_dw(TSqlParser::Class_type_for_parallel_dwContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitClass_type_for_parallel_dw(TSqlParser::Class_type_for_parallel_dwContext * ctx) { }

void ListenerInterface::enterLock_table(TSqlParser::Lock_tableContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitLock_table(TSqlParser::Lock_tableContext * ctx) { }

void ListenerInterface::enterTruncate_table(TSqlParser::Truncate_tableContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitTruncate_table(TSqlParser::Truncate_tableContext * ctx) { }

void ListenerInterface::enterEvent_session_predicate_expression(TSqlParser::Event_session_predicate_expressionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitEvent_session_predicate_expression(TSqlParser::Event_session_predicate_expressionContext * ctx) { }

void ListenerInterface::enterEvent_session_predicate_factor(TSqlParser::Event_session_predicate_factorContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitEvent_session_predicate_factor(TSqlParser::Event_session_predicate_factorContext * ctx) { }

void ListenerInterface::enterEvent_session_predicate_leaf(TSqlParser::Event_session_predicate_leafContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitEvent_session_predicate_leaf(TSqlParser::Event_session_predicate_leafContext * ctx) { }

void ListenerInterface::enterAlter_schema_sql(TSqlParser::Alter_schema_sqlContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_schema_sql(TSqlParser::Alter_schema_sqlContext * ctx) { }

void ListenerInterface::enterCreate_schema(TSqlParser::Create_schemaContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_schema(TSqlParser::Create_schemaContext * ctx) { }

void ListenerInterface::enterCreate_xml_schema_collection(TSqlParser::Create_xml_schema_collectionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_xml_schema_collection(TSqlParser::Create_xml_schema_collectionContext * ctx) { }

void ListenerInterface::enterCreate_queue(TSqlParser::Create_queueContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_queue(TSqlParser::Create_queueContext * ctx) { }

void ListenerInterface::enterQueue_settings(TSqlParser::Queue_settingsContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitQueue_settings(TSqlParser::Queue_settingsContext * ctx) { }

void ListenerInterface::enterAlter_queue(TSqlParser::Alter_queueContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_queue(TSqlParser::Alter_queueContext * ctx) { }

void ListenerInterface::enterQueue_action(TSqlParser::Queue_actionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitQueue_action(TSqlParser::Queue_actionContext * ctx) { }

void ListenerInterface::enterQueue_rebuild_options(TSqlParser::Queue_rebuild_optionsContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitQueue_rebuild_options(TSqlParser::Queue_rebuild_optionsContext * ctx) { }

void ListenerInterface::enterConversation_statement(TSqlParser::Conversation_statementContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitConversation_statement(TSqlParser::Conversation_statementContext * ctx) { }

void ListenerInterface::enterMessage_statement(TSqlParser::Message_statementContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitMessage_statement(TSqlParser::Message_statementContext * ctx) { }

void ListenerInterface::enterMerge_statement(TSqlParser::Merge_statementContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitMerge_statement(TSqlParser::Merge_statementContext * ctx) { }

void ListenerInterface::enterMerge_matched(TSqlParser::Merge_matchedContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitMerge_matched(TSqlParser::Merge_matchedContext * ctx) { }

void ListenerInterface::enterMerge_not_matched(TSqlParser::Merge_not_matchedContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitMerge_not_matched(TSqlParser::Merge_not_matchedContext * ctx) { }

void ListenerInterface::enterDelete_statement(TSqlParser::Delete_statementContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDelete_statement(TSqlParser::Delete_statementContext * ctx) { }

void ListenerInterface::enterDelete_statement_from(TSqlParser::Delete_statement_fromContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDelete_statement_from(TSqlParser::Delete_statement_fromContext * ctx) { }

void ListenerInterface::enterInsert_statement(TSqlParser::Insert_statementContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitInsert_statement(TSqlParser::Insert_statementContext * ctx) { }

void ListenerInterface::enterInsert_statement_value(TSqlParser::Insert_statement_valueContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitInsert_statement_value(TSqlParser::Insert_statement_valueContext * ctx) { }

void ListenerInterface::enterReceive_statement(TSqlParser::Receive_statementContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitReceive_statement(TSqlParser::Receive_statementContext * ctx) { }

void ListenerInterface::enterTime(TSqlParser::TimeContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitTime(TSqlParser::TimeContext * ctx) { }

void ListenerInterface::enterUpdate_statement(TSqlParser::Update_statementContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitUpdate_statement(TSqlParser::Update_statementContext * ctx) { }

void ListenerInterface::enterOutput_clause(TSqlParser::Output_clauseContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitOutput_clause(TSqlParser::Output_clauseContext * ctx) { }

void ListenerInterface::enterOutput_dml_list_elem(TSqlParser::Output_dml_list_elemContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitOutput_dml_list_elem(TSqlParser::Output_dml_list_elemContext * ctx) { }

void ListenerInterface::enterOutput_column_name(TSqlParser::Output_column_nameContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitOutput_column_name(TSqlParser::Output_column_nameContext * ctx) { }

void ListenerInterface::enterCreate_database(TSqlParser::Create_databaseContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_database(TSqlParser::Create_databaseContext * ctx) { }

void ListenerInterface::enterCreate_index(TSqlParser::Create_indexContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_index(TSqlParser::Create_indexContext * ctx) { }

void ListenerInterface::enterCreate_or_alter_procedure(TSqlParser::Create_or_alter_procedureContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_or_alter_procedure(TSqlParser::Create_or_alter_procedureContext * ctx) { }

void ListenerInterface::enterCreate_or_alter_function(TSqlParser::Create_or_alter_functionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_or_alter_function(TSqlParser::Create_or_alter_functionContext * ctx) { }

void ListenerInterface::enterFunc_body_returns_select(TSqlParser::Func_body_returns_selectContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitFunc_body_returns_select(TSqlParser::Func_body_returns_selectContext * ctx) { }

void ListenerInterface::enterFunc_body_returns_table(TSqlParser::Func_body_returns_tableContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitFunc_body_returns_table(TSqlParser::Func_body_returns_tableContext * ctx) { }

void ListenerInterface::enterFunc_body_returns_scalar(TSqlParser::Func_body_returns_scalarContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitFunc_body_returns_scalar(TSqlParser::Func_body_returns_scalarContext * ctx) { }

void ListenerInterface::enterProcedure_param(TSqlParser::Procedure_paramContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitProcedure_param(TSqlParser::Procedure_paramContext * ctx) { }

void ListenerInterface::enterProcedure_option(TSqlParser::Procedure_optionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitProcedure_option(TSqlParser::Procedure_optionContext * ctx) { }

void ListenerInterface::enterFunction_option(TSqlParser::Function_optionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitFunction_option(TSqlParser::Function_optionContext * ctx) { }

void ListenerInterface::enterCreate_table(TSqlParser::Create_tableContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_table(TSqlParser::Create_tableContext * ctx) { }

void ListenerInterface::enterTable_options(TSqlParser::Table_optionsContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitTable_options(TSqlParser::Table_optionsContext * ctx) { }

void ListenerInterface::enterCreate_view(TSqlParser::Create_viewContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_view(TSqlParser::Create_viewContext * ctx) { }

void ListenerInterface::enterView_attribute(TSqlParser::View_attributeContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitView_attribute(TSqlParser::View_attributeContext * ctx) { }

void ListenerInterface::enterAlter_table(TSqlParser::Alter_tableContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_table(TSqlParser::Alter_tableContext * ctx) { }

void ListenerInterface::enterCursor_option(TSqlParser::Cursor_optionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCursor_option(TSqlParser::Cursor_optionContext * ctx) { }

void ListenerInterface::enterTarget_recovery_time_option(TSqlParser::Target_recovery_time_optionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitTarget_recovery_time_option(TSqlParser::Target_recovery_time_optionContext * ctx) { }

void ListenerInterface::enterTermination(TSqlParser::TerminationContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitTermination(TSqlParser::TerminationContext * ctx) { }

void ListenerInterface::enterDrop_index(TSqlParser::Drop_indexContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_index(TSqlParser::Drop_indexContext * ctx) { }

void ListenerInterface::enterDrop_relational_or_xml_or_spatial_index(TSqlParser::Drop_relational_or_xml_or_spatial_indexContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_relational_or_xml_or_spatial_index(TSqlParser::Drop_relational_or_xml_or_spatial_indexContext * ctx) { }

void ListenerInterface::enterDrop_backward_compatible_index(TSqlParser::Drop_backward_compatible_indexContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_backward_compatible_index(TSqlParser::Drop_backward_compatible_indexContext * ctx) { }

void ListenerInterface::enterDrop_procedure(TSqlParser::Drop_procedureContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_procedure(TSqlParser::Drop_procedureContext * ctx) { }

void ListenerInterface::enterDrop_function(TSqlParser::Drop_functionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_function(TSqlParser::Drop_functionContext * ctx) { }

void ListenerInterface::enterDrop_table(TSqlParser::Drop_tableContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_table(TSqlParser::Drop_tableContext * ctx) { }

void ListenerInterface::enterDrop_view(TSqlParser::Drop_viewContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_view(TSqlParser::Drop_viewContext * ctx) { }

void ListenerInterface::enterCreate_type(TSqlParser::Create_typeContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_type(TSqlParser::Create_typeContext * ctx) { }

void ListenerInterface::enterDrop_type(TSqlParser::Drop_typeContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_type(TSqlParser::Drop_typeContext * ctx) { }

void ListenerInterface::enterRowset_function_limited(TSqlParser::Rowset_function_limitedContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitRowset_function_limited(TSqlParser::Rowset_function_limitedContext * ctx) { }

void ListenerInterface::enterOpenquery(TSqlParser::OpenqueryContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitOpenquery(TSqlParser::OpenqueryContext * ctx) { }

void ListenerInterface::enterOpendatasource(TSqlParser::OpendatasourceContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitOpendatasource(TSqlParser::OpendatasourceContext * ctx) { }

void ListenerInterface::enterDeclare_statement(TSqlParser::Declare_statementContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDeclare_statement(TSqlParser::Declare_statementContext * ctx) { }

void ListenerInterface::enterCursor_statement(TSqlParser::Cursor_statementContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCursor_statement(TSqlParser::Cursor_statementContext * ctx) { }

void ListenerInterface::enterKill_statement(TSqlParser::Kill_statementContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitKill_statement(TSqlParser::Kill_statementContext * ctx) { }

void ListenerInterface::enterKill_process(TSqlParser::Kill_processContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitKill_process(TSqlParser::Kill_processContext * ctx) { }

void ListenerInterface::enterKill_query_notification(TSqlParser::Kill_query_notificationContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitKill_query_notification(TSqlParser::Kill_query_notificationContext * ctx) { }

void ListenerInterface::enterKill_stats_job(TSqlParser::Kill_stats_jobContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitKill_stats_job(TSqlParser::Kill_stats_jobContext * ctx) { }

void ListenerInterface::enterExecute_statement(TSqlParser::Execute_statementContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitExecute_statement(TSqlParser::Execute_statementContext * ctx) { }

void ListenerInterface::enterExecute_body(TSqlParser::Execute_bodyContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitExecute_body(TSqlParser::Execute_bodyContext * ctx) { }

void ListenerInterface::enterExecute_statement_arg(TSqlParser::Execute_statement_argContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitExecute_statement_arg(TSqlParser::Execute_statement_argContext * ctx) { }

void ListenerInterface::enterExecute_var_string(TSqlParser::Execute_var_stringContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitExecute_var_string(TSqlParser::Execute_var_stringContext * ctx) { }

void ListenerInterface::enterSet_statement(TSqlParser::Set_statementContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitSet_statement(TSqlParser::Set_statementContext * ctx) { }

void ListenerInterface::enterTransaction_statement(TSqlParser::Transaction_statementContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitTransaction_statement(TSqlParser::Transaction_statementContext * ctx) { }

void ListenerInterface::enterGo_statement(TSqlParser::Go_statementContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitGo_statement(TSqlParser::Go_statementContext * ctx) { }

void ListenerInterface::enterUse_statement(TSqlParser::Use_statementContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitUse_statement(TSqlParser::Use_statementContext * ctx) { }

void ListenerInterface::enterSetuser_statement(TSqlParser::Setuser_statementContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitSetuser_statement(TSqlParser::Setuser_statementContext * ctx) { }

void ListenerInterface::enterReconfigure_statement(TSqlParser::Reconfigure_statementContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitReconfigure_statement(TSqlParser::Reconfigure_statementContext * ctx) { }

void ListenerInterface::enterShutdown_statement(TSqlParser::Shutdown_statementContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitShutdown_statement(TSqlParser::Shutdown_statementContext * ctx) { }

void ListenerInterface::enterDbcc_clause(TSqlParser::Dbcc_clauseContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDbcc_clause(TSqlParser::Dbcc_clauseContext * ctx) { }

void ListenerInterface::enterDbcc_options(TSqlParser::Dbcc_optionsContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDbcc_options(TSqlParser::Dbcc_optionsContext * ctx) { }

void ListenerInterface::enterExecute_clause(TSqlParser::Execute_clauseContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitExecute_clause(TSqlParser::Execute_clauseContext * ctx) { }

void ListenerInterface::enterDeclare_local(TSqlParser::Declare_localContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDeclare_local(TSqlParser::Declare_localContext * ctx) { }

void ListenerInterface::enterTable_type_definition(TSqlParser::Table_type_definitionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitTable_type_definition(TSqlParser::Table_type_definitionContext * ctx) { }

void ListenerInterface::enterXml_type_definition(TSqlParser::Xml_type_definitionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitXml_type_definition(TSqlParser::Xml_type_definitionContext * ctx) { }

void ListenerInterface::enterXml_schema_collection(TSqlParser::Xml_schema_collectionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitXml_schema_collection(TSqlParser::Xml_schema_collectionContext * ctx) { }

void ListenerInterface::enterColumn_def_table_constraints(TSqlParser::Column_def_table_constraintsContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitColumn_def_table_constraints(TSqlParser::Column_def_table_constraintsContext * ctx) { }

void ListenerInterface::enterColumn_def_table_constraint(TSqlParser::Column_def_table_constraintContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitColumn_def_table_constraint(TSqlParser::Column_def_table_constraintContext * ctx) { }

void ListenerInterface::enterColumn_definition(TSqlParser::Column_definitionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitColumn_definition(TSqlParser::Column_definitionContext * ctx) { }

void ListenerInterface::enterMaterialized_column_definition(TSqlParser::Materialized_column_definitionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitMaterialized_column_definition(TSqlParser::Materialized_column_definitionContext * ctx) { }

void ListenerInterface::enterColumn_constraint(TSqlParser::Column_constraintContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitColumn_constraint(TSqlParser::Column_constraintContext * ctx) { }

void ListenerInterface::enterTable_constraint(TSqlParser::Table_constraintContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitTable_constraint(TSqlParser::Table_constraintContext * ctx) { }

void ListenerInterface::enterOn_delete(TSqlParser::On_deleteContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitOn_delete(TSqlParser::On_deleteContext * ctx) { }

void ListenerInterface::enterOn_update(TSqlParser::On_updateContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitOn_update(TSqlParser::On_updateContext * ctx) { }

void ListenerInterface::enterIndex_options(TSqlParser::Index_optionsContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitIndex_options(TSqlParser::Index_optionsContext * ctx) { }

void ListenerInterface::enterIndex_option(TSqlParser::Index_optionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitIndex_option(TSqlParser::Index_optionContext * ctx) { }

void ListenerInterface::enterDeclare_cursor(TSqlParser::Declare_cursorContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDeclare_cursor(TSqlParser::Declare_cursorContext * ctx) { }

void ListenerInterface::enterDeclare_set_cursor_common(TSqlParser::Declare_set_cursor_commonContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDeclare_set_cursor_common(TSqlParser::Declare_set_cursor_commonContext * ctx) { }

void ListenerInterface::enterDeclare_set_cursor_common_partial(TSqlParser::Declare_set_cursor_common_partialContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDeclare_set_cursor_common_partial(TSqlParser::Declare_set_cursor_common_partialContext * ctx) { }

void ListenerInterface::enterFetch_cursor(TSqlParser::Fetch_cursorContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitFetch_cursor(TSqlParser::Fetch_cursorContext * ctx) { }

void ListenerInterface::enterSet_special(TSqlParser::Set_specialContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitSet_special(TSqlParser::Set_specialContext * ctx) { }

void ListenerInterface::enterConstant_LOCAL_ID(TSqlParser::Constant_LOCAL_IDContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitConstant_LOCAL_ID(TSqlParser::Constant_LOCAL_IDContext * ctx) { }

void ListenerInterface::enterConstant_expression(TSqlParser::Constant_expressionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitConstant_expression(TSqlParser::Constant_expressionContext * ctx) { }

void ListenerInterface::enterWith_expression(TSqlParser::With_expressionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitWith_expression(TSqlParser::With_expressionContext * ctx) { }

void ListenerInterface::enterCommon_table_expression(TSqlParser::Common_table_expressionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCommon_table_expression(TSqlParser::Common_table_expressionContext * ctx) { }

void ListenerInterface::enterUpdate_elem(TSqlParser::Update_elemContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitUpdate_elem(TSqlParser::Update_elemContext * ctx) { }

void ListenerInterface::enterSearch_condition_list(TSqlParser::Search_condition_listContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitSearch_condition_list(TSqlParser::Search_condition_listContext * ctx) { }

void ListenerInterface::enterTop_percent(TSqlParser::Top_percentContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitTop_percent(TSqlParser::Top_percentContext * ctx) { }

void ListenerInterface::enterFor_clause(TSqlParser::For_clauseContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitFor_clause(TSqlParser::For_clauseContext * ctx) { }

void ListenerInterface::enterXml_common_directives(TSqlParser::Xml_common_directivesContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitXml_common_directives(TSqlParser::Xml_common_directivesContext * ctx) { }

void ListenerInterface::enterOption_clause(TSqlParser::Option_clauseContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitOption_clause(TSqlParser::Option_clauseContext * ctx) { }

void ListenerInterface::enterOption(TSqlParser::OptionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitOption(TSqlParser::OptionContext * ctx) { }

void ListenerInterface::enterOptimize_for_arg(TSqlParser::Optimize_for_argContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitOptimize_for_arg(TSqlParser::Optimize_for_argContext * ctx) { }

void ListenerInterface::enterUdt_method_arguments(TSqlParser::Udt_method_argumentsContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitUdt_method_arguments(TSqlParser::Udt_method_argumentsContext * ctx) { }

void ListenerInterface::enterUdt_elem(TSqlParser::Udt_elemContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitUdt_elem(TSqlParser::Udt_elemContext * ctx) { }

void ListenerInterface::enterOpen_xml(TSqlParser::Open_xmlContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitOpen_xml(TSqlParser::Open_xmlContext * ctx) { }

void ListenerInterface::enterSchema_declaration(TSqlParser::Schema_declarationContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitSchema_declaration(TSqlParser::Schema_declarationContext * ctx) { }

void ListenerInterface::enterColumn_declaration(TSqlParser::Column_declarationContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitColumn_declaration(TSqlParser::Column_declarationContext * ctx) { }

void ListenerInterface::enterChange_table(TSqlParser::Change_tableContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitChange_table(TSqlParser::Change_tableContext * ctx) { }

void ListenerInterface::enterPivot_clause(TSqlParser::Pivot_clauseContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitPivot_clause(TSqlParser::Pivot_clauseContext * ctx) { }

void ListenerInterface::enterUnpivot_clause(TSqlParser::Unpivot_clauseContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitUnpivot_clause(TSqlParser::Unpivot_clauseContext * ctx) { }

void ListenerInterface::enterFull_column_name_list(TSqlParser::Full_column_name_listContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitFull_column_name_list(TSqlParser::Full_column_name_listContext * ctx) { }

void ListenerInterface::enterRowset_function(TSqlParser::Rowset_functionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitRowset_function(TSqlParser::Rowset_functionContext * ctx) { }

void ListenerInterface::enterBulk_option(TSqlParser::Bulk_optionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitBulk_option(TSqlParser::Bulk_optionContext * ctx) { }

void ListenerInterface::enterBINARY_CHECKSUM(TSqlParser::BINARY_CHECKSUMContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitBINARY_CHECKSUM(TSqlParser::BINARY_CHECKSUMContext * ctx) { }

void ListenerInterface::enterCAST(TSqlParser::CASTContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCAST(TSqlParser::CASTContext * ctx) { }

void ListenerInterface::enterCONVERT(TSqlParser::CONVERTContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCONVERT(TSqlParser::CONVERTContext * ctx) { }

void ListenerInterface::enterCHECKSUM(TSqlParser::CHECKSUMContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCHECKSUM(TSqlParser::CHECKSUMContext * ctx) { }

void ListenerInterface::enterCOALESCE(TSqlParser::COALESCEContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCOALESCE(TSqlParser::COALESCEContext * ctx) { }

void ListenerInterface::enterCURRENT_TIMESTAMP(TSqlParser::CURRENT_TIMESTAMPContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCURRENT_TIMESTAMP(TSqlParser::CURRENT_TIMESTAMPContext * ctx) { }

void ListenerInterface::enterCURRENT_USER(TSqlParser::CURRENT_USERContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCURRENT_USER(TSqlParser::CURRENT_USERContext * ctx) { }

void ListenerInterface::enterDATEADD(TSqlParser::DATEADDContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDATEADD(TSqlParser::DATEADDContext * ctx) { }

void ListenerInterface::enterDATEDIFF(TSqlParser::DATEDIFFContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDATEDIFF(TSqlParser::DATEDIFFContext * ctx) { }

void ListenerInterface::enterDATENAME(TSqlParser::DATENAMEContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDATENAME(TSqlParser::DATENAMEContext * ctx) { }

void ListenerInterface::enterDATEPART(TSqlParser::DATEPARTContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDATEPART(TSqlParser::DATEPARTContext * ctx) { }

void ListenerInterface::enterGETDATE(TSqlParser::GETDATEContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitGETDATE(TSqlParser::GETDATEContext * ctx) { }

void ListenerInterface::enterGETUTCDATE(TSqlParser::GETUTCDATEContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitGETUTCDATE(TSqlParser::GETUTCDATEContext * ctx) { }

void ListenerInterface::enterIDENTITY(TSqlParser::IDENTITYContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitIDENTITY(TSqlParser::IDENTITYContext * ctx) { }

void ListenerInterface::enterMIN_ACTIVE_ROWVERSION(TSqlParser::MIN_ACTIVE_ROWVERSIONContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitMIN_ACTIVE_ROWVERSION(TSqlParser::MIN_ACTIVE_ROWVERSIONContext * ctx) { }

void ListenerInterface::enterNULLIF(TSqlParser::NULLIFContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitNULLIF(TSqlParser::NULLIFContext * ctx) { }

void ListenerInterface::enterSTUFF(TSqlParser::STUFFContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitSTUFF(TSqlParser::STUFFContext * ctx) { }

void ListenerInterface::enterSESSION_USER(TSqlParser::SESSION_USERContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitSESSION_USER(TSqlParser::SESSION_USERContext * ctx) { }

void ListenerInterface::enterSYSTEM_USER(TSqlParser::SYSTEM_USERContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitSYSTEM_USER(TSqlParser::SYSTEM_USERContext * ctx) { }

void ListenerInterface::enterISNULL(TSqlParser::ISNULLContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitISNULL(TSqlParser::ISNULLContext * ctx) { }

void ListenerInterface::enterXML_DATA_TYPE_FUNC(TSqlParser::XML_DATA_TYPE_FUNCContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitXML_DATA_TYPE_FUNC(TSqlParser::XML_DATA_TYPE_FUNCContext * ctx) { }

void ListenerInterface::enterIFF(TSqlParser::IFFContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitIFF(TSqlParser::IFFContext * ctx) { }

void ListenerInterface::enterRANKING_WINDOWED_FUNC(TSqlParser::RANKING_WINDOWED_FUNCContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitRANKING_WINDOWED_FUNC(TSqlParser::RANKING_WINDOWED_FUNCContext * ctx) { }

void ListenerInterface::enterANALYTIC_WINDOWED_FUNC(TSqlParser::ANALYTIC_WINDOWED_FUNCContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitANALYTIC_WINDOWED_FUNC(TSqlParser::ANALYTIC_WINDOWED_FUNCContext * ctx) { }

void ListenerInterface::enterSTRINGAGG(TSqlParser::STRINGAGGContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitSTRINGAGG(TSqlParser::STRINGAGGContext * ctx) { }

void ListenerInterface::enterXml_data_type_methods(TSqlParser::Xml_data_type_methodsContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitXml_data_type_methods(TSqlParser::Xml_data_type_methodsContext * ctx) { }

void ListenerInterface::enterValue_method(TSqlParser::Value_methodContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitValue_method(TSqlParser::Value_methodContext * ctx) { }

void ListenerInterface::enterQuery_method(TSqlParser::Query_methodContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitQuery_method(TSqlParser::Query_methodContext * ctx) { }

void ListenerInterface::enterExist_method(TSqlParser::Exist_methodContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitExist_method(TSqlParser::Exist_methodContext * ctx) { }

void ListenerInterface::enterModify_method(TSqlParser::Modify_methodContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitModify_method(TSqlParser::Modify_methodContext * ctx) { }

void ListenerInterface::enterNodes_method(TSqlParser::Nodes_methodContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitNodes_method(TSqlParser::Nodes_methodContext * ctx) { }

void ListenerInterface::enterWith_table_hints(TSqlParser::With_table_hintsContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitWith_table_hints(TSqlParser::With_table_hintsContext * ctx) { }

void ListenerInterface::enterInsert_with_table_hints(TSqlParser::Insert_with_table_hintsContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitInsert_with_table_hints(TSqlParser::Insert_with_table_hintsContext * ctx) { }

void ListenerInterface::enterTable_hint(TSqlParser::Table_hintContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitTable_hint(TSqlParser::Table_hintContext * ctx) { }

void ListenerInterface::enterIndex_value(TSqlParser::Index_valueContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitIndex_value(TSqlParser::Index_valueContext * ctx) { }

void ListenerInterface::enterColumn_alias_list(TSqlParser::Column_alias_listContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitColumn_alias_list(TSqlParser::Column_alias_listContext * ctx) { }

void ListenerInterface::enterTable_value_constructor(TSqlParser::Table_value_constructorContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitTable_value_constructor(TSqlParser::Table_value_constructorContext * ctx) { }

void ListenerInterface::enterRanking_windowed_function(TSqlParser::Ranking_windowed_functionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitRanking_windowed_function(TSqlParser::Ranking_windowed_functionContext * ctx) { }

void ListenerInterface::enterAnalytic_windowed_function(TSqlParser::Analytic_windowed_functionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAnalytic_windowed_function(TSqlParser::Analytic_windowed_functionContext * ctx) { }

void ListenerInterface::enterOver_clause(TSqlParser::Over_clauseContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitOver_clause(TSqlParser::Over_clauseContext * ctx) { }

void ListenerInterface::enterRow_or_range_clause(TSqlParser::Row_or_range_clauseContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitRow_or_range_clause(TSqlParser::Row_or_range_clauseContext * ctx) { }

void ListenerInterface::enterWindow_frame_extent(TSqlParser::Window_frame_extentContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitWindow_frame_extent(TSqlParser::Window_frame_extentContext * ctx) { }

void ListenerInterface::enterWindow_frame_bound(TSqlParser::Window_frame_boundContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitWindow_frame_bound(TSqlParser::Window_frame_boundContext * ctx) { }

void ListenerInterface::enterWindow_frame_preceding(TSqlParser::Window_frame_precedingContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitWindow_frame_preceding(TSqlParser::Window_frame_precedingContext * ctx) { }

void ListenerInterface::enterWindow_frame_following(TSqlParser::Window_frame_followingContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitWindow_frame_following(TSqlParser::Window_frame_followingContext * ctx) { }

void ListenerInterface::enterCreate_database_option(TSqlParser::Create_database_optionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_database_option(TSqlParser::Create_database_optionContext * ctx) { }

void ListenerInterface::enterDatabase_filestream_option(TSqlParser::Database_filestream_optionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDatabase_filestream_option(TSqlParser::Database_filestream_optionContext * ctx) { }

void ListenerInterface::enterDatabase_file_spec(TSqlParser::Database_file_specContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDatabase_file_spec(TSqlParser::Database_file_specContext * ctx) { }

void ListenerInterface::enterFile_group(TSqlParser::File_groupContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitFile_group(TSqlParser::File_groupContext * ctx) { }

void ListenerInterface::enterFile_spec(TSqlParser::File_specContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitFile_spec(TSqlParser::File_specContext * ctx) { }

void ListenerInterface::enterEntity_name(TSqlParser::Entity_nameContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitEntity_name(TSqlParser::Entity_nameContext * ctx) { }

void ListenerInterface::enterEntity_name_for_azure_dw(TSqlParser::Entity_name_for_azure_dwContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitEntity_name_for_azure_dw(TSqlParser::Entity_name_for_azure_dwContext * ctx) { }

void ListenerInterface::enterEntity_name_for_parallel_dw(TSqlParser::Entity_name_for_parallel_dwContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitEntity_name_for_parallel_dw(TSqlParser::Entity_name_for_parallel_dwContext * ctx) { }

void ListenerInterface::enterFull_table_name(TSqlParser::Full_table_nameContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitFull_table_name(TSqlParser::Full_table_nameContext * ctx) { }

void ListenerInterface::enterSimple_name(TSqlParser::Simple_nameContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitSimple_name(TSqlParser::Simple_nameContext * ctx) { }

void ListenerInterface::enterDdl_object(TSqlParser::Ddl_objectContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDdl_object(TSqlParser::Ddl_objectContext * ctx) { }

void ListenerInterface::enterColumn_name_list_with_order(TSqlParser::Column_name_list_with_orderContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitColumn_name_list_with_order(TSqlParser::Column_name_list_with_orderContext * ctx) { }

void ListenerInterface::enterColumn_name_list(TSqlParser::Column_name_listContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitColumn_name_list(TSqlParser::Column_name_listContext * ctx) { }

void ListenerInterface::enterCursor_name(TSqlParser::Cursor_nameContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCursor_name(TSqlParser::Cursor_nameContext * ctx) { }

void ListenerInterface::enterOn_off(TSqlParser::On_offContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitOn_off(TSqlParser::On_offContext * ctx) { }

void ListenerInterface::enterClustered(TSqlParser::ClusteredContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitClustered(TSqlParser::ClusteredContext * ctx) { }

void ListenerInterface::enterNull_or_default(TSqlParser::Null_or_defaultContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitNull_or_default(TSqlParser::Null_or_defaultContext * ctx) { }

void ListenerInterface::enterBegin_conversation_timer(TSqlParser::Begin_conversation_timerContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitBegin_conversation_timer(TSqlParser::Begin_conversation_timerContext * ctx) { }

void ListenerInterface::enterBegin_conversation_dialog(TSqlParser::Begin_conversation_dialogContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitBegin_conversation_dialog(TSqlParser::Begin_conversation_dialogContext * ctx) { }

void ListenerInterface::enterContract_name(TSqlParser::Contract_nameContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitContract_name(TSqlParser::Contract_nameContext * ctx) { }

void ListenerInterface::enterService_name(TSqlParser::Service_nameContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitService_name(TSqlParser::Service_nameContext * ctx) { }

void ListenerInterface::enterEnd_conversation(TSqlParser::End_conversationContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitEnd_conversation(TSqlParser::End_conversationContext * ctx) { }

void ListenerInterface::enterWaitfor_conversation(TSqlParser::Waitfor_conversationContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitWaitfor_conversation(TSqlParser::Waitfor_conversationContext * ctx) { }

void ListenerInterface::enterGet_conversation(TSqlParser::Get_conversationContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitGet_conversation(TSqlParser::Get_conversationContext * ctx) { }

void ListenerInterface::enterQueue_id(TSqlParser::Queue_idContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitQueue_id(TSqlParser::Queue_idContext * ctx) { }

void ListenerInterface::enterSend_conversation(TSqlParser::Send_conversationContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitSend_conversation(TSqlParser::Send_conversationContext * ctx) { }

void ListenerInterface::enterData_type(TSqlParser::Data_typeContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitData_type(TSqlParser::Data_typeContext * ctx) { }

void ListenerInterface::enterDefault_value(TSqlParser::Default_valueContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDefault_value(TSqlParser::Default_valueContext * ctx) { }

void ListenerInterface::enterAssignment_operator(TSqlParser::Assignment_operatorContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAssignment_operator(TSqlParser::Assignment_operatorContext * ctx) { }

void ListenerInterface::enterFile_size(TSqlParser::File_sizeContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitFile_size(TSqlParser::File_sizeContext * ctx) { }


