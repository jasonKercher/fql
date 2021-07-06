#ifndef LISTENERINTERFACE_H
#define LISTENERINTERFACE_H

#include "gen/TSqlParserBaseListener.h"
#include "treewalker.h"

#include "fqlhandle.h"
#include "fqlimits.h"
#include "query.h"
#include "util/queue.h"
#include "util/stack.h"

class ListenerInterface : public TSqlParserBaseListener {
      private:
	enum tok_type {
		TOK_UNDEFINED,
		TOK_INTO_TABLE,
		TOK_COLUMN_NAME,
		TOK_COLUMN_ALIAS,
		TOK_TABLE_NAME,
		TOK_TABLE_SOURCE,
		TOK_TABLE_ALIAS,
	};

	std::vector<std::string> _error_tokens;
	std::vector<std::string> _rule_names;

	TreeWalker* _walker = NULL;

	struct fql_handle* _fql = NULL;
	struct stack* _query_stack = NULL;
        struct query* _subquery = NULL;
	struct query* _query = NULL;

	/* Likely these remaining variables
	 * need to move to struct query in
	 * order to handle subqueries
	 */
	struct stack* _source_stack = NULL;

	char _table_name[TABLE_NAME_MAX];
	char _table_alias[TABLE_NAME_MAX];

	int _query_id = 0;
	int _return_code = 0;
	enum tok_type _next_tok_type;
	enum tok_type _tok_type;
	bool _on_asterisk = false;

	void _no_impl(const std::string&, int);

      public:
	ListenerInterface(struct fql_handle*, TreeWalker*, const std::vector<std::string>&);
	void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override;
	void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override;
	int get_return_code();



	/* Not yet implemented */
	void enterTsql_file(TSqlParser::Tsql_fileContext * /*ctx*/) override;
	void exitTsql_file(TSqlParser::Tsql_fileContext * /*ctx*/) override;

	void enterBatch(TSqlParser::BatchContext * /*ctx*/) override;
	void exitBatch(TSqlParser::BatchContext * /*ctx*/) override;

	void enterSql_clauses(TSqlParser::Sql_clausesContext * /*ctx*/) override;
	void exitSql_clauses(TSqlParser::Sql_clausesContext * /*ctx*/) override;

	void enterSql_clause(TSqlParser::Sql_clauseContext * /*ctx*/) override;
	void exitSql_clause(TSqlParser::Sql_clauseContext * /*ctx*/) override;

	void enterDml_clause(TSqlParser::Dml_clauseContext * /*ctx*/) override;
	void exitDml_clause(TSqlParser::Dml_clauseContext * /*ctx*/) override;

	void enterDdl_clause(TSqlParser::Ddl_clauseContext * /*ctx*/) override;
	void exitDdl_clause(TSqlParser::Ddl_clauseContext * /*ctx*/) override;

	void enterCfl_statement(TSqlParser::Cfl_statementContext * /*ctx*/) override;
	void exitCfl_statement(TSqlParser::Cfl_statementContext * /*ctx*/) override;

	void enterBlock_statement(TSqlParser::Block_statementContext * /*ctx*/) override;
	void exitBlock_statement(TSqlParser::Block_statementContext * /*ctx*/) override;

	void enterBreak_statement(TSqlParser::Break_statementContext * /*ctx*/) override;
	void exitBreak_statement(TSqlParser::Break_statementContext * /*ctx*/) override;

	void enterContinue_statement(TSqlParser::Continue_statementContext * /*ctx*/) override;
	void exitContinue_statement(TSqlParser::Continue_statementContext * /*ctx*/) override;

	void enterGoto_statement(TSqlParser::Goto_statementContext * /*ctx*/) override;
	void exitGoto_statement(TSqlParser::Goto_statementContext * /*ctx*/) override;

	void enterReturn_statement(TSqlParser::Return_statementContext * /*ctx*/) override;
	void exitReturn_statement(TSqlParser::Return_statementContext * /*ctx*/) override;

	void enterIf_statement(TSqlParser::If_statementContext * /*ctx*/) override;
	void exitIf_statement(TSqlParser::If_statementContext * /*ctx*/) override;

	void enterThrow_statement(TSqlParser::Throw_statementContext * /*ctx*/) override;
	void exitThrow_statement(TSqlParser::Throw_statementContext * /*ctx*/) override;

	void enterThrow_error_number(TSqlParser::Throw_error_numberContext * /*ctx*/) override;
	void exitThrow_error_number(TSqlParser::Throw_error_numberContext * /*ctx*/) override;

	void enterThrow_message(TSqlParser::Throw_messageContext * /*ctx*/) override;
	void exitThrow_message(TSqlParser::Throw_messageContext * /*ctx*/) override;

	void enterThrow_state(TSqlParser::Throw_stateContext * /*ctx*/) override;
	void exitThrow_state(TSqlParser::Throw_stateContext * /*ctx*/) override;

	void enterTry_catch_statement(TSqlParser::Try_catch_statementContext * /*ctx*/) override;
	void exitTry_catch_statement(TSqlParser::Try_catch_statementContext * /*ctx*/) override;

	void enterWaitfor_statement(TSqlParser::Waitfor_statementContext * /*ctx*/) override;
	void exitWaitfor_statement(TSqlParser::Waitfor_statementContext * /*ctx*/) override;

	void enterWhile_statement(TSqlParser::While_statementContext * /*ctx*/) override;
	void exitWhile_statement(TSqlParser::While_statementContext * /*ctx*/) override;

	void enterPrint_statement(TSqlParser::Print_statementContext * /*ctx*/) override;
	void exitPrint_statement(TSqlParser::Print_statementContext * /*ctx*/) override;

	void enterRaiseerror_statement(TSqlParser::Raiseerror_statementContext * /*ctx*/) override;
	void exitRaiseerror_statement(TSqlParser::Raiseerror_statementContext * /*ctx*/) override;

	void enterEmpty_statement(TSqlParser::Empty_statementContext * /*ctx*/) override;
	void exitEmpty_statement(TSqlParser::Empty_statementContext * /*ctx*/) override;

	void enterAnother_statement(TSqlParser::Another_statementContext * /*ctx*/) override;
	void exitAnother_statement(TSqlParser::Another_statementContext * /*ctx*/) override;

	void enterDrop_aggregate(TSqlParser::Drop_aggregateContext * /*ctx*/) override;
	void exitDrop_aggregate(TSqlParser::Drop_aggregateContext * /*ctx*/) override;

	void enterEntity_to(TSqlParser::Entity_toContext * /*ctx*/) override;
	void exitEntity_to(TSqlParser::Entity_toContext * /*ctx*/) override;

	void enterColon_colon(TSqlParser::Colon_colonContext * /*ctx*/) override;
	void exitColon_colon(TSqlParser::Colon_colonContext * /*ctx*/) override;

	void enterClass_type(TSqlParser::Class_typeContext * /*ctx*/) override;
	void exitClass_type(TSqlParser::Class_typeContext * /*ctx*/) override;

	void enterClass_type_for_sql_database(TSqlParser::Class_type_for_sql_databaseContext * /*ctx*/) override;
	void exitClass_type_for_sql_database(TSqlParser::Class_type_for_sql_databaseContext * /*ctx*/) override;

