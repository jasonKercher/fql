#ifndef LISTENERINTERFACE_H
#define LISTENERINTERFACE_H

#include "gen/TSqlParserBaseListener.h"
#include "treewalker.h"

#include "fqlhandle.h"
#include "fqlimits.h"
#include "query.h"
#include "util/node.h"

class ListenerInterface : public TSqlParserBaseListener {
      private:
	enum tok_type {
		TOK_UNDEFINED,
		TOK_OP_TABLE,
		TOK_INTO_TABLE,
		TOK_COLUMN_NAME,
		TOK_COLUMN_ALIAS,
		TOK_TABLE_NAME,
		TOK_TABLE_SOURCE,
		TOK_TABLE_ALIAS,
		TOK_DATA_TYPE,
	};

	std::vector<std::string> _rule_names;

	TreeWalker* _walker = NULL;

	struct fql_handle* _fql = NULL;
	struct node* _query_stack = NULL;
	struct query* _subquery = NULL;
	struct query* _query = NULL;

	struct node* _source_stack = NULL;
	struct node* _column_stack = NULL;

	char _table_name[TABLE_NAME_MAX + 1];
	char _table_alias[TABLE_NAME_MAX + 1];

	int _query_id = 0;
	int _return_code = 0;
	enum tok_type _next_tok_type;
	enum tok_type _tok_type;
	bool _on_asterisk = false;

	void _set_failure();
	void _no_impl(const std::string&, int);

      public:
	ListenerInterface(struct fql_handle*, TreeWalker*, const std::vector<std::string>&);
	~ListenerInterface();
	int get_return_code();

	virtual void enterTsql_file(TSqlParser::Tsql_fileContext * /*ctx*/) override;
	virtual void exitTsql_file(TSqlParser::Tsql_fileContext * /*ctx*/) override;

	virtual void enterBatch(TSqlParser::BatchContext * /*ctx*/) override;
	virtual void exitBatch(TSqlParser::BatchContext * /*ctx*/) override;

	virtual void enterBatch_level_statement(TSqlParser::Batch_level_statementContext * /*ctx*/) override;
	virtual void exitBatch_level_statement(TSqlParser::Batch_level_statementContext * /*ctx*/) override;

	virtual void enterSql_clauses(TSqlParser::Sql_clausesContext * /*ctx*/) override;
	virtual void exitSql_clauses(TSqlParser::Sql_clausesContext * /*ctx*/) override;

	virtual void enterDml_clause(TSqlParser::Dml_clauseContext * /*ctx*/) override;
	virtual void exitDml_clause(TSqlParser::Dml_clauseContext * /*ctx*/) override;

	virtual void enterDdl_clause(TSqlParser::Ddl_clauseContext * /*ctx*/) override;
	virtual void exitDdl_clause(TSqlParser::Ddl_clauseContext * /*ctx*/) override;

	virtual void enterCfl_statement(TSqlParser::Cfl_statementContext * /*ctx*/) override;
	virtual void exitCfl_statement(TSqlParser::Cfl_statementContext * /*ctx*/) override;

	virtual void enterBlock_statement(TSqlParser::Block_statementContext * /*ctx*/) override;
	virtual void exitBlock_statement(TSqlParser::Block_statementContext * /*ctx*/) override;

	virtual void enterBreak_statement(TSqlParser::Break_statementContext * /*ctx*/) override;
	virtual void exitBreak_statement(TSqlParser::Break_statementContext * /*ctx*/) override;

	virtual void enterContinue_statement(TSqlParser::Continue_statementContext * /*ctx*/) override;
	virtual void exitContinue_statement(TSqlParser::Continue_statementContext * /*ctx*/) override;

	virtual void enterGoto_statement(TSqlParser::Goto_statementContext * /*ctx*/) override;
	virtual void exitGoto_statement(TSqlParser::Goto_statementContext * /*ctx*/) override;

	virtual void enterReturn_statement(TSqlParser::Return_statementContext * /*ctx*/) override;
	virtual void exitReturn_statement(TSqlParser::Return_statementContext * /*ctx*/) override;

	virtual void enterIf_statement(TSqlParser::If_statementContext * /*ctx*/) override;
	virtual void exitIf_statement(TSqlParser::If_statementContext * /*ctx*/) override;

	virtual void enterThrow_statement(TSqlParser::Throw_statementContext * /*ctx*/) override;
	virtual void exitThrow_statement(TSqlParser::Throw_statementContext * /*ctx*/) override;

	virtual void enterThrow_error_number(TSqlParser::Throw_error_numberContext * /*ctx*/) override;
	virtual void exitThrow_error_number(TSqlParser::Throw_error_numberContext * /*ctx*/) override;

	virtual void enterThrow_message(TSqlParser::Throw_messageContext * /*ctx*/) override;
	virtual void exitThrow_message(TSqlParser::Throw_messageContext * /*ctx*/) override;

	virtual void enterThrow_state(TSqlParser::Throw_stateContext * /*ctx*/) override;
	virtual void exitThrow_state(TSqlParser::Throw_stateContext * /*ctx*/) override;

	virtual void enterTry_catch_statement(TSqlParser::Try_catch_statementContext * /*ctx*/) override;
	virtual void exitTry_catch_statement(TSqlParser::Try_catch_statementContext * /*ctx*/) override;

	virtual void enterWhile_statement(TSqlParser::While_statementContext * /*ctx*/) override;
	virtual void exitWhile_statement(TSqlParser::While_statementContext * /*ctx*/) override;

	virtual void enterPrint_statement(TSqlParser::Print_statementContext * /*ctx*/) override;
	virtual void exitPrint_statement(TSqlParser::Print_statementContext * /*ctx*/) override;

	virtual void enterRaiseerror_statement(TSqlParser::Raiseerror_statementContext * /*ctx*/) override;
	virtual void exitRaiseerror_statement(TSqlParser::Raiseerror_statementContext * /*ctx*/) override;

	virtual void enterEmpty_statement(TSqlParser::Empty_statementContext * /*ctx*/) override;
	virtual void exitEmpty_statement(TSqlParser::Empty_statementContext * /*ctx*/) override;

	virtual void enterAnother_statement(TSqlParser::Another_statementContext * /*ctx*/) override;
	virtual void exitAnother_statement(TSqlParser::Another_statementContext * /*ctx*/) override;

	virtual void enterEntity_to(TSqlParser::Entity_toContext * /*ctx*/) override;
	virtual void exitEntity_to(TSqlParser::Entity_toContext * /*ctx*/) override;

	virtual void enterColon_colon(TSqlParser::Colon_colonContext * /*ctx*/) override;
	virtual void exitColon_colon(TSqlParser::Colon_colonContext * /*ctx*/) override;

