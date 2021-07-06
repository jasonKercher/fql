
// Generated from TSqlParser.g4 by ANTLR 4.9.2

#pragma once


#include "antlr4-runtime.h"
#include "TSqlParserListener.h"


/**
 * This class provides an empty implementation of TSqlParserListener,
 * which can be extended to create a listener which only needs to handle a subset
 * of the available methods.
 */
class  TSqlParserBaseListener : public TSqlParserListener {
public:

  virtual void enterTsql_file(TSqlParser::Tsql_fileContext * /*ctx*/) override { }
  virtual void exitTsql_file(TSqlParser::Tsql_fileContext * /*ctx*/) override { }

  virtual void enterBatch(TSqlParser::BatchContext * /*ctx*/) override { }
  virtual void exitBatch(TSqlParser::BatchContext * /*ctx*/) override { }

  virtual void enterSql_clauses(TSqlParser::Sql_clausesContext * /*ctx*/) override { }
  virtual void exitSql_clauses(TSqlParser::Sql_clausesContext * /*ctx*/) override { }

  virtual void enterSql_clause(TSqlParser::Sql_clauseContext * /*ctx*/) override { }
  virtual void exitSql_clause(TSqlParser::Sql_clauseContext * /*ctx*/) override { }

  virtual void enterDml_clause(TSqlParser::Dml_clauseContext * /*ctx*/) override { }
  virtual void exitDml_clause(TSqlParser::Dml_clauseContext * /*ctx*/) override { }

  virtual void enterDdl_clause(TSqlParser::Ddl_clauseContext * /*ctx*/) override { }
  virtual void exitDdl_clause(TSqlParser::Ddl_clauseContext * /*ctx*/) override { }

  virtual void enterCfl_statement(TSqlParser::Cfl_statementContext * /*ctx*/) override { }
  virtual void exitCfl_statement(TSqlParser::Cfl_statementContext * /*ctx*/) override { }

  virtual void enterBlock_statement(TSqlParser::Block_statementContext * /*ctx*/) override { }
  virtual void exitBlock_statement(TSqlParser::Block_statementContext * /*ctx*/) override { }

  virtual void enterBreak_statement(TSqlParser::Break_statementContext * /*ctx*/) override { }
  virtual void exitBreak_statement(TSqlParser::Break_statementContext * /*ctx*/) override { }

  virtual void enterContinue_statement(TSqlParser::Continue_statementContext * /*ctx*/) override { }
  virtual void exitContinue_statement(TSqlParser::Continue_statementContext * /*ctx*/) override { }

  virtual void enterGoto_statement(TSqlParser::Goto_statementContext * /*ctx*/) override { }
  virtual void exitGoto_statement(TSqlParser::Goto_statementContext * /*ctx*/) override { }

  virtual void enterReturn_statement(TSqlParser::Return_statementContext * /*ctx*/) override { }
  virtual void exitReturn_statement(TSqlParser::Return_statementContext * /*ctx*/) override { }

  virtual void enterIf_statement(TSqlParser::If_statementContext * /*ctx*/) override { }
  virtual void exitIf_statement(TSqlParser::If_statementContext * /*ctx*/) override { }

  virtual void enterThrow_statement(TSqlParser::Throw_statementContext * /*ctx*/) override { }
  virtual void exitThrow_statement(TSqlParser::Throw_statementContext * /*ctx*/) override { }

  virtual void enterThrow_error_number(TSqlParser::Throw_error_numberContext * /*ctx*/) override { }
  virtual void exitThrow_error_number(TSqlParser::Throw_error_numberContext * /*ctx*/) override { }

  virtual void enterThrow_message(TSqlParser::Throw_messageContext * /*ctx*/) override { }
  virtual void exitThrow_message(TSqlParser::Throw_messageContext * /*ctx*/) override { }

  virtual void enterThrow_state(TSqlParser::Throw_stateContext * /*ctx*/) override { }
  virtual void exitThrow_state(TSqlParser::Throw_stateContext * /*ctx*/) override { }

  virtual void enterTry_catch_statement(TSqlParser::Try_catch_statementContext * /*ctx*/) override { }
  virtual void exitTry_catch_statement(TSqlParser::Try_catch_statementContext * /*ctx*/) override { }

  virtual void enterWaitfor_statement(TSqlParser::Waitfor_statementContext * /*ctx*/) override { }
  virtual void exitWaitfor_statement(TSqlParser::Waitfor_statementContext * /*ctx*/) override { }

  virtual void enterWhile_statement(TSqlParser::While_statementContext * /*ctx*/) override { }
  virtual void exitWhile_statement(TSqlParser::While_statementContext * /*ctx*/) override { }

  virtual void enterPrint_statement(TSqlParser::Print_statementContext * /*ctx*/) override { }
  virtual void exitPrint_statement(TSqlParser::Print_statementContext * /*ctx*/) override { }

  virtual void enterRaiseerror_statement(TSqlParser::Raiseerror_statementContext * /*ctx*/) override { }
  virtual void exitRaiseerror_statement(TSqlParser::Raiseerror_statementContext * /*ctx*/) override { }

  virtual void enterEmpty_statement(TSqlParser::Empty_statementContext * /*ctx*/) override { }
  virtual void exitEmpty_statement(TSqlParser::Empty_statementContext * /*ctx*/) override { }

  virtual void enterAnother_statement(TSqlParser::Another_statementContext * /*ctx*/) override { }
  virtual void exitAnother_statement(TSqlParser::Another_statementContext * /*ctx*/) override { }

  virtual void enterDrop_aggregate(TSqlParser::Drop_aggregateContext * /*ctx*/) override { }
  virtual void exitDrop_aggregate(TSqlParser::Drop_aggregateContext * /*ctx*/) override { }

  virtual void enterEntity_to(TSqlParser::Entity_toContext * /*ctx*/) override { }
  virtual void exitEntity_to(TSqlParser::Entity_toContext * /*ctx*/) override { }

  virtual void enterColon_colon(TSqlParser::Colon_colonContext * /*ctx*/) override { }
  virtual void exitColon_colon(TSqlParser::Colon_colonContext * /*ctx*/) override { }

  virtual void enterClass_type(TSqlParser::Class_typeContext * /*ctx*/) override { }
  virtual void exitClass_type(TSqlParser::Class_typeContext * /*ctx*/) override { }

  virtual void enterClass_type_for_sql_database(TSqlParser::Class_type_for_sql_databaseContext * /*ctx*/) override { }
  virtual void exitClass_type_for_sql_database(TSqlParser::Class_type_for_sql_databaseContext * /*ctx*/) override { }

  virtual void enterClass_type_for_parallel_dw(TSqlParser::Class_type_for_parallel_dwContext * /*ctx*/) override { }
  virtual void exitClass_type_for_parallel_dw(TSqlParser::Class_type_for_parallel_dwContext * /*ctx*/) override { }

  virtual void enterDrop_schema(TSqlParser::Drop_schemaContext * /*ctx*/) override { }
  virtual void exitDrop_schema(TSqlParser::Drop_schemaContext * /*ctx*/) override { }

  virtual void enterLock_table(TSqlParser::Lock_tableContext * /*ctx*/) override { }
  virtual void exitLock_table(TSqlParser::Lock_tableContext * /*ctx*/) override { }

  virtual void enterTruncate_table(TSqlParser::Truncate_tableContext * /*ctx*/) override { }
  virtual void exitTruncate_table(TSqlParser::Truncate_tableContext * /*ctx*/) override { }

  virtual void enterEvent_session_predicate_expression(TSqlParser::Event_session_predicate_expressionContext * /*ctx*/) override { }
  virtual void exitEvent_session_predicate_expression(TSqlParser::Event_session_predicate_expressionContext * /*ctx*/) override { }