	void enterClass_type_for_parallel_dw(TSqlParser::Class_type_for_parallel_dwContext * /*ctx*/) override;
	void exitClass_type_for_parallel_dw(TSqlParser::Class_type_for_parallel_dwContext * /*ctx*/) override;

	void enterLock_table(TSqlParser::Lock_tableContext * /*ctx*/) override;
	void exitLock_table(TSqlParser::Lock_tableContext * /*ctx*/) override;

	void enterTruncate_table(TSqlParser::Truncate_tableContext * /*ctx*/) override;
	void exitTruncate_table(TSqlParser::Truncate_tableContext * /*ctx*/) override;

	void enterEvent_session_predicate_expression(TSqlParser::Event_session_predicate_expressionContext * /*ctx*/) override;
	void exitEvent_session_predicate_expression(TSqlParser::Event_session_predicate_expressionContext * /*ctx*/) override;

	void enterEvent_session_predicate_factor(TSqlParser::Event_session_predicate_factorContext * /*ctx*/) override;
	void exitEvent_session_predicate_factor(TSqlParser::Event_session_predicate_factorContext * /*ctx*/) override;

	void enterEvent_session_predicate_leaf(TSqlParser::Event_session_predicate_leafContext * /*ctx*/) override;
	void exitEvent_session_predicate_leaf(TSqlParser::Event_session_predicate_leafContext * /*ctx*/) override;

	void enterAlter_schema_sql(TSqlParser::Alter_schema_sqlContext * /*ctx*/) override;
	void exitAlter_schema_sql(TSqlParser::Alter_schema_sqlContext * /*ctx*/) override;

	void enterCreate_schema(TSqlParser::Create_schemaContext * /*ctx*/) override;
	void exitCreate_schema(TSqlParser::Create_schemaContext * /*ctx*/) override;

	void enterCreate_xml_schema_collection(TSqlParser::Create_xml_schema_collectionContext * /*ctx*/) override;
	void exitCreate_xml_schema_collection(TSqlParser::Create_xml_schema_collectionContext * /*ctx*/) override;

	void enterCreate_queue(TSqlParser::Create_queueContext * /*ctx*/) override;
	void exitCreate_queue(TSqlParser::Create_queueContext * /*ctx*/) override;

	void enterQueue_settings(TSqlParser::Queue_settingsContext * /*ctx*/) override;
	void exitQueue_settings(TSqlParser::Queue_settingsContext * /*ctx*/) override;

	void enterAlter_queue(TSqlParser::Alter_queueContext * /*ctx*/) override;
	void exitAlter_queue(TSqlParser::Alter_queueContext * /*ctx*/) override;

	void enterQueue_action(TSqlParser::Queue_actionContext * /*ctx*/) override;
	void exitQueue_action(TSqlParser::Queue_actionContext * /*ctx*/) override;

	void enterQueue_rebuild_options(TSqlParser::Queue_rebuild_optionsContext * /*ctx*/) override;
	void exitQueue_rebuild_options(TSqlParser::Queue_rebuild_optionsContext * /*ctx*/) override;

	void enterConversation_statement(TSqlParser::Conversation_statementContext * /*ctx*/) override;
	void exitConversation_statement(TSqlParser::Conversation_statementContext * /*ctx*/) override;

	void enterMessage_statement(TSqlParser::Message_statementContext * /*ctx*/) override;
	void exitMessage_statement(TSqlParser::Message_statementContext * /*ctx*/) override;

	void enterMerge_statement(TSqlParser::Merge_statementContext * /*ctx*/) override;
	void exitMerge_statement(TSqlParser::Merge_statementContext * /*ctx*/) override;

	void enterMerge_matched(TSqlParser::Merge_matchedContext * /*ctx*/) override;
	void exitMerge_matched(TSqlParser::Merge_matchedContext * /*ctx*/) override;

	void enterMerge_not_matched(TSqlParser::Merge_not_matchedContext * /*ctx*/) override;
	void exitMerge_not_matched(TSqlParser::Merge_not_matchedContext * /*ctx*/) override;

	void enterDelete_statement(TSqlParser::Delete_statementContext * /*ctx*/) override;
	void exitDelete_statement(TSqlParser::Delete_statementContext * /*ctx*/) override;

	void enterDelete_statement_from(TSqlParser::Delete_statement_fromContext * /*ctx*/) override;
	void exitDelete_statement_from(TSqlParser::Delete_statement_fromContext * /*ctx*/) override;

	void enterInsert_statement(TSqlParser::Insert_statementContext * /*ctx*/) override;
	void exitInsert_statement(TSqlParser::Insert_statementContext * /*ctx*/) override;

	void enterInsert_statement_value(TSqlParser::Insert_statement_valueContext * /*ctx*/) override;
	void exitInsert_statement_value(TSqlParser::Insert_statement_valueContext * /*ctx*/) override;

	void enterReceive_statement(TSqlParser::Receive_statementContext * /*ctx*/) override;
	void exitReceive_statement(TSqlParser::Receive_statementContext * /*ctx*/) override;

	void enterSelect_statement(TSqlParser::Select_statementContext * /*ctx*/) override;
	void exitSelect_statement(TSqlParser::Select_statementContext * /*ctx*/) override;

	void enterTime(TSqlParser::TimeContext * /*ctx*/) override;
	void exitTime(TSqlParser::TimeContext * /*ctx*/) override;

	void enterUpdate_statement(TSqlParser::Update_statementContext * /*ctx*/) override;
	void exitUpdate_statement(TSqlParser::Update_statementContext * /*ctx*/) override;

	void enterOutput_clause(TSqlParser::Output_clauseContext * /*ctx*/) override;
	void exitOutput_clause(TSqlParser::Output_clauseContext * /*ctx*/) override;

	void enterOutput_dml_list_elem(TSqlParser::Output_dml_list_elemContext * /*ctx*/) override;
	void exitOutput_dml_list_elem(TSqlParser::Output_dml_list_elemContext * /*ctx*/) override;

	void enterOutput_column_name(TSqlParser::Output_column_nameContext * /*ctx*/) override;
	void exitOutput_column_name(TSqlParser::Output_column_nameContext * /*ctx*/) override;

	void enterCreate_database(TSqlParser::Create_databaseContext * /*ctx*/) override;
	void exitCreate_database(TSqlParser::Create_databaseContext * /*ctx*/) override;

	void enterCreate_index(TSqlParser::Create_indexContext * /*ctx*/) override;
	void exitCreate_index(TSqlParser::Create_indexContext * /*ctx*/) override;

	void enterCreate_or_alter_procedure(TSqlParser::Create_or_alter_procedureContext * /*ctx*/) override;
	void exitCreate_or_alter_procedure(TSqlParser::Create_or_alter_procedureContext * /*ctx*/) override;

	void enterCreate_or_alter_function(TSqlParser::Create_or_alter_functionContext * /*ctx*/) override;
	void exitCreate_or_alter_function(TSqlParser::Create_or_alter_functionContext * /*ctx*/) override;

	void enterFunc_body_returns_select(TSqlParser::Func_body_returns_selectContext * /*ctx*/) override;
	void exitFunc_body_returns_select(TSqlParser::Func_body_returns_selectContext * /*ctx*/) override;