	virtual void enterClass_type(TSqlParser::Class_typeContext * /*ctx*/) override;
	virtual void exitClass_type(TSqlParser::Class_typeContext * /*ctx*/) override;

	virtual void enterClass_type_for_sql_database(TSqlParser::Class_type_for_sql_databaseContext * /*ctx*/) override;
	virtual void exitClass_type_for_sql_database(TSqlParser::Class_type_for_sql_databaseContext * /*ctx*/) override;

	virtual void enterClass_type_for_azure_dw(TSqlParser::Class_type_for_azure_dwContext * /*ctx*/) override;
	virtual void exitClass_type_for_azure_dw(TSqlParser::Class_type_for_azure_dwContext * /*ctx*/) override;

	virtual void enterClass_type_for_parallel_dw(TSqlParser::Class_type_for_parallel_dwContext * /*ctx*/) override;
	virtual void exitClass_type_for_parallel_dw(TSqlParser::Class_type_for_parallel_dwContext * /*ctx*/) override;

	virtual void enterLock_table(TSqlParser::Lock_tableContext * /*ctx*/) override;
	virtual void exitLock_table(TSqlParser::Lock_tableContext * /*ctx*/) override;

	virtual void enterTruncate_table(TSqlParser::Truncate_tableContext * /*ctx*/) override;
	virtual void exitTruncate_table(TSqlParser::Truncate_tableContext * /*ctx*/) override;

	virtual void enterAlter_sequence(TSqlParser::Alter_sequenceContext * /*ctx*/) override;
	virtual void exitAlter_sequence(TSqlParser::Alter_sequenceContext * /*ctx*/) override;

	virtual void enterCreate_sequence(TSqlParser::Create_sequenceContext * /*ctx*/) override;
	virtual void exitCreate_sequence(TSqlParser::Create_sequenceContext * /*ctx*/) override;

	virtual void enterMerge_statement(TSqlParser::Merge_statementContext * /*ctx*/) override;
	virtual void exitMerge_statement(TSqlParser::Merge_statementContext * /*ctx*/) override;

	virtual void enterWhen_matches(TSqlParser::When_matchesContext * /*ctx*/) override;
	virtual void exitWhen_matches(TSqlParser::When_matchesContext * /*ctx*/) override;

	virtual void enterMerge_matched(TSqlParser::Merge_matchedContext * /*ctx*/) override;
	virtual void exitMerge_matched(TSqlParser::Merge_matchedContext * /*ctx*/) override;

	virtual void enterMerge_not_matched(TSqlParser::Merge_not_matchedContext * /*ctx*/) override;
	virtual void exitMerge_not_matched(TSqlParser::Merge_not_matchedContext * /*ctx*/) override;

	virtual void enterDelete_statement(TSqlParser::Delete_statementContext * /*ctx*/) override;
	virtual void exitDelete_statement(TSqlParser::Delete_statementContext * /*ctx*/) override;

	virtual void enterDelete_statement_from(TSqlParser::Delete_statement_fromContext * /*ctx*/) override;
	virtual void exitDelete_statement_from(TSqlParser::Delete_statement_fromContext * /*ctx*/) override;

	virtual void enterInsert_statement(TSqlParser::Insert_statementContext * /*ctx*/) override;
	virtual void exitInsert_statement(TSqlParser::Insert_statementContext * /*ctx*/) override;

	virtual void enterInsert_statement_value(TSqlParser::Insert_statement_valueContext * /*ctx*/) override;
	virtual void exitInsert_statement_value(TSqlParser::Insert_statement_valueContext * /*ctx*/) override;

	virtual void enterSelect_statement_standalone(TSqlParser::Select_statement_standaloneContext * /*ctx*/) override;
	virtual void exitSelect_statement_standalone(TSqlParser::Select_statement_standaloneContext * /*ctx*/) override;

	virtual void enterSelect_statement(TSqlParser::Select_statementContext * /*ctx*/) override;
	virtual void exitSelect_statement(TSqlParser::Select_statementContext * /*ctx*/) override;

	virtual void enterTime(TSqlParser::TimeContext * /*ctx*/) override;
	virtual void exitTime(TSqlParser::TimeContext * /*ctx*/) override;

	virtual void enterUpdate_statement(TSqlParser::Update_statementContext * /*ctx*/) override;
	virtual void exitUpdate_statement(TSqlParser::Update_statementContext * /*ctx*/) override;

	virtual void enterOutput_clause(TSqlParser::Output_clauseContext * /*ctx*/) override;
	virtual void exitOutput_clause(TSqlParser::Output_clauseContext * /*ctx*/) override;

	virtual void enterOutput_dml_list_elem(TSqlParser::Output_dml_list_elemContext * /*ctx*/) override;
	virtual void exitOutput_dml_list_elem(TSqlParser::Output_dml_list_elemContext * /*ctx*/) override;

	virtual void enterCreate_or_alter_procedure(TSqlParser::Create_or_alter_procedureContext * /*ctx*/) override;
	virtual void exitCreate_or_alter_procedure(TSqlParser::Create_or_alter_procedureContext * /*ctx*/) override;

	virtual void enterCreate_or_alter_function(TSqlParser::Create_or_alter_functionContext * /*ctx*/) override;
	virtual void exitCreate_or_alter_function(TSqlParser::Create_or_alter_functionContext * /*ctx*/) override;

	virtual void enterFunc_body_returns_select(TSqlParser::Func_body_returns_selectContext * /*ctx*/) override;
	virtual void exitFunc_body_returns_select(TSqlParser::Func_body_returns_selectContext * /*ctx*/) override;

	virtual void enterFunc_body_returns_table(TSqlParser::Func_body_returns_tableContext * /*ctx*/) override;
	virtual void exitFunc_body_returns_table(TSqlParser::Func_body_returns_tableContext * /*ctx*/) override;

	virtual void enterFunc_body_returns_scalar(TSqlParser::Func_body_returns_scalarContext * /*ctx*/) override;
	virtual void exitFunc_body_returns_scalar(TSqlParser::Func_body_returns_scalarContext * /*ctx*/) override;

	virtual void enterProcedure_param(TSqlParser::Procedure_paramContext * /*ctx*/) override;
	virtual void exitProcedure_param(TSqlParser::Procedure_paramContext * /*ctx*/) override;

	virtual void enterProcedure_option(TSqlParser::Procedure_optionContext * /*ctx*/) override;
	virtual void exitProcedure_option(TSqlParser::Procedure_optionContext * /*ctx*/) override;

	virtual void enterFunction_option(TSqlParser::Function_optionContext * /*ctx*/) override;
	virtual void exitFunction_option(TSqlParser::Function_optionContext * /*ctx*/) override;