  virtual void enterEvent_session_predicate_factor(TSqlParser::Event_session_predicate_factorContext * /*ctx*/) override { }
  virtual void exitEvent_session_predicate_factor(TSqlParser::Event_session_predicate_factorContext * /*ctx*/) override { }

  virtual void enterEvent_session_predicate_leaf(TSqlParser::Event_session_predicate_leafContext * /*ctx*/) override { }
  virtual void exitEvent_session_predicate_leaf(TSqlParser::Event_session_predicate_leafContext * /*ctx*/) override { }

  virtual void enterAlter_schema_sql(TSqlParser::Alter_schema_sqlContext * /*ctx*/) override { }
  virtual void exitAlter_schema_sql(TSqlParser::Alter_schema_sqlContext * /*ctx*/) override { }

  virtual void enterCreate_schema(TSqlParser::Create_schemaContext * /*ctx*/) override { }
  virtual void exitCreate_schema(TSqlParser::Create_schemaContext * /*ctx*/) override { }

  virtual void enterCreate_xml_schema_collection(TSqlParser::Create_xml_schema_collectionContext * /*ctx*/) override { }
  virtual void exitCreate_xml_schema_collection(TSqlParser::Create_xml_schema_collectionContext * /*ctx*/) override { }

  virtual void enterCreate_queue(TSqlParser::Create_queueContext * /*ctx*/) override { }
  virtual void exitCreate_queue(TSqlParser::Create_queueContext * /*ctx*/) override { }

  virtual void enterQueue_settings(TSqlParser::Queue_settingsContext * /*ctx*/) override { }
  virtual void exitQueue_settings(TSqlParser::Queue_settingsContext * /*ctx*/) override { }

  virtual void enterAlter_queue(TSqlParser::Alter_queueContext * /*ctx*/) override { }
  virtual void exitAlter_queue(TSqlParser::Alter_queueContext * /*ctx*/) override { }

  virtual void enterQueue_action(TSqlParser::Queue_actionContext * /*ctx*/) override { }
  virtual void exitQueue_action(TSqlParser::Queue_actionContext * /*ctx*/) override { }

  virtual void enterQueue_rebuild_options(TSqlParser::Queue_rebuild_optionsContext * /*ctx*/) override { }
  virtual void exitQueue_rebuild_options(TSqlParser::Queue_rebuild_optionsContext * /*ctx*/) override { }

  virtual void enterConversation_statement(TSqlParser::Conversation_statementContext * /*ctx*/) override { }
  virtual void exitConversation_statement(TSqlParser::Conversation_statementContext * /*ctx*/) override { }

  virtual void enterMessage_statement(TSqlParser::Message_statementContext * /*ctx*/) override { }
  virtual void exitMessage_statement(TSqlParser::Message_statementContext * /*ctx*/) override { }

  virtual void enterMerge_statement(TSqlParser::Merge_statementContext * /*ctx*/) override { }
  virtual void exitMerge_statement(TSqlParser::Merge_statementContext * /*ctx*/) override { }

  virtual void enterMerge_matched(TSqlParser::Merge_matchedContext * /*ctx*/) override { }
  virtual void exitMerge_matched(TSqlParser::Merge_matchedContext * /*ctx*/) override { }

  virtual void enterMerge_not_matched(TSqlParser::Merge_not_matchedContext * /*ctx*/) override { }
  virtual void exitMerge_not_matched(TSqlParser::Merge_not_matchedContext * /*ctx*/) override { }

  virtual void enterDelete_statement(TSqlParser::Delete_statementContext * /*ctx*/) override { }
  virtual void exitDelete_statement(TSqlParser::Delete_statementContext * /*ctx*/) override { }

  virtual void enterDelete_statement_from(TSqlParser::Delete_statement_fromContext * /*ctx*/) override { }
  virtual void exitDelete_statement_from(TSqlParser::Delete_statement_fromContext * /*ctx*/) override { }

  virtual void enterInsert_statement(TSqlParser::Insert_statementContext * /*ctx*/) override { }
  virtual void exitInsert_statement(TSqlParser::Insert_statementContext * /*ctx*/) override { }

  virtual void enterInsert_statement_value(TSqlParser::Insert_statement_valueContext * /*ctx*/) override { }
  virtual void exitInsert_statement_value(TSqlParser::Insert_statement_valueContext * /*ctx*/) override { }

  virtual void enterReceive_statement(TSqlParser::Receive_statementContext * /*ctx*/) override { }
  virtual void exitReceive_statement(TSqlParser::Receive_statementContext * /*ctx*/) override { }

  virtual void enterSelect_statement(TSqlParser::Select_statementContext * /*ctx*/) override { }
  virtual void exitSelect_statement(TSqlParser::Select_statementContext * /*ctx*/) override { }

  virtual void enterTime(TSqlParser::TimeContext * /*ctx*/) override { }
  virtual void exitTime(TSqlParser::TimeContext * /*ctx*/) override { }

  virtual void enterUpdate_statement(TSqlParser::Update_statementContext * /*ctx*/) override { }
  virtual void exitUpdate_statement(TSqlParser::Update_statementContext * /*ctx*/) override { }

  virtual void enterOutput_clause(TSqlParser::Output_clauseContext * /*ctx*/) override { }
  virtual void exitOutput_clause(TSqlParser::Output_clauseContext * /*ctx*/) override { }

  virtual void enterOutput_dml_list_elem(TSqlParser::Output_dml_list_elemContext * /*ctx*/) override { }
  virtual void exitOutput_dml_list_elem(TSqlParser::Output_dml_list_elemContext * /*ctx*/) override { }

  virtual void enterOutput_column_name(TSqlParser::Output_column_nameContext * /*ctx*/) override { }
  virtual void exitOutput_column_name(TSqlParser::Output_column_nameContext * /*ctx*/) override { }

  virtual void enterCreate_database(TSqlParser::Create_databaseContext * /*ctx*/) override { }
  virtual void exitCreate_database(TSqlParser::Create_databaseContext * /*ctx*/) override { }

  virtual void enterCreate_index(TSqlParser::Create_indexContext * /*ctx*/) override { }
  virtual void exitCreate_index(TSqlParser::Create_indexContext * /*ctx*/) override { }

  virtual void enterCreate_or_alter_procedure(TSqlParser::Create_or_alter_procedureContext * /*ctx*/) override { }
  virtual void exitCreate_or_alter_procedure(TSqlParser::Create_or_alter_procedureContext * /*ctx*/) override { }

  virtual void enterCreate_or_alter_function(TSqlParser::Create_or_alter_functionContext * /*ctx*/) override { }
  virtual void exitCreate_or_alter_function(TSqlParser::Create_or_alter_functionContext * /*ctx*/) override { }

  virtual void enterFunc_body_returns_select(TSqlParser::Func_body_returns_selectContext * /*ctx*/) override { }
  virtual void exitFunc_body_returns_select(TSqlParser::Func_body_returns_selectContext * /*ctx*/) override { }

  virtual void enterFunc_body_returns_table(TSqlParser::Func_body_returns_tableContext * /*ctx*/) override { }
  virtual void exitFunc_body_returns_table(TSqlParser::Func_body_returns_tableContext * /*ctx*/) override { }

  virtual void enterFunc_body_returns_scalar(TSqlParser::Func_body_returns_scalarContext * /*ctx*/) override { }
  virtual void exitFunc_body_returns_scalar(TSqlParser::Func_body_returns_scalarContext * /*ctx*/) override { }

  virtual void enterProcedure_param(TSqlParser::Procedure_paramContext * /*ctx*/) override { }
  virtual void exitProcedure_param(TSqlParser::Procedure_paramContext * /*ctx*/) override { }

  virtual void enterProcedure_option(TSqlParser::Procedure_optionContext * /*ctx*/) override { }
  virtual void exitProcedure_option(TSqlParser::Procedure_optionContext * /*ctx*/) override { }