	void enterFunc_body_returns_table(TSqlParser::Func_body_returns_tableContext * /*ctx*/) override;
	void exitFunc_body_returns_table(TSqlParser::Func_body_returns_tableContext * /*ctx*/) override;

	void enterFunc_body_returns_scalar(TSqlParser::Func_body_returns_scalarContext * /*ctx*/) override;
	void exitFunc_body_returns_scalar(TSqlParser::Func_body_returns_scalarContext * /*ctx*/) override;

	void enterProcedure_param(TSqlParser::Procedure_paramContext * /*ctx*/) override;
	void exitProcedure_param(TSqlParser::Procedure_paramContext * /*ctx*/) override;

	void enterProcedure_option(TSqlParser::Procedure_optionContext * /*ctx*/) override;
	void exitProcedure_option(TSqlParser::Procedure_optionContext * /*ctx*/) override;

	void enterFunction_option(TSqlParser::Function_optionContext * /*ctx*/) override;
	void exitFunction_option(TSqlParser::Function_optionContext * /*ctx*/) override;

	void enterCreate_table(TSqlParser::Create_tableContext * /*ctx*/) override;
	void exitCreate_table(TSqlParser::Create_tableContext * /*ctx*/) override;

	void enterTable_options(TSqlParser::Table_optionsContext * /*ctx*/) override;
	void exitTable_options(TSqlParser::Table_optionsContext * /*ctx*/) override;

	void enterCreate_view(TSqlParser::Create_viewContext * /*ctx*/) override;
	void exitCreate_view(TSqlParser::Create_viewContext * /*ctx*/) override;

	void enterView_attribute(TSqlParser::View_attributeContext * /*ctx*/) override;
	void exitView_attribute(TSqlParser::View_attributeContext * /*ctx*/) override;

	void enterAlter_table(TSqlParser::Alter_tableContext * /*ctx*/) override;
	void exitAlter_table(TSqlParser::Alter_tableContext * /*ctx*/) override;

	void enterCursor_option(TSqlParser::Cursor_optionContext * /*ctx*/) override;
	void exitCursor_option(TSqlParser::Cursor_optionContext * /*ctx*/) override;

	void enterTarget_recovery_time_option(TSqlParser::Target_recovery_time_optionContext * /*ctx*/) override;
	void exitTarget_recovery_time_option(TSqlParser::Target_recovery_time_optionContext * /*ctx*/) override;

	void enterTermination(TSqlParser::TerminationContext * /*ctx*/) override;
	void exitTermination(TSqlParser::TerminationContext * /*ctx*/) override;

	void enterDrop_index(TSqlParser::Drop_indexContext * /*ctx*/) override;
	void exitDrop_index(TSqlParser::Drop_indexContext * /*ctx*/) override;

	void enterDrop_relational_or_xml_or_spatial_index(TSqlParser::Drop_relational_or_xml_or_spatial_indexContext * /*ctx*/) override;
	void exitDrop_relational_or_xml_or_spatial_index(TSqlParser::Drop_relational_or_xml_or_spatial_indexContext * /*ctx*/) override;

	void enterDrop_backward_compatible_index(TSqlParser::Drop_backward_compatible_indexContext * /*ctx*/) override;
	void exitDrop_backward_compatible_index(TSqlParser::Drop_backward_compatible_indexContext * /*ctx*/) override;

	void enterDrop_procedure(TSqlParser::Drop_procedureContext * /*ctx*/) override;
	void exitDrop_procedure(TSqlParser::Drop_procedureContext * /*ctx*/) override;

	void enterDrop_function(TSqlParser::Drop_functionContext * /*ctx*/) override;
	void exitDrop_function(TSqlParser::Drop_functionContext * /*ctx*/) override;

	void enterDrop_table(TSqlParser::Drop_tableContext * /*ctx*/) override;
	void exitDrop_table(TSqlParser::Drop_tableContext * /*ctx*/) override;

	void enterDrop_view(TSqlParser::Drop_viewContext * /*ctx*/) override;
	void exitDrop_view(TSqlParser::Drop_viewContext * /*ctx*/) override;

	void enterCreate_type(TSqlParser::Create_typeContext * /*ctx*/) override;
	void exitCreate_type(TSqlParser::Create_typeContext * /*ctx*/) override;

	void enterDrop_type(TSqlParser::Drop_typeContext * /*ctx*/) override;
	void exitDrop_type(TSqlParser::Drop_typeContext * /*ctx*/) override;

	void enterRowset_function_limited(TSqlParser::Rowset_function_limitedContext * /*ctx*/) override;
	void exitRowset_function_limited(TSqlParser::Rowset_function_limitedContext * /*ctx*/) override;

	void enterOpenquery(TSqlParser::OpenqueryContext * /*ctx*/) override;
	void exitOpenquery(TSqlParser::OpenqueryContext * /*ctx*/) override;

	void enterOpendatasource(TSqlParser::OpendatasourceContext * /*ctx*/) override;
	void exitOpendatasource(TSqlParser::OpendatasourceContext * /*ctx*/) override;

	void enterDeclare_statement(TSqlParser::Declare_statementContext * /*ctx*/) override;
	void exitDeclare_statement(TSqlParser::Declare_statementContext * /*ctx*/) override;

	void enterCursor_statement(TSqlParser::Cursor_statementContext * /*ctx*/) override;
	void exitCursor_statement(TSqlParser::Cursor_statementContext * /*ctx*/) override;

	void enterKill_statement(TSqlParser::Kill_statementContext * /*ctx*/) override;
	void exitKill_statement(TSqlParser::Kill_statementContext * /*ctx*/) override;

	void enterKill_process(TSqlParser::Kill_processContext * /*ctx*/) override;
	void exitKill_process(TSqlParser::Kill_processContext * /*ctx*/) override;

	void enterKill_query_notification(TSqlParser::Kill_query_notificationContext * /*ctx*/) override;
	void exitKill_query_notification(TSqlParser::Kill_query_notificationContext * /*ctx*/) override;

	void enterKill_stats_job(TSqlParser::Kill_stats_jobContext * /*ctx*/) override;
	void exitKill_stats_job(TSqlParser::Kill_stats_jobContext * /*ctx*/) override;

	void enterExecute_statement(TSqlParser::Execute_statementContext * /*ctx*/) override;
	void exitExecute_statement(TSqlParser::Execute_statementContext * /*ctx*/) override;

	void enterExecute_body(TSqlParser::Execute_bodyContext * /*ctx*/) override;
	void exitExecute_body(TSqlParser::Execute_bodyContext * /*ctx*/) override;

	void enterExecute_statement_arg(TSqlParser::Execute_statement_argContext * /*ctx*/) override;
	void exitExecute_statement_arg(TSqlParser::Execute_statement_argContext * /*ctx*/) override;

	void enterExecute_var_string(TSqlParser::Execute_var_stringContext * /*ctx*/) override;
	void exitExecute_var_string(TSqlParser::Execute_var_stringContext * /*ctx*/) override;

	void enterSet_statement(TSqlParser::Set_statementContext * /*ctx*/) override;
	void exitSet_statement(TSqlParser::Set_statementContext * /*ctx*/) override;