	virtual void enterCreate_table(TSqlParser::Create_tableContext * /*ctx*/) override;
	virtual void exitCreate_table(TSqlParser::Create_tableContext * /*ctx*/) override;

	virtual void enterTable_options(TSqlParser::Table_optionsContext * /*ctx*/) override;
	virtual void exitTable_options(TSqlParser::Table_optionsContext * /*ctx*/) override;

	virtual void enterAlter_table(TSqlParser::Alter_tableContext * /*ctx*/) override;
	virtual void exitAlter_table(TSqlParser::Alter_tableContext * /*ctx*/) override;

	virtual void enterDatabase_optionspec(TSqlParser::Database_optionspecContext * /*ctx*/) override;
	virtual void exitDatabase_optionspec(TSqlParser::Database_optionspecContext * /*ctx*/) override;

	virtual void enterCursor_option(TSqlParser::Cursor_optionContext * /*ctx*/) override;
	virtual void exitCursor_option(TSqlParser::Cursor_optionContext * /*ctx*/) override;

	virtual void enterDrop_procedure(TSqlParser::Drop_procedureContext * /*ctx*/) override;
	virtual void exitDrop_procedure(TSqlParser::Drop_procedureContext * /*ctx*/) override;

	virtual void enterDrop_function(TSqlParser::Drop_functionContext * /*ctx*/) override;
	virtual void exitDrop_function(TSqlParser::Drop_functionContext * /*ctx*/) override;

	virtual void enterDrop_table(TSqlParser::Drop_tableContext * /*ctx*/) override;
	virtual void exitDrop_table(TSqlParser::Drop_tableContext * /*ctx*/) override;

	virtual void enterDeclare_statement(TSqlParser::Declare_statementContext * /*ctx*/) override;
	virtual void exitDeclare_statement(TSqlParser::Declare_statementContext * /*ctx*/) override;

	virtual void enterCursor_statement(TSqlParser::Cursor_statementContext * /*ctx*/) override;
	virtual void exitCursor_statement(TSqlParser::Cursor_statementContext * /*ctx*/) override;

	virtual void enterKill_statement(TSqlParser::Kill_statementContext * /*ctx*/) override;
	virtual void exitKill_statement(TSqlParser::Kill_statementContext * /*ctx*/) override;

	virtual void enterKill_process(TSqlParser::Kill_processContext * /*ctx*/) override;
	virtual void exitKill_process(TSqlParser::Kill_processContext * /*ctx*/) override;

	virtual void enterKill_query_notification(TSqlParser::Kill_query_notificationContext * /*ctx*/) override;
	virtual void exitKill_query_notification(TSqlParser::Kill_query_notificationContext * /*ctx*/) override;

	virtual void enterExecute_statement(TSqlParser::Execute_statementContext * /*ctx*/) override;
	virtual void exitExecute_statement(TSqlParser::Execute_statementContext * /*ctx*/) override;

	virtual void enterExecute_body_batch(TSqlParser::Execute_body_batchContext * /*ctx*/) override;
	virtual void exitExecute_body_batch(TSqlParser::Execute_body_batchContext * /*ctx*/) override;

	virtual void enterExecute_body(TSqlParser::Execute_bodyContext * /*ctx*/) override;
	virtual void exitExecute_body(TSqlParser::Execute_bodyContext * /*ctx*/) override;

	virtual void enterExecute_statement_arg(TSqlParser::Execute_statement_argContext * /*ctx*/) override;
	virtual void exitExecute_statement_arg(TSqlParser::Execute_statement_argContext * /*ctx*/) override;

	virtual void enterExecute_statement_arg_named(TSqlParser::Execute_statement_arg_namedContext * /*ctx*/) override;
	virtual void exitExecute_statement_arg_named(TSqlParser::Execute_statement_arg_namedContext * /*ctx*/) override;

	virtual void enterExecute_statement_arg_unnamed(TSqlParser::Execute_statement_arg_unnamedContext * /*ctx*/) override;
	virtual void exitExecute_statement_arg_unnamed(TSqlParser::Execute_statement_arg_unnamedContext * /*ctx*/) override;

	virtual void enterExecute_parameter(TSqlParser::Execute_parameterContext * /*ctx*/) override;
	virtual void exitExecute_parameter(TSqlParser::Execute_parameterContext * /*ctx*/) override;

	virtual void enterExecute_var_string(TSqlParser::Execute_var_stringContext * /*ctx*/) override;
	virtual void exitExecute_var_string(TSqlParser::Execute_var_stringContext * /*ctx*/) override;

	virtual void enterAlgorithm(TSqlParser::AlgorithmContext * /*ctx*/) override;
	virtual void exitAlgorithm(TSqlParser::AlgorithmContext * /*ctx*/) override;

	virtual void enterSet_statement(TSqlParser::Set_statementContext * /*ctx*/) override;
	virtual void exitSet_statement(TSqlParser::Set_statementContext * /*ctx*/) override;

	virtual void enterTransaction_statement(TSqlParser::Transaction_statementContext * /*ctx*/) override;
	virtual void exitTransaction_statement(TSqlParser::Transaction_statementContext * /*ctx*/) override;

	virtual void enterGo_batch_statement(TSqlParser::Go_batch_statementContext * /*ctx*/) override;
	virtual void exitGo_batch_statement(TSqlParser::Go_batch_statementContext * /*ctx*/) override;

	virtual void enterGo_statement(TSqlParser::Go_statementContext * /*ctx*/) override;
	virtual void exitGo_statement(TSqlParser::Go_statementContext * /*ctx*/) override;

	virtual void enterUse_statement(TSqlParser::Use_statementContext * /*ctx*/) override;
	virtual void exitUse_statement(TSqlParser::Use_statementContext * /*ctx*/) override;

	virtual void enterShutdown_statement(TSqlParser::Shutdown_statementContext * /*ctx*/) override;
	virtual void exitShutdown_statement(TSqlParser::Shutdown_statementContext * /*ctx*/) override;

	virtual void enterDbcc_special(TSqlParser::Dbcc_specialContext * /*ctx*/) override;
	virtual void exitDbcc_special(TSqlParser::Dbcc_specialContext * /*ctx*/) override;

	virtual void enterDbcc_clause(TSqlParser::Dbcc_clauseContext * /*ctx*/) override;
	virtual void exitDbcc_clause(TSqlParser::Dbcc_clauseContext * /*ctx*/) override;

	virtual void enterDbcc_command(TSqlParser::Dbcc_commandContext * /*ctx*/) override;
	virtual void exitDbcc_command(TSqlParser::Dbcc_commandContext * /*ctx*/) override;

	virtual void enterDbcc_options(TSqlParser::Dbcc_optionsContext * /*ctx*/) override;
	virtual void exitDbcc_options(TSqlParser::Dbcc_optionsContext * /*ctx*/) override;