  virtual void enterFunction_option(TSqlParser::Function_optionContext * /*ctx*/) override { }
  virtual void exitFunction_option(TSqlParser::Function_optionContext * /*ctx*/) override { }

  virtual void enterCreate_table(TSqlParser::Create_tableContext * /*ctx*/) override { }
  virtual void exitCreate_table(TSqlParser::Create_tableContext * /*ctx*/) override { }

  virtual void enterTable_options(TSqlParser::Table_optionsContext * /*ctx*/) override { }
  virtual void exitTable_options(TSqlParser::Table_optionsContext * /*ctx*/) override { }

  virtual void enterCreate_view(TSqlParser::Create_viewContext * /*ctx*/) override { }
  virtual void exitCreate_view(TSqlParser::Create_viewContext * /*ctx*/) override { }

  virtual void enterView_attribute(TSqlParser::View_attributeContext * /*ctx*/) override { }
  virtual void exitView_attribute(TSqlParser::View_attributeContext * /*ctx*/) override { }

  virtual void enterAlter_table(TSqlParser::Alter_tableContext * /*ctx*/) override { }
  virtual void exitAlter_table(TSqlParser::Alter_tableContext * /*ctx*/) override { }

  virtual void enterCursor_option(TSqlParser::Cursor_optionContext * /*ctx*/) override { }
  virtual void exitCursor_option(TSqlParser::Cursor_optionContext * /*ctx*/) override { }

  virtual void enterTarget_recovery_time_option(TSqlParser::Target_recovery_time_optionContext * /*ctx*/) override { }
  virtual void exitTarget_recovery_time_option(TSqlParser::Target_recovery_time_optionContext * /*ctx*/) override { }

  virtual void enterTermination(TSqlParser::TerminationContext * /*ctx*/) override { }
  virtual void exitTermination(TSqlParser::TerminationContext * /*ctx*/) override { }

  virtual void enterDrop_index(TSqlParser::Drop_indexContext * /*ctx*/) override { }
  virtual void exitDrop_index(TSqlParser::Drop_indexContext * /*ctx*/) override { }

  virtual void enterDrop_relational_or_xml_or_spatial_index(TSqlParser::Drop_relational_or_xml_or_spatial_indexContext * /*ctx*/) override { }
  virtual void exitDrop_relational_or_xml_or_spatial_index(TSqlParser::Drop_relational_or_xml_or_spatial_indexContext * /*ctx*/) override { }

  virtual void enterDrop_backward_compatible_index(TSqlParser::Drop_backward_compatible_indexContext * /*ctx*/) override { }
  virtual void exitDrop_backward_compatible_index(TSqlParser::Drop_backward_compatible_indexContext * /*ctx*/) override { }

  virtual void enterDrop_procedure(TSqlParser::Drop_procedureContext * /*ctx*/) override { }
  virtual void exitDrop_procedure(TSqlParser::Drop_procedureContext * /*ctx*/) override { }

  virtual void enterDrop_function(TSqlParser::Drop_functionContext * /*ctx*/) override { }
  virtual void exitDrop_function(TSqlParser::Drop_functionContext * /*ctx*/) override { }

  virtual void enterDrop_table(TSqlParser::Drop_tableContext * /*ctx*/) override { }
  virtual void exitDrop_table(TSqlParser::Drop_tableContext * /*ctx*/) override { }

  virtual void enterDrop_view(TSqlParser::Drop_viewContext * /*ctx*/) override { }
  virtual void exitDrop_view(TSqlParser::Drop_viewContext * /*ctx*/) override { }

  virtual void enterCreate_type(TSqlParser::Create_typeContext * /*ctx*/) override { }
  virtual void exitCreate_type(TSqlParser::Create_typeContext * /*ctx*/) override { }

  virtual void enterDrop_type(TSqlParser::Drop_typeContext * /*ctx*/) override { }
  virtual void exitDrop_type(TSqlParser::Drop_typeContext * /*ctx*/) override { }

  virtual void enterRowset_function_limited(TSqlParser::Rowset_function_limitedContext * /*ctx*/) override { }
  virtual void exitRowset_function_limited(TSqlParser::Rowset_function_limitedContext * /*ctx*/) override { }

  virtual void enterOpenquery(TSqlParser::OpenqueryContext * /*ctx*/) override { }
  virtual void exitOpenquery(TSqlParser::OpenqueryContext * /*ctx*/) override { }

  virtual void enterOpendatasource(TSqlParser::OpendatasourceContext * /*ctx*/) override { }
  virtual void exitOpendatasource(TSqlParser::OpendatasourceContext * /*ctx*/) override { }

  virtual void enterDeclare_statement(TSqlParser::Declare_statementContext * /*ctx*/) override { }
  virtual void exitDeclare_statement(TSqlParser::Declare_statementContext * /*ctx*/) override { }

  virtual void enterCursor_statement(TSqlParser::Cursor_statementContext * /*ctx*/) override { }
  virtual void exitCursor_statement(TSqlParser::Cursor_statementContext * /*ctx*/) override { }

  virtual void enterKill_statement(TSqlParser::Kill_statementContext * /*ctx*/) override { }
  virtual void exitKill_statement(TSqlParser::Kill_statementContext * /*ctx*/) override { }

  virtual void enterKill_process(TSqlParser::Kill_processContext * /*ctx*/) override { }
  virtual void exitKill_process(TSqlParser::Kill_processContext * /*ctx*/) override { }

  virtual void enterKill_query_notification(TSqlParser::Kill_query_notificationContext * /*ctx*/) override { }
  virtual void exitKill_query_notification(TSqlParser::Kill_query_notificationContext * /*ctx*/) override { }

  virtual void enterKill_stats_job(TSqlParser::Kill_stats_jobContext * /*ctx*/) override { }
  virtual void exitKill_stats_job(TSqlParser::Kill_stats_jobContext * /*ctx*/) override { }

  virtual void enterExecute_statement(TSqlParser::Execute_statementContext * /*ctx*/) override { }
  virtual void exitExecute_statement(TSqlParser::Execute_statementContext * /*ctx*/) override { }

  virtual void enterExecute_body(TSqlParser::Execute_bodyContext * /*ctx*/) override { }
  virtual void exitExecute_body(TSqlParser::Execute_bodyContext * /*ctx*/) override { }

  virtual void enterExecute_statement_arg(TSqlParser::Execute_statement_argContext * /*ctx*/) override { }
  virtual void exitExecute_statement_arg(TSqlParser::Execute_statement_argContext * /*ctx*/) override { }

  virtual void enterExecute_var_string(TSqlParser::Execute_var_stringContext * /*ctx*/) override { }
  virtual void exitExecute_var_string(TSqlParser::Execute_var_stringContext * /*ctx*/) override { }

  virtual void enterSet_statement(TSqlParser::Set_statementContext * /*ctx*/) override { }
  virtual void exitSet_statement(TSqlParser::Set_statementContext * /*ctx*/) override { }

  virtual void enterTransaction_statement(TSqlParser::Transaction_statementContext * /*ctx*/) override { }
  virtual void exitTransaction_statement(TSqlParser::Transaction_statementContext * /*ctx*/) override { }

  virtual void enterGo_statement(TSqlParser::Go_statementContext * /*ctx*/) override { }
  virtual void exitGo_statement(TSqlParser::Go_statementContext * /*ctx*/) override { }

  virtual void enterUse_statement(TSqlParser::Use_statementContext * /*ctx*/) override { }
  virtual void exitUse_statement(TSqlParser::Use_statementContext * /*ctx*/) override { }

  virtual void enterSetuser_statement(TSqlParser::Setuser_statementContext * /*ctx*/) override { }
  virtual void exitSetuser_statement(TSqlParser::Setuser_statementContext * /*ctx*/) override { }