	void enterTransaction_statement(TSqlParser::Transaction_statementContext * /*ctx*/) override;
	void exitTransaction_statement(TSqlParser::Transaction_statementContext * /*ctx*/) override;

	void enterGo_statement(TSqlParser::Go_statementContext * /*ctx*/) override;
	void exitGo_statement(TSqlParser::Go_statementContext * /*ctx*/) override;

	void enterUse_statement(TSqlParser::Use_statementContext * /*ctx*/) override;
	void exitUse_statement(TSqlParser::Use_statementContext * /*ctx*/) override;

	void enterSetuser_statement(TSqlParser::Setuser_statementContext * /*ctx*/) override;
	void exitSetuser_statement(TSqlParser::Setuser_statementContext * /*ctx*/) override;

	void enterReconfigure_statement(TSqlParser::Reconfigure_statementContext * /*ctx*/) override;
	void exitReconfigure_statement(TSqlParser::Reconfigure_statementContext * /*ctx*/) override;

	void enterShutdown_statement(TSqlParser::Shutdown_statementContext * /*ctx*/) override;
	void exitShutdown_statement(TSqlParser::Shutdown_statementContext * /*ctx*/) override;

	void enterDbcc_clause(TSqlParser::Dbcc_clauseContext * /*ctx*/) override;
	void exitDbcc_clause(TSqlParser::Dbcc_clauseContext * /*ctx*/) override;

	void enterDbcc_options(TSqlParser::Dbcc_optionsContext * /*ctx*/) override;
	void exitDbcc_options(TSqlParser::Dbcc_optionsContext * /*ctx*/) override;

	void enterExecute_clause(TSqlParser::Execute_clauseContext * /*ctx*/) override;
	void exitExecute_clause(TSqlParser::Execute_clauseContext * /*ctx*/) override;

	void enterDeclare_local(TSqlParser::Declare_localContext * /*ctx*/) override;
	void exitDeclare_local(TSqlParser::Declare_localContext * /*ctx*/) override;

	void enterTable_type_definition(TSqlParser::Table_type_definitionContext * /*ctx*/) override;
	void exitTable_type_definition(TSqlParser::Table_type_definitionContext * /*ctx*/) override;

	void enterXml_type_definition(TSqlParser::Xml_type_definitionContext * /*ctx*/) override;
	void exitXml_type_definition(TSqlParser::Xml_type_definitionContext * /*ctx*/) override;

	void enterXml_schema_collection(TSqlParser::Xml_schema_collectionContext * /*ctx*/) override;
	void exitXml_schema_collection(TSqlParser::Xml_schema_collectionContext * /*ctx*/) override;

	void enterColumn_def_table_constraints(TSqlParser::Column_def_table_constraintsContext * /*ctx*/) override;
	void exitColumn_def_table_constraints(TSqlParser::Column_def_table_constraintsContext * /*ctx*/) override;

	void enterColumn_def_table_constraint(TSqlParser::Column_def_table_constraintContext * /*ctx*/) override;
	void exitColumn_def_table_constraint(TSqlParser::Column_def_table_constraintContext * /*ctx*/) override;

	void enterColumn_definition(TSqlParser::Column_definitionContext * /*ctx*/) override;
	void exitColumn_definition(TSqlParser::Column_definitionContext * /*ctx*/) override;

	void enterMaterialized_column_definition(TSqlParser::Materialized_column_definitionContext * /*ctx*/) override;
	void exitMaterialized_column_definition(TSqlParser::Materialized_column_definitionContext * /*ctx*/) override;

	void enterColumn_constraint(TSqlParser::Column_constraintContext * /*ctx*/) override;
	void exitColumn_constraint(TSqlParser::Column_constraintContext * /*ctx*/) override;

	void enterTable_constraint(TSqlParser::Table_constraintContext * /*ctx*/) override;
	void exitTable_constraint(TSqlParser::Table_constraintContext * /*ctx*/) override;

	void enterOn_delete(TSqlParser::On_deleteContext * /*ctx*/) override;
	void exitOn_delete(TSqlParser::On_deleteContext * /*ctx*/) override;

	void enterOn_update(TSqlParser::On_updateContext * /*ctx*/) override;
	void exitOn_update(TSqlParser::On_updateContext * /*ctx*/) override;

	void enterIndex_options(TSqlParser::Index_optionsContext * /*ctx*/) override;
	void exitIndex_options(TSqlParser::Index_optionsContext * /*ctx*/) override;

	void enterIndex_option(TSqlParser::Index_optionContext * /*ctx*/) override;
	void exitIndex_option(TSqlParser::Index_optionContext * /*ctx*/) override;

	void enterDeclare_cursor(TSqlParser::Declare_cursorContext * /*ctx*/) override;
	void exitDeclare_cursor(TSqlParser::Declare_cursorContext * /*ctx*/) override;

	void enterDeclare_set_cursor_common(TSqlParser::Declare_set_cursor_commonContext * /*ctx*/) override;
	void exitDeclare_set_cursor_common(TSqlParser::Declare_set_cursor_commonContext * /*ctx*/) override;

	void enterDeclare_set_cursor_common_partial(TSqlParser::Declare_set_cursor_common_partialContext * /*ctx*/) override;
	void exitDeclare_set_cursor_common_partial(TSqlParser::Declare_set_cursor_common_partialContext * /*ctx*/) override;

	void enterFetch_cursor(TSqlParser::Fetch_cursorContext * /*ctx*/) override;
	void exitFetch_cursor(TSqlParser::Fetch_cursorContext * /*ctx*/) override;

	void enterSet_special(TSqlParser::Set_specialContext * /*ctx*/) override;
	void exitSet_special(TSqlParser::Set_specialContext * /*ctx*/) override;

	void enterConstant_LOCAL_ID(TSqlParser::Constant_LOCAL_IDContext * /*ctx*/) override;
	void exitConstant_LOCAL_ID(TSqlParser::Constant_LOCAL_IDContext * /*ctx*/) override;

	void enterExpression(TSqlParser::ExpressionContext * /*ctx*/) override;
	void exitExpression(TSqlParser::ExpressionContext * /*ctx*/) override;

	void enterPrimitive_expression(TSqlParser::Primitive_expressionContext * /*ctx*/) override;
	void exitPrimitive_expression(TSqlParser::Primitive_expressionContext * /*ctx*/) override;

	void enterCase_expression(TSqlParser::Case_expressionContext * /*ctx*/) override;
	void exitCase_expression(TSqlParser::Case_expressionContext * /*ctx*/) override;

	void enterUnary_operator_expression(TSqlParser::Unary_operator_expressionContext * /*ctx*/) override;
	void exitUnary_operator_expression(TSqlParser::Unary_operator_expressionContext * /*ctx*/) override;

	void enterBracket_expression(TSqlParser::Bracket_expressionContext * /*ctx*/) override;
	void exitBracket_expression(TSqlParser::Bracket_expressionContext * /*ctx*/) override;

	void enterConstant_expression(TSqlParser::Constant_expressionContext * /*ctx*/) override;
	void exitConstant_expression(TSqlParser::Constant_expressionContext * /*ctx*/) override;

	void enterSubquery(TSqlParser::SubqueryContext * /*ctx*/) override;
	void exitSubquery(TSqlParser::SubqueryContext * /*ctx*/) override;