	virtual void enterExecute_clause(TSqlParser::Execute_clauseContext * /*ctx*/) override;
	virtual void exitExecute_clause(TSqlParser::Execute_clauseContext * /*ctx*/) override;

	virtual void enterDeclare_local(TSqlParser::Declare_localContext * /*ctx*/) override;
	virtual void exitDeclare_local(TSqlParser::Declare_localContext * /*ctx*/) override;

	virtual void enterTable_type_definition(TSqlParser::Table_type_definitionContext * /*ctx*/) override;
	virtual void exitTable_type_definition(TSqlParser::Table_type_definitionContext * /*ctx*/) override;

	virtual void enterColumn_def_table_constraints(TSqlParser::Column_def_table_constraintsContext * /*ctx*/) override;
	virtual void exitColumn_def_table_constraints(TSqlParser::Column_def_table_constraintsContext * /*ctx*/) override;

	virtual void enterColumn_def_table_constraint(TSqlParser::Column_def_table_constraintContext * /*ctx*/) override;
	virtual void exitColumn_def_table_constraint(TSqlParser::Column_def_table_constraintContext * /*ctx*/) override;

	virtual void enterColumn_definition(TSqlParser::Column_definitionContext * /*ctx*/) override;
	virtual void exitColumn_definition(TSqlParser::Column_definitionContext * /*ctx*/) override;

	virtual void enterColumn_constraint(TSqlParser::Column_constraintContext * /*ctx*/) override;
	virtual void exitColumn_constraint(TSqlParser::Column_constraintContext * /*ctx*/) override;

	virtual void enterTable_constraint(TSqlParser::Table_constraintContext * /*ctx*/) override;
	virtual void exitTable_constraint(TSqlParser::Table_constraintContext * /*ctx*/) override;

	virtual void enterIndex_options(TSqlParser::Index_optionsContext * /*ctx*/) override;
	virtual void exitIndex_options(TSqlParser::Index_optionsContext * /*ctx*/) override;

	virtual void enterIndex_option(TSqlParser::Index_optionContext * /*ctx*/) override;
	virtual void exitIndex_option(TSqlParser::Index_optionContext * /*ctx*/) override;

	virtual void enterDeclare_cursor(TSqlParser::Declare_cursorContext * /*ctx*/) override;
	virtual void exitDeclare_cursor(TSqlParser::Declare_cursorContext * /*ctx*/) override;

	virtual void enterDeclare_set_cursor_common(TSqlParser::Declare_set_cursor_commonContext * /*ctx*/) override;
	virtual void exitDeclare_set_cursor_common(TSqlParser::Declare_set_cursor_commonContext * /*ctx*/) override;

	virtual void enterDeclare_set_cursor_common_partial(TSqlParser::Declare_set_cursor_common_partialContext * /*ctx*/) override;
	virtual void exitDeclare_set_cursor_common_partial(TSqlParser::Declare_set_cursor_common_partialContext * /*ctx*/) override;

	virtual void enterFetch_cursor(TSqlParser::Fetch_cursorContext * /*ctx*/) override;
	virtual void exitFetch_cursor(TSqlParser::Fetch_cursorContext * /*ctx*/) override;

	virtual void enterSet_special(TSqlParser::Set_specialContext * /*ctx*/) override;
	virtual void exitSet_special(TSqlParser::Set_specialContext * /*ctx*/) override;

	virtual void enterConstant_LOCAL_ID(TSqlParser::Constant_LOCAL_IDContext * /*ctx*/) override;
	virtual void exitConstant_LOCAL_ID(TSqlParser::Constant_LOCAL_IDContext * /*ctx*/) override;

	virtual void enterExpression(TSqlParser::ExpressionContext * /*ctx*/) override;
	virtual void exitExpression(TSqlParser::ExpressionContext * /*ctx*/) override;

	virtual void enterTime_zone(TSqlParser::Time_zoneContext * /*ctx*/) override;
	virtual void exitTime_zone(TSqlParser::Time_zoneContext * /*ctx*/) override;

	virtual void enterPrimitive_expression(TSqlParser::Primitive_expressionContext * /*ctx*/) override;
	virtual void exitPrimitive_expression(TSqlParser::Primitive_expressionContext * /*ctx*/) override;

	virtual void enterCase_expression(TSqlParser::Case_expressionContext * /*ctx*/) override;
	virtual void exitCase_expression(TSqlParser::Case_expressionContext * /*ctx*/) override;

	virtual void enterUnary_operator_expression(TSqlParser::Unary_operator_expressionContext * /*ctx*/) override;
	virtual void exitUnary_operator_expression(TSqlParser::Unary_operator_expressionContext * /*ctx*/) override;

	virtual void enterBracket_expression(TSqlParser::Bracket_expressionContext * /*ctx*/) override;
	virtual void exitBracket_expression(TSqlParser::Bracket_expressionContext * /*ctx*/) override;

	virtual void enterConstant_expression(TSqlParser::Constant_expressionContext * /*ctx*/) override;
	virtual void exitConstant_expression(TSqlParser::Constant_expressionContext * /*ctx*/) override;

	virtual void enterSubquery(TSqlParser::SubqueryContext * /*ctx*/) override;
	virtual void exitSubquery(TSqlParser::SubqueryContext * /*ctx*/) override;

	virtual void enterWith_expression(TSqlParser::With_expressionContext * /*ctx*/) override;
	virtual void exitWith_expression(TSqlParser::With_expressionContext * /*ctx*/) override;

	virtual void enterCommon_table_expression(TSqlParser::Common_table_expressionContext * /*ctx*/) override;
	virtual void exitCommon_table_expression(TSqlParser::Common_table_expressionContext * /*ctx*/) override;

	virtual void enterUpdate_elem(TSqlParser::Update_elemContext * /*ctx*/) override;
	virtual void exitUpdate_elem(TSqlParser::Update_elemContext * /*ctx*/) override;

	virtual void enterUpdate_elem_merge(TSqlParser::Update_elem_mergeContext * /*ctx*/) override;
	virtual void exitUpdate_elem_merge(TSqlParser::Update_elem_mergeContext * /*ctx*/) override;

	virtual void enterSearch_condition(TSqlParser::Search_conditionContext * /*ctx*/) override;
	virtual void exitSearch_condition(TSqlParser::Search_conditionContext * /*ctx*/) override;

	virtual void enterPredicate(TSqlParser::PredicateContext * /*ctx*/) override;
	virtual void exitPredicate(TSqlParser::PredicateContext * /*ctx*/) override;

	virtual void enterQuery_expression(TSqlParser::Query_expressionContext * /*ctx*/) override;
	virtual void exitQuery_expression(TSqlParser::Query_expressionContext * /*ctx*/) override;

