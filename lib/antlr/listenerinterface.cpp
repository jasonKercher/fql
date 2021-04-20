#include "listenerinterface.h"

#include <cstring>

#include "query.h"
#include "column.h"
#include "select.h"
#include "util/util.h"

/** Utility functions **/

void _trim_end_chars(std::string& s, char start, char end)
{
	if(s.length() > 1)  //next if statement would crash if 0 or 1
	    if(s[0] == start && s[s.length() - 1] == end)
		s = s.substr(1, s.length() - 2);
}

void _find_replace(std::string& s, const std::string& match, const std::string replacement)
{
	size_t pos = 0;
	while ((pos = s.find(match, pos)) != std::string::npos)
	{
	     s.replace(pos, match.length(), replacement);
	     pos += replacement.length();
	}
}

ListenerInterface::ListenerInterface(struct fql_handle* fql, const std::vector<std::string>& rules)
{
	_table_name[0] = '\0';
	_table_alias[0] = '\0';
	_rule_names = rules;
	_fql = fql;

	_next_list = TOK_UNDEFINED;
	_current_list = TOK_UNDEFINED;
}

void ListenerInterface::enterSelect_list(TSqlParser::Select_listContext * ctx)
{
	_current_list = TOK_COLUMN_NAME;
	_next_list = TOK_TABLE_NAME;
}
void ListenerInterface::exitSelect_list(TSqlParser::Select_listContext * ctx)
{
	//query_select_finalize(_query);
	_query->mode = MODE_UNDEFINED;
}