  virtual void enterReconfigure_statement(TSqlParser::Reconfigure_statementContext * /*ctx*/) override { }
  virtual void exitReconfigure_statement(TSqlParser::Reconfigure_statementContext * /*ctx*/) override { }

  virtual void enterShutdown_statement(TSqlParser::Shutdown_statementContext * /*ctx*/) override { }
  virtual void exitShutdown_statement(TSqlParser::Shutdown_statementContext * /*ctx*/) override { }

  virtual void enterDbcc_clause(TSqlParser::Dbcc_clauseContext * /*ctx*/) override { }
  virtual void exitDbcc_clause(TSqlParser::Dbcc_clauseContext * /*ctx*/) override { }

  virtual void enterDbcc_options(TSqlParser::Dbcc_optionsContext * /*ctx*/) override { }
  virtual void exitDbcc_options(TSqlParser::Dbcc_optionsContext * /*ctx*/) override { }

  virtual void enterExecute_clause(TSqlParser::Execute_clauseContext * /*ctx*/) override { }
  virtual void exitExecute_clause(TSqlParser::Execute_clauseContext * /*ctx*/) override { }

  virtual void enterDeclare_local(TSqlParser::Declare_localContext * /*ctx*/) override { }
  virtual void exitDeclare_local(TSqlParser::Declare_localContext * /*ctx*/) override { }

  virtual void enterTable_type_definition(TSqlParser::Table_type_definitionContext * /*ctx*/) override { }
  virtual void exitTable_type_definition(TSqlParser::Table_type_definitionContext * /*ctx*/) override { }

  virtual void enterXml_type_definition(TSqlParser::Xml_type_definitionContext * /*ctx*/) override { }
  virtual void exitXml_type_definition(TSqlParser::Xml_type_definitionContext * /*ctx*/) override { }

  virtual void enterXml_schema_collection(TSqlParser::Xml_schema_collectionContext * /*ctx*/) override { }
  virtual void exitXml_schema_collection(TSqlParser::Xml_schema_collectionContext * /*ctx*/) override { }

  virtual void enterColumn_def_table_constraints(TSqlParser::Column_def_table_constraintsContext * /*ctx*/) override { }
  virtual void exitColumn_def_table_constraints(TSqlParser::Column_def_table_constraintsContext * /*ctx*/) override { }

  virtual void enterColumn_def_table_constraint(TSqlParser::Column_def_table_constraintContext * /*ctx*/) override { }
  virtual void exitColumn_def_table_constraint(TSqlParser::Column_def_table_constraintContext * /*ctx*/) override { }

  virtual void enterColumn_definition(TSqlParser::Column_definitionContext * /*ctx*/) override { }
  virtual void exitColumn_definition(TSqlParser::Column_definitionContext * /*ctx*/) override { }

  virtual void enterMaterialized_column_definition(TSqlParser::Materialized_column_definitionContext * /*ctx*/) override { }
  virtual void exitMaterialized_column_definition(TSqlParser::Materialized_column_definitionContext * /*ctx*/) override { }

  virtual void enterColumn_constraint(TSqlParser::Column_constraintContext * /*ctx*/) override { }
  virtual void exitColumn_constraint(TSqlParser::Column_constraintContext * /*ctx*/) override { }

  virtual void enterTable_constraint(TSqlParser::Table_constraintContext * /*ctx*/) override { }
  virtual void exitTable_constraint(TSqlParser::Table_constraintContext * /*ctx*/) override { }

  virtual void enterOn_delete(TSqlParser::On_deleteContext * /*ctx*/) override { }
  virtual void exitOn_delete(TSqlParser::On_deleteContext * /*ctx*/) override { }

  virtual void enterOn_update(TSqlParser::On_updateContext * /*ctx*/) override { }
  virtual void exitOn_update(TSqlParser::On_updateContext * /*ctx*/) override { }

  virtual void enterIndex_options(TSqlParser::Index_optionsContext * /*ctx*/) override { }
  virtual void exitIndex_options(TSqlParser::Index_optionsContext * /*ctx*/) override { }

  virtual void enterIndex_option(TSqlParser::Index_optionContext * /*ctx*/) override { }
  virtual void exitIndex_option(TSqlParser::Index_optionContext * /*ctx*/) override { }

  virtual void enterDeclare_cursor(TSqlParser::Declare_cursorContext * /*ctx*/) override { }
  virtual void exitDeclare_cursor(TSqlParser::Declare_cursorContext * /*ctx*/) override { }

  virtual void enterDeclare_set_cursor_common(TSqlParser::Declare_set_cursor_commonContext * /*ctx*/) override { }
  virtual void exitDeclare_set_cursor_common(TSqlParser::Declare_set_cursor_commonContext * /*ctx*/) override { }

  virtual void enterDeclare_set_cursor_common_partial(TSqlParser::Declare_set_cursor_common_partialContext * /*ctx*/) override { }
  virtual void exitDeclare_set_cursor_common_partial(TSqlParser::Declare_set_cursor_common_partialContext * /*ctx*/) override { }

  virtual void enterFetch_cursor(TSqlParser::Fetch_cursorContext * /*ctx*/) override { }
  virtual void exitFetch_cursor(TSqlParser::Fetch_cursorContext * /*ctx*/) override { }

  virtual void enterSet_special(TSqlParser::Set_specialContext * /*ctx*/) override { }
  virtual void exitSet_special(TSqlParser::Set_specialContext * /*ctx*/) override { }

  virtual void enterConstant_LOCAL_ID(TSqlParser::Constant_LOCAL_IDContext * /*ctx*/) override { }
  virtual void exitConstant_LOCAL_ID(TSqlParser::Constant_LOCAL_IDContext * /*ctx*/) override { }

  virtual void enterExpression(TSqlParser::ExpressionContext * /*ctx*/) override { }
  virtual void exitExpression(TSqlParser::ExpressionContext * /*ctx*/) override { }

  virtual void enterPrimitive_expression(TSqlParser::Primitive_expressionContext * /*ctx*/) override { }
  virtual void exitPrimitive_expression(TSqlParser::Primitive_expressionContext * /*ctx*/) override { }

  virtual void enterCase_expression(TSqlParser::Case_expressionContext * /*ctx*/) override { }
  virtual void exitCase_expression(TSqlParser::Case_expressionContext * /*ctx*/) override { }

  virtual void enterUnary_operator_expression(TSqlParser::Unary_operator_expressionContext * /*ctx*/) override { }
  virtual void exitUnary_operator_expression(TSqlParser::Unary_operator_expressionContext * /*ctx*/) override { }

  virtual void enterBracket_expression(TSqlParser::Bracket_expressionContext * /*ctx*/) override { }
  virtual void exitBracket_expression(TSqlParser::Bracket_expressionContext * /*ctx*/) override { }

  virtual void enterConstant_expression(TSqlParser::Constant_expressionContext * /*ctx*/) override { }
  virtual void exitConstant_expression(TSqlParser::Constant_expressionContext * /*ctx*/) override { }

  virtual void enterSubquery(TSqlParser::SubqueryContext * /*ctx*/) override { }
  virtual void exitSubquery(TSqlParser::SubqueryContext * /*ctx*/) override { }

  virtual void enterWith_expression(TSqlParser::With_expressionContext * /*ctx*/) override { }
  virtual void exitWith_expression(TSqlParser::With_expressionContext * /*ctx*/) override { }

  virtual void enterCommon_table_expression(TSqlParser::Common_table_expressionContext * /*ctx*/) override { }
  virtual void exitCommon_table_expression(TSqlParser::Common_table_expressionContext * /*ctx*/) override { }

