
// Generated from TSqlParser.g4 by ANTLR 4.9.2

#pragma once


#include "antlr4-runtime.h"
#include "TSqlParser.h"


/**
 * This interface defines an abstract listener for a parse tree produced by TSqlParser.
 */
class  TSqlParserListener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterTsql_file(TSqlParser::Tsql_fileContext *ctx) = 0;
  virtual void exitTsql_file(TSqlParser::Tsql_fileContext *ctx) = 0;

  virtual void enterBatch(TSqlParser::BatchContext *ctx) = 0;
  virtual void exitBatch(TSqlParser::BatchContext *ctx) = 0;

  virtual void enterSql_clauses(TSqlParser::Sql_clausesContext *ctx) = 0;
  virtual void exitSql_clauses(TSqlParser::Sql_clausesContext *ctx) = 0;

  virtual void enterSql_clause(TSqlParser::Sql_clauseContext *ctx) = 0;
  virtual void exitSql_clause(TSqlParser::Sql_clauseContext *ctx) = 0;

  virtual void enterDml_clause(TSqlParser::Dml_clauseContext *ctx) = 0;
  virtual void exitDml_clause(TSqlParser::Dml_clauseContext *ctx) = 0;

  virtual void enterDdl_clause(TSqlParser::Ddl_clauseContext *ctx) = 0;
  virtual void exitDdl_clause(TSqlParser::Ddl_clauseContext *ctx) = 0;

  virtual void enterCfl_statement(TSqlParser::Cfl_statementContext *ctx) = 0;
  virtual void exitCfl_statement(TSqlParser::Cfl_statementContext *ctx) = 0;

  virtual void enterBlock_statement(TSqlParser::Block_statementContext *ctx) = 0;
  virtual void exitBlock_statement(TSqlParser::Block_statementContext *ctx) = 0;

  virtual void enterBreak_statement(TSqlParser::Break_statementContext *ctx) = 0;
  virtual void exitBreak_statement(TSqlParser::Break_statementContext *ctx) = 0;

  virtual void enterContinue_statement(TSqlParser::Continue_statementContext *ctx) = 0;
  virtual void exitContinue_statement(TSqlParser::Continue_statementContext *ctx) = 0;

  virtual void enterGoto_statement(TSqlParser::Goto_statementContext *ctx) = 0;
  virtual void exitGoto_statement(TSqlParser::Goto_statementContext *ctx) = 0;

  virtual void enterReturn_statement(TSqlParser::Return_statementContext *ctx) = 0;
  virtual void exitReturn_statement(TSqlParser::Return_statementContext *ctx) = 0;

  virtual void enterIf_statement(TSqlParser::If_statementContext *ctx) = 0;
  virtual void exitIf_statement(TSqlParser::If_statementContext *ctx) = 0;

  virtual void enterThrow_statement(TSqlParser::Throw_statementContext *ctx) = 0;
  virtual void exitThrow_statement(TSqlParser::Throw_statementContext *ctx) = 0;

  virtual void enterThrow_error_number(TSqlParser::Throw_error_numberContext *ctx) = 0;
  virtual void exitThrow_error_number(TSqlParser::Throw_error_numberContext *ctx) = 0;

  virtual void enterThrow_message(TSqlParser::Throw_messageContext *ctx) = 0;
  virtual void exitThrow_message(TSqlParser::Throw_messageContext *ctx) = 0;

  virtual void enterThrow_state(TSqlParser::Throw_stateContext *ctx) = 0;
  virtual void exitThrow_state(TSqlParser::Throw_stateContext *ctx) = 0;

  virtual void enterTry_catch_statement(TSqlParser::Try_catch_statementContext *ctx) = 0;
  virtual void exitTry_catch_statement(TSqlParser::Try_catch_statementContext *ctx) = 0;

  virtual void enterWaitfor_statement(TSqlParser::Waitfor_statementContext *ctx) = 0;
  virtual void exitWaitfor_statement(TSqlParser::Waitfor_statementContext *ctx) = 0;

  virtual void enterWhile_statement(TSqlParser::While_statementContext *ctx) = 0;
  virtual void exitWhile_statement(TSqlParser::While_statementContext *ctx) = 0;

  virtual void enterPrint_statement(TSqlParser::Print_statementContext *ctx) = 0;
  virtual void exitPrint_statement(TSqlParser::Print_statementContext *ctx) = 0;

  virtual void enterRaiseerror_statement(TSqlParser::Raiseerror_statementContext *ctx) = 0;
  virtual void exitRaiseerror_statement(TSqlParser::Raiseerror_statementContext *ctx) = 0;

  virtual void enterEmpty_statement(TSqlParser::Empty_statementContext *ctx) = 0;
  virtual void exitEmpty_statement(TSqlParser::Empty_statementContext *ctx) = 0;

  virtual void enterAnother_statement(TSqlParser::Another_statementContext *ctx) = 0;
  virtual void exitAnother_statement(TSqlParser::Another_statementContext *ctx) = 0;

  virtual void enterDrop_aggregate(TSqlParser::Drop_aggregateContext *ctx) = 0;
  virtual void exitDrop_aggregate(TSqlParser::Drop_aggregateContext *ctx) = 0;

  virtual void enterEntity_to(TSqlParser::Entity_toContext *ctx) = 0;
  virtual void exitEntity_to(TSqlParser::Entity_toContext *ctx) = 0;

  virtual void enterColon_colon(TSqlParser::Colon_colonContext *ctx) = 0;
  virtual void exitColon_colon(TSqlParser::Colon_colonContext *ctx) = 0;

  virtual void enterClass_type(TSqlParser::Class_typeContext *ctx) = 0;
  virtual void exitClass_type(TSqlParser::Class_typeContext *ctx) = 0;

  virtual void enterClass_type_for_sql_database(TSqlParser::Class_type_for_sql_databaseContext *ctx) = 0;
  virtual void exitClass_type_for_sql_database(TSqlParser::Class_type_for_sql_databaseContext *ctx) = 0;

  virtual void enterClass_type_for_parallel_dw(TSqlParser::Class_type_for_parallel_dwContext *ctx) = 0;
  virtual void exitClass_type_for_parallel_dw(TSqlParser::Class_type_for_parallel_dwContext *ctx) = 0;

  virtual void enterDrop_schema(TSqlParser::Drop_schemaContext *ctx) = 0;
  virtual void exitDrop_schema(TSqlParser::Drop_schemaContext *ctx) = 0;

  virtual void enterLock_table(TSqlParser::Lock_tableContext *ctx) = 0;
  virtual void exitLock_table(TSqlParser::Lock_tableContext *ctx) = 0;

  virtual void enterTruncate_table(TSqlParser::Truncate_tableContext *ctx) = 0;
  virtual void exitTruncate_table(TSqlParser::Truncate_tableContext *ctx) = 0;

  virtual void enterEvent_session_predicate_expression(TSqlParser::Event_session_predicate_expressionContext *ctx) = 0;
  virtual void exitEvent_session_predicate_expression(TSqlParser::Event_session_predicate_expressionContext *ctx) = 0;

  virtual void enterEvent_session_predicate_factor(TSqlParser::Event_session_predicate_factorContext *ctx) = 0;
  virtual void exitEvent_session_predicate_factor(TSqlParser::Event_session_predicate_factorContext *ctx) = 0;

  virtual void enterEvent_session_predicate_leaf(TSqlParser::Event_session_predicate_leafContext *ctx) = 0;
  virtual void exitEvent_session_predicate_leaf(TSqlParser::Event_session_predicate_leafContext *ctx) = 0;

  virtual void enterAlter_schema_sql(TSqlParser::Alter_schema_sqlContext *ctx) = 0;
  virtual void exitAlter_schema_sql(TSqlParser::Alter_schema_sqlContext *ctx) = 0;

  virtual void enterCreate_schema(TSqlParser::Create_schemaContext *ctx) = 0;
  virtual void exitCreate_schema(TSqlParser::Create_schemaContext *ctx) = 0;

  virtual void enterCreate_xml_schema_collection(TSqlParser::Create_xml_schema_collectionContext *ctx) = 0;
  virtual void exitCreate_xml_schema_collection(TSqlParser::Create_xml_schema_collectionContext *ctx) = 0;

  virtual void enterCreate_queue(TSqlParser::Create_queueContext *ctx) = 0;
  virtual void exitCreate_queue(TSqlParser::Create_queueContext *ctx) = 0;

  virtual void enterQueue_settings(TSqlParser::Queue_settingsContext *ctx) = 0;
  virtual void exitQueue_settings(TSqlParser::Queue_settingsContext *ctx) = 0;

  virtual void enterAlter_queue(TSqlParser::Alter_queueContext *ctx) = 0;
  virtual void exitAlter_queue(TSqlParser::Alter_queueContext *ctx) = 0;

  virtual void enterQueue_action(TSqlParser::Queue_actionContext *ctx) = 0;
  virtual void exitQueue_action(TSqlParser::Queue_actionContext *ctx) = 0;

  virtual void enterQueue_rebuild_options(TSqlParser::Queue_rebuild_optionsContext *ctx) = 0;
  virtual void exitQueue_rebuild_options(TSqlParser::Queue_rebuild_optionsContext *ctx) = 0;

  virtual void enterConversation_statement(TSqlParser::Conversation_statementContext *ctx) = 0;
  virtual void exitConversation_statement(TSqlParser::Conversation_statementContext *ctx) = 0;

  virtual void enterMessage_statement(TSqlParser::Message_statementContext *ctx) = 0;
  virtual void exitMessage_statement(TSqlParser::Message_statementContext *ctx) = 0;

  virtual void enterMerge_statement(TSqlParser::Merge_statementContext *ctx) = 0;
  virtual void exitMerge_statement(TSqlParser::Merge_statementContext *ctx) = 0;

  virtual void enterMerge_matched(TSqlParser::Merge_matchedContext *ctx) = 0;
  virtual void exitMerge_matched(TSqlParser::Merge_matchedContext *ctx) = 0;

  virtual void enterMerge_not_matched(TSqlParser::Merge_not_matchedContext *ctx) = 0;
  virtual void exitMerge_not_matched(TSqlParser::Merge_not_matchedContext *ctx) = 0;

  virtual void enterDelete_statement(TSqlParser::Delete_statementContext *ctx) = 0;
  virtual void exitDelete_statement(TSqlParser::Delete_statementContext *ctx) = 0;

  virtual void enterDelete_statement_from(TSqlParser::Delete_statement_fromContext *ctx) = 0;
  virtual void exitDelete_statement_from(TSqlParser::Delete_statement_fromContext *ctx) = 0;

  virtual void enterInsert_statement(TSqlParser::Insert_statementContext *ctx) = 0;
  virtual void exitInsert_statement(TSqlParser::Insert_statementContext *ctx) = 0;

  virtual void enterInsert_statement_value(TSqlParser::Insert_statement_valueContext *ctx) = 0;
  virtual void exitInsert_statement_value(TSqlParser::Insert_statement_valueContext *ctx) = 0;

  virtual void enterReceive_statement(TSqlParser::Receive_statementContext *ctx) = 0;
  virtual void exitReceive_statement(TSqlParser::Receive_statementContext *ctx) = 0;

  virtual void enterSelect_statement(TSqlParser::Select_statementContext *ctx) = 0;
  virtual void exitSelect_statement(TSqlParser::Select_statementContext *ctx) = 0;

  virtual void enterTime(TSqlParser::TimeContext *ctx) = 0;
  virtual void exitTime(TSqlParser::TimeContext *ctx) = 0;

  virtual void enterUpdate_statement(TSqlParser::Update_statementContext *ctx) = 0;
  virtual void exitUpdate_statement(TSqlParser::Update_statementContext *ctx) = 0;

  virtual void enterOutput_clause(TSqlParser::Output_clauseContext *ctx) = 0;
  virtual void exitOutput_clause(TSqlParser::Output_clauseContext *ctx) = 0;

  virtual void enterOutput_dml_list_elem(TSqlParser::Output_dml_list_elemContext *ctx) = 0;
  virtual void exitOutput_dml_list_elem(TSqlParser::Output_dml_list_elemContext *ctx) = 0;

  virtual void enterOutput_column_name(TSqlParser::Output_column_nameContext *ctx) = 0;
  virtual void exitOutput_column_name(TSqlParser::Output_column_nameContext *ctx) = 0;

  virtual void enterCreate_database(TSqlParser::Create_databaseContext *ctx) = 0;
  virtual void exitCreate_database(TSqlParser::Create_databaseContext *ctx) = 0;

  virtual void enterCreate_index(TSqlParser::Create_indexContext *ctx) = 0;
  virtual void exitCreate_index(TSqlParser::Create_indexContext *ctx) = 0;

  virtual void enterCreate_or_alter_procedure(TSqlParser::Create_or_alter_procedureContext *ctx) = 0;
  virtual void exitCreate_or_alter_procedure(TSqlParser::Create_or_alter_procedureContext *ctx) = 0;

  virtual void enterCreate_or_alter_function(TSqlParser::Create_or_alter_functionContext *ctx) = 0;
  virtual void exitCreate_or_alter_function(TSqlParser::Create_or_alter_functionContext *ctx) = 0;

  virtual void enterFunc_body_returns_select(TSqlParser::Func_body_returns_selectContext *ctx) = 0;
  virtual void exitFunc_body_returns_select(TSqlParser::Func_body_returns_selectContext *ctx) = 0;

  virtual void enterFunc_body_returns_table(TSqlParser::Func_body_returns_tableContext *ctx) = 0;
  virtual void exitFunc_body_returns_table(TSqlParser::Func_body_returns_tableContext *ctx) = 0;

  virtual void enterFunc_body_returns_scalar(TSqlParser::Func_body_returns_scalarContext *ctx) = 0;
  virtual void exitFunc_body_returns_scalar(TSqlParser::Func_body_returns_scalarContext *ctx) = 0;

  virtual void enterProcedure_param(TSqlParser::Procedure_paramContext *ctx) = 0;
  virtual void exitProcedure_param(TSqlParser::Procedure_paramContext *ctx) = 0;

  virtual void enterProcedure_option(TSqlParser::Procedure_optionContext *ctx) = 0;
  virtual void exitProcedure_option(TSqlParser::Procedure_optionContext *ctx) = 0;

  virtual void enterFunction_option(TSqlParser::Function_optionContext *ctx) = 0;
  virtual void exitFunction_option(TSqlParser::Function_optionContext *ctx) = 0;

  virtual void enterCreate_table(TSqlParser::Create_tableContext *ctx) = 0;
  virtual void exitCreate_table(TSqlParser::Create_tableContext *ctx) = 0;

  virtual void enterTable_options(TSqlParser::Table_optionsContext *ctx) = 0;
  virtual void exitTable_options(TSqlParser::Table_optionsContext *ctx) = 0;

  virtual void enterCreate_view(TSqlParser::Create_viewContext *ctx) = 0;
  virtual void exitCreate_view(TSqlParser::Create_viewContext *ctx) = 0;

  virtual void enterView_attribute(TSqlParser::View_attributeContext *ctx) = 0;
  virtual void exitView_attribute(TSqlParser::View_attributeContext *ctx) = 0;

  virtual void enterAlter_table(TSqlParser::Alter_tableContext *ctx) = 0;
  virtual void exitAlter_table(TSqlParser::Alter_tableContext *ctx) = 0;

  virtual void enterCursor_option(TSqlParser::Cursor_optionContext *ctx) = 0;
  virtual void exitCursor_option(TSqlParser::Cursor_optionContext *ctx) = 0;

  virtual void enterTarget_recovery_time_option(TSqlParser::Target_recovery_time_optionContext *ctx) = 0;
  virtual void exitTarget_recovery_time_option(TSqlParser::Target_recovery_time_optionContext *ctx) = 0;

  virtual void enterTermination(TSqlParser::TerminationContext *ctx) = 0;
  virtual void exitTermination(TSqlParser::TerminationContext *ctx) = 0;

  virtual void enterDrop_index(TSqlParser::Drop_indexContext *ctx) = 0;
  virtual void exitDrop_index(TSqlParser::Drop_indexContext *ctx) = 0;

  virtual void enterDrop_relational_or_xml_or_spatial_index(TSqlParser::Drop_relational_or_xml_or_spatial_indexContext *ctx) = 0;
  virtual void exitDrop_relational_or_xml_or_spatial_index(TSqlParser::Drop_relational_or_xml_or_spatial_indexContext *ctx) = 0;

  virtual void enterDrop_backward_compatible_index(TSqlParser::Drop_backward_compatible_indexContext *ctx) = 0;
  virtual void exitDrop_backward_compatible_index(TSqlParser::Drop_backward_compatible_indexContext *ctx) = 0;

  virtual void enterDrop_procedure(TSqlParser::Drop_procedureContext *ctx) = 0;
  virtual void exitDrop_procedure(TSqlParser::Drop_procedureContext *ctx) = 0;

  virtual void enterDrop_function(TSqlParser::Drop_functionContext *ctx) = 0;
  virtual void exitDrop_function(TSqlParser::Drop_functionContext *ctx) = 0;

  virtual void enterDrop_table(TSqlParser::Drop_tableContext *ctx) = 0;
  virtual void exitDrop_table(TSqlParser::Drop_tableContext *ctx) = 0;

  virtual void enterDrop_view(TSqlParser::Drop_viewContext *ctx) = 0;
  virtual void exitDrop_view(TSqlParser::Drop_viewContext *ctx) = 0;

  virtual void enterCreate_type(TSqlParser::Create_typeContext *ctx) = 0;
  virtual void exitCreate_type(TSqlParser::Create_typeContext *ctx) = 0;

  virtual void enterDrop_type(TSqlParser::Drop_typeContext *ctx) = 0;
  virtual void exitDrop_type(TSqlParser::Drop_typeContext *ctx) = 0;

  virtual void enterRowset_function_limited(TSqlParser::Rowset_function_limitedContext *ctx) = 0;
  virtual void exitRowset_function_limited(TSqlParser::Rowset_function_limitedContext *ctx) = 0;

  virtual void enterOpenquery(TSqlParser::OpenqueryContext *ctx) = 0;
  virtual void exitOpenquery(TSqlParser::OpenqueryContext *ctx) = 0;

  virtual void enterOpendatasource(TSqlParser::OpendatasourceContext *ctx) = 0;
  virtual void exitOpendatasource(TSqlParser::OpendatasourceContext *ctx) = 0;

  virtual void enterDeclare_statement(TSqlParser::Declare_statementContext *ctx) = 0;
  virtual void exitDeclare_statement(TSqlParser::Declare_statementContext *ctx) = 0;

  virtual void enterCursor_statement(TSqlParser::Cursor_statementContext *ctx) = 0;
  virtual void exitCursor_statement(TSqlParser::Cursor_statementContext *ctx) = 0;

  virtual void enterKill_statement(TSqlParser::Kill_statementContext *ctx) = 0;
  virtual void exitKill_statement(TSqlParser::Kill_statementContext *ctx) = 0;

  virtual void enterKill_process(TSqlParser::Kill_processContext *ctx) = 0;
  virtual void exitKill_process(TSqlParser::Kill_processContext *ctx) = 0;

  virtual void enterKill_query_notification(TSqlParser::Kill_query_notificationContext *ctx) = 0;
  virtual void exitKill_query_notification(TSqlParser::Kill_query_notificationContext *ctx) = 0;

  virtual void enterKill_stats_job(TSqlParser::Kill_stats_jobContext *ctx) = 0;
  virtual void exitKill_stats_job(TSqlParser::Kill_stats_jobContext *ctx) = 0;

  virtual void enterExecute_statement(TSqlParser::Execute_statementContext *ctx) = 0;
  virtual void exitExecute_statement(TSqlParser::Execute_statementContext *ctx) = 0;

  virtual void enterExecute_body(TSqlParser::Execute_bodyContext *ctx) = 0;
  virtual void exitExecute_body(TSqlParser::Execute_bodyContext *ctx) = 0;

  virtual void enterExecute_statement_arg(TSqlParser::Execute_statement_argContext *ctx) = 0;
  virtual void exitExecute_statement_arg(TSqlParser::Execute_statement_argContext *ctx) = 0;

  virtual void enterExecute_var_string(TSqlParser::Execute_var_stringContext *ctx) = 0;
  virtual void exitExecute_var_string(TSqlParser::Execute_var_stringContext *ctx) = 0;

  virtual void enterSet_statement(TSqlParser::Set_statementContext *ctx) = 0;
  virtual void exitSet_statement(TSqlParser::Set_statementContext *ctx) = 0;

  virtual void enterTransaction_statement(TSqlParser::Transaction_statementContext *ctx) = 0;
  virtual void exitTransaction_statement(TSqlParser::Transaction_statementContext *ctx) = 0;

  virtual void enterGo_statement(TSqlParser::Go_statementContext *ctx) = 0;
  virtual void exitGo_statement(TSqlParser::Go_statementContext *ctx) = 0;

  virtual void enterUse_statement(TSqlParser::Use_statementContext *ctx) = 0;
  virtual void exitUse_statement(TSqlParser::Use_statementContext *ctx) = 0;

  virtual void enterSetuser_statement(TSqlParser::Setuser_statementContext *ctx) = 0;
  virtual void exitSetuser_statement(TSqlParser::Setuser_statementContext *ctx) = 0;

  virtual void enterReconfigure_statement(TSqlParser::Reconfigure_statementContext *ctx) = 0;
  virtual void exitReconfigure_statement(TSqlParser::Reconfigure_statementContext *ctx) = 0;

  virtual void enterShutdown_statement(TSqlParser::Shutdown_statementContext *ctx) = 0;
  virtual void exitShutdown_statement(TSqlParser::Shutdown_statementContext *ctx) = 0;

  virtual void enterDbcc_clause(TSqlParser::Dbcc_clauseContext *ctx) = 0;
  virtual void exitDbcc_clause(TSqlParser::Dbcc_clauseContext *ctx) = 0;

  virtual void enterDbcc_options(TSqlParser::Dbcc_optionsContext *ctx) = 0;
  virtual void exitDbcc_options(TSqlParser::Dbcc_optionsContext *ctx) = 0;

  virtual void enterExecute_clause(TSqlParser::Execute_clauseContext *ctx) = 0;
  virtual void exitExecute_clause(TSqlParser::Execute_clauseContext *ctx) = 0;

  virtual void enterDeclare_local(TSqlParser::Declare_localContext *ctx) = 0;
  virtual void exitDeclare_local(TSqlParser::Declare_localContext *ctx) = 0;

  virtual void enterTable_type_definition(TSqlParser::Table_type_definitionContext *ctx) = 0;
  virtual void exitTable_type_definition(TSqlParser::Table_type_definitionContext *ctx) = 0;

  virtual void enterXml_type_definition(TSqlParser::Xml_type_definitionContext *ctx) = 0;
  virtual void exitXml_type_definition(TSqlParser::Xml_type_definitionContext *ctx) = 0;

  virtual void enterXml_schema_collection(TSqlParser::Xml_schema_collectionContext *ctx) = 0;
  virtual void exitXml_schema_collection(TSqlParser::Xml_schema_collectionContext *ctx) = 0;

  virtual void enterColumn_def_table_constraints(TSqlParser::Column_def_table_constraintsContext *ctx) = 0;
  virtual void exitColumn_def_table_constraints(TSqlParser::Column_def_table_constraintsContext *ctx) = 0;

  virtual void enterColumn_def_table_constraint(TSqlParser::Column_def_table_constraintContext *ctx) = 0;
  virtual void exitColumn_def_table_constraint(TSqlParser::Column_def_table_constraintContext *ctx) = 0;

  virtual void enterColumn_definition(TSqlParser::Column_definitionContext *ctx) = 0;
  virtual void exitColumn_definition(TSqlParser::Column_definitionContext *ctx) = 0;

  virtual void enterMaterialized_column_definition(TSqlParser::Materialized_column_definitionContext *ctx) = 0;
  virtual void exitMaterialized_column_definition(TSqlParser::Materialized_column_definitionContext *ctx) = 0;

  virtual void enterColumn_constraint(TSqlParser::Column_constraintContext *ctx) = 0;
  virtual void exitColumn_constraint(TSqlParser::Column_constraintContext *ctx) = 0;

  virtual void enterTable_constraint(TSqlParser::Table_constraintContext *ctx) = 0;
  virtual void exitTable_constraint(TSqlParser::Table_constraintContext *ctx) = 0;

  virtual void enterOn_delete(TSqlParser::On_deleteContext *ctx) = 0;
  virtual void exitOn_delete(TSqlParser::On_deleteContext *ctx) = 0;

  virtual void enterOn_update(TSqlParser::On_updateContext *ctx) = 0;
  virtual void exitOn_update(TSqlParser::On_updateContext *ctx) = 0;

  virtual void enterIndex_options(TSqlParser::Index_optionsContext *ctx) = 0;
  virtual void exitIndex_options(TSqlParser::Index_optionsContext *ctx) = 0;

  virtual void enterIndex_option(TSqlParser::Index_optionContext *ctx) = 0;
  virtual void exitIndex_option(TSqlParser::Index_optionContext *ctx) = 0;

  virtual void enterDeclare_cursor(TSqlParser::Declare_cursorContext *ctx) = 0;
  virtual void exitDeclare_cursor(TSqlParser::Declare_cursorContext *ctx) = 0;

  virtual void enterDeclare_set_cursor_common(TSqlParser::Declare_set_cursor_commonContext *ctx) = 0;
  virtual void exitDeclare_set_cursor_common(TSqlParser::Declare_set_cursor_commonContext *ctx) = 0;

  virtual void enterDeclare_set_cursor_common_partial(TSqlParser::Declare_set_cursor_common_partialContext *ctx) = 0;
  virtual void exitDeclare_set_cursor_common_partial(TSqlParser::Declare_set_cursor_common_partialContext *ctx) = 0;

  virtual void enterFetch_cursor(TSqlParser::Fetch_cursorContext *ctx) = 0;
  virtual void exitFetch_cursor(TSqlParser::Fetch_cursorContext *ctx) = 0;

  virtual void enterSet_special(TSqlParser::Set_specialContext *ctx) = 0;
  virtual void exitSet_special(TSqlParser::Set_specialContext *ctx) = 0;

  virtual void enterConstant_LOCAL_ID(TSqlParser::Constant_LOCAL_IDContext *ctx) = 0;
  virtual void exitConstant_LOCAL_ID(TSqlParser::Constant_LOCAL_IDContext *ctx) = 0;

  virtual void enterExpression(TSqlParser::ExpressionContext *ctx) = 0;
  virtual void exitExpression(TSqlParser::ExpressionContext *ctx) = 0;

  virtual void enterPrimitive_expression(TSqlParser::Primitive_expressionContext *ctx) = 0;
  virtual void exitPrimitive_expression(TSqlParser::Primitive_expressionContext *ctx) = 0;

  virtual void enterCase_expression(TSqlParser::Case_expressionContext *ctx) = 0;
  virtual void exitCase_expression(TSqlParser::Case_expressionContext *ctx) = 0;

  virtual void enterUnary_operator_expression(TSqlParser::Unary_operator_expressionContext *ctx) = 0;
  virtual void exitUnary_operator_expression(TSqlParser::Unary_operator_expressionContext *ctx) = 0;

  virtual void enterBracket_expression(TSqlParser::Bracket_expressionContext *ctx) = 0;
  virtual void exitBracket_expression(TSqlParser::Bracket_expressionContext *ctx) = 0;

  virtual void enterConstant_expression(TSqlParser::Constant_expressionContext *ctx) = 0;
  virtual void exitConstant_expression(TSqlParser::Constant_expressionContext *ctx) = 0;

  virtual void enterSubquery(TSqlParser::SubqueryContext *ctx) = 0;
  virtual void exitSubquery(TSqlParser::SubqueryContext *ctx) = 0;

  virtual void enterWith_expression(TSqlParser::With_expressionContext *ctx) = 0;
  virtual void exitWith_expression(TSqlParser::With_expressionContext *ctx) = 0;

  virtual void enterCommon_table_expression(TSqlParser::Common_table_expressionContext *ctx) = 0;
  virtual void exitCommon_table_expression(TSqlParser::Common_table_expressionContext *ctx) = 0;

  virtual void enterUpdate_elem(TSqlParser::Update_elemContext *ctx) = 0;
  virtual void exitUpdate_elem(TSqlParser::Update_elemContext *ctx) = 0;

  virtual void enterSearch_condition_list(TSqlParser::Search_condition_listContext *ctx) = 0;
  virtual void exitSearch_condition_list(TSqlParser::Search_condition_listContext *ctx) = 0;

  virtual void enterSearch_condition(TSqlParser::Search_conditionContext *ctx) = 0;
  virtual void exitSearch_condition(TSqlParser::Search_conditionContext *ctx) = 0;

  virtual void enterSearch_condition_and(TSqlParser::Search_condition_andContext *ctx) = 0;
  virtual void exitSearch_condition_and(TSqlParser::Search_condition_andContext *ctx) = 0;

  virtual void enterSearch_condition_not(TSqlParser::Search_condition_notContext *ctx) = 0;
  virtual void exitSearch_condition_not(TSqlParser::Search_condition_notContext *ctx) = 0;

  virtual void enterPredicate(TSqlParser::PredicateContext *ctx) = 0;
  virtual void exitPredicate(TSqlParser::PredicateContext *ctx) = 0;

  virtual void enterQuery_expression(TSqlParser::Query_expressionContext *ctx) = 0;
  virtual void exitQuery_expression(TSqlParser::Query_expressionContext *ctx) = 0;

  virtual void enterSql_union(TSqlParser::Sql_unionContext *ctx) = 0;
  virtual void exitSql_union(TSqlParser::Sql_unionContext *ctx) = 0;

  virtual void enterQuery_specification(TSqlParser::Query_specificationContext *ctx) = 0;
  virtual void exitQuery_specification(TSqlParser::Query_specificationContext *ctx) = 0;

  virtual void enterTop_clause(TSqlParser::Top_clauseContext *ctx) = 0;
  virtual void exitTop_clause(TSqlParser::Top_clauseContext *ctx) = 0;

  virtual void enterTop_percent(TSqlParser::Top_percentContext *ctx) = 0;
  virtual void exitTop_percent(TSqlParser::Top_percentContext *ctx) = 0;

  virtual void enterTop_count(TSqlParser::Top_countContext *ctx) = 0;
  virtual void exitTop_count(TSqlParser::Top_countContext *ctx) = 0;

  virtual void enterOrder_by_clause(TSqlParser::Order_by_clauseContext *ctx) = 0;
  virtual void exitOrder_by_clause(TSqlParser::Order_by_clauseContext *ctx) = 0;

  virtual void enterFor_clause(TSqlParser::For_clauseContext *ctx) = 0;
  virtual void exitFor_clause(TSqlParser::For_clauseContext *ctx) = 0;

  virtual void enterXml_common_directives(TSqlParser::Xml_common_directivesContext *ctx) = 0;
  virtual void exitXml_common_directives(TSqlParser::Xml_common_directivesContext *ctx) = 0;

  virtual void enterOrder_by_expression(TSqlParser::Order_by_expressionContext *ctx) = 0;
  virtual void exitOrder_by_expression(TSqlParser::Order_by_expressionContext *ctx) = 0;

  virtual void enterGroup_by_item(TSqlParser::Group_by_itemContext *ctx) = 0;
  virtual void exitGroup_by_item(TSqlParser::Group_by_itemContext *ctx) = 0;

  virtual void enterOption_clause(TSqlParser::Option_clauseContext *ctx) = 0;
  virtual void exitOption_clause(TSqlParser::Option_clauseContext *ctx) = 0;

  virtual void enterOption(TSqlParser::OptionContext *ctx) = 0;
  virtual void exitOption(TSqlParser::OptionContext *ctx) = 0;

  virtual void enterOptimize_for_arg(TSqlParser::Optimize_for_argContext *ctx) = 0;
  virtual void exitOptimize_for_arg(TSqlParser::Optimize_for_argContext *ctx) = 0;

  virtual void enterSelect_list(TSqlParser::Select_listContext *ctx) = 0;
  virtual void exitSelect_list(TSqlParser::Select_listContext *ctx) = 0;

  virtual void enterUdt_method_arguments(TSqlParser::Udt_method_argumentsContext *ctx) = 0;
  virtual void exitUdt_method_arguments(TSqlParser::Udt_method_argumentsContext *ctx) = 0;

  virtual void enterAsterisk(TSqlParser::AsteriskContext *ctx) = 0;
  virtual void exitAsterisk(TSqlParser::AsteriskContext *ctx) = 0;

  virtual void enterColumn_elem(TSqlParser::Column_elemContext *ctx) = 0;
  virtual void exitColumn_elem(TSqlParser::Column_elemContext *ctx) = 0;

  virtual void enterUdt_elem(TSqlParser::Udt_elemContext *ctx) = 0;
  virtual void exitUdt_elem(TSqlParser::Udt_elemContext *ctx) = 0;

  virtual void enterExpression_elem(TSqlParser::Expression_elemContext *ctx) = 0;
  virtual void exitExpression_elem(TSqlParser::Expression_elemContext *ctx) = 0;

  virtual void enterSelect_list_elem(TSqlParser::Select_list_elemContext *ctx) = 0;
  virtual void exitSelect_list_elem(TSqlParser::Select_list_elemContext *ctx) = 0;

  virtual void enterTable_sources(TSqlParser::Table_sourcesContext *ctx) = 0;
  virtual void exitTable_sources(TSqlParser::Table_sourcesContext *ctx) = 0;

  virtual void enterTable_source(TSqlParser::Table_sourceContext *ctx) = 0;
  virtual void exitTable_source(TSqlParser::Table_sourceContext *ctx) = 0;

  virtual void enterTable_source_item_joined(TSqlParser::Table_source_item_joinedContext *ctx) = 0;
  virtual void exitTable_source_item_joined(TSqlParser::Table_source_item_joinedContext *ctx) = 0;

  virtual void enterTable_source_item(TSqlParser::Table_source_itemContext *ctx) = 0;
  virtual void exitTable_source_item(TSqlParser::Table_source_itemContext *ctx) = 0;

  virtual void enterOpen_xml(TSqlParser::Open_xmlContext *ctx) = 0;
  virtual void exitOpen_xml(TSqlParser::Open_xmlContext *ctx) = 0;

  virtual void enterSchema_declaration(TSqlParser::Schema_declarationContext *ctx) = 0;
  virtual void exitSchema_declaration(TSqlParser::Schema_declarationContext *ctx) = 0;

  virtual void enterColumn_declaration(TSqlParser::Column_declarationContext *ctx) = 0;
  virtual void exitColumn_declaration(TSqlParser::Column_declarationContext *ctx) = 0;

  virtual void enterChange_table(TSqlParser::Change_tableContext *ctx) = 0;
  virtual void exitChange_table(TSqlParser::Change_tableContext *ctx) = 0;

  virtual void enterJoin_part(TSqlParser::Join_partContext *ctx) = 0;
  virtual void exitJoin_part(TSqlParser::Join_partContext *ctx) = 0;

  virtual void enterPivot_clause(TSqlParser::Pivot_clauseContext *ctx) = 0;
  virtual void exitPivot_clause(TSqlParser::Pivot_clauseContext *ctx) = 0;

  virtual void enterUnpivot_clause(TSqlParser::Unpivot_clauseContext *ctx) = 0;
  virtual void exitUnpivot_clause(TSqlParser::Unpivot_clauseContext *ctx) = 0;

  virtual void enterFull_column_name_list(TSqlParser::Full_column_name_listContext *ctx) = 0;
  virtual void exitFull_column_name_list(TSqlParser::Full_column_name_listContext *ctx) = 0;

  virtual void enterTable_name_with_hint(TSqlParser::Table_name_with_hintContext *ctx) = 0;
  virtual void exitTable_name_with_hint(TSqlParser::Table_name_with_hintContext *ctx) = 0;

  virtual void enterRowset_function(TSqlParser::Rowset_functionContext *ctx) = 0;
  virtual void exitRowset_function(TSqlParser::Rowset_functionContext *ctx) = 0;

  virtual void enterBulk_option(TSqlParser::Bulk_optionContext *ctx) = 0;
  virtual void exitBulk_option(TSqlParser::Bulk_optionContext *ctx) = 0;

  virtual void enterDerived_table(TSqlParser::Derived_tableContext *ctx) = 0;
  virtual void exitDerived_table(TSqlParser::Derived_tableContext *ctx) = 0;

  virtual void enterBINARY_CHECKSUM(TSqlParser::BINARY_CHECKSUMContext *ctx) = 0;
  virtual void exitBINARY_CHECKSUM(TSqlParser::BINARY_CHECKSUMContext *ctx) = 0;

  virtual void enterCAST(TSqlParser::CASTContext *ctx) = 0;
  virtual void exitCAST(TSqlParser::CASTContext *ctx) = 0;

  virtual void enterCONVERT(TSqlParser::CONVERTContext *ctx) = 0;
  virtual void exitCONVERT(TSqlParser::CONVERTContext *ctx) = 0;

  virtual void enterCHECKSUM(TSqlParser::CHECKSUMContext *ctx) = 0;
  virtual void exitCHECKSUM(TSqlParser::CHECKSUMContext *ctx) = 0;

  virtual void enterCOALESCE(TSqlParser::COALESCEContext *ctx) = 0;
  virtual void exitCOALESCE(TSqlParser::COALESCEContext *ctx) = 0;

  virtual void enterCURRENT_TIMESTAMP(TSqlParser::CURRENT_TIMESTAMPContext *ctx) = 0;
  virtual void exitCURRENT_TIMESTAMP(TSqlParser::CURRENT_TIMESTAMPContext *ctx) = 0;

  virtual void enterCURRENT_USER(TSqlParser::CURRENT_USERContext *ctx) = 0;
  virtual void exitCURRENT_USER(TSqlParser::CURRENT_USERContext *ctx) = 0;

  virtual void enterDATEADD(TSqlParser::DATEADDContext *ctx) = 0;
  virtual void exitDATEADD(TSqlParser::DATEADDContext *ctx) = 0;

  virtual void enterDATEDIFF(TSqlParser::DATEDIFFContext *ctx) = 0;
  virtual void exitDATEDIFF(TSqlParser::DATEDIFFContext *ctx) = 0;

  virtual void enterDATENAME(TSqlParser::DATENAMEContext *ctx) = 0;
  virtual void exitDATENAME(TSqlParser::DATENAMEContext *ctx) = 0;

  virtual void enterDATEPART(TSqlParser::DATEPARTContext *ctx) = 0;
  virtual void exitDATEPART(TSqlParser::DATEPARTContext *ctx) = 0;

  virtual void enterGETDATE(TSqlParser::GETDATEContext *ctx) = 0;
  virtual void exitGETDATE(TSqlParser::GETDATEContext *ctx) = 0;

  virtual void enterGETUTCDATE(TSqlParser::GETUTCDATEContext *ctx) = 0;
  virtual void exitGETUTCDATE(TSqlParser::GETUTCDATEContext *ctx) = 0;

  virtual void enterIDENTITY(TSqlParser::IDENTITYContext *ctx) = 0;
  virtual void exitIDENTITY(TSqlParser::IDENTITYContext *ctx) = 0;

  virtual void enterMIN_ACTIVE_ROWVERSION(TSqlParser::MIN_ACTIVE_ROWVERSIONContext *ctx) = 0;
  virtual void exitMIN_ACTIVE_ROWVERSION(TSqlParser::MIN_ACTIVE_ROWVERSIONContext *ctx) = 0;

  virtual void enterNULLIF(TSqlParser::NULLIFContext *ctx) = 0;
  virtual void exitNULLIF(TSqlParser::NULLIFContext *ctx) = 0;

  virtual void enterSTUFF(TSqlParser::STUFFContext *ctx) = 0;
  virtual void exitSTUFF(TSqlParser::STUFFContext *ctx) = 0;

  virtual void enterSESSION_USER(TSqlParser::SESSION_USERContext *ctx) = 0;
  virtual void exitSESSION_USER(TSqlParser::SESSION_USERContext *ctx) = 0;

  virtual void enterSYSTEM_USER(TSqlParser::SYSTEM_USERContext *ctx) = 0;
  virtual void exitSYSTEM_USER(TSqlParser::SYSTEM_USERContext *ctx) = 0;

  virtual void enterISNULL(TSqlParser::ISNULLContext *ctx) = 0;
  virtual void exitISNULL(TSqlParser::ISNULLContext *ctx) = 0;

  virtual void enterXML_DATA_TYPE_FUNC(TSqlParser::XML_DATA_TYPE_FUNCContext *ctx) = 0;
  virtual void exitXML_DATA_TYPE_FUNC(TSqlParser::XML_DATA_TYPE_FUNCContext *ctx) = 0;

  virtual void enterIFF(TSqlParser::IFFContext *ctx) = 0;
  virtual void exitIFF(TSqlParser::IFFContext *ctx) = 0;

  virtual void enterRANKING_WINDOWED_FUNC(TSqlParser::RANKING_WINDOWED_FUNCContext *ctx) = 0;
  virtual void exitRANKING_WINDOWED_FUNC(TSqlParser::RANKING_WINDOWED_FUNCContext *ctx) = 0;

  virtual void enterAGGREGATE_WINDOWED_FUNC(TSqlParser::AGGREGATE_WINDOWED_FUNCContext *ctx) = 0;
  virtual void exitAGGREGATE_WINDOWED_FUNC(TSqlParser::AGGREGATE_WINDOWED_FUNCContext *ctx) = 0;

  virtual void enterANALYTIC_WINDOWED_FUNC(TSqlParser::ANALYTIC_WINDOWED_FUNCContext *ctx) = 0;
  virtual void exitANALYTIC_WINDOWED_FUNC(TSqlParser::ANALYTIC_WINDOWED_FUNCContext *ctx) = 0;

  virtual void enterSCALAR_FUNCTION(TSqlParser::SCALAR_FUNCTIONContext *ctx) = 0;
  virtual void exitSCALAR_FUNCTION(TSqlParser::SCALAR_FUNCTIONContext *ctx) = 0;

  virtual void enterSTRINGAGG(TSqlParser::STRINGAGGContext *ctx) = 0;
  virtual void exitSTRINGAGG(TSqlParser::STRINGAGGContext *ctx) = 0;

  virtual void enterXml_data_type_methods(TSqlParser::Xml_data_type_methodsContext *ctx) = 0;
  virtual void exitXml_data_type_methods(TSqlParser::Xml_data_type_methodsContext *ctx) = 0;

  virtual void enterValue_method(TSqlParser::Value_methodContext *ctx) = 0;
  virtual void exitValue_method(TSqlParser::Value_methodContext *ctx) = 0;

  virtual void enterQuery_method(TSqlParser::Query_methodContext *ctx) = 0;
  virtual void exitQuery_method(TSqlParser::Query_methodContext *ctx) = 0;

  virtual void enterExist_method(TSqlParser::Exist_methodContext *ctx) = 0;
  virtual void exitExist_method(TSqlParser::Exist_methodContext *ctx) = 0;

  virtual void enterModify_method(TSqlParser::Modify_methodContext *ctx) = 0;
  virtual void exitModify_method(TSqlParser::Modify_methodContext *ctx) = 0;

  virtual void enterNodes_method(TSqlParser::Nodes_methodContext *ctx) = 0;
  virtual void exitNodes_method(TSqlParser::Nodes_methodContext *ctx) = 0;

  virtual void enterSwitch_section(TSqlParser::Switch_sectionContext *ctx) = 0;
  virtual void exitSwitch_section(TSqlParser::Switch_sectionContext *ctx) = 0;

  virtual void enterSwitch_search_condition_section(TSqlParser::Switch_search_condition_sectionContext *ctx) = 0;
  virtual void exitSwitch_search_condition_section(TSqlParser::Switch_search_condition_sectionContext *ctx) = 0;

  virtual void enterAs_column_alias(TSqlParser::As_column_aliasContext *ctx) = 0;
  virtual void exitAs_column_alias(TSqlParser::As_column_aliasContext *ctx) = 0;

  virtual void enterAs_table_alias(TSqlParser::As_table_aliasContext *ctx) = 0;
  virtual void exitAs_table_alias(TSqlParser::As_table_aliasContext *ctx) = 0;

  virtual void enterTable_alias(TSqlParser::Table_aliasContext *ctx) = 0;
  virtual void exitTable_alias(TSqlParser::Table_aliasContext *ctx) = 0;

  virtual void enterWith_table_hints(TSqlParser::With_table_hintsContext *ctx) = 0;
  virtual void exitWith_table_hints(TSqlParser::With_table_hintsContext *ctx) = 0;

  virtual void enterInsert_with_table_hints(TSqlParser::Insert_with_table_hintsContext *ctx) = 0;
  virtual void exitInsert_with_table_hints(TSqlParser::Insert_with_table_hintsContext *ctx) = 0;

  virtual void enterTable_hint(TSqlParser::Table_hintContext *ctx) = 0;
  virtual void exitTable_hint(TSqlParser::Table_hintContext *ctx) = 0;

  virtual void enterIndex_value(TSqlParser::Index_valueContext *ctx) = 0;
  virtual void exitIndex_value(TSqlParser::Index_valueContext *ctx) = 0;

  virtual void enterColumn_alias_list(TSqlParser::Column_alias_listContext *ctx) = 0;
  virtual void exitColumn_alias_list(TSqlParser::Column_alias_listContext *ctx) = 0;

  virtual void enterColumn_alias(TSqlParser::Column_aliasContext *ctx) = 0;
  virtual void exitColumn_alias(TSqlParser::Column_aliasContext *ctx) = 0;

  virtual void enterTable_value_constructor(TSqlParser::Table_value_constructorContext *ctx) = 0;
  virtual void exitTable_value_constructor(TSqlParser::Table_value_constructorContext *ctx) = 0;

  virtual void enterExpression_list(TSqlParser::Expression_listContext *ctx) = 0;
  virtual void exitExpression_list(TSqlParser::Expression_listContext *ctx) = 0;

  virtual void enterRanking_windowed_function(TSqlParser::Ranking_windowed_functionContext *ctx) = 0;
  virtual void exitRanking_windowed_function(TSqlParser::Ranking_windowed_functionContext *ctx) = 0;

  virtual void enterAggregate_windowed_function(TSqlParser::Aggregate_windowed_functionContext *ctx) = 0;
  virtual void exitAggregate_windowed_function(TSqlParser::Aggregate_windowed_functionContext *ctx) = 0;

  virtual void enterAnalytic_windowed_function(TSqlParser::Analytic_windowed_functionContext *ctx) = 0;
  virtual void exitAnalytic_windowed_function(TSqlParser::Analytic_windowed_functionContext *ctx) = 0;

  virtual void enterAll_distinct_expression(TSqlParser::All_distinct_expressionContext *ctx) = 0;
  virtual void exitAll_distinct_expression(TSqlParser::All_distinct_expressionContext *ctx) = 0;

  virtual void enterOver_clause(TSqlParser::Over_clauseContext *ctx) = 0;
  virtual void exitOver_clause(TSqlParser::Over_clauseContext *ctx) = 0;

  virtual void enterRow_or_range_clause(TSqlParser::Row_or_range_clauseContext *ctx) = 0;
  virtual void exitRow_or_range_clause(TSqlParser::Row_or_range_clauseContext *ctx) = 0;

  virtual void enterWindow_frame_extent(TSqlParser::Window_frame_extentContext *ctx) = 0;
  virtual void exitWindow_frame_extent(TSqlParser::Window_frame_extentContext *ctx) = 0;

  virtual void enterWindow_frame_bound(TSqlParser::Window_frame_boundContext *ctx) = 0;
  virtual void exitWindow_frame_bound(TSqlParser::Window_frame_boundContext *ctx) = 0;

  virtual void enterWindow_frame_preceding(TSqlParser::Window_frame_precedingContext *ctx) = 0;
  virtual void exitWindow_frame_preceding(TSqlParser::Window_frame_precedingContext *ctx) = 0;

  virtual void enterWindow_frame_following(TSqlParser::Window_frame_followingContext *ctx) = 0;
  virtual void exitWindow_frame_following(TSqlParser::Window_frame_followingContext *ctx) = 0;

  virtual void enterCreate_database_option(TSqlParser::Create_database_optionContext *ctx) = 0;
  virtual void exitCreate_database_option(TSqlParser::Create_database_optionContext *ctx) = 0;

  virtual void enterDatabase_filestream_option(TSqlParser::Database_filestream_optionContext *ctx) = 0;
  virtual void exitDatabase_filestream_option(TSqlParser::Database_filestream_optionContext *ctx) = 0;

  virtual void enterDatabase_file_spec(TSqlParser::Database_file_specContext *ctx) = 0;
  virtual void exitDatabase_file_spec(TSqlParser::Database_file_specContext *ctx) = 0;

  virtual void enterFile_group(TSqlParser::File_groupContext *ctx) = 0;
  virtual void exitFile_group(TSqlParser::File_groupContext *ctx) = 0;

  virtual void enterFile_spec(TSqlParser::File_specContext *ctx) = 0;
  virtual void exitFile_spec(TSqlParser::File_specContext *ctx) = 0;

  virtual void enterEntity_name(TSqlParser::Entity_nameContext *ctx) = 0;
  virtual void exitEntity_name(TSqlParser::Entity_nameContext *ctx) = 0;

  virtual void enterEntity_name_for_azure_dw(TSqlParser::Entity_name_for_azure_dwContext *ctx) = 0;
  virtual void exitEntity_name_for_azure_dw(TSqlParser::Entity_name_for_azure_dwContext *ctx) = 0;

  virtual void enterEntity_name_for_parallel_dw(TSqlParser::Entity_name_for_parallel_dwContext *ctx) = 0;
  virtual void exitEntity_name_for_parallel_dw(TSqlParser::Entity_name_for_parallel_dwContext *ctx) = 0;

  virtual void enterFull_table_name(TSqlParser::Full_table_nameContext *ctx) = 0;
  virtual void exitFull_table_name(TSqlParser::Full_table_nameContext *ctx) = 0;

  virtual void enterTable_name(TSqlParser::Table_nameContext *ctx) = 0;
  virtual void exitTable_name(TSqlParser::Table_nameContext *ctx) = 0;

  virtual void enterSimple_name(TSqlParser::Simple_nameContext *ctx) = 0;
  virtual void exitSimple_name(TSqlParser::Simple_nameContext *ctx) = 0;

  virtual void enterFunc_proc_name_schema(TSqlParser::Func_proc_name_schemaContext *ctx) = 0;
  virtual void exitFunc_proc_name_schema(TSqlParser::Func_proc_name_schemaContext *ctx) = 0;

  virtual void enterFunc_proc_name_database_schema(TSqlParser::Func_proc_name_database_schemaContext *ctx) = 0;
  virtual void exitFunc_proc_name_database_schema(TSqlParser::Func_proc_name_database_schemaContext *ctx) = 0;

  virtual void enterFunc_proc_name_server_database_schema(TSqlParser::Func_proc_name_server_database_schemaContext *ctx) = 0;
  virtual void exitFunc_proc_name_server_database_schema(TSqlParser::Func_proc_name_server_database_schemaContext *ctx) = 0;

  virtual void enterDdl_object(TSqlParser::Ddl_objectContext *ctx) = 0;
  virtual void exitDdl_object(TSqlParser::Ddl_objectContext *ctx) = 0;

  virtual void enterFull_column_name(TSqlParser::Full_column_nameContext *ctx) = 0;
  virtual void exitFull_column_name(TSqlParser::Full_column_nameContext *ctx) = 0;

  virtual void enterColumn_name_list_with_order(TSqlParser::Column_name_list_with_orderContext *ctx) = 0;
  virtual void exitColumn_name_list_with_order(TSqlParser::Column_name_list_with_orderContext *ctx) = 0;

  virtual void enterColumn_name_list(TSqlParser::Column_name_listContext *ctx) = 0;
  virtual void exitColumn_name_list(TSqlParser::Column_name_listContext *ctx) = 0;

  virtual void enterCursor_name(TSqlParser::Cursor_nameContext *ctx) = 0;
  virtual void exitCursor_name(TSqlParser::Cursor_nameContext *ctx) = 0;

  virtual void enterOn_off(TSqlParser::On_offContext *ctx) = 0;
  virtual void exitOn_off(TSqlParser::On_offContext *ctx) = 0;

  virtual void enterClustered(TSqlParser::ClusteredContext *ctx) = 0;
  virtual void exitClustered(TSqlParser::ClusteredContext *ctx) = 0;

  virtual void enterNull_notnull(TSqlParser::Null_notnullContext *ctx) = 0;
  virtual void exitNull_notnull(TSqlParser::Null_notnullContext *ctx) = 0;

  virtual void enterNull_or_default(TSqlParser::Null_or_defaultContext *ctx) = 0;
  virtual void exitNull_or_default(TSqlParser::Null_or_defaultContext *ctx) = 0;

  virtual void enterScalar_function_name(TSqlParser::Scalar_function_nameContext *ctx) = 0;
  virtual void exitScalar_function_name(TSqlParser::Scalar_function_nameContext *ctx) = 0;

  virtual void enterBegin_conversation_timer(TSqlParser::Begin_conversation_timerContext *ctx) = 0;
  virtual void exitBegin_conversation_timer(TSqlParser::Begin_conversation_timerContext *ctx) = 0;

  virtual void enterBegin_conversation_dialog(TSqlParser::Begin_conversation_dialogContext *ctx) = 0;
  virtual void exitBegin_conversation_dialog(TSqlParser::Begin_conversation_dialogContext *ctx) = 0;

  virtual void enterContract_name(TSqlParser::Contract_nameContext *ctx) = 0;
  virtual void exitContract_name(TSqlParser::Contract_nameContext *ctx) = 0;

  virtual void enterService_name(TSqlParser::Service_nameContext *ctx) = 0;
  virtual void exitService_name(TSqlParser::Service_nameContext *ctx) = 0;

  virtual void enterEnd_conversation(TSqlParser::End_conversationContext *ctx) = 0;
  virtual void exitEnd_conversation(TSqlParser::End_conversationContext *ctx) = 0;

  virtual void enterWaitfor_conversation(TSqlParser::Waitfor_conversationContext *ctx) = 0;
  virtual void exitWaitfor_conversation(TSqlParser::Waitfor_conversationContext *ctx) = 0;

  virtual void enterGet_conversation(TSqlParser::Get_conversationContext *ctx) = 0;
  virtual void exitGet_conversation(TSqlParser::Get_conversationContext *ctx) = 0;

  virtual void enterQueue_id(TSqlParser::Queue_idContext *ctx) = 0;
  virtual void exitQueue_id(TSqlParser::Queue_idContext *ctx) = 0;

  virtual void enterSend_conversation(TSqlParser::Send_conversationContext *ctx) = 0;
  virtual void exitSend_conversation(TSqlParser::Send_conversationContext *ctx) = 0;

  virtual void enterData_type(TSqlParser::Data_typeContext *ctx) = 0;
  virtual void exitData_type(TSqlParser::Data_typeContext *ctx) = 0;

  virtual void enterDefault_value(TSqlParser::Default_valueContext *ctx) = 0;
  virtual void exitDefault_value(TSqlParser::Default_valueContext *ctx) = 0;

  virtual void enterConstant(TSqlParser::ConstantContext *ctx) = 0;
  virtual void exitConstant(TSqlParser::ConstantContext *ctx) = 0;

  virtual void enterSign(TSqlParser::SignContext *ctx) = 0;
  virtual void exitSign(TSqlParser::SignContext *ctx) = 0;

  virtual void enterId(TSqlParser::IdContext *ctx) = 0;
  virtual void exitId(TSqlParser::IdContext *ctx) = 0;

  virtual void enterSimple_id(TSqlParser::Simple_idContext *ctx) = 0;
  virtual void exitSimple_id(TSqlParser::Simple_idContext *ctx) = 0;

  virtual void enterComparison_operator(TSqlParser::Comparison_operatorContext *ctx) = 0;
  virtual void exitComparison_operator(TSqlParser::Comparison_operatorContext *ctx) = 0;

  virtual void enterAssignment_operator(TSqlParser::Assignment_operatorContext *ctx) = 0;
  virtual void exitAssignment_operator(TSqlParser::Assignment_operatorContext *ctx) = 0;

  virtual void enterFile_size(TSqlParser::File_sizeContext *ctx) = 0;
  virtual void exitFile_size(TSqlParser::File_sizeContext *ctx) = 0;


};