	virtual void enterSql_union(TSqlParser::Sql_unionContext * /*ctx*/) override;
	virtual void exitSql_union(TSqlParser::Sql_unionContext * /*ctx*/) override;

	virtual void enterQuery_specification(TSqlParser::Query_specificationContext * /*ctx*/) override;
	virtual void exitQuery_specification(TSqlParser::Query_specificationContext * /*ctx*/) override;

	virtual void enterTop_clause(TSqlParser::Top_clauseContext * /*ctx*/) override;
	virtual void exitTop_clause(TSqlParser::Top_clauseContext * /*ctx*/) override;

	virtual void enterTop_percent(TSqlParser::Top_percentContext * /*ctx*/) override;
	virtual void exitTop_percent(TSqlParser::Top_percentContext * /*ctx*/) override;

	virtual void enterTop_count(TSqlParser::Top_countContext * /*ctx*/) override;
	virtual void exitTop_count(TSqlParser::Top_countContext * /*ctx*/) override;

	virtual void enterOrder_by_clause(TSqlParser::Order_by_clauseContext * /*ctx*/) override;
	virtual void exitOrder_by_clause(TSqlParser::Order_by_clauseContext * /*ctx*/) override;

	virtual void enterOrder_by_expression(TSqlParser::Order_by_expressionContext * /*ctx*/) override;
	virtual void exitOrder_by_expression(TSqlParser::Order_by_expressionContext * /*ctx*/) override;

	virtual void enterGroup_by_item(TSqlParser::Group_by_itemContext * /*ctx*/) override;
	virtual void exitGroup_by_item(TSqlParser::Group_by_itemContext * /*ctx*/) override;

	virtual void enterOption_clause(TSqlParser::Option_clauseContext * /*ctx*/) override;
	virtual void exitOption_clause(TSqlParser::Option_clauseContext * /*ctx*/) override;

	virtual void enterOption(TSqlParser::OptionContext * /*ctx*/) override;
	virtual void exitOption(TSqlParser::OptionContext * /*ctx*/) override;

	virtual void enterOptimize_for_arg(TSqlParser::Optimize_for_argContext * /*ctx*/) override;
	virtual void exitOptimize_for_arg(TSqlParser::Optimize_for_argContext * /*ctx*/) override;

	virtual void enterSelect_list(TSqlParser::Select_listContext * /*ctx*/) override;
	virtual void exitSelect_list(TSqlParser::Select_listContext * /*ctx*/) override;

	virtual void enterUdt_method_arguments(TSqlParser::Udt_method_argumentsContext * /*ctx*/) override;
	virtual void exitUdt_method_arguments(TSqlParser::Udt_method_argumentsContext * /*ctx*/) override;

	virtual void enterAsterisk(TSqlParser::AsteriskContext * /*ctx*/) override;
	virtual void exitAsterisk(TSqlParser::AsteriskContext * /*ctx*/) override;

	virtual void enterColumn_elem(TSqlParser::Column_elemContext * /*ctx*/) override;
	virtual void exitColumn_elem(TSqlParser::Column_elemContext * /*ctx*/) override;

	virtual void enterUdt_elem(TSqlParser::Udt_elemContext * /*ctx*/) override;
	virtual void exitUdt_elem(TSqlParser::Udt_elemContext * /*ctx*/) override;

	virtual void enterExpression_elem(TSqlParser::Expression_elemContext * /*ctx*/) override;
	virtual void exitExpression_elem(TSqlParser::Expression_elemContext * /*ctx*/) override;

	virtual void enterSelect_list_elem(TSqlParser::Select_list_elemContext * /*ctx*/) override;
	virtual void exitSelect_list_elem(TSqlParser::Select_list_elemContext * /*ctx*/) override;

	virtual void enterTable_sources(TSqlParser::Table_sourcesContext * /*ctx*/) override;
	virtual void exitTable_sources(TSqlParser::Table_sourcesContext * /*ctx*/) override;

	virtual void enterTable_source(TSqlParser::Table_sourceContext * /*ctx*/) override;
	virtual void exitTable_source(TSqlParser::Table_sourceContext * /*ctx*/) override;

	virtual void enterTable_source_item_joined(TSqlParser::Table_source_item_joinedContext * /*ctx*/) override;
	virtual void exitTable_source_item_joined(TSqlParser::Table_source_item_joinedContext * /*ctx*/) override;

	virtual void enterTable_source_item(TSqlParser::Table_source_itemContext * /*ctx*/) override;
	virtual void exitTable_source_item(TSqlParser::Table_source_itemContext * /*ctx*/) override;

	virtual void enterSchema_declaration(TSqlParser::Schema_declarationContext * /*ctx*/) override;
	virtual void exitSchema_declaration(TSqlParser::Schema_declarationContext * /*ctx*/) override;

	virtual void enterColumn_declaration(TSqlParser::Column_declarationContext * /*ctx*/) override;
	virtual void exitColumn_declaration(TSqlParser::Column_declarationContext * /*ctx*/) override;

	virtual void enterChange_table(TSqlParser::Change_tableContext * /*ctx*/) override;
	virtual void exitChange_table(TSqlParser::Change_tableContext * /*ctx*/) override;

	virtual void enterChange_table_changes(TSqlParser::Change_table_changesContext * /*ctx*/) override;
	virtual void exitChange_table_changes(TSqlParser::Change_table_changesContext * /*ctx*/) override;

	virtual void enterChange_table_version(TSqlParser::Change_table_versionContext * /*ctx*/) override;
	virtual void exitChange_table_version(TSqlParser::Change_table_versionContext * /*ctx*/) override;

	virtual void enterJoin_part(TSqlParser::Join_partContext * /*ctx*/) override;
	virtual void exitJoin_part(TSqlParser::Join_partContext * /*ctx*/) override;

	virtual void enterJoin_on(TSqlParser::Join_onContext * /*ctx*/) override;
	virtual void exitJoin_on(TSqlParser::Join_onContext * /*ctx*/) override;

	virtual void enterCross_join(TSqlParser::Cross_joinContext * /*ctx*/) override;
	virtual void exitCross_join(TSqlParser::Cross_joinContext * /*ctx*/) override;

	virtual void enterApply_(TSqlParser::Apply_Context * /*ctx*/) override;
	virtual void exitApply_(TSqlParser::Apply_Context * /*ctx*/) override;

	virtual void enterPivot(TSqlParser::PivotContext * /*ctx*/) override;
	virtual void exitPivot(TSqlParser::PivotContext * /*ctx*/) override;

	virtual void enterUnpivot(TSqlParser::UnpivotContext * /*ctx*/) override;
	virtual void exitUnpivot(TSqlParser::UnpivotContext * /*ctx*/) override;