void ListenerInterface::enterGroup_by_item(TSqlParser::Group_by_itemContext * ctx)
{
	_query->mode = MODE_GROUPBY;
}
void ListenerInterface::exitGroup_by_item(TSqlParser::Group_by_itemContext * ctx)
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
	_current_list = TOK_COLUMN_NAME;
}
void ListenerInterface::exitSelect_list_elem(TSqlParser::Select_list_elemContext * ctx)
{
	if (_on_asterisk) {
		query_add_asterisk(_query, _table_name);
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

void ListenerInterface::enterTable_source_item(TSqlParser::Table_source_itemContext * ctx) { }
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
	if (ctx->getTokens(TSqlParser::LEFT).size()) {
		_query->join = JOIN_LEFT;
	} else if (ctx->getTokens(TSqlParser::RIGHT).size()) {
		_query->join = JOIN_RIGHT;
	} else if (ctx->getTokens(TSqlParser::FULL).size()) {
		_query->join = JOIN_FULL;
	} else if (ctx->getTokens(TSqlParser::CROSS).size()) {
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
	_next_list = TOK_TABLE_SOURCE;
}
void ListenerInterface::exitTable_name_with_hint(TSqlParser::Table_name_with_hintContext * ctx) { }

void ListenerInterface::enterDerived_table(TSqlParser::Derived_tableContext * ctx) { }
void ListenerInterface::exitDerived_table(TSqlParser::Derived_tableContext * ctx) { }

void ListenerInterface::enterAs_column_alias(TSqlParser::As_column_aliasContext * ctx) { }
void ListenerInterface::exitAs_column_alias(TSqlParser::As_column_aliasContext * ctx) { }

void ListenerInterface::enterColumn_alias(TSqlParser::Column_aliasContext * ctx)
{
	_current_list = TOK_COLUMN_ALIAS;
}

void ListenerInterface::exitColumn_alias(TSqlParser::Column_aliasContext * ctx) { }

void ListenerInterface::enterAs_table_alias(TSqlParser::As_table_aliasContext * ctx) { }
void ListenerInterface::exitAs_table_alias(TSqlParser::As_table_aliasContext * ctx) { }

void ListenerInterface::enterTable_alias(TSqlParser::Table_aliasContext * ctx)
{
	_current_list = TOK_TABLE_ALIAS;
}
void ListenerInterface::exitTable_alias(TSqlParser::Table_aliasContext * ctx) { }

void ListenerInterface::enterExpression_list(TSqlParser::Expression_listContext * ctx) { }
void ListenerInterface::exitExpression_list(TSqlParser::Expression_listContext * ctx) { }

void ListenerInterface::enterTable_name(TSqlParser::Table_nameContext * ctx)
{
	_current_list = _next_list;
}
void ListenerInterface::exitTable_name(TSqlParser::Table_nameContext * ctx)
{
	if (_current_list == TOK_TABLE_NAME) {
		_current_list = TOK_COLUMN_NAME;
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
	_current_list = TOK_COLUMN_NAME;
	_next_list = TOK_TABLE_NAME;
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
	/* TODO: handle FQL_FAIL here */
	query_add_constant(_query, new_str.c_str(), new_str.length());
}
void ListenerInterface::exitConstant(TSqlParser::ConstantContext * ctx) { }

void ListenerInterface::enterSign(TSqlParser::SignContext * ctx) { }
void ListenerInterface::exitSign(TSqlParser::SignContext * ctx) { }

void ListenerInterface::enterUnary_operator_expression(TSqlParser::Unary_operator_expressionContext * ctx) { }
void ListenerInterface::exitUnary_operator_expression(TSqlParser::Unary_operator_expressionContext * ctx) { }

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

	switch (_current_list) {
	case TOK_COLUMN_NAME:
		query_add_column(_query, token, _table_name);
		/* consume table designation */
		_table_name[0] = '\0';
		free_(token);
		break;
	case TOK_COLUMN_ALIAS:
		select_apply_column_alias((struct select*)_query->op, token);
		free_(token);
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
		std::cerr << "Undefined list: " << _current_list << '\n';
		free_(token);
	}
}
void ListenerInterface::exitId(TSqlParser::IdContext * ctx) { }

void ListenerInterface::enterSimple_id(TSqlParser::Simple_idContext * ctx) { }
void ListenerInterface::exitSimple_id(TSqlParser::Simple_idContext * ctx) { }

void ListenerInterface::enterComparison_operator(TSqlParser::Comparison_operatorContext * ctx)
{
	query_set_logic_comparison(_query, ctx->getText().c_str());
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
	query_enter_function(_query, ctx->getText().c_str());
}
void ListenerInterface::exitScalar_function_name(TSqlParser::Scalar_function_nameContext * ctx) { }

void ListenerInterface::enterSelect_statement(TSqlParser::Select_statementContext * ctx)
{
	_query->mode = MODE_SELECT;
	_query->op = select_new();
}

void ListenerInterface::exitSelect_statement(TSqlParser::Select_statementContext * ctx)
{
}

void ListenerInterface::enterExpression(TSqlParser::ExpressionContext * ctx)
{
	if        (!ctx->getTokens(TSqlParser::PLUS).empty()) {
		query_enter_operator(_query, OPERATOR_PLUS);
	} else if (!ctx->getTokens(TSqlParser::MINUS).empty()) {
		query_enter_operator(_query, OPERATOR_MINUS);
	} else if (!ctx->getTokens(TSqlParser::STAR).empty()) {
		query_enter_operator(_query, OPERATOR_MULTIPY);
	} else if (!ctx->getTokens(TSqlParser::DIVIDE).empty()) {
		query_enter_operator(_query, OPERATOR_DIVIDE);
	} else if (!ctx->getTokens(TSqlParser::MODULE).empty()) {
		query_enter_operator(_query, OPERATOR_MODULE);
	} else if (!ctx->getTokens(TSqlParser::BIT_OR).empty()) {
		query_enter_operator(_query, OPERATOR_BIT_OR);
	} else if (!ctx->getTokens(TSqlParser::BIT_AND).empty()) {
		query_enter_operator(_query, OPERATOR_BIT_AND);
	} else if (!ctx->getTokens(TSqlParser::BIT_XOR).empty()) {
		query_enter_operator(_query, OPERATOR_BIT_XOR);
	}
}
void ListenerInterface::exitExpression(TSqlParser::ExpressionContext * ctx)
{
	if (
		!ctx->getTokens(TSqlParser::PLUS).empty()
	     || !ctx->getTokens(TSqlParser::MINUS).empty()
	     || !ctx->getTokens(TSqlParser::STAR).empty()
	     || !ctx->getTokens(TSqlParser::DIVIDE).empty()
	     || !ctx->getTokens(TSqlParser::MODULE).empty()
	     || !ctx->getTokens(TSqlParser::BIT_OR).empty()
	     || !ctx->getTokens(TSqlParser::BIT_AND).empty()
	     || !ctx->getTokens(TSqlParser::BIT_XOR).empty()) {
		query_exit_function(_query);
	}

}

void ListenerInterface::enterPrimitive_expression(TSqlParser::Primitive_expressionContext * ctx) { }
void ListenerInterface::exitPrimitive_expression(TSqlParser::Primitive_expressionContext * ctx) { }

void ListenerInterface::enterBracket_expression(TSqlParser::Bracket_expressionContext * ctx) { }
void ListenerInterface::exitBracket_expression(TSqlParser::Bracket_expressionContext * ctx) { }

void ListenerInterface::enterSubquery(TSqlParser::SubqueryContext * ctx)
{
	/* Check if an operation is already defined.
	 * If it is, this is a sub-query
	 */
	if (_query->op != NULL) {
		_query = query_new(++_query_id);
		stack_push(&_query_stack, _query);
	}

}
void ListenerInterface::exitSubquery(TSqlParser::SubqueryContext * ctx)
{
	_subquery = (struct query*) stack_pop(&_query_stack);
	_query = (struct query*) _query_stack->data;

	//switch(_query->mode) {
	//case MODE_SELECT:
	//        new_expr->type = EXPR_SUBQUERY_CONST;
	//        select_add_column((struct select*) _query->op, new_expr, "");
	//        break;
	//case MODE_UPDATE:
	//        new_expr->type = EXPR_SUBQUERY;
	//        /* TODO */
	//        break;
	//case MODE_SOURCES:
	//        new_expr->type = EXPR_SUBQUERY;
	//        /* TODO */
	//        break;
	//case MODE_SEARCH:
	//        break;
	//default:
	//        std::cerr << "Undefined mode\n";
	//        exit(EXIT_FAILURE);
	//}
}

void ListenerInterface::enterSearch_condition(TSqlParser::Search_conditionContext * ctx)
{
	_query->mode = MODE_SEARCH;
	if (_query->logic_mode == LOGIC_UNDEFINED) {
		_query->logic_mode = LOGIC_WHERE;
	}
	enter_search(_query);
}
void ListenerInterface::exitSearch_condition(TSqlParser::Search_conditionContext * ctx)
{
	exit_search(_query);
}

void ListenerInterface::enterSearch_condition_and(TSqlParser::Search_condition_andContext * ctx)
{
	enter_search_and(_query);
}
void ListenerInterface::exitSearch_condition_and(TSqlParser::Search_condition_andContext * ctx)
{
	exit_search_and(_query);
}

void ListenerInterface::enterSearch_condition_not(TSqlParser::Search_condition_notContext * ctx)
{
	enter_search_not(_query);
}
void ListenerInterface::exitSearch_condition_not(TSqlParser::Search_condition_notContext * ctx)
{
	exit_search_not(_query);
}

void ListenerInterface::enterPredicate(TSqlParser::PredicateContext * ctx) { }
void ListenerInterface::exitPredicate(TSqlParser::PredicateContext * ctx) { }

void ListenerInterface::enterQuery_expression(TSqlParser::Query_expressionContext * ctx) { }
void ListenerInterface::exitQuery_expression(TSqlParser::Query_expressionContext * ctx) { }

void ListenerInterface::enterQuery_specification(TSqlParser::Query_specificationContext * ctx) { }
void ListenerInterface::exitQuery_specification(TSqlParser::Query_specificationContext * ctx) { }


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
			exit(EXIT_FAILURE);
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

void ListenerInterface::enterBackup_statement(TSqlParser::Backup_statementContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitBackup_statement(TSqlParser::Backup_statementContext * ctx) { }

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

void ListenerInterface::enterAlter_application_role(TSqlParser::Alter_application_roleContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_application_role(TSqlParser::Alter_application_roleContext * ctx) { }

void ListenerInterface::enterCreate_application_role(TSqlParser::Create_application_roleContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_application_role(TSqlParser::Create_application_roleContext * ctx) { }

void ListenerInterface::enterDrop_aggregate(TSqlParser::Drop_aggregateContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_aggregate(TSqlParser::Drop_aggregateContext * ctx) { }

void ListenerInterface::enterDrop_application_role(TSqlParser::Drop_application_roleContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_application_role(TSqlParser::Drop_application_roleContext * ctx) { }

void ListenerInterface::enterAlter_assembly(TSqlParser::Alter_assemblyContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_assembly(TSqlParser::Alter_assemblyContext * ctx) { }

void ListenerInterface::enterAlter_assembly_start(TSqlParser::Alter_assembly_startContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_assembly_start(TSqlParser::Alter_assembly_startContext * ctx) { }

void ListenerInterface::enterAlter_assembly_clause(TSqlParser::Alter_assembly_clauseContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_assembly_clause(TSqlParser::Alter_assembly_clauseContext * ctx) { }

void ListenerInterface::enterAlter_assembly_from_clause(TSqlParser::Alter_assembly_from_clauseContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_assembly_from_clause(TSqlParser::Alter_assembly_from_clauseContext * ctx) { }

void ListenerInterface::enterAlter_assembly_from_clause_start(TSqlParser::Alter_assembly_from_clause_startContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_assembly_from_clause_start(TSqlParser::Alter_assembly_from_clause_startContext * ctx) { }

void ListenerInterface::enterAlter_assembly_drop_clause(TSqlParser::Alter_assembly_drop_clauseContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_assembly_drop_clause(TSqlParser::Alter_assembly_drop_clauseContext * ctx) { }

void ListenerInterface::enterAlter_assembly_drop_multiple_files(TSqlParser::Alter_assembly_drop_multiple_filesContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_assembly_drop_multiple_files(TSqlParser::Alter_assembly_drop_multiple_filesContext * ctx) { }

void ListenerInterface::enterAlter_assembly_drop(TSqlParser::Alter_assembly_dropContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_assembly_drop(TSqlParser::Alter_assembly_dropContext * ctx) { }

void ListenerInterface::enterAlter_assembly_add_clause(TSqlParser::Alter_assembly_add_clauseContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_assembly_add_clause(TSqlParser::Alter_assembly_add_clauseContext * ctx) { }

void ListenerInterface::enterAlter_asssembly_add_clause_start(TSqlParser::Alter_asssembly_add_clause_startContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_asssembly_add_clause_start(TSqlParser::Alter_asssembly_add_clause_startContext * ctx) { }

void ListenerInterface::enterAlter_assembly_client_file_clause(TSqlParser::Alter_assembly_client_file_clauseContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_assembly_client_file_clause(TSqlParser::Alter_assembly_client_file_clauseContext * ctx) { }

void ListenerInterface::enterAlter_assembly_file_name(TSqlParser::Alter_assembly_file_nameContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_assembly_file_name(TSqlParser::Alter_assembly_file_nameContext * ctx) { }

void ListenerInterface::enterAlter_assembly_file_bits(TSqlParser::Alter_assembly_file_bitsContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_assembly_file_bits(TSqlParser::Alter_assembly_file_bitsContext * ctx) { }

void ListenerInterface::enterAlter_assembly_as(TSqlParser::Alter_assembly_asContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_assembly_as(TSqlParser::Alter_assembly_asContext * ctx) { }

void ListenerInterface::enterAlter_assembly_with_clause(TSqlParser::Alter_assembly_with_clauseContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_assembly_with_clause(TSqlParser::Alter_assembly_with_clauseContext * ctx) { }

void ListenerInterface::enterAlter_assembly_with(TSqlParser::Alter_assembly_withContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_assembly_with(TSqlParser::Alter_assembly_withContext * ctx) { }

void ListenerInterface::enterClient_assembly_specifier(TSqlParser::Client_assembly_specifierContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitClient_assembly_specifier(TSqlParser::Client_assembly_specifierContext * ctx) { }

void ListenerInterface::enterAssembly_option(TSqlParser::Assembly_optionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAssembly_option(TSqlParser::Assembly_optionContext * ctx) { }

void ListenerInterface::enterNetwork_file_share(TSqlParser::Network_file_shareContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitNetwork_file_share(TSqlParser::Network_file_shareContext * ctx) { }

void ListenerInterface::enterNetwork_computer(TSqlParser::Network_computerContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitNetwork_computer(TSqlParser::Network_computerContext * ctx) { }

void ListenerInterface::enterNetwork_file_start(TSqlParser::Network_file_startContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitNetwork_file_start(TSqlParser::Network_file_startContext * ctx) { }

void ListenerInterface::enterFile_path(TSqlParser::File_pathContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitFile_path(TSqlParser::File_pathContext * ctx) { }

void ListenerInterface::enterFile_directory_path_separator(TSqlParser::File_directory_path_separatorContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitFile_directory_path_separator(TSqlParser::File_directory_path_separatorContext * ctx) { }

void ListenerInterface::enterLocal_file(TSqlParser::Local_fileContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitLocal_file(TSqlParser::Local_fileContext * ctx) { }

void ListenerInterface::enterLocal_drive(TSqlParser::Local_driveContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitLocal_drive(TSqlParser::Local_driveContext * ctx) { }

void ListenerInterface::enterMultiple_local_files(TSqlParser::Multiple_local_filesContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitMultiple_local_files(TSqlParser::Multiple_local_filesContext * ctx) { }

void ListenerInterface::enterMultiple_local_file_start(TSqlParser::Multiple_local_file_startContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitMultiple_local_file_start(TSqlParser::Multiple_local_file_startContext * ctx) { }

void ListenerInterface::enterCreate_assembly(TSqlParser::Create_assemblyContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_assembly(TSqlParser::Create_assemblyContext * ctx) { }

void ListenerInterface::enterDrop_assembly(TSqlParser::Drop_assemblyContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_assembly(TSqlParser::Drop_assemblyContext * ctx) { }

void ListenerInterface::enterAlter_asymmetric_key(TSqlParser::Alter_asymmetric_keyContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_asymmetric_key(TSqlParser::Alter_asymmetric_keyContext * ctx) { }

void ListenerInterface::enterAlter_asymmetric_key_start(TSqlParser::Alter_asymmetric_key_startContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_asymmetric_key_start(TSqlParser::Alter_asymmetric_key_startContext * ctx) { }

void ListenerInterface::enterAsymmetric_key_option(TSqlParser::Asymmetric_key_optionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAsymmetric_key_option(TSqlParser::Asymmetric_key_optionContext * ctx) { }

void ListenerInterface::enterAsymmetric_key_option_start(TSqlParser::Asymmetric_key_option_startContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAsymmetric_key_option_start(TSqlParser::Asymmetric_key_option_startContext * ctx) { }

void ListenerInterface::enterAsymmetric_key_password_change_option(TSqlParser::Asymmetric_key_password_change_optionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAsymmetric_key_password_change_option(TSqlParser::Asymmetric_key_password_change_optionContext * ctx) { }

void ListenerInterface::enterCreate_asymmetric_key(TSqlParser::Create_asymmetric_keyContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_asymmetric_key(TSqlParser::Create_asymmetric_keyContext * ctx) { }

void ListenerInterface::enterDrop_asymmetric_key(TSqlParser::Drop_asymmetric_keyContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_asymmetric_key(TSqlParser::Drop_asymmetric_keyContext * ctx) { }

void ListenerInterface::enterAlter_authorization(TSqlParser::Alter_authorizationContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_authorization(TSqlParser::Alter_authorizationContext * ctx) { }

void ListenerInterface::enterAuthorization_grantee(TSqlParser::Authorization_granteeContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAuthorization_grantee(TSqlParser::Authorization_granteeContext * ctx) { }

void ListenerInterface::enterEntity_to(TSqlParser::Entity_toContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitEntity_to(TSqlParser::Entity_toContext * ctx) { }

void ListenerInterface::enterColon_colon(TSqlParser::Colon_colonContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitColon_colon(TSqlParser::Colon_colonContext * ctx) { }

void ListenerInterface::enterAlter_authorization_start(TSqlParser::Alter_authorization_startContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_authorization_start(TSqlParser::Alter_authorization_startContext * ctx) { }

void ListenerInterface::enterAlter_authorization_for_sql_database(TSqlParser::Alter_authorization_for_sql_databaseContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_authorization_for_sql_database(TSqlParser::Alter_authorization_for_sql_databaseContext * ctx) { }

void ListenerInterface::enterAlter_authorization_for_azure_dw(TSqlParser::Alter_authorization_for_azure_dwContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_authorization_for_azure_dw(TSqlParser::Alter_authorization_for_azure_dwContext * ctx) { }

void ListenerInterface::enterAlter_authorization_for_parallel_dw(TSqlParser::Alter_authorization_for_parallel_dwContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_authorization_for_parallel_dw(TSqlParser::Alter_authorization_for_parallel_dwContext * ctx) { }

void ListenerInterface::enterClass_type(TSqlParser::Class_typeContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitClass_type(TSqlParser::Class_typeContext * ctx) { }

void ListenerInterface::enterClass_type_for_sql_database(TSqlParser::Class_type_for_sql_databaseContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitClass_type_for_sql_database(TSqlParser::Class_type_for_sql_databaseContext * ctx) { }

void ListenerInterface::enterClass_type_for_azure_dw(TSqlParser::Class_type_for_azure_dwContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitClass_type_for_azure_dw(TSqlParser::Class_type_for_azure_dwContext * ctx) { }

void ListenerInterface::enterClass_type_for_parallel_dw(TSqlParser::Class_type_for_parallel_dwContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitClass_type_for_parallel_dw(TSqlParser::Class_type_for_parallel_dwContext * ctx) { }

void ListenerInterface::enterDrop_availability_group(TSqlParser::Drop_availability_groupContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_availability_group(TSqlParser::Drop_availability_groupContext * ctx) { }

void ListenerInterface::enterAlter_availability_group(TSqlParser::Alter_availability_groupContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_availability_group(TSqlParser::Alter_availability_groupContext * ctx) { }

void ListenerInterface::enterAlter_availability_group_start(TSqlParser::Alter_availability_group_startContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_availability_group_start(TSqlParser::Alter_availability_group_startContext * ctx) { }

void ListenerInterface::enterAlter_availability_group_options(TSqlParser::Alter_availability_group_optionsContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_availability_group_options(TSqlParser::Alter_availability_group_optionsContext * ctx) { }

void ListenerInterface::enterCreate_or_alter_broker_priority(TSqlParser::Create_or_alter_broker_priorityContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_or_alter_broker_priority(TSqlParser::Create_or_alter_broker_priorityContext * ctx) { }

void ListenerInterface::enterDrop_broker_priority(TSqlParser::Drop_broker_priorityContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_broker_priority(TSqlParser::Drop_broker_priorityContext * ctx) { }

void ListenerInterface::enterAlter_certificate(TSqlParser::Alter_certificateContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_certificate(TSqlParser::Alter_certificateContext * ctx) { }

void ListenerInterface::enterAlter_column_encryption_key(TSqlParser::Alter_column_encryption_keyContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_column_encryption_key(TSqlParser::Alter_column_encryption_keyContext * ctx) { }

void ListenerInterface::enterCreate_column_encryption_key(TSqlParser::Create_column_encryption_keyContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_column_encryption_key(TSqlParser::Create_column_encryption_keyContext * ctx) { }

void ListenerInterface::enterDrop_certificate(TSqlParser::Drop_certificateContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_certificate(TSqlParser::Drop_certificateContext * ctx) { }

void ListenerInterface::enterDrop_column_encryption_key(TSqlParser::Drop_column_encryption_keyContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_column_encryption_key(TSqlParser::Drop_column_encryption_keyContext * ctx) { }

void ListenerInterface::enterDrop_column_master_key(TSqlParser::Drop_column_master_keyContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_column_master_key(TSqlParser::Drop_column_master_keyContext * ctx) { }

void ListenerInterface::enterDrop_contract(TSqlParser::Drop_contractContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_contract(TSqlParser::Drop_contractContext * ctx) { }

void ListenerInterface::enterDrop_credential(TSqlParser::Drop_credentialContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_credential(TSqlParser::Drop_credentialContext * ctx) { }

void ListenerInterface::enterDrop_cryptograhic_provider(TSqlParser::Drop_cryptograhic_providerContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_cryptograhic_provider(TSqlParser::Drop_cryptograhic_providerContext * ctx) { }

void ListenerInterface::enterDrop_database(TSqlParser::Drop_databaseContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_database(TSqlParser::Drop_databaseContext * ctx) { }

void ListenerInterface::enterDrop_database_audit_specification(TSqlParser::Drop_database_audit_specificationContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_database_audit_specification(TSqlParser::Drop_database_audit_specificationContext * ctx) { }

void ListenerInterface::enterDrop_database_scoped_credential(TSqlParser::Drop_database_scoped_credentialContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_database_scoped_credential(TSqlParser::Drop_database_scoped_credentialContext * ctx) { }

void ListenerInterface::enterDrop_default(TSqlParser::Drop_defaultContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_default(TSqlParser::Drop_defaultContext * ctx) { }

void ListenerInterface::enterDrop_endpoint(TSqlParser::Drop_endpointContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_endpoint(TSqlParser::Drop_endpointContext * ctx) { }

void ListenerInterface::enterDrop_external_data_source(TSqlParser::Drop_external_data_sourceContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_external_data_source(TSqlParser::Drop_external_data_sourceContext * ctx) { }

void ListenerInterface::enterDrop_external_file_format(TSqlParser::Drop_external_file_formatContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_external_file_format(TSqlParser::Drop_external_file_formatContext * ctx) { }

void ListenerInterface::enterDrop_external_library(TSqlParser::Drop_external_libraryContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_external_library(TSqlParser::Drop_external_libraryContext * ctx) { }

void ListenerInterface::enterDrop_external_resource_pool(TSqlParser::Drop_external_resource_poolContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_external_resource_pool(TSqlParser::Drop_external_resource_poolContext * ctx) { }

void ListenerInterface::enterDrop_external_table(TSqlParser::Drop_external_tableContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_external_table(TSqlParser::Drop_external_tableContext * ctx) { }

void ListenerInterface::enterDrop_event_notifications(TSqlParser::Drop_event_notificationsContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_event_notifications(TSqlParser::Drop_event_notificationsContext * ctx) { }

void ListenerInterface::enterDrop_event_session(TSqlParser::Drop_event_sessionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_event_session(TSqlParser::Drop_event_sessionContext * ctx) { }

void ListenerInterface::enterDrop_fulltext_catalog(TSqlParser::Drop_fulltext_catalogContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_fulltext_catalog(TSqlParser::Drop_fulltext_catalogContext * ctx) { }

void ListenerInterface::enterDrop_fulltext_index(TSqlParser::Drop_fulltext_indexContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_fulltext_index(TSqlParser::Drop_fulltext_indexContext * ctx) { }

void ListenerInterface::enterDrop_fulltext_stoplist(TSqlParser::Drop_fulltext_stoplistContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_fulltext_stoplist(TSqlParser::Drop_fulltext_stoplistContext * ctx) { }

void ListenerInterface::enterDrop_login(TSqlParser::Drop_loginContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_login(TSqlParser::Drop_loginContext * ctx) { }

void ListenerInterface::enterDrop_master_key(TSqlParser::Drop_master_keyContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_master_key(TSqlParser::Drop_master_keyContext * ctx) { }

void ListenerInterface::enterDrop_message_type(TSqlParser::Drop_message_typeContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_message_type(TSqlParser::Drop_message_typeContext * ctx) { }

void ListenerInterface::enterDrop_partition_function(TSqlParser::Drop_partition_functionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_partition_function(TSqlParser::Drop_partition_functionContext * ctx) { }

void ListenerInterface::enterDrop_partition_scheme(TSqlParser::Drop_partition_schemeContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_partition_scheme(TSqlParser::Drop_partition_schemeContext * ctx) { }

void ListenerInterface::enterDrop_queue(TSqlParser::Drop_queueContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_queue(TSqlParser::Drop_queueContext * ctx) { }

void ListenerInterface::enterDrop_remote_service_binding(TSqlParser::Drop_remote_service_bindingContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_remote_service_binding(TSqlParser::Drop_remote_service_bindingContext * ctx) { }

void ListenerInterface::enterDrop_resource_pool(TSqlParser::Drop_resource_poolContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_resource_pool(TSqlParser::Drop_resource_poolContext * ctx) { }

void ListenerInterface::enterDrop_db_role(TSqlParser::Drop_db_roleContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_db_role(TSqlParser::Drop_db_roleContext * ctx) { }

void ListenerInterface::enterDrop_route(TSqlParser::Drop_routeContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_route(TSqlParser::Drop_routeContext * ctx) { }

void ListenerInterface::enterDrop_rule(TSqlParser::Drop_ruleContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_rule(TSqlParser::Drop_ruleContext * ctx) { }

void ListenerInterface::enterDrop_schema(TSqlParser::Drop_schemaContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_schema(TSqlParser::Drop_schemaContext * ctx) { }

void ListenerInterface::enterDrop_search_property_list(TSqlParser::Drop_search_property_listContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_search_property_list(TSqlParser::Drop_search_property_listContext * ctx) { }

void ListenerInterface::enterDrop_security_policy(TSqlParser::Drop_security_policyContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_security_policy(TSqlParser::Drop_security_policyContext * ctx) { }

void ListenerInterface::enterDrop_sequence(TSqlParser::Drop_sequenceContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_sequence(TSqlParser::Drop_sequenceContext * ctx) { }

void ListenerInterface::enterDrop_server_audit(TSqlParser::Drop_server_auditContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_server_audit(TSqlParser::Drop_server_auditContext * ctx) { }

void ListenerInterface::enterDrop_server_audit_specification(TSqlParser::Drop_server_audit_specificationContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_server_audit_specification(TSqlParser::Drop_server_audit_specificationContext * ctx) { }

void ListenerInterface::enterDrop_server_role(TSqlParser::Drop_server_roleContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_server_role(TSqlParser::Drop_server_roleContext * ctx) { }

void ListenerInterface::enterDrop_service(TSqlParser::Drop_serviceContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_service(TSqlParser::Drop_serviceContext * ctx) { }

void ListenerInterface::enterDrop_signature(TSqlParser::Drop_signatureContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_signature(TSqlParser::Drop_signatureContext * ctx) { }

void ListenerInterface::enterDrop_statistics_name_azure_dw_and_pdw(TSqlParser::Drop_statistics_name_azure_dw_and_pdwContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_statistics_name_azure_dw_and_pdw(TSqlParser::Drop_statistics_name_azure_dw_and_pdwContext * ctx) { }

void ListenerInterface::enterDrop_symmetric_key(TSqlParser::Drop_symmetric_keyContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_symmetric_key(TSqlParser::Drop_symmetric_keyContext * ctx) { }

void ListenerInterface::enterDrop_synonym(TSqlParser::Drop_synonymContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_synonym(TSqlParser::Drop_synonymContext * ctx) { }

void ListenerInterface::enterDrop_user(TSqlParser::Drop_userContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_user(TSqlParser::Drop_userContext * ctx) { }

void ListenerInterface::enterDrop_workload_group(TSqlParser::Drop_workload_groupContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_workload_group(TSqlParser::Drop_workload_groupContext * ctx) { }

void ListenerInterface::enterDrop_xml_schema_collection(TSqlParser::Drop_xml_schema_collectionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_xml_schema_collection(TSqlParser::Drop_xml_schema_collectionContext * ctx) { }

void ListenerInterface::enterDisable_trigger(TSqlParser::Disable_triggerContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDisable_trigger(TSqlParser::Disable_triggerContext * ctx) { }

void ListenerInterface::enterEnable_trigger(TSqlParser::Enable_triggerContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitEnable_trigger(TSqlParser::Enable_triggerContext * ctx) { }

void ListenerInterface::enterLock_table(TSqlParser::Lock_tableContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitLock_table(TSqlParser::Lock_tableContext * ctx) { }

void ListenerInterface::enterTruncate_table(TSqlParser::Truncate_tableContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitTruncate_table(TSqlParser::Truncate_tableContext * ctx) { }

void ListenerInterface::enterCreate_column_master_key(TSqlParser::Create_column_master_keyContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_column_master_key(TSqlParser::Create_column_master_keyContext * ctx) { }

void ListenerInterface::enterAlter_credential(TSqlParser::Alter_credentialContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_credential(TSqlParser::Alter_credentialContext * ctx) { }

void ListenerInterface::enterCreate_credential(TSqlParser::Create_credentialContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_credential(TSqlParser::Create_credentialContext * ctx) { }

void ListenerInterface::enterAlter_cryptographic_provider(TSqlParser::Alter_cryptographic_providerContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_cryptographic_provider(TSqlParser::Alter_cryptographic_providerContext * ctx) { }

void ListenerInterface::enterCreate_cryptographic_provider(TSqlParser::Create_cryptographic_providerContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_cryptographic_provider(TSqlParser::Create_cryptographic_providerContext * ctx) { }

void ListenerInterface::enterCreate_event_notification(TSqlParser::Create_event_notificationContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_event_notification(TSqlParser::Create_event_notificationContext * ctx) { }

void ListenerInterface::enterCreate_or_alter_event_session(TSqlParser::Create_or_alter_event_sessionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_or_alter_event_session(TSqlParser::Create_or_alter_event_sessionContext * ctx) { }

void ListenerInterface::enterEvent_session_predicate_expression(TSqlParser::Event_session_predicate_expressionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitEvent_session_predicate_expression(TSqlParser::Event_session_predicate_expressionContext * ctx) { }

void ListenerInterface::enterEvent_session_predicate_factor(TSqlParser::Event_session_predicate_factorContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitEvent_session_predicate_factor(TSqlParser::Event_session_predicate_factorContext * ctx) { }

void ListenerInterface::enterEvent_session_predicate_leaf(TSqlParser::Event_session_predicate_leafContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitEvent_session_predicate_leaf(TSqlParser::Event_session_predicate_leafContext * ctx) { }

void ListenerInterface::enterAlter_external_data_source(TSqlParser::Alter_external_data_sourceContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_external_data_source(TSqlParser::Alter_external_data_sourceContext * ctx) { }

void ListenerInterface::enterAlter_external_library(TSqlParser::Alter_external_libraryContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_external_library(TSqlParser::Alter_external_libraryContext * ctx) { }

void ListenerInterface::enterCreate_external_library(TSqlParser::Create_external_libraryContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_external_library(TSqlParser::Create_external_libraryContext * ctx) { }

void ListenerInterface::enterAlter_external_resource_pool(TSqlParser::Alter_external_resource_poolContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_external_resource_pool(TSqlParser::Alter_external_resource_poolContext * ctx) { }

void ListenerInterface::enterCreate_external_resource_pool(TSqlParser::Create_external_resource_poolContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_external_resource_pool(TSqlParser::Create_external_resource_poolContext * ctx) { }

void ListenerInterface::enterAlter_fulltext_catalog(TSqlParser::Alter_fulltext_catalogContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_fulltext_catalog(TSqlParser::Alter_fulltext_catalogContext * ctx) { }

void ListenerInterface::enterCreate_fulltext_catalog(TSqlParser::Create_fulltext_catalogContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_fulltext_catalog(TSqlParser::Create_fulltext_catalogContext * ctx) { }

void ListenerInterface::enterAlter_fulltext_stoplist(TSqlParser::Alter_fulltext_stoplistContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_fulltext_stoplist(TSqlParser::Alter_fulltext_stoplistContext * ctx) { }

void ListenerInterface::enterCreate_fulltext_stoplist(TSqlParser::Create_fulltext_stoplistContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_fulltext_stoplist(TSqlParser::Create_fulltext_stoplistContext * ctx) { }

void ListenerInterface::enterAlter_login_sql_server(TSqlParser::Alter_login_sql_serverContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_login_sql_server(TSqlParser::Alter_login_sql_serverContext * ctx) { }

void ListenerInterface::enterCreate_login_sql_server(TSqlParser::Create_login_sql_serverContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_login_sql_server(TSqlParser::Create_login_sql_serverContext * ctx) { }

void ListenerInterface::enterAlter_login_azure_sql(TSqlParser::Alter_login_azure_sqlContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_login_azure_sql(TSqlParser::Alter_login_azure_sqlContext * ctx) { }

void ListenerInterface::enterCreate_login_azure_sql(TSqlParser::Create_login_azure_sqlContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_login_azure_sql(TSqlParser::Create_login_azure_sqlContext * ctx) { }

void ListenerInterface::enterAlter_login_azure_sql_dw_and_pdw(TSqlParser::Alter_login_azure_sql_dw_and_pdwContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_login_azure_sql_dw_and_pdw(TSqlParser::Alter_login_azure_sql_dw_and_pdwContext * ctx) { }

void ListenerInterface::enterCreate_login_pdw(TSqlParser::Create_login_pdwContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_login_pdw(TSqlParser::Create_login_pdwContext * ctx) { }

void ListenerInterface::enterAlter_master_key_sql_server(TSqlParser::Alter_master_key_sql_serverContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_master_key_sql_server(TSqlParser::Alter_master_key_sql_serverContext * ctx) { }

void ListenerInterface::enterCreate_master_key_sql_server(TSqlParser::Create_master_key_sql_serverContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_master_key_sql_server(TSqlParser::Create_master_key_sql_serverContext * ctx) { }

void ListenerInterface::enterAlter_master_key_azure_sql(TSqlParser::Alter_master_key_azure_sqlContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_master_key_azure_sql(TSqlParser::Alter_master_key_azure_sqlContext * ctx) { }

void ListenerInterface::enterCreate_master_key_azure_sql(TSqlParser::Create_master_key_azure_sqlContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_master_key_azure_sql(TSqlParser::Create_master_key_azure_sqlContext * ctx) { }

void ListenerInterface::enterAlter_message_type(TSqlParser::Alter_message_typeContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_message_type(TSqlParser::Alter_message_typeContext * ctx) { }

void ListenerInterface::enterAlter_partition_function(TSqlParser::Alter_partition_functionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_partition_function(TSqlParser::Alter_partition_functionContext * ctx) { }

void ListenerInterface::enterAlter_partition_scheme(TSqlParser::Alter_partition_schemeContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_partition_scheme(TSqlParser::Alter_partition_schemeContext * ctx) { }

void ListenerInterface::enterAlter_remote_service_binding(TSqlParser::Alter_remote_service_bindingContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_remote_service_binding(TSqlParser::Alter_remote_service_bindingContext * ctx) { }

void ListenerInterface::enterCreate_remote_service_binding(TSqlParser::Create_remote_service_bindingContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_remote_service_binding(TSqlParser::Create_remote_service_bindingContext * ctx) { }

void ListenerInterface::enterCreate_resource_pool(TSqlParser::Create_resource_poolContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_resource_pool(TSqlParser::Create_resource_poolContext * ctx) { }

void ListenerInterface::enterAlter_resource_governor(TSqlParser::Alter_resource_governorContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_resource_governor(TSqlParser::Alter_resource_governorContext * ctx) { }

void ListenerInterface::enterAlter_db_role(TSqlParser::Alter_db_roleContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_db_role(TSqlParser::Alter_db_roleContext * ctx) { }

void ListenerInterface::enterCreate_db_role(TSqlParser::Create_db_roleContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_db_role(TSqlParser::Create_db_roleContext * ctx) { }

void ListenerInterface::enterCreate_route(TSqlParser::Create_routeContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_route(TSqlParser::Create_routeContext * ctx) { }

void ListenerInterface::enterCreate_rule(TSqlParser::Create_ruleContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_rule(TSqlParser::Create_ruleContext * ctx) { }

void ListenerInterface::enterAlter_schema_sql(TSqlParser::Alter_schema_sqlContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_schema_sql(TSqlParser::Alter_schema_sqlContext * ctx) { }

void ListenerInterface::enterCreate_schema(TSqlParser::Create_schemaContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_schema(TSqlParser::Create_schemaContext * ctx) { }

void ListenerInterface::enterCreate_schema_azure_sql_dw_and_pdw(TSqlParser::Create_schema_azure_sql_dw_and_pdwContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_schema_azure_sql_dw_and_pdw(TSqlParser::Create_schema_azure_sql_dw_and_pdwContext * ctx) { }

void ListenerInterface::enterAlter_schema_azure_sql_dw_and_pdw(TSqlParser::Alter_schema_azure_sql_dw_and_pdwContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_schema_azure_sql_dw_and_pdw(TSqlParser::Alter_schema_azure_sql_dw_and_pdwContext * ctx) { }

void ListenerInterface::enterCreate_search_property_list(TSqlParser::Create_search_property_listContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_search_property_list(TSqlParser::Create_search_property_listContext * ctx) { }

void ListenerInterface::enterCreate_security_policy(TSqlParser::Create_security_policyContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_security_policy(TSqlParser::Create_security_policyContext * ctx) { }

void ListenerInterface::enterAlter_sequence(TSqlParser::Alter_sequenceContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_sequence(TSqlParser::Alter_sequenceContext * ctx) { }

void ListenerInterface::enterCreate_sequence(TSqlParser::Create_sequenceContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_sequence(TSqlParser::Create_sequenceContext * ctx) { }

void ListenerInterface::enterAlter_server_audit(TSqlParser::Alter_server_auditContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_server_audit(TSqlParser::Alter_server_auditContext * ctx) { }

void ListenerInterface::enterCreate_server_audit(TSqlParser::Create_server_auditContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_server_audit(TSqlParser::Create_server_auditContext * ctx) { }

void ListenerInterface::enterAlter_server_audit_specification(TSqlParser::Alter_server_audit_specificationContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_server_audit_specification(TSqlParser::Alter_server_audit_specificationContext * ctx) { }

void ListenerInterface::enterCreate_server_audit_specification(TSqlParser::Create_server_audit_specificationContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_server_audit_specification(TSqlParser::Create_server_audit_specificationContext * ctx) { }

void ListenerInterface::enterAlter_server_configuration(TSqlParser::Alter_server_configurationContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_server_configuration(TSqlParser::Alter_server_configurationContext * ctx) { }

void ListenerInterface::enterAlter_server_role(TSqlParser::Alter_server_roleContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_server_role(TSqlParser::Alter_server_roleContext * ctx) { }

void ListenerInterface::enterCreate_server_role(TSqlParser::Create_server_roleContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_server_role(TSqlParser::Create_server_roleContext * ctx) { }

void ListenerInterface::enterAlter_server_role_pdw(TSqlParser::Alter_server_role_pdwContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_server_role_pdw(TSqlParser::Alter_server_role_pdwContext * ctx) { }

void ListenerInterface::enterAlter_service(TSqlParser::Alter_serviceContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_service(TSqlParser::Alter_serviceContext * ctx) { }

void ListenerInterface::enterCreate_service(TSqlParser::Create_serviceContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_service(TSqlParser::Create_serviceContext * ctx) { }

void ListenerInterface::enterAlter_service_master_key(TSqlParser::Alter_service_master_keyContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_service_master_key(TSqlParser::Alter_service_master_keyContext * ctx) { }

void ListenerInterface::enterAlter_symmetric_key(TSqlParser::Alter_symmetric_keyContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_symmetric_key(TSqlParser::Alter_symmetric_keyContext * ctx) { }

void ListenerInterface::enterCreate_symmetric_key(TSqlParser::Create_symmetric_keyContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_symmetric_key(TSqlParser::Create_symmetric_keyContext * ctx) { }

void ListenerInterface::enterCreate_synonym(TSqlParser::Create_synonymContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_synonym(TSqlParser::Create_synonymContext * ctx) { }

void ListenerInterface::enterAlter_user(TSqlParser::Alter_userContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_user(TSqlParser::Alter_userContext * ctx) { }

void ListenerInterface::enterCreate_user(TSqlParser::Create_userContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_user(TSqlParser::Create_userContext * ctx) { }

void ListenerInterface::enterCreate_user_azure_sql_dw(TSqlParser::Create_user_azure_sql_dwContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_user_azure_sql_dw(TSqlParser::Create_user_azure_sql_dwContext * ctx) { }

void ListenerInterface::enterAlter_user_azure_sql(TSqlParser::Alter_user_azure_sqlContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_user_azure_sql(TSqlParser::Alter_user_azure_sqlContext * ctx) { }

void ListenerInterface::enterAlter_workload_group(TSqlParser::Alter_workload_groupContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_workload_group(TSqlParser::Alter_workload_groupContext * ctx) { }

void ListenerInterface::enterCreate_workload_group(TSqlParser::Create_workload_groupContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_workload_group(TSqlParser::Create_workload_groupContext * ctx) { }

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

void ListenerInterface::enterCreate_contract(TSqlParser::Create_contractContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_contract(TSqlParser::Create_contractContext * ctx) { }

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

void ListenerInterface::enterCreate_or_alter_trigger(TSqlParser::Create_or_alter_triggerContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_or_alter_trigger(TSqlParser::Create_or_alter_triggerContext * ctx) { }

void ListenerInterface::enterCreate_or_alter_dml_trigger(TSqlParser::Create_or_alter_dml_triggerContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_or_alter_dml_trigger(TSqlParser::Create_or_alter_dml_triggerContext * ctx) { }

void ListenerInterface::enterDml_trigger_option(TSqlParser::Dml_trigger_optionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDml_trigger_option(TSqlParser::Dml_trigger_optionContext * ctx) { }

void ListenerInterface::enterDml_trigger_operation(TSqlParser::Dml_trigger_operationContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDml_trigger_operation(TSqlParser::Dml_trigger_operationContext * ctx) { }

void ListenerInterface::enterCreate_or_alter_ddl_trigger(TSqlParser::Create_or_alter_ddl_triggerContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_or_alter_ddl_trigger(TSqlParser::Create_or_alter_ddl_triggerContext * ctx) { }

void ListenerInterface::enterDdl_trigger_operation(TSqlParser::Ddl_trigger_operationContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDdl_trigger_operation(TSqlParser::Ddl_trigger_operationContext * ctx) { }

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

void ListenerInterface::enterCreate_statistics(TSqlParser::Create_statisticsContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_statistics(TSqlParser::Create_statisticsContext * ctx) { }

void ListenerInterface::enterUpdate_statistics(TSqlParser::Update_statisticsContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitUpdate_statistics(TSqlParser::Update_statisticsContext * ctx) { }

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

void ListenerInterface::enterAlter_database(TSqlParser::Alter_databaseContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_database(TSqlParser::Alter_databaseContext * ctx) { }

void ListenerInterface::enterDatabase_optionspec(TSqlParser::Database_optionspecContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDatabase_optionspec(TSqlParser::Database_optionspecContext * ctx) { }

void ListenerInterface::enterAuto_option(TSqlParser::Auto_optionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAuto_option(TSqlParser::Auto_optionContext * ctx) { }

void ListenerInterface::enterChange_tracking_option(TSqlParser::Change_tracking_optionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitChange_tracking_option(TSqlParser::Change_tracking_optionContext * ctx) { }

void ListenerInterface::enterChange_tracking_option_list(TSqlParser::Change_tracking_option_listContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitChange_tracking_option_list(TSqlParser::Change_tracking_option_listContext * ctx) { }

void ListenerInterface::enterContainment_option(TSqlParser::Containment_optionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitContainment_option(TSqlParser::Containment_optionContext * ctx) { }

void ListenerInterface::enterCursor_option(TSqlParser::Cursor_optionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCursor_option(TSqlParser::Cursor_optionContext * ctx) { }

void ListenerInterface::enterAlter_endpoint(TSqlParser::Alter_endpointContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlter_endpoint(TSqlParser::Alter_endpointContext * ctx) { }

void ListenerInterface::enterDatabase_mirroring_option(TSqlParser::Database_mirroring_optionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDatabase_mirroring_option(TSqlParser::Database_mirroring_optionContext * ctx) { }

void ListenerInterface::enterMirroring_set_option(TSqlParser::Mirroring_set_optionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitMirroring_set_option(TSqlParser::Mirroring_set_optionContext * ctx) { }

void ListenerInterface::enterMirroring_partner(TSqlParser::Mirroring_partnerContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitMirroring_partner(TSqlParser::Mirroring_partnerContext * ctx) { }

void ListenerInterface::enterMirroring_witness(TSqlParser::Mirroring_witnessContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitMirroring_witness(TSqlParser::Mirroring_witnessContext * ctx) { }

void ListenerInterface::enterWitness_partner_equal(TSqlParser::Witness_partner_equalContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitWitness_partner_equal(TSqlParser::Witness_partner_equalContext * ctx) { }

void ListenerInterface::enterPartner_option(TSqlParser::Partner_optionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitPartner_option(TSqlParser::Partner_optionContext * ctx) { }

void ListenerInterface::enterWitness_option(TSqlParser::Witness_optionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitWitness_option(TSqlParser::Witness_optionContext * ctx) { }

void ListenerInterface::enterWitness_server(TSqlParser::Witness_serverContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitWitness_server(TSqlParser::Witness_serverContext * ctx) { }

void ListenerInterface::enterPartner_server(TSqlParser::Partner_serverContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitPartner_server(TSqlParser::Partner_serverContext * ctx) { }

void ListenerInterface::enterMirroring_host_port_seperator(TSqlParser::Mirroring_host_port_seperatorContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitMirroring_host_port_seperator(TSqlParser::Mirroring_host_port_seperatorContext * ctx) { }

void ListenerInterface::enterPartner_server_tcp_prefix(TSqlParser::Partner_server_tcp_prefixContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitPartner_server_tcp_prefix(TSqlParser::Partner_server_tcp_prefixContext * ctx) { }

void ListenerInterface::enterPort_number(TSqlParser::Port_numberContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitPort_number(TSqlParser::Port_numberContext * ctx) { }

void ListenerInterface::enterHost(TSqlParser::HostContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitHost(TSqlParser::HostContext * ctx) { }

void ListenerInterface::enterDate_correlation_optimization_option(TSqlParser::Date_correlation_optimization_optionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDate_correlation_optimization_option(TSqlParser::Date_correlation_optimization_optionContext * ctx) { }

void ListenerInterface::enterDb_encryption_option(TSqlParser::Db_encryption_optionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDb_encryption_option(TSqlParser::Db_encryption_optionContext * ctx) { }

void ListenerInterface::enterDb_state_option(TSqlParser::Db_state_optionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDb_state_option(TSqlParser::Db_state_optionContext * ctx) { }

void ListenerInterface::enterDb_update_option(TSqlParser::Db_update_optionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDb_update_option(TSqlParser::Db_update_optionContext * ctx) { }

void ListenerInterface::enterDb_user_access_option(TSqlParser::Db_user_access_optionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDb_user_access_option(TSqlParser::Db_user_access_optionContext * ctx) { }

void ListenerInterface::enterDelayed_durability_option(TSqlParser::Delayed_durability_optionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDelayed_durability_option(TSqlParser::Delayed_durability_optionContext * ctx) { }

void ListenerInterface::enterExternal_access_option(TSqlParser::External_access_optionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitExternal_access_option(TSqlParser::External_access_optionContext * ctx) { }

void ListenerInterface::enterHadr_options(TSqlParser::Hadr_optionsContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitHadr_options(TSqlParser::Hadr_optionsContext * ctx) { }

void ListenerInterface::enterMixed_page_allocation_option(TSqlParser::Mixed_page_allocation_optionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitMixed_page_allocation_option(TSqlParser::Mixed_page_allocation_optionContext * ctx) { }

void ListenerInterface::enterParameterization_option(TSqlParser::Parameterization_optionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitParameterization_option(TSqlParser::Parameterization_optionContext * ctx) { }

void ListenerInterface::enterRecovery_option(TSqlParser::Recovery_optionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitRecovery_option(TSqlParser::Recovery_optionContext * ctx) { }

void ListenerInterface::enterService_broker_option(TSqlParser::Service_broker_optionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitService_broker_option(TSqlParser::Service_broker_optionContext * ctx) { }

void ListenerInterface::enterSnapshot_option(TSqlParser::Snapshot_optionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitSnapshot_option(TSqlParser::Snapshot_optionContext * ctx) { }

void ListenerInterface::enterSql_option(TSqlParser::Sql_optionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitSql_option(TSqlParser::Sql_optionContext * ctx) { }

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

void ListenerInterface::enterDrop_trigger(TSqlParser::Drop_triggerContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_trigger(TSqlParser::Drop_triggerContext * ctx) { }

void ListenerInterface::enterDrop_dml_trigger(TSqlParser::Drop_dml_triggerContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_dml_trigger(TSqlParser::Drop_dml_triggerContext * ctx) { }

void ListenerInterface::enterDrop_ddl_trigger(TSqlParser::Drop_ddl_triggerContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_ddl_trigger(TSqlParser::Drop_ddl_triggerContext * ctx) { }

void ListenerInterface::enterDrop_function(TSqlParser::Drop_functionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_function(TSqlParser::Drop_functionContext * ctx) { }

void ListenerInterface::enterDrop_statistics(TSqlParser::Drop_statisticsContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDrop_statistics(TSqlParser::Drop_statisticsContext * ctx) { }

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

void ListenerInterface::enterBackup_database(TSqlParser::Backup_databaseContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitBackup_database(TSqlParser::Backup_databaseContext * ctx) { }

void ListenerInterface::enterBackup_log(TSqlParser::Backup_logContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitBackup_log(TSqlParser::Backup_logContext * ctx) { }

void ListenerInterface::enterBackup_certificate(TSqlParser::Backup_certificateContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitBackup_certificate(TSqlParser::Backup_certificateContext * ctx) { }

void ListenerInterface::enterBackup_master_key(TSqlParser::Backup_master_keyContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitBackup_master_key(TSqlParser::Backup_master_keyContext * ctx) { }

void ListenerInterface::enterBackup_service_master_key(TSqlParser::Backup_service_master_keyContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitBackup_service_master_key(TSqlParser::Backup_service_master_keyContext * ctx) { }

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

void ListenerInterface::enterSecurity_statement(TSqlParser::Security_statementContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitSecurity_statement(TSqlParser::Security_statementContext * ctx) { }

void ListenerInterface::enterCreate_certificate(TSqlParser::Create_certificateContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_certificate(TSqlParser::Create_certificateContext * ctx) { }

void ListenerInterface::enterExisting_keys(TSqlParser::Existing_keysContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitExisting_keys(TSqlParser::Existing_keysContext * ctx) { }

void ListenerInterface::enterPrivate_key_options(TSqlParser::Private_key_optionsContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitPrivate_key_options(TSqlParser::Private_key_optionsContext * ctx) { }

void ListenerInterface::enterGenerate_new_keys(TSqlParser::Generate_new_keysContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitGenerate_new_keys(TSqlParser::Generate_new_keysContext * ctx) { }

void ListenerInterface::enterDate_options(TSqlParser::Date_optionsContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDate_options(TSqlParser::Date_optionsContext * ctx) { }

void ListenerInterface::enterOpen_key(TSqlParser::Open_keyContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitOpen_key(TSqlParser::Open_keyContext * ctx) { }

void ListenerInterface::enterClose_key(TSqlParser::Close_keyContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitClose_key(TSqlParser::Close_keyContext * ctx) { }

void ListenerInterface::enterCreate_key(TSqlParser::Create_keyContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCreate_key(TSqlParser::Create_keyContext * ctx) { }

void ListenerInterface::enterKey_options(TSqlParser::Key_optionsContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitKey_options(TSqlParser::Key_optionsContext * ctx) { }

void ListenerInterface::enterAlgorithm(TSqlParser::AlgorithmContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAlgorithm(TSqlParser::AlgorithmContext * ctx) { }

void ListenerInterface::enterEncryption_mechanism(TSqlParser::Encryption_mechanismContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitEncryption_mechanism(TSqlParser::Encryption_mechanismContext * ctx) { }

void ListenerInterface::enterDecryption_mechanism(TSqlParser::Decryption_mechanismContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitDecryption_mechanism(TSqlParser::Decryption_mechanismContext * ctx) { }

void ListenerInterface::enterGrant_permission(TSqlParser::Grant_permissionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitGrant_permission(TSqlParser::Grant_permissionContext * ctx) { }

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

void ListenerInterface::enterCase_expression(TSqlParser::Case_expressionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitCase_expression(TSqlParser::Case_expressionContext * ctx) { }

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

void ListenerInterface::enterSql_union(TSqlParser::Sql_unionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitSql_union(TSqlParser::Sql_unionContext * ctx) { }

void ListenerInterface::enterTop_clause(TSqlParser::Top_clauseContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitTop_clause(TSqlParser::Top_clauseContext * ctx) { }

void ListenerInterface::enterTop_percent(TSqlParser::Top_percentContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitTop_percent(TSqlParser::Top_percentContext * ctx) { }

void ListenerInterface::enterTop_count(TSqlParser::Top_countContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitTop_count(TSqlParser::Top_countContext * ctx) { }

void ListenerInterface::enterOrder_by_clause(TSqlParser::Order_by_clauseContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitOrder_by_clause(TSqlParser::Order_by_clauseContext * ctx) { }

void ListenerInterface::enterFor_clause(TSqlParser::For_clauseContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitFor_clause(TSqlParser::For_clauseContext * ctx) { }

void ListenerInterface::enterXml_common_directives(TSqlParser::Xml_common_directivesContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitXml_common_directives(TSqlParser::Xml_common_directivesContext * ctx) { }

void ListenerInterface::enterOrder_by_expression(TSqlParser::Order_by_expressionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitOrder_by_expression(TSqlParser::Order_by_expressionContext * ctx) { }

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

void ListenerInterface::enterAGGREGATE_WINDOWED_FUNC(TSqlParser::AGGREGATE_WINDOWED_FUNCContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAGGREGATE_WINDOWED_FUNC(TSqlParser::AGGREGATE_WINDOWED_FUNCContext * ctx) { }

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

void ListenerInterface::enterSwitch_section(TSqlParser::Switch_sectionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitSwitch_section(TSqlParser::Switch_sectionContext * ctx) { }

void ListenerInterface::enterSwitch_search_condition_section(TSqlParser::Switch_search_condition_sectionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitSwitch_search_condition_section(TSqlParser::Switch_search_condition_sectionContext * ctx) { }

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

void ListenerInterface::enterAggregate_windowed_function(TSqlParser::Aggregate_windowed_functionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAggregate_windowed_function(TSqlParser::Aggregate_windowed_functionContext * ctx) { }

void ListenerInterface::enterAnalytic_windowed_function(TSqlParser::Analytic_windowed_functionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAnalytic_windowed_function(TSqlParser::Analytic_windowed_functionContext * ctx) { }

void ListenerInterface::enterAll_distinct_expression(TSqlParser::All_distinct_expressionContext * ctx) { _no_impl(ctx->getStart()->getText(), ctx->getRuleIndex()); }
void ListenerInterface::exitAll_distinct_expression(TSqlParser::All_distinct_expressionContext * ctx) { }

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