  virtual void enterUpdate_elem(TSqlParser::Update_elemContext * /*ctx*/) override { }
  virtual void exitUpdate_elem(TSqlParser::Update_elemContext * /*ctx*/) override { }

  virtual void enterSearch_condition_list(TSqlParser::Search_condition_listContext * /*ctx*/) override { }
  virtual void exitSearch_condition_list(TSqlParser::Search_condition_listContext * /*ctx*/) override { }

  virtual void enterSearch_condition(TSqlParser::Search_conditionContext * /*ctx*/) override { }
  virtual void exitSearch_condition(TSqlParser::Search_conditionContext * /*ctx*/) override { }

  virtual void enterSearch_condition_and(TSqlParser::Search_condition_andContext * /*ctx*/) override { }
  virtual void exitSearch_condition_and(TSqlParser::Search_condition_andContext * /*ctx*/) override { }

  virtual void enterSearch_condition_not(TSqlParser::Search_condition_notContext * /*ctx*/) override { }
  virtual void exitSearch_condition_not(TSqlParser::Search_condition_notContext * /*ctx*/) override { }

  virtual void enterPredicate(TSqlParser::PredicateContext * /*ctx*/) override { }
  virtual void exitPredicate(TSqlParser::PredicateContext * /*ctx*/) override { }

  virtual void enterQuery_expression(TSqlParser::Query_expressionContext * /*ctx*/) override { }
  virtual void exitQuery_expression(TSqlParser::Query_expressionContext * /*ctx*/) override { }

  virtual void enterSql_union(TSqlParser::Sql_unionContext * /*ctx*/) override { }
  virtual void exitSql_union(TSqlParser::Sql_unionContext * /*ctx*/) override { }

  virtual void enterQuery_specification(TSqlParser::Query_specificationContext * /*ctx*/) override { }
  virtual void exitQuery_specification(TSqlParser::Query_specificationContext * /*ctx*/) override { }

  virtual void enterTop_clause(TSqlParser::Top_clauseContext * /*ctx*/) override { }
  virtual void exitTop_clause(TSqlParser::Top_clauseContext * /*ctx*/) override { }

  virtual void enterTop_percent(TSqlParser::Top_percentContext * /*ctx*/) override { }
  virtual void exitTop_percent(TSqlParser::Top_percentContext * /*ctx*/) override { }

  virtual void enterTop_count(TSqlParser::Top_countContext * /*ctx*/) override { }
  virtual void exitTop_count(TSqlParser::Top_countContext * /*ctx*/) override { }

  virtual void enterOrder_by_clause(TSqlParser::Order_by_clauseContext * /*ctx*/) override { }
  virtual void exitOrder_by_clause(TSqlParser::Order_by_clauseContext * /*ctx*/) override { }

  virtual void enterFor_clause(TSqlParser::For_clauseContext * /*ctx*/) override { }
  virtual void exitFor_clause(TSqlParser::For_clauseContext * /*ctx*/) override { }

  virtual void enterXml_common_directives(TSqlParser::Xml_common_directivesContext * /*ctx*/) override { }
  virtual void exitXml_common_directives(TSqlParser::Xml_common_directivesContext * /*ctx*/) override { }

  virtual void enterOrder_by_expression(TSqlParser::Order_by_expressionContext * /*ctx*/) override { }
  virtual void exitOrder_by_expression(TSqlParser::Order_by_expressionContext * /*ctx*/) override { }

  virtual void enterGroup_by_item(TSqlParser::Group_by_itemContext * /*ctx*/) override { }
  virtual void exitGroup_by_item(TSqlParser::Group_by_itemContext * /*ctx*/) override { }

  virtual void enterOption_clause(TSqlParser::Option_clauseContext * /*ctx*/) override { }
  virtual void exitOption_clause(TSqlParser::Option_clauseContext * /*ctx*/) override { }

  virtual void enterOption(TSqlParser::OptionContext * /*ctx*/) override { }
  virtual void exitOption(TSqlParser::OptionContext * /*ctx*/) override { }

  virtual void enterOptimize_for_arg(TSqlParser::Optimize_for_argContext * /*ctx*/) override { }
  virtual void exitOptimize_for_arg(TSqlParser::Optimize_for_argContext * /*ctx*/) override { }

  virtual void enterSelect_list(TSqlParser::Select_listContext * /*ctx*/) override { }
  virtual void exitSelect_list(TSqlParser::Select_listContext * /*ctx*/) override { }

  virtual void enterUdt_method_arguments(TSqlParser::Udt_method_argumentsContext * /*ctx*/) override { }
  virtual void exitUdt_method_arguments(TSqlParser::Udt_method_argumentsContext * /*ctx*/) override { }

  virtual void enterAsterisk(TSqlParser::AsteriskContext * /*ctx*/) override { }
  virtual void exitAsterisk(TSqlParser::AsteriskContext * /*ctx*/) override { }

  virtual void enterColumn_elem(TSqlParser::Column_elemContext * /*ctx*/) override { }
  virtual void exitColumn_elem(TSqlParser::Column_elemContext * /*ctx*/) override { }

  virtual void enterUdt_elem(TSqlParser::Udt_elemContext * /*ctx*/) override { }
  virtual void exitUdt_elem(TSqlParser::Udt_elemContext * /*ctx*/) override { }

  virtual void enterExpression_elem(TSqlParser::Expression_elemContext * /*ctx*/) override { }
  virtual void exitExpression_elem(TSqlParser::Expression_elemContext * /*ctx*/) override { }

  virtual void enterSelect_list_elem(TSqlParser::Select_list_elemContext * /*ctx*/) override { }
  virtual void exitSelect_list_elem(TSqlParser::Select_list_elemContext * /*ctx*/) override { }

  virtual void enterTable_sources(TSqlParser::Table_sourcesContext * /*ctx*/) override { }
  virtual void exitTable_sources(TSqlParser::Table_sourcesContext * /*ctx*/) override { }

  virtual void enterTable_source(TSqlParser::Table_sourceContext * /*ctx*/) override { }
  virtual void exitTable_source(TSqlParser::Table_sourceContext * /*ctx*/) override { }

  virtual void enterTable_source_item_joined(TSqlParser::Table_source_item_joinedContext * /*ctx*/) override { }
  virtual void exitTable_source_item_joined(TSqlParser::Table_source_item_joinedContext * /*ctx*/) override { }

  virtual void enterTable_source_item(TSqlParser::Table_source_itemContext * /*ctx*/) override { }
  virtual void exitTable_source_item(TSqlParser::Table_source_itemContext * /*ctx*/) override { }

  virtual void enterOpen_xml(TSqlParser::Open_xmlContext * /*ctx*/) override { }
  virtual void exitOpen_xml(TSqlParser::Open_xmlContext * /*ctx*/) override { }

  virtual void enterSchema_declaration(TSqlParser::Schema_declarationContext * /*ctx*/) override { }
  virtual void exitSchema_declaration(TSqlParser::Schema_declarationContext * /*ctx*/) override { }

  virtual void enterColumn_declaration(TSqlParser::Column_declarationContext * /*ctx*/) override { }
  virtual void exitColumn_declaration(TSqlParser::Column_declarationContext * /*ctx*/) override { }

  virtual void enterChange_table(TSqlParser::Change_tableContext * /*ctx*/) override { }
  virtual void exitChange_table(TSqlParser::Change_tableContext * /*ctx*/) override { }

  virtual void enterJoin_part(TSqlParser::Join_partContext * /*ctx*/) override { }
  virtual void exitJoin_part(TSqlParser::Join_partContext * /*ctx*/) override { }

  virtual void enterPivot_clause(TSqlParser::Pivot_clauseContext * /*ctx*/) override { }
  virtual void exitPivot_clause(TSqlParser::Pivot_clauseContext * /*ctx*/) override { }