	void enterWith_expression(TSqlParser::With_expressionContext * /*ctx*/) override;
	void exitWith_expression(TSqlParser::With_expressionContext * /*ctx*/) override;

	void enterCommon_table_expression(TSqlParser::Common_table_expressionContext * /*ctx*/) override;
	void exitCommon_table_expression(TSqlParser::Common_table_expressionContext * /*ctx*/) override;

	void enterUpdate_elem(TSqlParser::Update_elemContext * /*ctx*/) override;
	void exitUpdate_elem(TSqlParser::Update_elemContext * /*ctx*/) override;

	void enterSearch_condition_list(TSqlParser::Search_condition_listContext * /*ctx*/) override;
	void exitSearch_condition_list(TSqlParser::Search_condition_listContext * /*ctx*/) override;

	void enterSearch_condition(TSqlParser::Search_conditionContext * /*ctx*/) override;
	void exitSearch_condition(TSqlParser::Search_conditionContext * /*ctx*/) override;

	void enterSearch_condition_and(TSqlParser::Search_condition_andContext * /*ctx*/) override;
	void exitSearch_condition_and(TSqlParser::Search_condition_andContext * /*ctx*/) override;

	void enterSearch_condition_not(TSqlParser::Search_condition_notContext * /*ctx*/) override;
	void exitSearch_condition_not(TSqlParser::Search_condition_notContext * /*ctx*/) override;

	void enterPredicate(TSqlParser::PredicateContext * /*ctx*/) override;
	void exitPredicate(TSqlParser::PredicateContext * /*ctx*/) override;

	void enterQuery_expression(TSqlParser::Query_expressionContext * /*ctx*/) override;
	void exitQuery_expression(TSqlParser::Query_expressionContext * /*ctx*/) override;

	void enterSql_union(TSqlParser::Sql_unionContext * /*ctx*/) override;
	void exitSql_union(TSqlParser::Sql_unionContext * /*ctx*/) override;

	void enterQuery_specification(TSqlParser::Query_specificationContext * /*ctx*/) override;
	void exitQuery_specification(TSqlParser::Query_specificationContext * /*ctx*/) override;

	void enterTop_clause(TSqlParser::Top_clauseContext * /*ctx*/) override;
	void exitTop_clause(TSqlParser::Top_clauseContext * /*ctx*/) override;

	void enterTop_percent(TSqlParser::Top_percentContext * /*ctx*/) override;
	void exitTop_percent(TSqlParser::Top_percentContext * /*ctx*/) override;

	void enterTop_count(TSqlParser::Top_countContext * /*ctx*/) override;
	void exitTop_count(TSqlParser::Top_countContext * /*ctx*/) override;

	void enterOrder_by_clause(TSqlParser::Order_by_clauseContext * /*ctx*/) override;
	void exitOrder_by_clause(TSqlParser::Order_by_clauseContext * /*ctx*/) override;

	void enterFor_clause(TSqlParser::For_clauseContext * /*ctx*/) override;
	void exitFor_clause(TSqlParser::For_clauseContext * /*ctx*/) override;

	void enterXml_common_directives(TSqlParser::Xml_common_directivesContext * /*ctx*/) override;
	void exitXml_common_directives(TSqlParser::Xml_common_directivesContext * /*ctx*/) override;

	void enterOrder_by_expression(TSqlParser::Order_by_expressionContext * /*ctx*/) override;
	void exitOrder_by_expression(TSqlParser::Order_by_expressionContext * /*ctx*/) override;

	void enterGroup_by_item(TSqlParser::Group_by_itemContext * /*ctx*/) override;
	void exitGroup_by_item(TSqlParser::Group_by_itemContext * /*ctx*/) override;

	void enterOption_clause(TSqlParser::Option_clauseContext * /*ctx*/) override;
	void exitOption_clause(TSqlParser::Option_clauseContext * /*ctx*/) override;

	void enterOption(TSqlParser::OptionContext * /*ctx*/) override;
	void exitOption(TSqlParser::OptionContext * /*ctx*/) override;

	void enterOptimize_for_arg(TSqlParser::Optimize_for_argContext * /*ctx*/) override;
	void exitOptimize_for_arg(TSqlParser::Optimize_for_argContext * /*ctx*/) override;

	void enterSelect_list(TSqlParser::Select_listContext * /*ctx*/) override;
	void exitSelect_list(TSqlParser::Select_listContext * /*ctx*/) override;

	void enterUdt_method_arguments(TSqlParser::Udt_method_argumentsContext * /*ctx*/) override;
	void exitUdt_method_arguments(TSqlParser::Udt_method_argumentsContext * /*ctx*/) override;

	void enterAsterisk(TSqlParser::AsteriskContext * /*ctx*/) override;
	void exitAsterisk(TSqlParser::AsteriskContext * /*ctx*/) override;

	void enterColumn_elem(TSqlParser::Column_elemContext * /*ctx*/) override;
	void exitColumn_elem(TSqlParser::Column_elemContext * /*ctx*/) override;

	void enterUdt_elem(TSqlParser::Udt_elemContext * /*ctx*/) override;
	void exitUdt_elem(TSqlParser::Udt_elemContext * /*ctx*/) override;

	void enterExpression_elem(TSqlParser::Expression_elemContext * /*ctx*/) override;
	void exitExpression_elem(TSqlParser::Expression_elemContext * /*ctx*/) override;

	void enterSelect_list_elem(TSqlParser::Select_list_elemContext * /*ctx*/) override;
	void exitSelect_list_elem(TSqlParser::Select_list_elemContext * /*ctx*/) override;

	void enterTable_sources(TSqlParser::Table_sourcesContext * /*ctx*/) override;
	void exitTable_sources(TSqlParser::Table_sourcesContext * /*ctx*/) override;

	void enterTable_source(TSqlParser::Table_sourceContext * /*ctx*/) override;
	void exitTable_source(TSqlParser::Table_sourceContext * /*ctx*/) override;

	void enterTable_source_item_joined(TSqlParser::Table_source_item_joinedContext * /*ctx*/) override;
	void exitTable_source_item_joined(TSqlParser::Table_source_item_joinedContext * /*ctx*/) override;

	void enterTable_source_item(TSqlParser::Table_source_itemContext * /*ctx*/) override;
	void exitTable_source_item(TSqlParser::Table_source_itemContext * /*ctx*/) override;

	void enterOpen_xml(TSqlParser::Open_xmlContext * /*ctx*/) override;
	void exitOpen_xml(TSqlParser::Open_xmlContext * /*ctx*/) override;

	void enterSchema_declaration(TSqlParser::Schema_declarationContext * /*ctx*/) override;
	void exitSchema_declaration(TSqlParser::Schema_declarationContext * /*ctx*/) override;

	void enterColumn_declaration(TSqlParser::Column_declarationContext * /*ctx*/) override;
	void exitColumn_declaration(TSqlParser::Column_declarationContext * /*ctx*/) override;

	void enterChange_table(TSqlParser::Change_tableContext * /*ctx*/) override;
	void exitChange_table(TSqlParser::Change_tableContext * /*ctx*/) override;

