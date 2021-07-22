
// Generated from TSqlParser.g4 by ANTLR 4.7.2

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

  virtual void enterBatch_level_statement(TSqlParser::Batch_level_statementContext *ctx) = 0;
  virtual void exitBatch_level_statement(TSqlParser::Batch_level_statementContext *ctx) = 0;

  virtual void enterSql_clauses(TSqlParser::Sql_clausesContext *ctx) = 0;
  virtual void exitSql_clauses(TSqlParser::Sql_clausesContext *ctx) = 0;

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

  virtual void enterEntity_to(TSqlParser::Entity_toContext *ctx) = 0;
  virtual void exitEntity_to(TSqlParser::Entity_toContext *ctx) = 0;

  virtual void enterColon_colon(TSqlParser::Colon_colonContext *ctx) = 0;
  virtual void exitColon_colon(TSqlParser::Colon_colonContext *ctx) = 0;

  virtual void enterClass_type(TSqlParser::Class_typeContext *ctx) = 0;
  virtual void exitClass_type(TSqlParser::Class_typeContext *ctx) = 0;

  virtual void enterClass_type_for_sql_database(TSqlParser::Class_type_for_sql_databaseContext *ctx) = 0;
  virtual void exitClass_type_for_sql_database(TSqlParser::Class_type_for_sql_databaseContext *ctx) = 0;

  virtual void enterClass_type_for_azure_dw(TSqlParser::Class_type_for_azure_dwContext *ctx) = 0;
  virtual void exitClass_type_for_azure_dw(TSqlParser::Class_type_for_azure_dwContext *ctx) = 0;

  virtual void enterClass_type_for_parallel_dw(TSqlParser::Class_type_for_parallel_dwContext *ctx) = 0;
  virtual void exitClass_type_for_parallel_dw(TSqlParser::Class_type_for_parallel_dwContext *ctx) = 0;

  virtual void enterLock_table(TSqlParser::Lock_tableContext *ctx) = 0;
  virtual void exitLock_table(TSqlParser::Lock_tableContext *ctx) = 0;

  virtual void enterTruncate_table(TSqlParser::Truncate_tableContext *ctx) = 0;
  virtual void exitTruncate_table(TSqlParser::Truncate_tableContext *ctx) = 0;

  virtual void enterAlter_sequence(TSqlParser::Alter_sequenceContext *ctx) = 0;
  virtual void exitAlter_sequence(TSqlParser::Alter_sequenceContext *ctx) = 0;

  virtual void enterCreate_sequence(TSqlParser::Create_sequenceContext *ctx) = 0;
  virtual void exitCreate_sequence(TSqlParser::Create_sequenceContext *ctx) = 0;

  virtual void enterMerge_statement(TSqlParser::Merge_statementContext *ctx) = 0;
  virtual void exitMerge_statement(TSqlParser::Merge_statementContext *ctx) = 0;

  virtual void enterWhen_matches(TSqlParser::When_matchesContext *ctx) = 0;
  virtual void exitWhen_matches(TSqlParser::When_matchesContext *ctx) = 0;

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

  virtual void enterSelect_statement_standalone(TSqlParser::Select_statement_standaloneContext *ctx) = 0;
  virtual void exitSelect_statement_standalone(TSqlParser::Select_statement_standaloneContext *ctx) = 0;

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

  virtual void enterAlter_table(TSqlParser::Alter_tableContext *ctx) = 0;
  virtual void exitAlter_table(TSqlParser::Alter_tableContext *ctx) = 0;

  virtual void enterDatabase_optionspec(TSqlParser::Database_optionspecContext *ctx) = 0;
  virtual void exitDatabase_optionspec(TSqlParser::Database_optionspecContext *ctx) = 0;

  virtual void enterCursor_option(TSqlParser::Cursor_optionContext *ctx) = 0;
  virtual void exitCursor_option(TSqlParser::Cursor_optionContext *ctx) = 0;

  virtual void enterDrop_procedure(TSqlParser::Drop_procedureContext *ctx) = 0;
  virtual void exitDrop_procedure(TSqlParser::Drop_procedureContext *ctx) = 0;

  virtual void enterDrop_function(TSqlParser::Drop_functionContext *ctx) = 0;
  virtual void exitDrop_function(TSqlParser::Drop_functionContext *ctx) = 0;

  virtual void enterDrop_table(TSqlParser::Drop_tableContext *ctx) = 0;
  virtual void exitDrop_table(TSqlParser::Drop_tableContext *ctx) = 0;

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

  virtual void enterExecute_statement(TSqlParser::Execute_statementContext *ctx) = 0;
  virtual void exitExecute_statement(TSqlParser::Execute_statementContext *ctx) = 0;

  virtual void enterExecute_body_batch(TSqlParser::Execute_body_batchContext *ctx) = 0;
  virtual void exitExecute_body_batch(TSqlParser::Execute_body_batchContext *ctx) = 0;

  virtual void enterExecute_body(TSqlParser::Execute_bodyContext *ctx) = 0;
  virtual void exitExecute_body(TSqlParser::Execute_bodyContext *ctx) = 0;

  virtual void enterExecute_statement_arg(TSqlParser::Execute_statement_argContext *ctx) = 0;
  virtual void exitExecute_statement_arg(TSqlParser::Execute_statement_argContext *ctx) = 0;

  virtual void enterExecute_statement_arg_named(TSqlParser::Execute_statement_arg_namedContext *ctx) = 0;
  virtual void exitExecute_statement_arg_named(TSqlParser::Execute_statement_arg_namedContext *ctx) = 0;

  virtual void enterExecute_statement_arg_unnamed(TSqlParser::Execute_statement_arg_unnamedContext *ctx) = 0;
  virtual void exitExecute_statement_arg_unnamed(TSqlParser::Execute_statement_arg_unnamedContext *ctx) = 0;

  virtual void enterExecute_parameter(TSqlParser::Execute_parameterContext *ctx) = 0;
  virtual void exitExecute_parameter(TSqlParser::Execute_parameterContext *ctx) = 0;

  virtual void enterExecute_var_string(TSqlParser::Execute_var_stringContext *ctx) = 0;
  virtual void exitExecute_var_string(TSqlParser::Execute_var_stringContext *ctx) = 0;

  virtual void enterAlgorithm(TSqlParser::AlgorithmContext *ctx) = 0;
  virtual void exitAlgorithm(TSqlParser::AlgorithmContext *ctx) = 0;

  virtual void enterSet_statement(TSqlParser::Set_statementContext *ctx) = 0;
  virtual void exitSet_statement(TSqlParser::Set_statementContext *ctx) = 0;

  virtual void enterTransaction_statement(TSqlParser::Transaction_statementContext *ctx) = 0;
  virtual void exitTransaction_statement(TSqlParser::Transaction_statementContext *ctx) = 0;

  virtual void enterGo_batch_statement(TSqlParser::Go_batch_statementContext *ctx) = 0;
  virtual void exitGo_batch_statement(TSqlParser::Go_batch_statementContext *ctx) = 0;

  virtual void enterGo_statement(TSqlParser::Go_statementContext *ctx) = 0;
  virtual void exitGo_statement(TSqlParser::Go_statementContext *ctx) = 0;

  virtual void enterUse_statement(TSqlParser::Use_statementContext *ctx) = 0;
  virtual void exitUse_statement(TSqlParser::Use_statementContext *ctx) = 0;

  virtual void enterShutdown_statement(TSqlParser::Shutdown_statementContext *ctx) = 0;
  virtual void exitShutdown_statement(TSqlParser::Shutdown_statementContext *ctx) = 0;

  virtual void enterDbcc_special(TSqlParser::Dbcc_specialContext *ctx) = 0;
  virtual void exitDbcc_special(TSqlParser::Dbcc_specialContext *ctx) = 0;

  virtual void enterDbcc_clause(TSqlParser::Dbcc_clauseContext *ctx) = 0;
  virtual void exitDbcc_clause(TSqlParser::Dbcc_clauseContext *ctx) = 0;

  virtual void enterDbcc_command(TSqlParser::Dbcc_commandContext *ctx) = 0;
  virtual void exitDbcc_command(TSqlParser::Dbcc_commandContext *ctx) = 0;

  virtual void enterDbcc_options(TSqlParser::Dbcc_optionsContext *ctx) = 0;
  virtual void exitDbcc_options(TSqlParser::Dbcc_optionsContext *ctx) = 0;

  virtual void enterExecute_clause(TSqlParser::Execute_clauseContext *ctx) = 0;
  virtual void exitExecute_clause(TSqlParser::Execute_clauseContext *ctx) = 0;

  virtual void enterDeclare_local(TSqlParser::Declare_localContext *ctx) = 0;
  virtual void exitDeclare_local(TSqlParser::Declare_localContext *ctx) = 0;

  virtual void enterTable_type_definition(TSqlParser::Table_type_definitionContext *ctx) = 0;
  virtual void exitTable_type_definition(TSqlParser::Table_type_definitionContext *ctx) = 0;

  virtual void enterColumn_def_table_constraints(TSqlParser::Column_def_table_constraintsContext *ctx) = 0;
  virtual void exitColumn_def_table_constraints(TSqlParser::Column_def_table_constraintsContext *ctx) = 0;

  virtual void enterColumn_def_table_constraint(TSqlParser::Column_def_table_constraintContext *ctx) = 0;
  virtual void exitColumn_def_table_constraint(TSqlParser::Column_def_table_constraintContext *ctx) = 0;

  virtual void enterColumn_definition(TSqlParser::Column_definitionContext *ctx) = 0;
  virtual void exitColumn_definition(TSqlParser::Column_definitionContext *ctx) = 0;

  virtual void enterColumn_constraint(TSqlParser::Column_constraintContext *ctx) = 0;
  virtual void exitColumn_constraint(TSqlParser::Column_constraintContext *ctx) = 0;

  virtual void enterTable_constraint(TSqlParser::Table_constraintContext *ctx) = 0;
  virtual void exitTable_constraint(TSqlParser::Table_constraintContext *ctx) = 0;

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

  virtual void enterTime_zone(TSqlParser::Time_zoneContext *ctx) = 0;
  virtual void exitTime_zone(TSqlParser::Time_zoneContext *ctx) = 0;

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

  virtual void enterUpdate_elem_merge(TSqlParser::Update_elem_mergeContext *ctx) = 0;
  virtual void exitUpdate_elem_merge(TSqlParser::Update_elem_mergeContext *ctx) = 0;

  virtual void enterSearch_condition(TSqlParser::Search_conditionContext *ctx) = 0;
  virtual void exitSearch_condition(TSqlParser::Search_conditionContext *ctx) = 0;

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

  virtual void enterSchema_declaration(TSqlParser::Schema_declarationContext *ctx) = 0;
  virtual void exitSchema_declaration(TSqlParser::Schema_declarationContext *ctx) = 0;

  virtual void enterColumn_declaration(TSqlParser::Column_declarationContext *ctx) = 0;
  virtual void exitColumn_declaration(TSqlParser::Column_declarationContext *ctx) = 0;

  virtual void enterChange_table(TSqlParser::Change_tableContext *ctx) = 0;
  virtual void exitChange_table(TSqlParser::Change_tableContext *ctx) = 0;

  virtual void enterChange_table_changes(TSqlParser::Change_table_changesContext *ctx) = 0;
  virtual void exitChange_table_changes(TSqlParser::Change_table_changesContext *ctx) = 0;

  virtual void enterChange_table_version(TSqlParser::Change_table_versionContext *ctx) = 0;
  virtual void exitChange_table_version(TSqlParser::Change_table_versionContext *ctx) = 0;

  virtual void enterJoin_part(TSqlParser::Join_partContext *ctx) = 0;
  virtual void exitJoin_part(TSqlParser::Join_partContext *ctx) = 0;

  virtual void enterJoin_on(TSqlParser::Join_onContext *ctx) = 0;
  virtual void exitJoin_on(TSqlParser::Join_onContext *ctx) = 0;

  virtual void enterCross_join(TSqlParser::Cross_joinContext *ctx) = 0;
  virtual void exitCross_join(TSqlParser::Cross_joinContext *ctx) = 0;

  virtual void enterApply_(TSqlParser::Apply_Context *ctx) = 0;
  virtual void exitApply_(TSqlParser::Apply_Context *ctx) = 0;

  virtual void enterPivot(TSqlParser::PivotContext *ctx) = 0;
  virtual void exitPivot(TSqlParser::PivotContext *ctx) = 0;

  virtual void enterUnpivot(TSqlParser::UnpivotContext *ctx) = 0;
  virtual void exitUnpivot(TSqlParser::UnpivotContext *ctx) = 0;

  virtual void enterPivot_clause(TSqlParser::Pivot_clauseContext *ctx) = 0;
  virtual void exitPivot_clause(TSqlParser::Pivot_clauseContext *ctx) = 0;

  virtual void enterUnpivot_clause(TSqlParser::Unpivot_clauseContext *ctx) = 0;
  virtual void exitUnpivot_clause(TSqlParser::Unpivot_clauseContext *ctx) = 0;

  virtual void enterFull_column_name_list(TSqlParser::Full_column_name_listContext *ctx) = 0;
  virtual void exitFull_column_name_list(TSqlParser::Full_column_name_listContext *ctx) = 0;

  virtual void enterTable_name_with_hint(TSqlParser::Table_name_with_hintContext *ctx) = 0;
  virtual void exitTable_name_with_hint(TSqlParser::Table_name_with_hintContext *ctx) = 0;

  virtual void enterBulk_option(TSqlParser::Bulk_optionContext *ctx) = 0;
  virtual void exitBulk_option(TSqlParser::Bulk_optionContext *ctx) = 0;

  virtual void enterDerived_table(TSqlParser::Derived_tableContext *ctx) = 0;
  virtual void exitDerived_table(TSqlParser::Derived_tableContext *ctx) = 0;

  virtual void enterRANKING_WINDOWED_FUNC(TSqlParser::RANKING_WINDOWED_FUNCContext *ctx) = 0;
  virtual void exitRANKING_WINDOWED_FUNC(TSqlParser::RANKING_WINDOWED_FUNCContext *ctx) = 0;

  virtual void enterAGGREGATE_WINDOWED_FUNC(TSqlParser::AGGREGATE_WINDOWED_FUNCContext *ctx) = 0;
  virtual void exitAGGREGATE_WINDOWED_FUNC(TSqlParser::AGGREGATE_WINDOWED_FUNCContext *ctx) = 0;

  virtual void enterANALYTIC_WINDOWED_FUNC(TSqlParser::ANALYTIC_WINDOWED_FUNCContext *ctx) = 0;
  virtual void exitANALYTIC_WINDOWED_FUNC(TSqlParser::ANALYTIC_WINDOWED_FUNCContext *ctx) = 0;

  virtual void enterBUILT_IN_FUNC(TSqlParser::BUILT_IN_FUNCContext *ctx) = 0;
  virtual void exitBUILT_IN_FUNC(TSqlParser::BUILT_IN_FUNCContext *ctx) = 0;

  virtual void enterSCALAR_FUNCTION(TSqlParser::SCALAR_FUNCTIONContext *ctx) = 0;
  virtual void exitSCALAR_FUNCTION(TSqlParser::SCALAR_FUNCTIONContext *ctx) = 0;

  virtual void enterPARTITION_FUNC(TSqlParser::PARTITION_FUNCContext *ctx) = 0;
  virtual void exitPARTITION_FUNC(TSqlParser::PARTITION_FUNCContext *ctx) = 0;

  virtual void enterPartition_function(TSqlParser::Partition_functionContext *ctx) = 0;
  virtual void exitPartition_function(TSqlParser::Partition_functionContext *ctx) = 0;

  virtual void enterBINARY_CHECKSUM(TSqlParser::BINARY_CHECKSUMContext *ctx) = 0;
  virtual void exitBINARY_CHECKSUM(TSqlParser::BINARY_CHECKSUMContext *ctx) = 0;

  virtual void enterCAST(TSqlParser::CASTContext *ctx) = 0;
  virtual void exitCAST(TSqlParser::CASTContext *ctx) = 0;

  virtual void enterTRY_CAST(TSqlParser::TRY_CASTContext *ctx) = 0;
  virtual void exitTRY_CAST(TSqlParser::TRY_CASTContext *ctx) = 0;

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

  virtual void enterUSER(TSqlParser::USERContext *ctx) = 0;
  virtual void exitUSER(TSqlParser::USERContext *ctx) = 0;

  virtual void enterISNULL(TSqlParser::ISNULLContext *ctx) = 0;
  virtual void exitISNULL(TSqlParser::ISNULLContext *ctx) = 0;

  virtual void enterIIF(TSqlParser::IIFContext *ctx) = 0;
  virtual void exitIIF(TSqlParser::IIFContext *ctx) = 0;

  virtual void enterSTRINGAGG(TSqlParser::STRINGAGGContext *ctx) = 0;
  virtual void exitSTRINGAGG(TSqlParser::STRINGAGGContext *ctx) = 0;

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

  virtual void enterInsert_column_name_list(TSqlParser::Insert_column_name_listContext *ctx) = 0;
  virtual void exitInsert_column_name_list(TSqlParser::Insert_column_name_listContext *ctx) = 0;

  virtual void enterInsert_column_id(TSqlParser::Insert_column_idContext *ctx) = 0;
  virtual void exitInsert_column_id(TSqlParser::Insert_column_idContext *ctx) = 0;

  virtual void enterColumn_name_list(TSqlParser::Column_name_listContext *ctx) = 0;
  virtual void exitColumn_name_list(TSqlParser::Column_name_listContext *ctx) = 0;

  virtual void enterCursor_name(TSqlParser::Cursor_nameContext *ctx) = 0;
  virtual void exitCursor_name(TSqlParser::Cursor_nameContext *ctx) = 0;

  virtual void enterOn_off(TSqlParser::On_offContext *ctx) = 0;
  virtual void exitOn_off(TSqlParser::On_offContext *ctx) = 0;

  virtual void enterNull_notnull(TSqlParser::Null_notnullContext *ctx) = 0;
  virtual void exitNull_notnull(TSqlParser::Null_notnullContext *ctx) = 0;

  virtual void enterNull_or_default(TSqlParser::Null_or_defaultContext *ctx) = 0;
  virtual void exitNull_or_default(TSqlParser::Null_or_defaultContext *ctx) = 0;

  virtual void enterScalar_function_name(TSqlParser::Scalar_function_nameContext *ctx) = 0;
  virtual void exitScalar_function_name(TSqlParser::Scalar_function_nameContext *ctx) = 0;

  virtual void enterData_type(TSqlParser::Data_typeContext *ctx) = 0;
  virtual void exitData_type(TSqlParser::Data_typeContext *ctx) = 0;

  virtual void enterDefault_value(TSqlParser::Default_valueContext *ctx) = 0;
  virtual void exitDefault_value(TSqlParser::Default_valueContext *ctx) = 0;

  virtual void enterConstant(TSqlParser::ConstantContext *ctx) = 0;
  virtual void exitConstant(TSqlParser::ConstantContext *ctx) = 0;

  virtual void enterSign(TSqlParser::SignContext *ctx) = 0;
  virtual void exitSign(TSqlParser::SignContext *ctx) = 0;

  virtual void enterKeyword(TSqlParser::KeywordContext *ctx) = 0;
  virtual void exitKeyword(TSqlParser::KeywordContext *ctx) = 0;

  virtual void enterId_(TSqlParser::Id_Context *ctx) = 0;
  virtual void exitId_(TSqlParser::Id_Context *ctx) = 0;

  virtual void enterSimple_id(TSqlParser::Simple_idContext *ctx) = 0;
  virtual void exitSimple_id(TSqlParser::Simple_idContext *ctx) = 0;

  virtual void enterId_or_string(TSqlParser::Id_or_stringContext *ctx) = 0;
  virtual void exitId_or_string(TSqlParser::Id_or_stringContext *ctx) = 0;

  virtual void enterComparison_operator(TSqlParser::Comparison_operatorContext *ctx) = 0;
  virtual void exitComparison_operator(TSqlParser::Comparison_operatorContext *ctx) = 0;

  virtual void enterAssignment_operator(TSqlParser::Assignment_operatorContext *ctx) = 0;
  virtual void exitAssignment_operator(TSqlParser::Assignment_operatorContext *ctx) = 0;


};