  virtual void enterUnpivot_clause(TSqlParser::Unpivot_clauseContext * /*ctx*/) override { }
  virtual void exitUnpivot_clause(TSqlParser::Unpivot_clauseContext * /*ctx*/) override { }

  virtual void enterFull_column_name_list(TSqlParser::Full_column_name_listContext * /*ctx*/) override { }
  virtual void exitFull_column_name_list(TSqlParser::Full_column_name_listContext * /*ctx*/) override { }

  virtual void enterTable_name_with_hint(TSqlParser::Table_name_with_hintContext * /*ctx*/) override { }
  virtual void exitTable_name_with_hint(TSqlParser::Table_name_with_hintContext * /*ctx*/) override { }

  virtual void enterRowset_function(TSqlParser::Rowset_functionContext * /*ctx*/) override { }
  virtual void exitRowset_function(TSqlParser::Rowset_functionContext * /*ctx*/) override { }

  virtual void enterBulk_option(TSqlParser::Bulk_optionContext * /*ctx*/) override { }
  virtual void exitBulk_option(TSqlParser::Bulk_optionContext * /*ctx*/) override { }

  virtual void enterDerived_table(TSqlParser::Derived_tableContext * /*ctx*/) override { }
  virtual void exitDerived_table(TSqlParser::Derived_tableContext * /*ctx*/) override { }

  virtual void enterBINARY_CHECKSUM(TSqlParser::BINARY_CHECKSUMContext * /*ctx*/) override { }
  virtual void exitBINARY_CHECKSUM(TSqlParser::BINARY_CHECKSUMContext * /*ctx*/) override { }

  virtual void enterCAST(TSqlParser::CASTContext * /*ctx*/) override { }
  virtual void exitCAST(TSqlParser::CASTContext * /*ctx*/) override { }

  virtual void enterCONVERT(TSqlParser::CONVERTContext * /*ctx*/) override { }
  virtual void exitCONVERT(TSqlParser::CONVERTContext * /*ctx*/) override { }

  virtual void enterCHECKSUM(TSqlParser::CHECKSUMContext * /*ctx*/) override { }
  virtual void exitCHECKSUM(TSqlParser::CHECKSUMContext * /*ctx*/) override { }

  virtual void enterCOALESCE(TSqlParser::COALESCEContext * /*ctx*/) override { }
  virtual void exitCOALESCE(TSqlParser::COALESCEContext * /*ctx*/) override { }

  virtual void enterCURRENT_TIMESTAMP(TSqlParser::CURRENT_TIMESTAMPContext * /*ctx*/) override { }
  virtual void exitCURRENT_TIMESTAMP(TSqlParser::CURRENT_TIMESTAMPContext * /*ctx*/) override { }

  virtual void enterCURRENT_USER(TSqlParser::CURRENT_USERContext * /*ctx*/) override { }
  virtual void exitCURRENT_USER(TSqlParser::CURRENT_USERContext * /*ctx*/) override { }

  virtual void enterDATEADD(TSqlParser::DATEADDContext * /*ctx*/) override { }
  virtual void exitDATEADD(TSqlParser::DATEADDContext * /*ctx*/) override { }

  virtual void enterDATEDIFF(TSqlParser::DATEDIFFContext * /*ctx*/) override { }
  virtual void exitDATEDIFF(TSqlParser::DATEDIFFContext * /*ctx*/) override { }

  virtual void enterDATENAME(TSqlParser::DATENAMEContext * /*ctx*/) override { }
  virtual void exitDATENAME(TSqlParser::DATENAMEContext * /*ctx*/) override { }

  virtual void enterDATEPART(TSqlParser::DATEPARTContext * /*ctx*/) override { }
  virtual void exitDATEPART(TSqlParser::DATEPARTContext * /*ctx*/) override { }

  virtual void enterGETDATE(TSqlParser::GETDATEContext * /*ctx*/) override { }
  virtual void exitGETDATE(TSqlParser::GETDATEContext * /*ctx*/) override { }

  virtual void enterGETUTCDATE(TSqlParser::GETUTCDATEContext * /*ctx*/) override { }
  virtual void exitGETUTCDATE(TSqlParser::GETUTCDATEContext * /*ctx*/) override { }

  virtual void enterIDENTITY(TSqlParser::IDENTITYContext * /*ctx*/) override { }
  virtual void exitIDENTITY(TSqlParser::IDENTITYContext * /*ctx*/) override { }

  virtual void enterMIN_ACTIVE_ROWVERSION(TSqlParser::MIN_ACTIVE_ROWVERSIONContext * /*ctx*/) override { }
  virtual void exitMIN_ACTIVE_ROWVERSION(TSqlParser::MIN_ACTIVE_ROWVERSIONContext * /*ctx*/) override { }

  virtual void enterNULLIF(TSqlParser::NULLIFContext * /*ctx*/) override { }
  virtual void exitNULLIF(TSqlParser::NULLIFContext * /*ctx*/) override { }

  virtual void enterSTUFF(TSqlParser::STUFFContext * /*ctx*/) override { }
  virtual void exitSTUFF(TSqlParser::STUFFContext * /*ctx*/) override { }

  virtual void enterSESSION_USER(TSqlParser::SESSION_USERContext * /*ctx*/) override { }
  virtual void exitSESSION_USER(TSqlParser::SESSION_USERContext * /*ctx*/) override { }

  virtual void enterSYSTEM_USER(TSqlParser::SYSTEM_USERContext * /*ctx*/) override { }
  virtual void exitSYSTEM_USER(TSqlParser::SYSTEM_USERContext * /*ctx*/) override { }

  virtual void enterISNULL(TSqlParser::ISNULLContext * /*ctx*/) override { }
  virtual void exitISNULL(TSqlParser::ISNULLContext * /*ctx*/) override { }

  virtual void enterXML_DATA_TYPE_FUNC(TSqlParser::XML_DATA_TYPE_FUNCContext * /*ctx*/) override { }
  virtual void exitXML_DATA_TYPE_FUNC(TSqlParser::XML_DATA_TYPE_FUNCContext * /*ctx*/) override { }

  virtual void enterIFF(TSqlParser::IFFContext * /*ctx*/) override { }
  virtual void exitIFF(TSqlParser::IFFContext * /*ctx*/) override { }

  virtual void enterRANKING_WINDOWED_FUNC(TSqlParser::RANKING_WINDOWED_FUNCContext * /*ctx*/) override { }
  virtual void exitRANKING_WINDOWED_FUNC(TSqlParser::RANKING_WINDOWED_FUNCContext * /*ctx*/) override { }

  virtual void enterAGGREGATE_WINDOWED_FUNC(TSqlParser::AGGREGATE_WINDOWED_FUNCContext * /*ctx*/) override { }
  virtual void exitAGGREGATE_WINDOWED_FUNC(TSqlParser::AGGREGATE_WINDOWED_FUNCContext * /*ctx*/) override { }

  virtual void enterANALYTIC_WINDOWED_FUNC(TSqlParser::ANALYTIC_WINDOWED_FUNCContext * /*ctx*/) override { }
  virtual void exitANALYTIC_WINDOWED_FUNC(TSqlParser::ANALYTIC_WINDOWED_FUNCContext * /*ctx*/) override { }

  virtual void enterSCALAR_FUNCTION(TSqlParser::SCALAR_FUNCTIONContext * /*ctx*/) override { }
  virtual void exitSCALAR_FUNCTION(TSqlParser::SCALAR_FUNCTIONContext * /*ctx*/) override { }

  virtual void enterSTRINGAGG(TSqlParser::STRINGAGGContext * /*ctx*/) override { }
  virtual void exitSTRINGAGG(TSqlParser::STRINGAGGContext * /*ctx*/) override { }