	void enterJoin_part(TSqlParser::Join_partContext * /*ctx*/) override;
	void exitJoin_part(TSqlParser::Join_partContext * /*ctx*/) override;

	void enterPivot_clause(TSqlParser::Pivot_clauseContext * /*ctx*/) override;
	void exitPivot_clause(TSqlParser::Pivot_clauseContext * /*ctx*/) override;

	void enterUnpivot_clause(TSqlParser::Unpivot_clauseContext * /*ctx*/) override;
	void exitUnpivot_clause(TSqlParser::Unpivot_clauseContext * /*ctx*/) override;

	void enterFull_column_name_list(TSqlParser::Full_column_name_listContext * /*ctx*/) override;
	void exitFull_column_name_list(TSqlParser::Full_column_name_listContext * /*ctx*/) override;

	void enterTable_name_with_hint(TSqlParser::Table_name_with_hintContext * /*ctx*/) override;
	void exitTable_name_with_hint(TSqlParser::Table_name_with_hintContext * /*ctx*/) override;

	void enterRowset_function(TSqlParser::Rowset_functionContext * /*ctx*/) override;
	void exitRowset_function(TSqlParser::Rowset_functionContext * /*ctx*/) override;

	void enterBulk_option(TSqlParser::Bulk_optionContext * /*ctx*/) override;
	void exitBulk_option(TSqlParser::Bulk_optionContext * /*ctx*/) override;

	void enterDerived_table(TSqlParser::Derived_tableContext * /*ctx*/) override;
	void exitDerived_table(TSqlParser::Derived_tableContext * /*ctx*/) override;

	void enterBINARY_CHECKSUM(TSqlParser::BINARY_CHECKSUMContext * /*ctx*/) override;
	void exitBINARY_CHECKSUM(TSqlParser::BINARY_CHECKSUMContext * /*ctx*/) override;

	void enterCAST(TSqlParser::CASTContext * /*ctx*/) override;
	void exitCAST(TSqlParser::CASTContext * /*ctx*/) override;

	void enterCONVERT(TSqlParser::CONVERTContext * /*ctx*/) override;
	void exitCONVERT(TSqlParser::CONVERTContext * /*ctx*/) override;

	void enterCHECKSUM(TSqlParser::CHECKSUMContext * /*ctx*/) override;
	void exitCHECKSUM(TSqlParser::CHECKSUMContext * /*ctx*/) override;

	void enterCOALESCE(TSqlParser::COALESCEContext * /*ctx*/) override;
	void exitCOALESCE(TSqlParser::COALESCEContext * /*ctx*/) override;

	void enterCURRENT_TIMESTAMP(TSqlParser::CURRENT_TIMESTAMPContext * /*ctx*/) override;
	void exitCURRENT_TIMESTAMP(TSqlParser::CURRENT_TIMESTAMPContext * /*ctx*/) override;

	void enterCURRENT_USER(TSqlParser::CURRENT_USERContext * /*ctx*/) override;
	void exitCURRENT_USER(TSqlParser::CURRENT_USERContext * /*ctx*/) override;

	void enterDATEADD(TSqlParser::DATEADDContext * /*ctx*/) override;
	void exitDATEADD(TSqlParser::DATEADDContext * /*ctx*/) override;

	void enterDATEDIFF(TSqlParser::DATEDIFFContext * /*ctx*/) override;
	void exitDATEDIFF(TSqlParser::DATEDIFFContext * /*ctx*/) override;

	void enterDATENAME(TSqlParser::DATENAMEContext * /*ctx*/) override;
	void exitDATENAME(TSqlParser::DATENAMEContext * /*ctx*/) override;

	void enterDATEPART(TSqlParser::DATEPARTContext * /*ctx*/) override;
	void exitDATEPART(TSqlParser::DATEPARTContext * /*ctx*/) override;

	void enterGETDATE(TSqlParser::GETDATEContext * /*ctx*/) override;
	void exitGETDATE(TSqlParser::GETDATEContext * /*ctx*/) override;

	void enterGETUTCDATE(TSqlParser::GETUTCDATEContext * /*ctx*/) override;
	void exitGETUTCDATE(TSqlParser::GETUTCDATEContext * /*ctx*/) override;

	void enterIDENTITY(TSqlParser::IDENTITYContext * /*ctx*/) override;
	void exitIDENTITY(TSqlParser::IDENTITYContext * /*ctx*/) override;

	void enterMIN_ACTIVE_ROWVERSION(TSqlParser::MIN_ACTIVE_ROWVERSIONContext * /*ctx*/) override;
	void exitMIN_ACTIVE_ROWVERSION(TSqlParser::MIN_ACTIVE_ROWVERSIONContext * /*ctx*/) override;

	void enterNULLIF(TSqlParser::NULLIFContext * /*ctx*/) override;
	void exitNULLIF(TSqlParser::NULLIFContext * /*ctx*/) override;

	void enterSTUFF(TSqlParser::STUFFContext * /*ctx*/) override;
	void exitSTUFF(TSqlParser::STUFFContext * /*ctx*/) override;

	void enterSESSION_USER(TSqlParser::SESSION_USERContext * /*ctx*/) override;
	void exitSESSION_USER(TSqlParser::SESSION_USERContext * /*ctx*/) override;

	void enterSYSTEM_USER(TSqlParser::SYSTEM_USERContext * /*ctx*/) override;
	void exitSYSTEM_USER(TSqlParser::SYSTEM_USERContext * /*ctx*/) override;

	void enterISNULL(TSqlParser::ISNULLContext * /*ctx*/) override;
	void exitISNULL(TSqlParser::ISNULLContext * /*ctx*/) override;

	void enterXML_DATA_TYPE_FUNC(TSqlParser::XML_DATA_TYPE_FUNCContext * /*ctx*/) override;
	void exitXML_DATA_TYPE_FUNC(TSqlParser::XML_DATA_TYPE_FUNCContext * /*ctx*/) override;

	void enterIFF(TSqlParser::IFFContext * /*ctx*/) override;
	void exitIFF(TSqlParser::IFFContext * /*ctx*/) override;

	void enterRANKING_WINDOWED_FUNC(TSqlParser::RANKING_WINDOWED_FUNCContext * /*ctx*/) override;
	void exitRANKING_WINDOWED_FUNC(TSqlParser::RANKING_WINDOWED_FUNCContext * /*ctx*/) override;

	void enterAGGREGATE_WINDOWED_FUNC(TSqlParser::AGGREGATE_WINDOWED_FUNCContext * /*ctx*/) override;
	void exitAGGREGATE_WINDOWED_FUNC(TSqlParser::AGGREGATE_WINDOWED_FUNCContext * /*ctx*/) override;

	void enterANALYTIC_WINDOWED_FUNC(TSqlParser::ANALYTIC_WINDOWED_FUNCContext * /*ctx*/) override;
	void exitANALYTIC_WINDOWED_FUNC(TSqlParser::ANALYTIC_WINDOWED_FUNCContext * /*ctx*/) override;

	void enterSCALAR_FUNCTION(TSqlParser::SCALAR_FUNCTIONContext * /*ctx*/) override;
	void exitSCALAR_FUNCTION(TSqlParser::SCALAR_FUNCTIONContext * /*ctx*/) override;