	virtual void enterPivot_clause(TSqlParser::Pivot_clauseContext * /*ctx*/) override;
	virtual void exitPivot_clause(TSqlParser::Pivot_clauseContext * /*ctx*/) override;

	virtual void enterUnpivot_clause(TSqlParser::Unpivot_clauseContext * /*ctx*/) override;
	virtual void exitUnpivot_clause(TSqlParser::Unpivot_clauseContext * /*ctx*/) override;

	virtual void enterFull_column_name_list(TSqlParser::Full_column_name_listContext * /*ctx*/) override;
	virtual void exitFull_column_name_list(TSqlParser::Full_column_name_listContext * /*ctx*/) override;

	virtual void enterTable_name_with_hint(TSqlParser::Table_name_with_hintContext * /*ctx*/) override;
	virtual void exitTable_name_with_hint(TSqlParser::Table_name_with_hintContext * /*ctx*/) override;

	virtual void enterBulk_option(TSqlParser::Bulk_optionContext * /*ctx*/) override;
	virtual void exitBulk_option(TSqlParser::Bulk_optionContext * /*ctx*/) override;

	virtual void enterDerived_table(TSqlParser::Derived_tableContext * /*ctx*/) override;
	virtual void exitDerived_table(TSqlParser::Derived_tableContext * /*ctx*/) override;

	virtual void enterRANKING_WINDOWED_FUNC(TSqlParser::RANKING_WINDOWED_FUNCContext * /*ctx*/) override;
	virtual void exitRANKING_WINDOWED_FUNC(TSqlParser::RANKING_WINDOWED_FUNCContext * /*ctx*/) override;

	virtual void enterAGGREGATE_WINDOWED_FUNC(TSqlParser::AGGREGATE_WINDOWED_FUNCContext * /*ctx*/) override;
	virtual void exitAGGREGATE_WINDOWED_FUNC(TSqlParser::AGGREGATE_WINDOWED_FUNCContext * /*ctx*/) override;

	virtual void enterANALYTIC_WINDOWED_FUNC(TSqlParser::ANALYTIC_WINDOWED_FUNCContext * /*ctx*/) override;
	virtual void exitANALYTIC_WINDOWED_FUNC(TSqlParser::ANALYTIC_WINDOWED_FUNCContext * /*ctx*/) override;

	virtual void enterBUILT_IN_FUNC(TSqlParser::BUILT_IN_FUNCContext * /*ctx*/) override;
	virtual void exitBUILT_IN_FUNC(TSqlParser::BUILT_IN_FUNCContext * /*ctx*/) override;

	virtual void enterSCALAR_FUNCTION(TSqlParser::SCALAR_FUNCTIONContext * /*ctx*/) override;
	virtual void exitSCALAR_FUNCTION(TSqlParser::SCALAR_FUNCTIONContext * /*ctx*/) override;

	virtual void enterPARTITION_FUNC(TSqlParser::PARTITION_FUNCContext * /*ctx*/) override;
	virtual void exitPARTITION_FUNC(TSqlParser::PARTITION_FUNCContext * /*ctx*/) override;

	virtual void enterPartition_function(TSqlParser::Partition_functionContext * /*ctx*/) override;
	virtual void exitPartition_function(TSqlParser::Partition_functionContext * /*ctx*/) override;

	virtual void enterBINARY_CHECKSUM(TSqlParser::BINARY_CHECKSUMContext * /*ctx*/) override;
	virtual void exitBINARY_CHECKSUM(TSqlParser::BINARY_CHECKSUMContext * /*ctx*/) override;

	virtual void enterCAST(TSqlParser::CASTContext * /*ctx*/) override;
	virtual void exitCAST(TSqlParser::CASTContext * /*ctx*/) override;

	virtual void enterTRY_CAST(TSqlParser::TRY_CASTContext * /*ctx*/) override;
	virtual void exitTRY_CAST(TSqlParser::TRY_CASTContext * /*ctx*/) override;

	virtual void enterCONVERT(TSqlParser::CONVERTContext * /*ctx*/) override;
	virtual void exitCONVERT(TSqlParser::CONVERTContext * /*ctx*/) override;

	virtual void enterCHECKSUM(TSqlParser::CHECKSUMContext * /*ctx*/) override;
	virtual void exitCHECKSUM(TSqlParser::CHECKSUMContext * /*ctx*/) override;

	virtual void enterCOALESCE(TSqlParser::COALESCEContext * /*ctx*/) override;
	virtual void exitCOALESCE(TSqlParser::COALESCEContext * /*ctx*/) override;

	virtual void enterCURRENT_TIMESTAMP(TSqlParser::CURRENT_TIMESTAMPContext * /*ctx*/) override;
	virtual void exitCURRENT_TIMESTAMP(TSqlParser::CURRENT_TIMESTAMPContext * /*ctx*/) override;

	virtual void enterCURRENT_USER(TSqlParser::CURRENT_USERContext * /*ctx*/) override;
	virtual void exitCURRENT_USER(TSqlParser::CURRENT_USERContext * /*ctx*/) override;

	virtual void enterDATEADD(TSqlParser::DATEADDContext * /*ctx*/) override;
	virtual void exitDATEADD(TSqlParser::DATEADDContext * /*ctx*/) override;

	virtual void enterDATEDIFF(TSqlParser::DATEDIFFContext * /*ctx*/) override;
	virtual void exitDATEDIFF(TSqlParser::DATEDIFFContext * /*ctx*/) override;

	virtual void enterDATENAME(TSqlParser::DATENAMEContext * /*ctx*/) override;
	virtual void exitDATENAME(TSqlParser::DATENAMEContext * /*ctx*/) override;

	virtual void enterDATEPART(TSqlParser::DATEPARTContext * /*ctx*/) override;
	virtual void exitDATEPART(TSqlParser::DATEPARTContext * /*ctx*/) override;

	virtual void enterGETDATE(TSqlParser::GETDATEContext * /*ctx*/) override;
	virtual void exitGETDATE(TSqlParser::GETDATEContext * /*ctx*/) override;

	virtual void enterGETUTCDATE(TSqlParser::GETUTCDATEContext * /*ctx*/) override;
	virtual void exitGETUTCDATE(TSqlParser::GETUTCDATEContext * /*ctx*/) override;

	virtual void enterIDENTITY(TSqlParser::IDENTITYContext * /*ctx*/) override;
	virtual void exitIDENTITY(TSqlParser::IDENTITYContext * /*ctx*/) override;

	virtual void enterMIN_ACTIVE_ROWVERSION(TSqlParser::MIN_ACTIVE_ROWVERSIONContext * /*ctx*/) override;
	virtual void exitMIN_ACTIVE_ROWVERSION(TSqlParser::MIN_ACTIVE_ROWVERSIONContext * /*ctx*/) override;

