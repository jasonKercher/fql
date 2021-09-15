#include "listenerinterface.h"

#include <cstring>

#include "fql.h"
#include "query.h"
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

ListenerInterface::ListenerInterface(struct fqlhandle* fql,
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

ListenerInterface::~ListenerInterface()
{
	node_free(&_query_stack);
	node_free_data(&_source_stack);
	node_free_data(&_column_stack);
}

int ListenerInterface::get_return_code()
{
	return _return_code;
}

void ListenerInterface::enterTsql_file(TSqlParser::Tsql_fileContext * ctx) { }
void ListenerInterface::exitTsql_file(TSqlParser::Tsql_fileContext * ctx) { }

void ListenerInterface::enterBatch(TSqlParser::BatchContext * ctx) { }
void ListenerInterface::exitBatch(TSqlParser::BatchContext * ctx) { }

void ListenerInterface::enterCfl_statement(TSqlParser::Cfl_statementContext * ctx) { }
void ListenerInterface::exitCfl_statement(TSqlParser::Cfl_statementContext * ctx) { }

void ListenerInterface::enterIf_statement(TSqlParser::If_statementContext * ctx) 
{
	_query->mode = MODE_IF;
	_operation = OP_IF;

	/* TODO if (ctx->ELSE()) */

	if (query_init_op(_query, _fql, _operation)) {
		_set_failure();
	}
}
void ListenerInterface::exitIf_statement(TSqlParser::If_statementContext * ctx) 
{
	_query->query_total = _query_id + 1;
	node_free(&_query_stack);
}

void ListenerInterface::enterDml_clause(TSqlParser::Dml_clauseContext * ctx) { }
void ListenerInterface::exitDml_clause(TSqlParser::Dml_clauseContext * ctx) { }

void ListenerInterface::enterSql_clauses(TSqlParser::Sql_clausesContext* ctx)
{
	_query = query_add_query(_fql);
	_query_id = 0;
	node_push(&_query_stack, _query);
}
void ListenerInterface::exitSql_clauses(TSqlParser::Sql_clausesContext* ctx)
{
	_query->query_total = _query_id + 1;
	node_free(&_query_stack);
}

void ListenerInterface::enterSelect_statement_standalone(TSqlParser::Select_statement_standaloneContext* ctx) { }
void ListenerInterface::exitSelect_statement_standalone(TSqlParser::Select_statement_standaloneContext* ctx) { }

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

void ListenerInterface::enterColumn_elem(TSqlParser::Column_elemContext * ctx)
{
	if (ctx->NULL_()) {
		query_add_null_expression(_query);
	}
}
void ListenerInterface::exitColumn_elem(TSqlParser::Column_elemContext * ctx)
{
	_table_name[0] = '\0';
}

void ListenerInterface::enterExpression_elem(TSqlParser::Expression_elemContext * ctx) { }
void ListenerInterface::exitExpression_elem(TSqlParser::Expression_elemContext * ctx) { }

void ListenerInterface::enterSelect_list_elem(TSqlParser::Select_list_elemContext * ctx)
{
	_query->mode = MODE_SELECTLIST;
	_tok_type = TOK_COLUMN_NAME;
}
void ListenerInterface::exitSelect_list_elem(TSqlParser::Select_list_elemContext * ctx)
{
	if (_on_asterisk) {
		if (query_add_asterisk(_query, _table_name)) {
			_set_failure();
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
		if (query_add_source(_query, _fql, &_source_stack, _table_alias) == FQL_FAIL) {
			_set_failure();
		}
	} else {
		query_add_subquery_source(_query, _subquery, _table_alias);
	}

	_subquery = NULL;
	*_table_alias = '\0';
}

void ListenerInterface::enterJoin_part(TSqlParser::Join_partContext * ctx)
{
	std::string error_token = "";
	if (ctx->apply_()) {
		error_token = "APPLY";
	} else if (ctx->pivot()) {
		error_token = "PIVOT";
	} else if (ctx->unpivot()) {
		error_token = "UNPIVOT";
	} else if (!ctx->join_on()) {
		std::cerr << "Keyword `JOIN' expected but not found\n";
		_set_failure();
	}

	if (!error_token.empty()) {
		std::cerr << "Keyword `" << error_token << "' currently not supported\n";
		_set_failure();
	}

}
void ListenerInterface::exitJoin_part(TSqlParser::Join_partContext * ctx)
{
}


void ListenerInterface::enterJoin_on(TSqlParser::Join_onContext* ctx)
{
	if (ctx->LEFT()) {
		_query->join = JOIN_LEFT;
	} else if (ctx->RIGHT()) {
		_query->join = JOIN_RIGHT;
		std::cerr << "RIGHT JOIN not currently supported, try LEFT JOIN\n";
	} else if (ctx->FULL()) {
		_query->join = JOIN_FULL;
		std::cerr << "FULL JOIN not currently supported, try LEFT JOIN\n";
	} else {
		_query->join = JOIN_INNER;
	}

	_query->logic_mode = LOGIC_JOIN;
}
void ListenerInterface::exitJoin_on(TSqlParser::Join_onContext* ctx)
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
	//if (_tok_type == TOK_TABLE_NAME) {
	//	_tok_type = TOK_COLUMN_NAME;
	//}
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
void ListenerInterface::exitFull_column_name(TSqlParser::Full_column_nameContext * ctx)
{
	char* column_name = (char*) node_pop(&_column_stack);
	if (_column_stack != NULL) {
		char* table_name = (char*) node_pop(&_column_stack);
		if (query_add_expression(_query, column_name, table_name)) {
			_set_failure();
		}
		free_(table_name);
	} else {
		if (query_add_expression(_query, column_name, "")) {
			_set_failure();
		}
	}
	free_(column_name);

	node_free_data(&_column_stack);
}

void ListenerInterface::enterConstant(TSqlParser::ConstantContext * ctx)
{
	std::string new_str = ctx->getText();
	if (new_str[0] == '\'' && new_str.length() > 2) {
		_find_replace(new_str, "''", "'");
	}
	if (query_add_constant(_query, new_str.c_str(), new_str.length())) {
		_set_failure();
	}
}
void ListenerInterface::exitConstant(TSqlParser::ConstantContext * ctx) { }

void ListenerInterface::enterSign(TSqlParser::SignContext * ctx) { }
void ListenerInterface::exitSign(TSqlParser::SignContext * ctx) { }

void ListenerInterface::enterUnary_operator_expression(TSqlParser::Unary_operator_expressionContext * ctx)
{
	if (ctx->MINUS()) {
		if (query_enter_operator(_query, SCALAR_OP_UNARY_MINUS)) {
			_set_failure();
		}
		return;
	}

	if (ctx->BIT_NOT()) {
		if (query_enter_operator(_query, SCALAR_OP_UNARY_BIT_NOT)) {
			_set_failure();
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

void ListenerInterface::enterId_(TSqlParser::Id_Context* ctx)
{
	std::string new_str = ctx->getText();

	if (new_str[0] == '\'') {
		_trim_end_chars(new_str, '\'', '\'');
		_find_replace(new_str, "''", "'");
	} else if (new_str[0] == '[') {
		_trim_end_chars(new_str, '[', ']');
	}

	char* token = strdup(new_str.c_str());

	switch (_tok_type) {
	case TOK_OP_TABLE:
		query_set_op_table(_query, token);
		free_(token);
		break;
	case TOK_COLUMN_NAME:
		/* consume table designation */
		node_push(&_column_stack, token);
		break;
	case TOK_COLUMN_ALIAS:
		query_apply_expression_alias(_query, token);
		free_(token);
		break;
	case TOK_INTO_TABLE:
		/* No need to free here, into_table_name
		 * is now owned by the query.
		 */
		if (query_set_into_table(_query, token)) {
			_set_failure();
		}
		break;
	case TOK_TABLE_NAME:
		strncpy_(_table_name, token, TABLE_NAME_MAX);
		free_(token);
		break;
	case TOK_TABLE_SOURCE:
		node_push(&_source_stack, token);
		break;
	case TOK_TABLE_ALIAS:
		strncpy_(_table_alias, token, TABLE_NAME_MAX);
		free_(token);
		break;
	case TOK_DATA_TYPE:
		if (query_apply_data_type(_query, token)) {
			_set_failure();
		}
		free_(token);
		break;
	default:
		std::cerr << "Undefined list: " << _tok_type << '\n';
		free_(token);
	}
}
void ListenerInterface::exitId_(TSqlParser::Id_Context* ctx) { }

void ListenerInterface::enterSimple_id(TSqlParser::Simple_idContext * ctx) { }
void ListenerInterface::exitSimple_id(TSqlParser::Simple_idContext * ctx) { }

void ListenerInterface::enterComparison_operator(TSqlParser::Comparison_operatorContext * ctx)
{
	query_set_logic_comparison(_query, ctx->getText().c_str());
}
void ListenerInterface::exitComparison_operator(TSqlParser::Comparison_operatorContext * ctx)
{
}

void ListenerInterface::enterNull_notnull(TSqlParser::Null_notnullContext * ctx)
{
	query_set_logic_isnull(_query, ctx->NOT());
}
void ListenerInterface::exitNull_notnull(TSqlParser::Null_notnullContext * ctx) { }

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

	if (ctx->ABS())
		fn = SCALAR_ABS;
	if (ctx->ASCII())
		fn = SCALAR_ASCII;
	if (ctx->CEILING())
		fn = SCALAR_CEILING;
	if (ctx->CHAR())
		fn = SCALAR_CHAR;
	if (ctx->CHARINDEX())
		fn = SCALAR_CHARINDEX;
	if (ctx->CHECKSUM())
		fn = SCALAR_CHECKSUM;
	if (ctx->DATALENGTH())
		fn = SCALAR_DATALENGTH;
	if (ctx->DAY())
		fn = SCALAR_DAY;
	if (ctx->FLOOR())
		fn = SCALAR_FLOOR;
	if (ctx->ISDATE())
		fn = SCALAR_ISDATE;
	if (ctx->ISNUMERIC())
		fn = SCALAR_ISNUMERIC;
	if (ctx->LEFT())
		fn = SCALAR_LEFT;
	if (ctx->LEN())
		fn = SCALAR_LEN;
	if (ctx->LOWER())
		fn = SCALAR_LOWER;
	if (ctx->LTRIM())
		fn = SCALAR_LTRIM;
	if (ctx->MONTH())
		fn = SCALAR_MONTH;
	if (ctx->NCHAR())
		fn = SCALAR_NCHAR;
	if (ctx->PATINDEX())
		fn = SCALAR_PATINDEX;
	if (ctx->RAND())
		fn = SCALAR_RAND;
	if (ctx->REPLACE())
		fn = SCALAR_REPLACE;
	if (ctx->RIGHT())
		fn = SCALAR_RIGHT;
	if (ctx->ROUND())
		fn = SCALAR_ROUND;
	if (ctx->RTRIM())
		fn = SCALAR_RTRIM;
	if (ctx->SIGN())
		fn = SCALAR_SIGN;
	if (ctx->SPACE())
		fn = SCALAR_SPACE;
	if (ctx->STR())
		fn = SCALAR_STR;
	if (ctx->SUBSTRING())
		fn = SCALAR_SUBSTRING;
	if (ctx->UPPER())
		fn = SCALAR_UPPER;
	if (ctx->USER_NAME())
		fn = SCALAR_USER_NAME;
	if (ctx->YEAR())
		fn = SCALAR_YEAR;

	if (fn == SCALAR_UNDEFINED) {
		std::cerr << "Function " << ctx->getText() << "' not defined\n";
		_set_failure();
	}

	if (query_enter_function(_query, fn, _fql->props.char_as_byte)) {
		_set_failure();
	}
}
void ListenerInterface::exitScalar_function_name(TSqlParser::Scalar_function_nameContext * ctx) { }

void ListenerInterface::enterSelect_statement(TSqlParser::Select_statementContext * ctx)
{
	_query->mode = MODE_SELECTLIST;
	_operation = OP_SELECT;
	if (query_init_op(_query, _fql, _operation)) {
		_set_failure();
	}
}

void ListenerInterface::exitSelect_statement(TSqlParser::Select_statementContext * ctx) { }

void ListenerInterface::enterDelete_statement(TSqlParser::Delete_statementContext * ctx) 
{ 
	_operation = OP_DELETE;
	if (query_init_op(_query, _fql, _operation)) {
		_set_failure();
	}
	if (ctx->WHERE()) {
		_query->expect_where = true;
	}
}

void ListenerInterface::exitDelete_statement(TSqlParser::Delete_statementContext * ctx) 
{
	query_exit_non_select_op(_query, _fql);
}

void ListenerInterface::enterDelete_statement_from(TSqlParser::Delete_statement_fromContext * ctx) 
{ 
	_tok_type = TOK_OP_TABLE;
}
void ListenerInterface::exitDelete_statement_from(TSqlParser::Delete_statement_fromContext * ctx) 
{
	_tok_type = TOK_UNDEFINED;
}

void ListenerInterface::enterUpdate_statement(TSqlParser::Update_statementContext * ctx) 
{
	_query->mode = MODE_UPDATELIST;
	_operation = OP_UPDATE;
	if (query_init_op(_query, _fql, _operation)) {
		_set_failure();
	}
	if (ctx->WHERE()) {
		_query->expect_where = true;
	}
	_tok_type = TOK_OP_TABLE;
}
void ListenerInterface::exitUpdate_statement(TSqlParser::Update_statementContext * ctx) 
{
	_tok_type = TOK_UNDEFINED;
	query_exit_non_select_op(_query, _fql);
}

void ListenerInterface::enterUpdate_elem(TSqlParser::Update_elemContext * ctx) { }
void ListenerInterface::exitUpdate_elem(TSqlParser::Update_elemContext * ctx) { }

void ListenerInterface::enterDdl_object(TSqlParser::Ddl_objectContext * ctx) { }
void ListenerInterface::exitDdl_object(TSqlParser::Ddl_objectContext * ctx) { }

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

void ListenerInterface::enterPrimitive_expression(TSqlParser::Primitive_expressionContext * ctx)
{
	if (ctx->NULL_()) {
		query_add_null_expression(_query);
	}
}
void ListenerInterface::exitPrimitive_expression(TSqlParser::Primitive_expressionContext * ctx) { }

void ListenerInterface::enterData_type(TSqlParser::Data_typeContext * ctx)
{
	_tok_type = TOK_DATA_TYPE;

	const char* tok_str = strdup(ctx->getText().c_str());
	const char* it = tok_str;
	char numstr[32];
	char* num_iter = numstr;
	bool in_bracket = false;
	for (; *it && num_iter - numstr < 31; ++it) {
		if (!in_bracket && *it == '(') {
			in_bracket = true;
			continue;
		}
		if (in_bracket && *it == ')') {
			in_bracket = false;
			break;
		}
		if (in_bracket) {
			*num_iter = *it;
			++num_iter;
		}
	}

	free_(tok_str);

	*num_iter = '\0';
	if (num_iter == numstr) {
		return;
	}

	if (query_add_constant(_query, numstr, num_iter - numstr)) {
		_set_failure();
	}
}
void ListenerInterface::exitData_type(TSqlParser::Data_typeContext * ctx) { }

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
	node_push(&_query_stack, subquery);

	_operation = OP_SELECT;

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
		_set_failure();
		return;
	}

	_query = subquery;

}
void ListenerInterface::exitSubquery(TSqlParser::SubqueryContext * ctx)
{
	_subquery = (struct query*) node_pop(&_query_stack);
	_query = (struct query*) _query_stack->data;

	_operation = *(enum op*)_query->op;

	/* subquery as constant */
	if (_query->in_bracket_expression) {
		query_add_subquery_const(_query, _subquery);
	}
}

void ListenerInterface::enterSearch_condition(TSqlParser::Search_conditionContext * ctx)
{
	if (ctx->LR_BRACKET() && ctx->RR_BRACKET()) {
		return;
	}
	if (_query->logic_mode == LOGIC_UNDEFINED) {
		if (_query->mode == MODE_IF) {
			_query->logic_mode = LOGIC_IF;
		} else if (_query->expect_where && !_query->where) {
			_query->logic_mode = LOGIC_WHERE;
		} else {
			_query->logic_mode = LOGIC_HAVING;
			/* NOTE: SQL Server DOES support this. Until I see
			 *       a practical application for this, I will not.
			 */
			if (!_query->groupby) {
				std::cerr << "HAVING without GROUP BY\n";
				_set_failure();
			}
		}
	}
	if (ctx->AND()) {
		query_enter_search_and(_query);
	} else if (ctx->OR()) {
		query_enter_search_or(_query);
	}
}
void ListenerInterface::exitSearch_condition(TSqlParser::Search_conditionContext * ctx)
{
	if (ctx->AND() || ctx->OR()) {
		query_exit_search_item(_query);
	}
}

void ListenerInterface::enterPredicate(TSqlParser::PredicateContext * ctx)
{
	query_enter_predicate(_query, ctx->NOT().size(), (ctx->IN() != NULL));
}

void ListenerInterface::exitPredicate(TSqlParser::PredicateContext * ctx)
{
	query_exit_predicate(_query, (ctx->IN() != NULL), (ctx->LIKE() != NULL));
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
		_set_failure();
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

	if (query_enter_aggregate(_query, fn)) {
		_set_failure();
	}
	//_query->in_aggregate = true;
}
void ListenerInterface::exitAggregate_windowed_function(TSqlParser::Aggregate_windowed_functionContext * ctx)
{
	query_exit_aggregate(_query);
	//_query->in_aggregate = false;
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
		_set_failure();
	}

	query* union_query = query_new(++_query_id);
	if (query_enter_union(_query, union_query)) {
		_set_failure();
	}

	_query = union_query;
	node_push(&_query_stack, _query);
	_query->mode = MODE_SELECTLIST;
}

void ListenerInterface::exitSql_union(TSqlParser::Sql_unionContext * ctx)
{
	query* union_query = (query*)node_pop(&_query_stack);
	_query = (query*)_query_stack->data;
	query_exit_union(_query, union_query);
}

void ListenerInterface::enterCAST(TSqlParser::CASTContext * ctx)
{
	if (query_enter_function(_query, SCALAR_CAST, _fql->props.char_as_byte)) {
		_set_failure();
	}
}
void ListenerInterface::exitCAST(TSqlParser::CASTContext * ctx) 
{ 
	query_exit_function(_query);
}

void ListenerInterface::enterISNULL(TSqlParser::ISNULLContext * ctx)
{
	if (query_enter_function(_query, SCALAR_ISNULL, _fql->props.char_as_byte)) {
		_set_failure();
	}
}

void ListenerInterface::exitISNULL(TSqlParser::ISNULLContext * ctx) 
{
	query_exit_function(_query);
}

void ListenerInterface::enterBUILT_IN_FUNC(TSqlParser::BUILT_IN_FUNCContext* ctx) { }
void ListenerInterface::exitBUILT_IN_FUNC(TSqlParser::BUILT_IN_FUNCContext* ctx) { }

void ListenerInterface::enterKeyword(TSqlParser::KeywordContext* ctx) { }
void ListenerInterface::exitKeyword(TSqlParser::KeywordContext* ctx) { }

void ListenerInterface::enterFull_table_name(TSqlParser::Full_table_nameContext * ctx) { }
void ListenerInterface::exitFull_table_name(TSqlParser::Full_table_nameContext * ctx) { }

/**
 * Not Implemented
 */
void ListenerInterface::_no_impl(const std::string& text, int rule_idx)
{
	std::cerr << "SQL construct not supported: " << text << "(" << _rule_names[rule_idx] << ")\n";
	if(_fql->props.override_warnings) {
		std::cerr << "\nCAUTION: Overriding the above warnings! Results may be incorrect!\n";
	} else {
		std::cerr << "\nTerminating: Though not recommended, warnings can be overridden.\n";
		_set_failure();
	}
}

void ListenerInterface::_set_failure()
{
	_return_code = FQL_FAIL;
	_walker->set_walking(false);
}

void ListenerInterface::enterDdl_clause(TSqlParser::Ddl_clauseContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDdl_clause(TSqlParser::Ddl_clauseContext * ctx) { }

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

void ListenerInterface::enterMerge_statement(TSqlParser::Merge_statementContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitMerge_statement(TSqlParser::Merge_statementContext * ctx) { }

void ListenerInterface::enterMerge_matched(TSqlParser::Merge_matchedContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitMerge_matched(TSqlParser::Merge_matchedContext * ctx) { }

void ListenerInterface::enterMerge_not_matched(TSqlParser::Merge_not_matchedContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitMerge_not_matched(TSqlParser::Merge_not_matchedContext * ctx) { }

void ListenerInterface::enterInsert_statement(TSqlParser::Insert_statementContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitInsert_statement(TSqlParser::Insert_statementContext * ctx) { }

void ListenerInterface::enterInsert_statement_value(TSqlParser::Insert_statement_valueContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitInsert_statement_value(TSqlParser::Insert_statement_valueContext * ctx) { }

void ListenerInterface::enterTime(TSqlParser::TimeContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitTime(TSqlParser::TimeContext * ctx) { }

void ListenerInterface::enterOutput_clause(TSqlParser::Output_clauseContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitOutput_clause(TSqlParser::Output_clauseContext * ctx) { }

void ListenerInterface::enterOutput_dml_list_elem(TSqlParser::Output_dml_list_elemContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitOutput_dml_list_elem(TSqlParser::Output_dml_list_elemContext * ctx) { }

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

void ListenerInterface::enterAlter_table(TSqlParser::Alter_tableContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_table(TSqlParser::Alter_tableContext * ctx) { }

void ListenerInterface::enterCursor_option(TSqlParser::Cursor_optionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCursor_option(TSqlParser::Cursor_optionContext * ctx) { }

void ListenerInterface::enterDrop_procedure(TSqlParser::Drop_procedureContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_procedure(TSqlParser::Drop_procedureContext * ctx) { }

void ListenerInterface::enterDrop_function(TSqlParser::Drop_functionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_function(TSqlParser::Drop_functionContext * ctx) { }

void ListenerInterface::enterDrop_table(TSqlParser::Drop_tableContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_table(TSqlParser::Drop_tableContext * ctx) { }

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

void ListenerInterface::enterColumn_def_table_constraints(TSqlParser::Column_def_table_constraintsContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitColumn_def_table_constraints(TSqlParser::Column_def_table_constraintsContext * ctx) { }

void ListenerInterface::enterColumn_def_table_constraint(TSqlParser::Column_def_table_constraintContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitColumn_def_table_constraint(TSqlParser::Column_def_table_constraintContext * ctx) { }

void ListenerInterface::enterColumn_definition(TSqlParser::Column_definitionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitColumn_definition(TSqlParser::Column_definitionContext * ctx) { }

void ListenerInterface::enterColumn_constraint(TSqlParser::Column_constraintContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitColumn_constraint(TSqlParser::Column_constraintContext * ctx) { }

void ListenerInterface::enterTable_constraint(TSqlParser::Table_constraintContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitTable_constraint(TSqlParser::Table_constraintContext * ctx) { }

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

void ListenerInterface::enterTop_percent(TSqlParser::Top_percentContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitTop_percent(TSqlParser::Top_percentContext * ctx) { }

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

void ListenerInterface::enterBulk_option(TSqlParser::Bulk_optionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitBulk_option(TSqlParser::Bulk_optionContext * ctx) { }

void ListenerInterface::enterBINARY_CHECKSUM(TSqlParser::BINARY_CHECKSUMContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitBINARY_CHECKSUM(TSqlParser::BINARY_CHECKSUMContext * ctx) { }

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

void ListenerInterface::enterRANKING_WINDOWED_FUNC(TSqlParser::RANKING_WINDOWED_FUNCContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitRANKING_WINDOWED_FUNC(TSqlParser::RANKING_WINDOWED_FUNCContext * ctx) { }

void ListenerInterface::enterANALYTIC_WINDOWED_FUNC(TSqlParser::ANALYTIC_WINDOWED_FUNCContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitANALYTIC_WINDOWED_FUNC(TSqlParser::ANALYTIC_WINDOWED_FUNCContext * ctx) { }

void ListenerInterface::enterSTRINGAGG(TSqlParser::STRINGAGGContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitSTRINGAGG(TSqlParser::STRINGAGGContext * ctx) { }

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

void ListenerInterface::enterEntity_name(TSqlParser::Entity_nameContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitEntity_name(TSqlParser::Entity_nameContext * ctx) { }

void ListenerInterface::enterEntity_name_for_azure_dw(TSqlParser::Entity_name_for_azure_dwContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitEntity_name_for_azure_dw(TSqlParser::Entity_name_for_azure_dwContext * ctx) { }

void ListenerInterface::enterEntity_name_for_parallel_dw(TSqlParser::Entity_name_for_parallel_dwContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitEntity_name_for_parallel_dw(TSqlParser::Entity_name_for_parallel_dwContext * ctx) { }

void ListenerInterface::enterSimple_name(TSqlParser::Simple_nameContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitSimple_name(TSqlParser::Simple_nameContext * ctx) { }

void ListenerInterface::enterColumn_name_list_with_order(TSqlParser::Column_name_list_with_orderContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitColumn_name_list_with_order(TSqlParser::Column_name_list_with_orderContext * ctx) { }

void ListenerInterface::enterColumn_name_list(TSqlParser::Column_name_listContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitColumn_name_list(TSqlParser::Column_name_listContext * ctx) { }

void ListenerInterface::enterCursor_name(TSqlParser::Cursor_nameContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCursor_name(TSqlParser::Cursor_nameContext * ctx) { }

void ListenerInterface::enterOn_off(TSqlParser::On_offContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitOn_off(TSqlParser::On_offContext * ctx) { }

void ListenerInterface::enterNull_or_default(TSqlParser::Null_or_defaultContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitNull_or_default(TSqlParser::Null_or_defaultContext * ctx) { }

void ListenerInterface::enterDefault_value(TSqlParser::Default_valueContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDefault_value(TSqlParser::Default_valueContext * ctx) { }

void ListenerInterface::enterAssignment_operator(TSqlParser::Assignment_operatorContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAssignment_operator(TSqlParser::Assignment_operatorContext * ctx) { }

void ListenerInterface::enterId_or_string(TSqlParser::Id_or_stringContext* ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitId_or_string(TSqlParser::Id_or_stringContext* ctx) { }

void ListenerInterface::enterWhen_matches(TSqlParser::When_matchesContext* ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitWhen_matches(TSqlParser::When_matchesContext* ctx) { }

void ListenerInterface::enterAlter_sequence(TSqlParser::Alter_sequenceContext* ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_sequence(TSqlParser::Alter_sequenceContext* ctx) { }

void ListenerInterface::enterBatch_level_statement(TSqlParser::Batch_level_statementContext* ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitBatch_level_statement(TSqlParser::Batch_level_statementContext* ctx) { }

void ListenerInterface::enterClass_type_for_azure_dw(TSqlParser::Class_type_for_azure_dwContext* ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitClass_type_for_azure_dw(TSqlParser::Class_type_for_azure_dwContext* ctx) { }

void ListenerInterface::enterApply_(TSqlParser::Apply_Context* ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitApply_(TSqlParser::Apply_Context* ctx) { }

void ListenerInterface::enterCross_join(TSqlParser::Cross_joinContext* ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCross_join(TSqlParser::Cross_joinContext* ctx) { }

void ListenerInterface::enterCreate_sequence(TSqlParser::Create_sequenceContext* ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_sequence(TSqlParser::Create_sequenceContext* ctx) { }

void ListenerInterface::enterInsert_column_name_list(TSqlParser::Insert_column_name_listContext* ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitInsert_column_name_list(TSqlParser::Insert_column_name_listContext* ctx) { }

void ListenerInterface::enterUnpivot(TSqlParser::UnpivotContext* ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitUnpivot(TSqlParser::UnpivotContext* ctx) { }

void ListenerInterface::enterChange_table_version(TSqlParser::Change_table_versionContext* ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitChange_table_version(TSqlParser::Change_table_versionContext* ctx) { }

void ListenerInterface::enterIIF(TSqlParser::IIFContext* ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitIIF(TSqlParser::IIFContext* ctx) { }

void ListenerInterface::enterDatabase_optionspec(TSqlParser::Database_optionspecContext* ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDatabase_optionspec(TSqlParser::Database_optionspecContext* ctx) { }

void ListenerInterface::enterExecute_statement_arg_named(TSqlParser::Execute_statement_arg_namedContext* ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitExecute_statement_arg_named(TSqlParser::Execute_statement_arg_namedContext* ctx) { }

void ListenerInterface::enterGo_batch_statement(TSqlParser::Go_batch_statementContext* ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitGo_batch_statement(TSqlParser::Go_batch_statementContext* ctx) { }

void ListenerInterface::enterPartition_function(TSqlParser::Partition_functionContext* ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitPartition_function(TSqlParser::Partition_functionContext* ctx) { }

void ListenerInterface::enterPivot(TSqlParser::PivotContext* ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitPivot(TSqlParser::PivotContext* ctx) { }

void ListenerInterface::enterExecute_statement_arg_unnamed(TSqlParser::Execute_statement_arg_unnamedContext* ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitExecute_statement_arg_unnamed(TSqlParser::Execute_statement_arg_unnamedContext* ctx) { }

void ListenerInterface::enterTime_zone(TSqlParser::Time_zoneContext* ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitTime_zone(TSqlParser::Time_zoneContext* ctx) { }

void ListenerInterface::enterUSER(TSqlParser::USERContext* ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitUSER(TSqlParser::USERContext* ctx) { }

void ListenerInterface::enterAlgorithm(TSqlParser::AlgorithmContext* ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlgorithm(TSqlParser::AlgorithmContext* ctx) { }

void ListenerInterface::enterInsert_column_id(TSqlParser::Insert_column_idContext* ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitInsert_column_id(TSqlParser::Insert_column_idContext* ctx) { }

void ListenerInterface::enterExecute_body_batch(TSqlParser::Execute_body_batchContext* ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitExecute_body_batch(TSqlParser::Execute_body_batchContext* ctx) { }

void ListenerInterface::enterExecute_parameter(TSqlParser::Execute_parameterContext* ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitExecute_parameter(TSqlParser::Execute_parameterContext* ctx) { }

void ListenerInterface::enterUpdate_elem_merge(TSqlParser::Update_elem_mergeContext* ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitUpdate_elem_merge(TSqlParser::Update_elem_mergeContext* ctx) { }

void ListenerInterface::enterPARTITION_FUNC(TSqlParser::PARTITION_FUNCContext* ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitPARTITION_FUNC(TSqlParser::PARTITION_FUNCContext* ctx) { }

void ListenerInterface::enterTRY_CAST(TSqlParser::TRY_CASTContext* ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitTRY_CAST(TSqlParser::TRY_CASTContext* ctx) { }

void ListenerInterface::enterChange_table_changes(TSqlParser::Change_table_changesContext* ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitChange_table_changes(TSqlParser::Change_table_changesContext* ctx) { }

void ListenerInterface::enterDbcc_command(TSqlParser::Dbcc_commandContext* ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDbcc_command(TSqlParser::Dbcc_commandContext* ctx) { }

void ListenerInterface::enterDbcc_special(TSqlParser::Dbcc_specialContext* ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDbcc_special(TSqlParser::Dbcc_specialContext* ctx) { }