	void enterSTRINGAGG(TSqlParser::STRINGAGGContext * /*ctx*/) override;
	void exitSTRINGAGG(TSqlParser::STRINGAGGContext * /*ctx*/) override;

	void enterXml_data_type_methods(TSqlParser::Xml_data_type_methodsContext * /*ctx*/) override;
	void exitXml_data_type_methods(TSqlParser::Xml_data_type_methodsContext * /*ctx*/) override;

	void enterValue_method(TSqlParser::Value_methodContext * /*ctx*/) override;
	void exitValue_method(TSqlParser::Value_methodContext * /*ctx*/) override;

	void enterQuery_method(TSqlParser::Query_methodContext * /*ctx*/) override;
	void exitQuery_method(TSqlParser::Query_methodContext * /*ctx*/) override;

	void enterExist_method(TSqlParser::Exist_methodContext * /*ctx*/) override;
	void exitExist_method(TSqlParser::Exist_methodContext * /*ctx*/) override;

	void enterModify_method(TSqlParser::Modify_methodContext * /*ctx*/) override;
	void exitModify_method(TSqlParser::Modify_methodContext * /*ctx*/) override;

	void enterNodes_method(TSqlParser::Nodes_methodContext * /*ctx*/) override;
	void exitNodes_method(TSqlParser::Nodes_methodContext * /*ctx*/) override;

	void enterSwitch_section(TSqlParser::Switch_sectionContext * /*ctx*/) override;
	void exitSwitch_section(TSqlParser::Switch_sectionContext * /*ctx*/) override;

	void enterSwitch_search_condition_section(TSqlParser::Switch_search_condition_sectionContext * /*ctx*/) override;
	void exitSwitch_search_condition_section(TSqlParser::Switch_search_condition_sectionContext * /*ctx*/) override;

	void enterAs_column_alias(TSqlParser::As_column_aliasContext * /*ctx*/) override;
	void exitAs_column_alias(TSqlParser::As_column_aliasContext * /*ctx*/) override;

	void enterAs_table_alias(TSqlParser::As_table_aliasContext * /*ctx*/) override;
	void exitAs_table_alias(TSqlParser::As_table_aliasContext * /*ctx*/) override;

	void enterTable_alias(TSqlParser::Table_aliasContext * /*ctx*/) override;
	void exitTable_alias(TSqlParser::Table_aliasContext * /*ctx*/) override;

	void enterWith_table_hints(TSqlParser::With_table_hintsContext * /*ctx*/) override;
	void exitWith_table_hints(TSqlParser::With_table_hintsContext * /*ctx*/) override;

	void enterInsert_with_table_hints(TSqlParser::Insert_with_table_hintsContext * /*ctx*/) override;
	void exitInsert_with_table_hints(TSqlParser::Insert_with_table_hintsContext * /*ctx*/) override;

	void enterTable_hint(TSqlParser::Table_hintContext * /*ctx*/) override;
	void exitTable_hint(TSqlParser::Table_hintContext * /*ctx*/) override;

	void enterIndex_value(TSqlParser::Index_valueContext * /*ctx*/) override;
	void exitIndex_value(TSqlParser::Index_valueContext * /*ctx*/) override;

	void enterColumn_alias_list(TSqlParser::Column_alias_listContext * /*ctx*/) override;
	void exitColumn_alias_list(TSqlParser::Column_alias_listContext * /*ctx*/) override;

	void enterColumn_alias(TSqlParser::Column_aliasContext * /*ctx*/) override;
	void exitColumn_alias(TSqlParser::Column_aliasContext * /*ctx*/) override;

	void enterTable_value_constructor(TSqlParser::Table_value_constructorContext * /*ctx*/) override;
	void exitTable_value_constructor(TSqlParser::Table_value_constructorContext * /*ctx*/) override;

	void enterExpression_list(TSqlParser::Expression_listContext * /*ctx*/) override;
	void exitExpression_list(TSqlParser::Expression_listContext * /*ctx*/) override;

	void enterRanking_windowed_function(TSqlParser::Ranking_windowed_functionContext * /*ctx*/) override;
	void exitRanking_windowed_function(TSqlParser::Ranking_windowed_functionContext * /*ctx*/) override;

	void enterAggregate_windowed_function(TSqlParser::Aggregate_windowed_functionContext * /*ctx*/) override;
	void exitAggregate_windowed_function(TSqlParser::Aggregate_windowed_functionContext * /*ctx*/) override;

	void enterAnalytic_windowed_function(TSqlParser::Analytic_windowed_functionContext * /*ctx*/) override;
	void exitAnalytic_windowed_function(TSqlParser::Analytic_windowed_functionContext * /*ctx*/) override;

	void enterAll_distinct_expression(TSqlParser::All_distinct_expressionContext * /*ctx*/) override;
	void exitAll_distinct_expression(TSqlParser::All_distinct_expressionContext * /*ctx*/) override;

	void enterOver_clause(TSqlParser::Over_clauseContext * /*ctx*/) override;
	void exitOver_clause(TSqlParser::Over_clauseContext * /*ctx*/) override;

	void enterRow_or_range_clause(TSqlParser::Row_or_range_clauseContext * /*ctx*/) override;
	void exitRow_or_range_clause(TSqlParser::Row_or_range_clauseContext * /*ctx*/) override;

	void enterWindow_frame_extent(TSqlParser::Window_frame_extentContext * /*ctx*/) override;
	void exitWindow_frame_extent(TSqlParser::Window_frame_extentContext * /*ctx*/) override;

	void enterWindow_frame_bound(TSqlParser::Window_frame_boundContext * /*ctx*/) override;
	void exitWindow_frame_bound(TSqlParser::Window_frame_boundContext * /*ctx*/) override;

	void enterWindow_frame_preceding(TSqlParser::Window_frame_precedingContext * /*ctx*/) override;
	void exitWindow_frame_preceding(TSqlParser::Window_frame_precedingContext * /*ctx*/) override;

	void enterWindow_frame_following(TSqlParser::Window_frame_followingContext * /*ctx*/) override;
	void exitWindow_frame_following(TSqlParser::Window_frame_followingContext * /*ctx*/) override;

	void enterCreate_database_option(TSqlParser::Create_database_optionContext * /*ctx*/) override;
	void exitCreate_database_option(TSqlParser::Create_database_optionContext * /*ctx*/) override;

	void enterDatabase_filestream_option(TSqlParser::Database_filestream_optionContext * /*ctx*/) override;
	void exitDatabase_filestream_option(TSqlParser::Database_filestream_optionContext * /*ctx*/) override;

	void enterDatabase_file_spec(TSqlParser::Database_file_specContext * /*ctx*/) override;
	void exitDatabase_file_spec(TSqlParser::Database_file_specContext * /*ctx*/) override;

	void enterFile_group(TSqlParser::File_groupContext * /*ctx*/) override;
	void exitFile_group(TSqlParser::File_groupContext * /*ctx*/) override;

	void enterFile_spec(TSqlParser::File_specContext * /*ctx*/) override;
	void exitFile_spec(TSqlParser::File_specContext * /*ctx*/) override;

	void enterEntity_name(TSqlParser::Entity_nameContext * /*ctx*/) override;
	void exitEntity_name(TSqlParser::Entity_nameContext * /*ctx*/) override;