  virtual void enterXml_data_type_methods(TSqlParser::Xml_data_type_methodsContext * /*ctx*/) override { }
  virtual void exitXml_data_type_methods(TSqlParser::Xml_data_type_methodsContext * /*ctx*/) override { }

  virtual void enterValue_method(TSqlParser::Value_methodContext * /*ctx*/) override { }
  virtual void exitValue_method(TSqlParser::Value_methodContext * /*ctx*/) override { }

  virtual void enterQuery_method(TSqlParser::Query_methodContext * /*ctx*/) override { }
  virtual void exitQuery_method(TSqlParser::Query_methodContext * /*ctx*/) override { }

  virtual void enterExist_method(TSqlParser::Exist_methodContext * /*ctx*/) override { }
  virtual void exitExist_method(TSqlParser::Exist_methodContext * /*ctx*/) override { }

  virtual void enterModify_method(TSqlParser::Modify_methodContext * /*ctx*/) override { }
  virtual void exitModify_method(TSqlParser::Modify_methodContext * /*ctx*/) override { }

  virtual void enterNodes_method(TSqlParser::Nodes_methodContext * /*ctx*/) override { }
  virtual void exitNodes_method(TSqlParser::Nodes_methodContext * /*ctx*/) override { }

  virtual void enterSwitch_section(TSqlParser::Switch_sectionContext * /*ctx*/) override { }
  virtual void exitSwitch_section(TSqlParser::Switch_sectionContext * /*ctx*/) override { }

  virtual void enterSwitch_search_condition_section(TSqlParser::Switch_search_condition_sectionContext * /*ctx*/) override { }
  virtual void exitSwitch_search_condition_section(TSqlParser::Switch_search_condition_sectionContext * /*ctx*/) override { }

  virtual void enterAs_column_alias(TSqlParser::As_column_aliasContext * /*ctx*/) override { }
  virtual void exitAs_column_alias(TSqlParser::As_column_aliasContext * /*ctx*/) override { }

  virtual void enterAs_table_alias(TSqlParser::As_table_aliasContext * /*ctx*/) override { }
  virtual void exitAs_table_alias(TSqlParser::As_table_aliasContext * /*ctx*/) override { }

  virtual void enterTable_alias(TSqlParser::Table_aliasContext * /*ctx*/) override { }
  virtual void exitTable_alias(TSqlParser::Table_aliasContext * /*ctx*/) override { }

  virtual void enterWith_table_hints(TSqlParser::With_table_hintsContext * /*ctx*/) override { }
  virtual void exitWith_table_hints(TSqlParser::With_table_hintsContext * /*ctx*/) override { }

  virtual void enterInsert_with_table_hints(TSqlParser::Insert_with_table_hintsContext * /*ctx*/) override { }
  virtual void exitInsert_with_table_hints(TSqlParser::Insert_with_table_hintsContext * /*ctx*/) override { }

  virtual void enterTable_hint(TSqlParser::Table_hintContext * /*ctx*/) override { }
  virtual void exitTable_hint(TSqlParser::Table_hintContext * /*ctx*/) override { }

  virtual void enterIndex_value(TSqlParser::Index_valueContext * /*ctx*/) override { }
  virtual void exitIndex_value(TSqlParser::Index_valueContext * /*ctx*/) override { }

  virtual void enterColumn_alias_list(TSqlParser::Column_alias_listContext * /*ctx*/) override { }
  virtual void exitColumn_alias_list(TSqlParser::Column_alias_listContext * /*ctx*/) override { }

  virtual void enterColumn_alias(TSqlParser::Column_aliasContext * /*ctx*/) override { }
  virtual void exitColumn_alias(TSqlParser::Column_aliasContext * /*ctx*/) override { }

  virtual void enterTable_value_constructor(TSqlParser::Table_value_constructorContext * /*ctx*/) override { }
  virtual void exitTable_value_constructor(TSqlParser::Table_value_constructorContext * /*ctx*/) override { }

  virtual void enterExpression_list(TSqlParser::Expression_listContext * /*ctx*/) override { }
  virtual void exitExpression_list(TSqlParser::Expression_listContext * /*ctx*/) override { }

  virtual void enterRanking_windowed_function(TSqlParser::Ranking_windowed_functionContext * /*ctx*/) override { }
  virtual void exitRanking_windowed_function(TSqlParser::Ranking_windowed_functionContext * /*ctx*/) override { }

  virtual void enterAggregate_windowed_function(TSqlParser::Aggregate_windowed_functionContext * /*ctx*/) override { }
  virtual void exitAggregate_windowed_function(TSqlParser::Aggregate_windowed_functionContext * /*ctx*/) override { }

  virtual void enterAnalytic_windowed_function(TSqlParser::Analytic_windowed_functionContext * /*ctx*/) override { }
  virtual void exitAnalytic_windowed_function(TSqlParser::Analytic_windowed_functionContext * /*ctx*/) override { }

  virtual void enterAll_distinct_expression(TSqlParser::All_distinct_expressionContext * /*ctx*/) override { }
  virtual void exitAll_distinct_expression(TSqlParser::All_distinct_expressionContext * /*ctx*/) override { }

  virtual void enterOver_clause(TSqlParser::Over_clauseContext * /*ctx*/) override { }
  virtual void exitOver_clause(TSqlParser::Over_clauseContext * /*ctx*/) override { }

  virtual void enterRow_or_range_clause(TSqlParser::Row_or_range_clauseContext * /*ctx*/) override { }
  virtual void exitRow_or_range_clause(TSqlParser::Row_or_range_clauseContext * /*ctx*/) override { }

  virtual void enterWindow_frame_extent(TSqlParser::Window_frame_extentContext * /*ctx*/) override { }
  virtual void exitWindow_frame_extent(TSqlParser::Window_frame_extentContext * /*ctx*/) override { }

  virtual void enterWindow_frame_bound(TSqlParser::Window_frame_boundContext * /*ctx*/) override { }
  virtual void exitWindow_frame_bound(TSqlParser::Window_frame_boundContext * /*ctx*/) override { }

  virtual void enterWindow_frame_preceding(TSqlParser::Window_frame_precedingContext * /*ctx*/) override { }
  virtual void exitWindow_frame_preceding(TSqlParser::Window_frame_precedingContext * /*ctx*/) override { }

  virtual void enterWindow_frame_following(TSqlParser::Window_frame_followingContext * /*ctx*/) override { }
  virtual void exitWindow_frame_following(TSqlParser::Window_frame_followingContext * /*ctx*/) override { }

  virtual void enterCreate_database_option(TSqlParser::Create_database_optionContext * /*ctx*/) override { }
  virtual void exitCreate_database_option(TSqlParser::Create_database_optionContext * /*ctx*/) override { }

  virtual void enterDatabase_filestream_option(TSqlParser::Database_filestream_optionContext * /*ctx*/) override { }
  virtual void exitDatabase_filestream_option(TSqlParser::Database_filestream_optionContext * /*ctx*/) override { }

  virtual void enterDatabase_file_spec(TSqlParser::Database_file_specContext * /*ctx*/) override { }
  virtual void exitDatabase_file_spec(TSqlParser::Database_file_specContext * /*ctx*/) override { }

  virtual void enterFile_group(TSqlParser::File_groupContext * /*ctx*/) override { }
  virtual void exitFile_group(TSqlParser::File_groupContext * /*ctx*/) override { }

  virtual void enterFile_spec(TSqlParser::File_specContext * /*ctx*/) override { }
  virtual void exitFile_spec(TSqlParser::File_specContext * /*ctx*/) override { }

  virtual void enterEntity_name(TSqlParser::Entity_nameContext * /*ctx*/) override { }
  virtual void exitEntity_name(TSqlParser::Entity_nameContext * /*ctx*/) override { }