	virtual void enterNULLIF(TSqlParser::NULLIFContext * /*ctx*/) override;
	virtual void exitNULLIF(TSqlParser::NULLIFContext * /*ctx*/) override;

	virtual void enterSTUFF(TSqlParser::STUFFContext * /*ctx*/) override;
	virtual void exitSTUFF(TSqlParser::STUFFContext * /*ctx*/) override;

	virtual void enterSESSION_USER(TSqlParser::SESSION_USERContext * /*ctx*/) override;
	virtual void exitSESSION_USER(TSqlParser::SESSION_USERContext * /*ctx*/) override;

	virtual void enterSYSTEM_USER(TSqlParser::SYSTEM_USERContext * /*ctx*/) override;
	virtual void exitSYSTEM_USER(TSqlParser::SYSTEM_USERContext * /*ctx*/) override;

	virtual void enterUSER(TSqlParser::USERContext * /*ctx*/) override;
	virtual void exitUSER(TSqlParser::USERContext * /*ctx*/) override;

	virtual void enterISNULL(TSqlParser::ISNULLContext * /*ctx*/) override;
	virtual void exitISNULL(TSqlParser::ISNULLContext * /*ctx*/) override;

	virtual void enterIIF(TSqlParser::IIFContext * /*ctx*/) override;
	virtual void exitIIF(TSqlParser::IIFContext * /*ctx*/) override;

	virtual void enterSTRINGAGG(TSqlParser::STRINGAGGContext * /*ctx*/) override;
	virtual void exitSTRINGAGG(TSqlParser::STRINGAGGContext * /*ctx*/) override;

	virtual void enterNodes_method(TSqlParser::Nodes_methodContext * /*ctx*/) override;
	virtual void exitNodes_method(TSqlParser::Nodes_methodContext * /*ctx*/) override;

	virtual void enterSwitch_section(TSqlParser::Switch_sectionContext * /*ctx*/) override;
	virtual void exitSwitch_section(TSqlParser::Switch_sectionContext * /*ctx*/) override;

	virtual void enterSwitch_search_condition_section(TSqlParser::Switch_search_condition_sectionContext * /*ctx*/) override;
	virtual void exitSwitch_search_condition_section(TSqlParser::Switch_search_condition_sectionContext * /*ctx*/) override;

	virtual void enterAs_column_alias(TSqlParser::As_column_aliasContext * /*ctx*/) override;
	virtual void exitAs_column_alias(TSqlParser::As_column_aliasContext * /*ctx*/) override;

	virtual void enterAs_table_alias(TSqlParser::As_table_aliasContext * /*ctx*/) override;
	virtual void exitAs_table_alias(TSqlParser::As_table_aliasContext * /*ctx*/) override;

	virtual void enterTable_alias(TSqlParser::Table_aliasContext * /*ctx*/) override;
	virtual void exitTable_alias(TSqlParser::Table_aliasContext * /*ctx*/) override;

	virtual void enterWith_table_hints(TSqlParser::With_table_hintsContext * /*ctx*/) override;
	virtual void exitWith_table_hints(TSqlParser::With_table_hintsContext * /*ctx*/) override;

	virtual void enterInsert_with_table_hints(TSqlParser::Insert_with_table_hintsContext * /*ctx*/) override;
	virtual void exitInsert_with_table_hints(TSqlParser::Insert_with_table_hintsContext * /*ctx*/) override;

	virtual void enterTable_hint(TSqlParser::Table_hintContext * /*ctx*/) override;
	virtual void exitTable_hint(TSqlParser::Table_hintContext * /*ctx*/) override;

	virtual void enterIndex_value(TSqlParser::Index_valueContext * /*ctx*/) override;
	virtual void exitIndex_value(TSqlParser::Index_valueContext * /*ctx*/) override;

	virtual void enterColumn_alias_list(TSqlParser::Column_alias_listContext * /*ctx*/) override;
	virtual void exitColumn_alias_list(TSqlParser::Column_alias_listContext * /*ctx*/) override;

	virtual void enterColumn_alias(TSqlParser::Column_aliasContext * /*ctx*/) override;
	virtual void exitColumn_alias(TSqlParser::Column_aliasContext * /*ctx*/) override;

	virtual void enterTable_value_constructor(TSqlParser::Table_value_constructorContext * /*ctx*/) override;
	virtual void exitTable_value_constructor(TSqlParser::Table_value_constructorContext * /*ctx*/) override;

	virtual void enterExpression_list(TSqlParser::Expression_listContext * /*ctx*/) override;
	virtual void exitExpression_list(TSqlParser::Expression_listContext * /*ctx*/) override;

	virtual void enterRanking_windowed_function(TSqlParser::Ranking_windowed_functionContext * /*ctx*/) override;
	virtual void exitRanking_windowed_function(TSqlParser::Ranking_windowed_functionContext * /*ctx*/) override;

	virtual void enterAggregate_windowed_function(TSqlParser::Aggregate_windowed_functionContext * /*ctx*/) override;
	virtual void exitAggregate_windowed_function(TSqlParser::Aggregate_windowed_functionContext * /*ctx*/) override;

	virtual void enterAnalytic_windowed_function(TSqlParser::Analytic_windowed_functionContext * /*ctx*/) override;
	virtual void exitAnalytic_windowed_function(TSqlParser::Analytic_windowed_functionContext * /*ctx*/) override;

	virtual void enterAll_distinct_expression(TSqlParser::All_distinct_expressionContext * /*ctx*/) override;
	virtual void exitAll_distinct_expression(TSqlParser::All_distinct_expressionContext * /*ctx*/) override;

	virtual void enterOver_clause(TSqlParser::Over_clauseContext * /*ctx*/) override;
	virtual void exitOver_clause(TSqlParser::Over_clauseContext * /*ctx*/) override;

	virtual void enterRow_or_range_clause(TSqlParser::Row_or_range_clauseContext * /*ctx*/) override;
	virtual void exitRow_or_range_clause(TSqlParser::Row_or_range_clauseContext * /*ctx*/) override;

	virtual void enterWindow_frame_extent(TSqlParser::Window_frame_extentContext * /*ctx*/) override;
	virtual void exitWindow_frame_extent(TSqlParser::Window_frame_extentContext * /*ctx*/) override;

	virtual void enterWindow_frame_bound(TSqlParser::Window_frame_boundContext * /*ctx*/) override;
	virtual void exitWindow_frame_bound(TSqlParser::Window_frame_boundContext * /*ctx*/) override;

	virtual void enterWindow_frame_preceding(TSqlParser::Window_frame_precedingContext * /*ctx*/) override;
	virtual void exitWindow_frame_preceding(TSqlParser::Window_frame_precedingContext * /*ctx*/) override;