	void enterEntity_name_for_azure_dw(TSqlParser::Entity_name_for_azure_dwContext * /*ctx*/) override;
	void exitEntity_name_for_azure_dw(TSqlParser::Entity_name_for_azure_dwContext * /*ctx*/) override;

	void enterEntity_name_for_parallel_dw(TSqlParser::Entity_name_for_parallel_dwContext * /*ctx*/) override;
	void exitEntity_name_for_parallel_dw(TSqlParser::Entity_name_for_parallel_dwContext * /*ctx*/) override;

	void enterFull_table_name(TSqlParser::Full_table_nameContext * /*ctx*/) override;
	void exitFull_table_name(TSqlParser::Full_table_nameContext * /*ctx*/) override;

	void enterTable_name(TSqlParser::Table_nameContext * /*ctx*/) override;
	void exitTable_name(TSqlParser::Table_nameContext * /*ctx*/) override;

	void enterSimple_name(TSqlParser::Simple_nameContext * /*ctx*/) override;
	void exitSimple_name(TSqlParser::Simple_nameContext * /*ctx*/) override;

	void enterFunc_proc_name_schema(TSqlParser::Func_proc_name_schemaContext * /*ctx*/) override;
	void exitFunc_proc_name_schema(TSqlParser::Func_proc_name_schemaContext * /*ctx*/) override;

	void enterFunc_proc_name_database_schema(TSqlParser::Func_proc_name_database_schemaContext * /*ctx*/) override;
	void exitFunc_proc_name_database_schema(TSqlParser::Func_proc_name_database_schemaContext * /*ctx*/) override;

	void enterFunc_proc_name_server_database_schema(TSqlParser::Func_proc_name_server_database_schemaContext * /*ctx*/) override;
	void exitFunc_proc_name_server_database_schema(TSqlParser::Func_proc_name_server_database_schemaContext * /*ctx*/) override;

	void enterDdl_object(TSqlParser::Ddl_objectContext * /*ctx*/) override;
	void exitDdl_object(TSqlParser::Ddl_objectContext * /*ctx*/) override;

	void enterFull_column_name(TSqlParser::Full_column_nameContext * /*ctx*/) override;
	void exitFull_column_name(TSqlParser::Full_column_nameContext * /*ctx*/) override;

	void enterColumn_name_list_with_order(TSqlParser::Column_name_list_with_orderContext * /*ctx*/) override;
	void exitColumn_name_list_with_order(TSqlParser::Column_name_list_with_orderContext * /*ctx*/) override;

	void enterColumn_name_list(TSqlParser::Column_name_listContext * /*ctx*/) override;
	void exitColumn_name_list(TSqlParser::Column_name_listContext * /*ctx*/) override;

	void enterCursor_name(TSqlParser::Cursor_nameContext * /*ctx*/) override;
	void exitCursor_name(TSqlParser::Cursor_nameContext * /*ctx*/) override;

	void enterOn_off(TSqlParser::On_offContext * /*ctx*/) override;
	void exitOn_off(TSqlParser::On_offContext * /*ctx*/) override;

	void enterClustered(TSqlParser::ClusteredContext * /*ctx*/) override;
	void exitClustered(TSqlParser::ClusteredContext * /*ctx*/) override;

	void enterNull_notnull(TSqlParser::Null_notnullContext * /*ctx*/) override;
	void exitNull_notnull(TSqlParser::Null_notnullContext * /*ctx*/) override;

	void enterNull_or_default(TSqlParser::Null_or_defaultContext * /*ctx*/) override;
	void exitNull_or_default(TSqlParser::Null_or_defaultContext * /*ctx*/) override;

	void enterScalar_function_name(TSqlParser::Scalar_function_nameContext * /*ctx*/) override;
	void exitScalar_function_name(TSqlParser::Scalar_function_nameContext * /*ctx*/) override;

	void enterBegin_conversation_timer(TSqlParser::Begin_conversation_timerContext * /*ctx*/) override;
	void exitBegin_conversation_timer(TSqlParser::Begin_conversation_timerContext * /*ctx*/) override;

	void enterBegin_conversation_dialog(TSqlParser::Begin_conversation_dialogContext * /*ctx*/) override;
	void exitBegin_conversation_dialog(TSqlParser::Begin_conversation_dialogContext * /*ctx*/) override;

	void enterContract_name(TSqlParser::Contract_nameContext * /*ctx*/) override;
	void exitContract_name(TSqlParser::Contract_nameContext * /*ctx*/) override;

	void enterService_name(TSqlParser::Service_nameContext * /*ctx*/) override;
	void exitService_name(TSqlParser::Service_nameContext * /*ctx*/) override;

	void enterEnd_conversation(TSqlParser::End_conversationContext * /*ctx*/) override;
	void exitEnd_conversation(TSqlParser::End_conversationContext * /*ctx*/) override;

	void enterWaitfor_conversation(TSqlParser::Waitfor_conversationContext * /*ctx*/) override;
	void exitWaitfor_conversation(TSqlParser::Waitfor_conversationContext * /*ctx*/) override;

	void enterGet_conversation(TSqlParser::Get_conversationContext * /*ctx*/) override;
	void exitGet_conversation(TSqlParser::Get_conversationContext * /*ctx*/) override;

	void enterQueue_id(TSqlParser::Queue_idContext * /*ctx*/) override;
	void exitQueue_id(TSqlParser::Queue_idContext * /*ctx*/) override;

	void enterSend_conversation(TSqlParser::Send_conversationContext * /*ctx*/) override;
	void exitSend_conversation(TSqlParser::Send_conversationContext * /*ctx*/) override;

	void enterData_type(TSqlParser::Data_typeContext * /*ctx*/) override;
	void exitData_type(TSqlParser::Data_typeContext * /*ctx*/) override;

	void enterDefault_value(TSqlParser::Default_valueContext * /*ctx*/) override;
	void exitDefault_value(TSqlParser::Default_valueContext * /*ctx*/) override;

	void enterConstant(TSqlParser::ConstantContext * /*ctx*/) override;
	void exitConstant(TSqlParser::ConstantContext * /*ctx*/) override;

	void enterSign(TSqlParser::SignContext * /*ctx*/) override;
	void exitSign(TSqlParser::SignContext * /*ctx*/) override;

	void enterId(TSqlParser::IdContext * /*ctx*/) override;
	void exitId(TSqlParser::IdContext * /*ctx*/) override;

	void enterSimple_id(TSqlParser::Simple_idContext * /*ctx*/) override;
	void exitSimple_id(TSqlParser::Simple_idContext * /*ctx*/) override;

	void enterComparison_operator(TSqlParser::Comparison_operatorContext * /*ctx*/) override;
	void exitComparison_operator(TSqlParser::Comparison_operatorContext * /*ctx*/) override;

	void enterAssignment_operator(TSqlParser::Assignment_operatorContext * /*ctx*/) override;
	void exitAssignment_operator(TSqlParser::Assignment_operatorContext * /*ctx*/) override;

	void enterFile_size(TSqlParser::File_sizeContext * /*ctx*/) override;
	void exitFile_size(TSqlParser::File_sizeContext * /*ctx*/) override;

};


#endif