  virtual void enterEntity_name_for_azure_dw(TSqlParser::Entity_name_for_azure_dwContext * /*ctx*/) override { }
  virtual void exitEntity_name_for_azure_dw(TSqlParser::Entity_name_for_azure_dwContext * /*ctx*/) override { }

  virtual void enterEntity_name_for_parallel_dw(TSqlParser::Entity_name_for_parallel_dwContext * /*ctx*/) override { }
  virtual void exitEntity_name_for_parallel_dw(TSqlParser::Entity_name_for_parallel_dwContext * /*ctx*/) override { }

  virtual void enterFull_table_name(TSqlParser::Full_table_nameContext * /*ctx*/) override { }
  virtual void exitFull_table_name(TSqlParser::Full_table_nameContext * /*ctx*/) override { }

  virtual void enterTable_name(TSqlParser::Table_nameContext * /*ctx*/) override { }
  virtual void exitTable_name(TSqlParser::Table_nameContext * /*ctx*/) override { }

  virtual void enterSimple_name(TSqlParser::Simple_nameContext * /*ctx*/) override { }
  virtual void exitSimple_name(TSqlParser::Simple_nameContext * /*ctx*/) override { }

  virtual void enterFunc_proc_name_schema(TSqlParser::Func_proc_name_schemaContext * /*ctx*/) override { }
  virtual void exitFunc_proc_name_schema(TSqlParser::Func_proc_name_schemaContext * /*ctx*/) override { }

  virtual void enterFunc_proc_name_database_schema(TSqlParser::Func_proc_name_database_schemaContext * /*ctx*/) override { }
  virtual void exitFunc_proc_name_database_schema(TSqlParser::Func_proc_name_database_schemaContext * /*ctx*/) override { }

  virtual void enterFunc_proc_name_server_database_schema(TSqlParser::Func_proc_name_server_database_schemaContext * /*ctx*/) override { }
  virtual void exitFunc_proc_name_server_database_schema(TSqlParser::Func_proc_name_server_database_schemaContext * /*ctx*/) override { }

  virtual void enterDdl_object(TSqlParser::Ddl_objectContext * /*ctx*/) override { }
  virtual void exitDdl_object(TSqlParser::Ddl_objectContext * /*ctx*/) override { }

  virtual void enterFull_column_name(TSqlParser::Full_column_nameContext * /*ctx*/) override { }
  virtual void exitFull_column_name(TSqlParser::Full_column_nameContext * /*ctx*/) override { }

  virtual void enterColumn_name_list_with_order(TSqlParser::Column_name_list_with_orderContext * /*ctx*/) override { }
  virtual void exitColumn_name_list_with_order(TSqlParser::Column_name_list_with_orderContext * /*ctx*/) override { }

  virtual void enterColumn_name_list(TSqlParser::Column_name_listContext * /*ctx*/) override { }
  virtual void exitColumn_name_list(TSqlParser::Column_name_listContext * /*ctx*/) override { }

  virtual void enterCursor_name(TSqlParser::Cursor_nameContext * /*ctx*/) override { }
  virtual void exitCursor_name(TSqlParser::Cursor_nameContext * /*ctx*/) override { }

  virtual void enterOn_off(TSqlParser::On_offContext * /*ctx*/) override { }
  virtual void exitOn_off(TSqlParser::On_offContext * /*ctx*/) override { }

  virtual void enterClustered(TSqlParser::ClusteredContext * /*ctx*/) override { }
  virtual void exitClustered(TSqlParser::ClusteredContext * /*ctx*/) override { }

  virtual void enterNull_notnull(TSqlParser::Null_notnullContext * /*ctx*/) override { }
  virtual void exitNull_notnull(TSqlParser::Null_notnullContext * /*ctx*/) override { }

  virtual void enterNull_or_default(TSqlParser::Null_or_defaultContext * /*ctx*/) override { }
  virtual void exitNull_or_default(TSqlParser::Null_or_defaultContext * /*ctx*/) override { }

  virtual void enterScalar_function_name(TSqlParser::Scalar_function_nameContext * /*ctx*/) override { }
  virtual void exitScalar_function_name(TSqlParser::Scalar_function_nameContext * /*ctx*/) override { }

  virtual void enterBegin_conversation_timer(TSqlParser::Begin_conversation_timerContext * /*ctx*/) override { }
  virtual void exitBegin_conversation_timer(TSqlParser::Begin_conversation_timerContext * /*ctx*/) override { }

  virtual void enterBegin_conversation_dialog(TSqlParser::Begin_conversation_dialogContext * /*ctx*/) override { }
  virtual void exitBegin_conversation_dialog(TSqlParser::Begin_conversation_dialogContext * /*ctx*/) override { }

  virtual void enterContract_name(TSqlParser::Contract_nameContext * /*ctx*/) override { }
  virtual void exitContract_name(TSqlParser::Contract_nameContext * /*ctx*/) override { }

  virtual void enterService_name(TSqlParser::Service_nameContext * /*ctx*/) override { }
  virtual void exitService_name(TSqlParser::Service_nameContext * /*ctx*/) override { }

  virtual void enterEnd_conversation(TSqlParser::End_conversationContext * /*ctx*/) override { }
  virtual void exitEnd_conversation(TSqlParser::End_conversationContext * /*ctx*/) override { }

  virtual void enterWaitfor_conversation(TSqlParser::Waitfor_conversationContext * /*ctx*/) override { }
  virtual void exitWaitfor_conversation(TSqlParser::Waitfor_conversationContext * /*ctx*/) override { }

  virtual void enterGet_conversation(TSqlParser::Get_conversationContext * /*ctx*/) override { }
  virtual void exitGet_conversation(TSqlParser::Get_conversationContext * /*ctx*/) override { }

  virtual void enterQueue_id(TSqlParser::Queue_idContext * /*ctx*/) override { }
  virtual void exitQueue_id(TSqlParser::Queue_idContext * /*ctx*/) override { }

  virtual void enterSend_conversation(TSqlParser::Send_conversationContext * /*ctx*/) override { }
  virtual void exitSend_conversation(TSqlParser::Send_conversationContext * /*ctx*/) override { }

  virtual void enterData_type(TSqlParser::Data_typeContext * /*ctx*/) override { }
  virtual void exitData_type(TSqlParser::Data_typeContext * /*ctx*/) override { }

  virtual void enterDefault_value(TSqlParser::Default_valueContext * /*ctx*/) override { }
  virtual void exitDefault_value(TSqlParser::Default_valueContext * /*ctx*/) override { }

  virtual void enterConstant(TSqlParser::ConstantContext * /*ctx*/) override { }
  virtual void exitConstant(TSqlParser::ConstantContext * /*ctx*/) override { }

  virtual void enterSign(TSqlParser::SignContext * /*ctx*/) override { }
  virtual void exitSign(TSqlParser::SignContext * /*ctx*/) override { }

  virtual void enterId(TSqlParser::IdContext * /*ctx*/) override { }
  virtual void exitId(TSqlParser::IdContext * /*ctx*/) override { }

  virtual void enterSimple_id(TSqlParser::Simple_idContext * /*ctx*/) override { }
  virtual void exitSimple_id(TSqlParser::Simple_idContext * /*ctx*/) override { }

  virtual void enterComparison_operator(TSqlParser::Comparison_operatorContext * /*ctx*/) override { }
  virtual void exitComparison_operator(TSqlParser::Comparison_operatorContext * /*ctx*/) override { }

  virtual void enterAssignment_operator(TSqlParser::Assignment_operatorContext * /*ctx*/) override { }
  virtual void exitAssignment_operator(TSqlParser::Assignment_operatorContext * /*ctx*/) override { }

  virtual void enterFile_size(TSqlParser::File_sizeContext * /*ctx*/) override { }
  virtual void exitFile_size(TSqlParser::File_sizeContext * /*ctx*/) override { }


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { }
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};