	virtual void enterWindow_frame_following(TSqlParser::Window_frame_followingContext * /*ctx*/) override;
	virtual void exitWindow_frame_following(TSqlParser::Window_frame_followingContext * /*ctx*/) override;

	virtual void enterEntity_name(TSqlParser::Entity_nameContext * /*ctx*/) override;
	virtual void exitEntity_name(TSqlParser::Entity_nameContext * /*ctx*/) override;

	virtual void enterEntity_name_for_azure_dw(TSqlParser::Entity_name_for_azure_dwContext * /*ctx*/) override;
	virtual void exitEntity_name_for_azure_dw(TSqlParser::Entity_name_for_azure_dwContext * /*ctx*/) override;

	virtual void enterEntity_name_for_parallel_dw(TSqlParser::Entity_name_for_parallel_dwContext * /*ctx*/) override;
	virtual void exitEntity_name_for_parallel_dw(TSqlParser::Entity_name_for_parallel_dwContext * /*ctx*/) override;

	virtual void enterFull_table_name(TSqlParser::Full_table_nameContext * /*ctx*/) override;
	virtual void exitFull_table_name(TSqlParser::Full_table_nameContext * /*ctx*/) override;

	virtual void enterTable_name(TSqlParser::Table_nameContext * /*ctx*/) override;
	virtual void exitTable_name(TSqlParser::Table_nameContext * /*ctx*/) override;

	virtual void enterSimple_name(TSqlParser::Simple_nameContext * /*ctx*/) override;
	virtual void exitSimple_name(TSqlParser::Simple_nameContext * /*ctx*/) override;

	virtual void enterFunc_proc_name_schema(TSqlParser::Func_proc_name_schemaContext * /*ctx*/) override;
	virtual void exitFunc_proc_name_schema(TSqlParser::Func_proc_name_schemaContext * /*ctx*/) override;

	virtual void enterFunc_proc_name_database_schema(TSqlParser::Func_proc_name_database_schemaContext * /*ctx*/) override;
	virtual void exitFunc_proc_name_database_schema(TSqlParser::Func_proc_name_database_schemaContext * /*ctx*/) override;

	virtual void enterFunc_proc_name_server_database_schema(TSqlParser::Func_proc_name_server_database_schemaContext * /*ctx*/) override;
	virtual void exitFunc_proc_name_server_database_schema(TSqlParser::Func_proc_name_server_database_schemaContext * /*ctx*/) override;

	virtual void enterDdl_object(TSqlParser::Ddl_objectContext * /*ctx*/) override;
	virtual void exitDdl_object(TSqlParser::Ddl_objectContext * /*ctx*/) override;

	virtual void enterFull_column_name(TSqlParser::Full_column_nameContext * /*ctx*/) override;
	virtual void exitFull_column_name(TSqlParser::Full_column_nameContext * /*ctx*/) override;

	virtual void enterColumn_name_list_with_order(TSqlParser::Column_name_list_with_orderContext * /*ctx*/) override;
	virtual void exitColumn_name_list_with_order(TSqlParser::Column_name_list_with_orderContext * /*ctx*/) override;

	virtual void enterInsert_column_name_list(TSqlParser::Insert_column_name_listContext * /*ctx*/) override;
	virtual void exitInsert_column_name_list(TSqlParser::Insert_column_name_listContext * /*ctx*/) override;

	virtual void enterInsert_column_id(TSqlParser::Insert_column_idContext * /*ctx*/) override;
	virtual void exitInsert_column_id(TSqlParser::Insert_column_idContext * /*ctx*/) override;

	virtual void enterColumn_name_list(TSqlParser::Column_name_listContext * /*ctx*/) override;
	virtual void exitColumn_name_list(TSqlParser::Column_name_listContext * /*ctx*/) override;

	virtual void enterCursor_name(TSqlParser::Cursor_nameContext * /*ctx*/) override;
	virtual void exitCursor_name(TSqlParser::Cursor_nameContext * /*ctx*/) override;

	virtual void enterOn_off(TSqlParser::On_offContext * /*ctx*/) override;
	virtual void exitOn_off(TSqlParser::On_offContext * /*ctx*/) override;

	virtual void enterNull_notnull(TSqlParser::Null_notnullContext * /*ctx*/) override;
	virtual void exitNull_notnull(TSqlParser::Null_notnullContext * /*ctx*/) override;

	virtual void enterNull_or_default(TSqlParser::Null_or_defaultContext * /*ctx*/) override;
	virtual void exitNull_or_default(TSqlParser::Null_or_defaultContext * /*ctx*/) override;

	virtual void enterScalar_function_name(TSqlParser::Scalar_function_nameContext * /*ctx*/) override;
	virtual void exitScalar_function_name(TSqlParser::Scalar_function_nameContext * /*ctx*/) override;

	virtual void enterData_type(TSqlParser::Data_typeContext * /*ctx*/) override;
	virtual void exitData_type(TSqlParser::Data_typeContext * /*ctx*/) override;

	virtual void enterDefault_value(TSqlParser::Default_valueContext * /*ctx*/) override;
	virtual void exitDefault_value(TSqlParser::Default_valueContext * /*ctx*/) override;

	virtual void enterConstant(TSqlParser::ConstantContext * /*ctx*/) override;
	virtual void exitConstant(TSqlParser::ConstantContext * /*ctx*/) override;

	virtual void enterSign(TSqlParser::SignContext * /*ctx*/) override;
	virtual void exitSign(TSqlParser::SignContext * /*ctx*/) override;

	virtual void enterKeyword(TSqlParser::KeywordContext * /*ctx*/) override;
	virtual void exitKeyword(TSqlParser::KeywordContext * /*ctx*/) override;

	virtual void enterId_(TSqlParser::Id_Context * /*ctx*/) override;
	virtual void exitId_(TSqlParser::Id_Context * /*ctx*/) override;

	virtual void enterSimple_id(TSqlParser::Simple_idContext * /*ctx*/) override;
	virtual void exitSimple_id(TSqlParser::Simple_idContext * /*ctx*/) override;

	virtual void enterId_or_string(TSqlParser::Id_or_stringContext * /*ctx*/) override;
	virtual void exitId_or_string(TSqlParser::Id_or_stringContext * /*ctx*/) override;

	virtual void enterComparison_operator(TSqlParser::Comparison_operatorContext * /*ctx*/) override;
	virtual void exitComparison_operator(TSqlParser::Comparison_operatorContext * /*ctx*/) override;

	virtual void enterAssignment_operator(TSqlParser::Assignment_operatorContext * /*ctx*/) override;
	virtual void exitAssignment_operator(TSqlParser::Assignment_operatorContext * /*ctx*/) override;

};


#endif
