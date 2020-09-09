#ifndef LISTENERINTERFACE_H
#define LISTENERINTERFACE_H

#include "gen/TSqlParserBaseListener.h"
#include "plan.h"

class ListenerInterface : public TSqlParserBaseListener {
private:
        std::vector<std::string> _error_tokens;
        std::vector<std::string> _rule_names;
        plan_t* _plan;

        void _no_impl(const std::string&, int);
public:
        ListenerInterface(plan_t*, const std::vector<std::string>&);
        void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override;
        void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override;



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

        void enterBackup_statement(TSqlParser::Backup_statementContext * /*ctx*/) override;
        void exitBackup_statement(TSqlParser::Backup_statementContext * /*ctx*/) override;

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

        void enterAlter_application_role(TSqlParser::Alter_application_roleContext * /*ctx*/) override;
        void exitAlter_application_role(TSqlParser::Alter_application_roleContext * /*ctx*/) override;

        void enterCreate_application_role(TSqlParser::Create_application_roleContext * /*ctx*/) override;
        void exitCreate_application_role(TSqlParser::Create_application_roleContext * /*ctx*/) override;

        void enterDrop_aggregate(TSqlParser::Drop_aggregateContext * /*ctx*/) override;
        void exitDrop_aggregate(TSqlParser::Drop_aggregateContext * /*ctx*/) override;

        void enterDrop_application_role(TSqlParser::Drop_application_roleContext * /*ctx*/) override;
        void exitDrop_application_role(TSqlParser::Drop_application_roleContext * /*ctx*/) override;

        void enterAlter_assembly(TSqlParser::Alter_assemblyContext * /*ctx*/) override;
        void exitAlter_assembly(TSqlParser::Alter_assemblyContext * /*ctx*/) override;

        void enterAlter_assembly_start(TSqlParser::Alter_assembly_startContext * /*ctx*/) override;
        void exitAlter_assembly_start(TSqlParser::Alter_assembly_startContext * /*ctx*/) override;

        void enterAlter_assembly_clause(TSqlParser::Alter_assembly_clauseContext * /*ctx*/) override;
        void exitAlter_assembly_clause(TSqlParser::Alter_assembly_clauseContext * /*ctx*/) override;

        void enterAlter_assembly_from_clause(TSqlParser::Alter_assembly_from_clauseContext * /*ctx*/) override;
        void exitAlter_assembly_from_clause(TSqlParser::Alter_assembly_from_clauseContext * /*ctx*/) override;

        void enterAlter_assembly_from_clause_start(TSqlParser::Alter_assembly_from_clause_startContext * /*ctx*/) override;
        void exitAlter_assembly_from_clause_start(TSqlParser::Alter_assembly_from_clause_startContext * /*ctx*/) override;

        void enterAlter_assembly_drop_clause(TSqlParser::Alter_assembly_drop_clauseContext * /*ctx*/) override;
        void exitAlter_assembly_drop_clause(TSqlParser::Alter_assembly_drop_clauseContext * /*ctx*/) override;

        void enterAlter_assembly_drop_multiple_files(TSqlParser::Alter_assembly_drop_multiple_filesContext * /*ctx*/) override;
        void exitAlter_assembly_drop_multiple_files(TSqlParser::Alter_assembly_drop_multiple_filesContext * /*ctx*/) override;

        void enterAlter_assembly_drop(TSqlParser::Alter_assembly_dropContext * /*ctx*/) override;
        void exitAlter_assembly_drop(TSqlParser::Alter_assembly_dropContext * /*ctx*/) override;

        void enterAlter_assembly_add_clause(TSqlParser::Alter_assembly_add_clauseContext * /*ctx*/) override;
        void exitAlter_assembly_add_clause(TSqlParser::Alter_assembly_add_clauseContext * /*ctx*/) override;

        void enterAlter_asssembly_add_clause_start(TSqlParser::Alter_asssembly_add_clause_startContext * /*ctx*/) override;
        void exitAlter_asssembly_add_clause_start(TSqlParser::Alter_asssembly_add_clause_startContext * /*ctx*/) override;

        void enterAlter_assembly_client_file_clause(TSqlParser::Alter_assembly_client_file_clauseContext * /*ctx*/) override;
        void exitAlter_assembly_client_file_clause(TSqlParser::Alter_assembly_client_file_clauseContext * /*ctx*/) override;

        void enterAlter_assembly_file_name(TSqlParser::Alter_assembly_file_nameContext * /*ctx*/) override;
        void exitAlter_assembly_file_name(TSqlParser::Alter_assembly_file_nameContext * /*ctx*/) override;

        void enterAlter_assembly_file_bits(TSqlParser::Alter_assembly_file_bitsContext * /*ctx*/) override;
        void exitAlter_assembly_file_bits(TSqlParser::Alter_assembly_file_bitsContext * /*ctx*/) override;

        void enterAlter_assembly_as(TSqlParser::Alter_assembly_asContext * /*ctx*/) override;
        void exitAlter_assembly_as(TSqlParser::Alter_assembly_asContext * /*ctx*/) override;

        void enterAlter_assembly_with_clause(TSqlParser::Alter_assembly_with_clauseContext * /*ctx*/) override;
        void exitAlter_assembly_with_clause(TSqlParser::Alter_assembly_with_clauseContext * /*ctx*/) override;

        void enterAlter_assembly_with(TSqlParser::Alter_assembly_withContext * /*ctx*/) override;
        void exitAlter_assembly_with(TSqlParser::Alter_assembly_withContext * /*ctx*/) override;

        void enterClient_assembly_specifier(TSqlParser::Client_assembly_specifierContext * /*ctx*/) override;
        void exitClient_assembly_specifier(TSqlParser::Client_assembly_specifierContext * /*ctx*/) override;

        void enterAssembly_option(TSqlParser::Assembly_optionContext * /*ctx*/) override;
        void exitAssembly_option(TSqlParser::Assembly_optionContext * /*ctx*/) override;

        void enterNetwork_file_share(TSqlParser::Network_file_shareContext * /*ctx*/) override;
        void exitNetwork_file_share(TSqlParser::Network_file_shareContext * /*ctx*/) override;

        void enterNetwork_computer(TSqlParser::Network_computerContext * /*ctx*/) override;
        void exitNetwork_computer(TSqlParser::Network_computerContext * /*ctx*/) override;

        void enterNetwork_file_start(TSqlParser::Network_file_startContext * /*ctx*/) override;
        void exitNetwork_file_start(TSqlParser::Network_file_startContext * /*ctx*/) override;

        void enterFile_path(TSqlParser::File_pathContext * /*ctx*/) override;
        void exitFile_path(TSqlParser::File_pathContext * /*ctx*/) override;

        void enterFile_directory_path_separator(TSqlParser::File_directory_path_separatorContext * /*ctx*/) override;
        void exitFile_directory_path_separator(TSqlParser::File_directory_path_separatorContext * /*ctx*/) override;

        void enterLocal_file(TSqlParser::Local_fileContext * /*ctx*/) override;
        void exitLocal_file(TSqlParser::Local_fileContext * /*ctx*/) override;

        void enterLocal_drive(TSqlParser::Local_driveContext * /*ctx*/) override;
        void exitLocal_drive(TSqlParser::Local_driveContext * /*ctx*/) override;

        void enterMultiple_local_files(TSqlParser::Multiple_local_filesContext * /*ctx*/) override;
        void exitMultiple_local_files(TSqlParser::Multiple_local_filesContext * /*ctx*/) override;

        void enterMultiple_local_file_start(TSqlParser::Multiple_local_file_startContext * /*ctx*/) override;
        void exitMultiple_local_file_start(TSqlParser::Multiple_local_file_startContext * /*ctx*/) override;

        void enterCreate_assembly(TSqlParser::Create_assemblyContext * /*ctx*/) override;
        void exitCreate_assembly(TSqlParser::Create_assemblyContext * /*ctx*/) override;

        void enterDrop_assembly(TSqlParser::Drop_assemblyContext * /*ctx*/) override;
        void exitDrop_assembly(TSqlParser::Drop_assemblyContext * /*ctx*/) override;

        void enterAlter_asymmetric_key(TSqlParser::Alter_asymmetric_keyContext * /*ctx*/) override;
        void exitAlter_asymmetric_key(TSqlParser::Alter_asymmetric_keyContext * /*ctx*/) override;

        void enterAlter_asymmetric_key_start(TSqlParser::Alter_asymmetric_key_startContext * /*ctx*/) override;
        void exitAlter_asymmetric_key_start(TSqlParser::Alter_asymmetric_key_startContext * /*ctx*/) override;

        void enterAsymmetric_key_option(TSqlParser::Asymmetric_key_optionContext * /*ctx*/) override;
        void exitAsymmetric_key_option(TSqlParser::Asymmetric_key_optionContext * /*ctx*/) override;

        void enterAsymmetric_key_option_start(TSqlParser::Asymmetric_key_option_startContext * /*ctx*/) override;
        void exitAsymmetric_key_option_start(TSqlParser::Asymmetric_key_option_startContext * /*ctx*/) override;

        void enterAsymmetric_key_password_change_option(TSqlParser::Asymmetric_key_password_change_optionContext * /*ctx*/) override;
        void exitAsymmetric_key_password_change_option(TSqlParser::Asymmetric_key_password_change_optionContext * /*ctx*/) override;

        void enterCreate_asymmetric_key(TSqlParser::Create_asymmetric_keyContext * /*ctx*/) override;
        void exitCreate_asymmetric_key(TSqlParser::Create_asymmetric_keyContext * /*ctx*/) override;

        void enterDrop_asymmetric_key(TSqlParser::Drop_asymmetric_keyContext * /*ctx*/) override;
        void exitDrop_asymmetric_key(TSqlParser::Drop_asymmetric_keyContext * /*ctx*/) override;

        void enterAlter_authorization(TSqlParser::Alter_authorizationContext * /*ctx*/) override;
        void exitAlter_authorization(TSqlParser::Alter_authorizationContext * /*ctx*/) override;

        void enterAuthorization_grantee(TSqlParser::Authorization_granteeContext * /*ctx*/) override;
        void exitAuthorization_grantee(TSqlParser::Authorization_granteeContext * /*ctx*/) override;

        void enterEntity_to(TSqlParser::Entity_toContext * /*ctx*/) override;
        void exitEntity_to(TSqlParser::Entity_toContext * /*ctx*/) override;

        void enterColon_colon(TSqlParser::Colon_colonContext * /*ctx*/) override;
        void exitColon_colon(TSqlParser::Colon_colonContext * /*ctx*/) override;

        void enterAlter_authorization_start(TSqlParser::Alter_authorization_startContext * /*ctx*/) override;
        void exitAlter_authorization_start(TSqlParser::Alter_authorization_startContext * /*ctx*/) override;

        void enterAlter_authorization_for_sql_database(TSqlParser::Alter_authorization_for_sql_databaseContext * /*ctx*/) override;
        void exitAlter_authorization_for_sql_database(TSqlParser::Alter_authorization_for_sql_databaseContext * /*ctx*/) override;

        void enterAlter_authorization_for_azure_dw(TSqlParser::Alter_authorization_for_azure_dwContext * /*ctx*/) override;
        void exitAlter_authorization_for_azure_dw(TSqlParser::Alter_authorization_for_azure_dwContext * /*ctx*/) override;

        void enterAlter_authorization_for_parallel_dw(TSqlParser::Alter_authorization_for_parallel_dwContext * /*ctx*/) override;
        void exitAlter_authorization_for_parallel_dw(TSqlParser::Alter_authorization_for_parallel_dwContext * /*ctx*/) override;

        void enterClass_type(TSqlParser::Class_typeContext * /*ctx*/) override;
        void exitClass_type(TSqlParser::Class_typeContext * /*ctx*/) override;

        void enterClass_type_for_sql_database(TSqlParser::Class_type_for_sql_databaseContext * /*ctx*/) override;
        void exitClass_type_for_sql_database(TSqlParser::Class_type_for_sql_databaseContext * /*ctx*/) override;

        void enterClass_type_for_azure_dw(TSqlParser::Class_type_for_azure_dwContext * /*ctx*/) override;
        void exitClass_type_for_azure_dw(TSqlParser::Class_type_for_azure_dwContext * /*ctx*/) override;

        void enterClass_type_for_parallel_dw(TSqlParser::Class_type_for_parallel_dwContext * /*ctx*/) override;
        void exitClass_type_for_parallel_dw(TSqlParser::Class_type_for_parallel_dwContext * /*ctx*/) override;

        void enterDrop_availability_group(TSqlParser::Drop_availability_groupContext * /*ctx*/) override;
        void exitDrop_availability_group(TSqlParser::Drop_availability_groupContext * /*ctx*/) override;

        void enterAlter_availability_group(TSqlParser::Alter_availability_groupContext * /*ctx*/) override;
        void exitAlter_availability_group(TSqlParser::Alter_availability_groupContext * /*ctx*/) override;

        void enterAlter_availability_group_start(TSqlParser::Alter_availability_group_startContext * /*ctx*/) override;
        void exitAlter_availability_group_start(TSqlParser::Alter_availability_group_startContext * /*ctx*/) override;

        void enterAlter_availability_group_options(TSqlParser::Alter_availability_group_optionsContext * /*ctx*/) override;
        void exitAlter_availability_group_options(TSqlParser::Alter_availability_group_optionsContext * /*ctx*/) override;

        void enterCreate_or_alter_broker_priority(TSqlParser::Create_or_alter_broker_priorityContext * /*ctx*/) override;
        void exitCreate_or_alter_broker_priority(TSqlParser::Create_or_alter_broker_priorityContext * /*ctx*/) override;

        void enterDrop_broker_priority(TSqlParser::Drop_broker_priorityContext * /*ctx*/) override;
        void exitDrop_broker_priority(TSqlParser::Drop_broker_priorityContext * /*ctx*/) override;

        void enterAlter_certificate(TSqlParser::Alter_certificateContext * /*ctx*/) override;
        void exitAlter_certificate(TSqlParser::Alter_certificateContext * /*ctx*/) override;

        void enterAlter_column_encryption_key(TSqlParser::Alter_column_encryption_keyContext * /*ctx*/) override;
        void exitAlter_column_encryption_key(TSqlParser::Alter_column_encryption_keyContext * /*ctx*/) override;

        void enterCreate_column_encryption_key(TSqlParser::Create_column_encryption_keyContext * /*ctx*/) override;
        void exitCreate_column_encryption_key(TSqlParser::Create_column_encryption_keyContext * /*ctx*/) override;

        void enterDrop_certificate(TSqlParser::Drop_certificateContext * /*ctx*/) override;
        void exitDrop_certificate(TSqlParser::Drop_certificateContext * /*ctx*/) override;

        void enterDrop_column_encryption_key(TSqlParser::Drop_column_encryption_keyContext * /*ctx*/) override;
        void exitDrop_column_encryption_key(TSqlParser::Drop_column_encryption_keyContext * /*ctx*/) override;

        void enterDrop_column_master_key(TSqlParser::Drop_column_master_keyContext * /*ctx*/) override;
        void exitDrop_column_master_key(TSqlParser::Drop_column_master_keyContext * /*ctx*/) override;

        void enterDrop_contract(TSqlParser::Drop_contractContext * /*ctx*/) override;
        void exitDrop_contract(TSqlParser::Drop_contractContext * /*ctx*/) override;

        void enterDrop_credential(TSqlParser::Drop_credentialContext * /*ctx*/) override;
        void exitDrop_credential(TSqlParser::Drop_credentialContext * /*ctx*/) override;

        void enterDrop_cryptograhic_provider(TSqlParser::Drop_cryptograhic_providerContext * /*ctx*/) override;
        void exitDrop_cryptograhic_provider(TSqlParser::Drop_cryptograhic_providerContext * /*ctx*/) override;

        void enterDrop_database(TSqlParser::Drop_databaseContext * /*ctx*/) override;
        void exitDrop_database(TSqlParser::Drop_databaseContext * /*ctx*/) override;

        void enterDrop_database_audit_specification(TSqlParser::Drop_database_audit_specificationContext * /*ctx*/) override;
        void exitDrop_database_audit_specification(TSqlParser::Drop_database_audit_specificationContext * /*ctx*/) override;

        void enterDrop_database_scoped_credential(TSqlParser::Drop_database_scoped_credentialContext * /*ctx*/) override;
        void exitDrop_database_scoped_credential(TSqlParser::Drop_database_scoped_credentialContext * /*ctx*/) override;

        void enterDrop_default(TSqlParser::Drop_defaultContext * /*ctx*/) override;
        void exitDrop_default(TSqlParser::Drop_defaultContext * /*ctx*/) override;

        void enterDrop_endpoint(TSqlParser::Drop_endpointContext * /*ctx*/) override;
        void exitDrop_endpoint(TSqlParser::Drop_endpointContext * /*ctx*/) override;

        void enterDrop_external_data_source(TSqlParser::Drop_external_data_sourceContext * /*ctx*/) override;
        void exitDrop_external_data_source(TSqlParser::Drop_external_data_sourceContext * /*ctx*/) override;

        void enterDrop_external_file_format(TSqlParser::Drop_external_file_formatContext * /*ctx*/) override;
        void exitDrop_external_file_format(TSqlParser::Drop_external_file_formatContext * /*ctx*/) override;

        void enterDrop_external_library(TSqlParser::Drop_external_libraryContext * /*ctx*/) override;
        void exitDrop_external_library(TSqlParser::Drop_external_libraryContext * /*ctx*/) override;

        void enterDrop_external_resource_pool(TSqlParser::Drop_external_resource_poolContext * /*ctx*/) override;
        void exitDrop_external_resource_pool(TSqlParser::Drop_external_resource_poolContext * /*ctx*/) override;

        void enterDrop_external_table(TSqlParser::Drop_external_tableContext * /*ctx*/) override;
        void exitDrop_external_table(TSqlParser::Drop_external_tableContext * /*ctx*/) override;

        void enterDrop_event_notifications(TSqlParser::Drop_event_notificationsContext * /*ctx*/) override;
        void exitDrop_event_notifications(TSqlParser::Drop_event_notificationsContext * /*ctx*/) override;

        void enterDrop_event_session(TSqlParser::Drop_event_sessionContext * /*ctx*/) override;
        void exitDrop_event_session(TSqlParser::Drop_event_sessionContext * /*ctx*/) override;

        void enterDrop_fulltext_catalog(TSqlParser::Drop_fulltext_catalogContext * /*ctx*/) override;
        void exitDrop_fulltext_catalog(TSqlParser::Drop_fulltext_catalogContext * /*ctx*/) override;

        void enterDrop_fulltext_index(TSqlParser::Drop_fulltext_indexContext * /*ctx*/) override;
        void exitDrop_fulltext_index(TSqlParser::Drop_fulltext_indexContext * /*ctx*/) override;

        void enterDrop_fulltext_stoplist(TSqlParser::Drop_fulltext_stoplistContext * /*ctx*/) override;
        void exitDrop_fulltext_stoplist(TSqlParser::Drop_fulltext_stoplistContext * /*ctx*/) override;

        void enterDrop_login(TSqlParser::Drop_loginContext * /*ctx*/) override;
        void exitDrop_login(TSqlParser::Drop_loginContext * /*ctx*/) override;

        void enterDrop_master_key(TSqlParser::Drop_master_keyContext * /*ctx*/) override;
        void exitDrop_master_key(TSqlParser::Drop_master_keyContext * /*ctx*/) override;

        void enterDrop_message_type(TSqlParser::Drop_message_typeContext * /*ctx*/) override;
        void exitDrop_message_type(TSqlParser::Drop_message_typeContext * /*ctx*/) override;

        void enterDrop_partition_function(TSqlParser::Drop_partition_functionContext * /*ctx*/) override;
        void exitDrop_partition_function(TSqlParser::Drop_partition_functionContext * /*ctx*/) override;

        void enterDrop_partition_scheme(TSqlParser::Drop_partition_schemeContext * /*ctx*/) override;
        void exitDrop_partition_scheme(TSqlParser::Drop_partition_schemeContext * /*ctx*/) override;

        void enterDrop_queue(TSqlParser::Drop_queueContext * /*ctx*/) override;
        void exitDrop_queue(TSqlParser::Drop_queueContext * /*ctx*/) override;

        void enterDrop_remote_service_binding(TSqlParser::Drop_remote_service_bindingContext * /*ctx*/) override;
        void exitDrop_remote_service_binding(TSqlParser::Drop_remote_service_bindingContext * /*ctx*/) override;

        void enterDrop_resource_pool(TSqlParser::Drop_resource_poolContext * /*ctx*/) override;
        void exitDrop_resource_pool(TSqlParser::Drop_resource_poolContext * /*ctx*/) override;

        void enterDrop_db_role(TSqlParser::Drop_db_roleContext * /*ctx*/) override;
        void exitDrop_db_role(TSqlParser::Drop_db_roleContext * /*ctx*/) override;

        void enterDrop_route(TSqlParser::Drop_routeContext * /*ctx*/) override;
        void exitDrop_route(TSqlParser::Drop_routeContext * /*ctx*/) override;

        void enterDrop_rule(TSqlParser::Drop_ruleContext * /*ctx*/) override;
        void exitDrop_rule(TSqlParser::Drop_ruleContext * /*ctx*/) override;

        void enterDrop_schema(TSqlParser::Drop_schemaContext * /*ctx*/) override;
        void exitDrop_schema(TSqlParser::Drop_schemaContext * /*ctx*/) override;

        void enterDrop_search_property_list(TSqlParser::Drop_search_property_listContext * /*ctx*/) override;
        void exitDrop_search_property_list(TSqlParser::Drop_search_property_listContext * /*ctx*/) override;

        void enterDrop_security_policy(TSqlParser::Drop_security_policyContext * /*ctx*/) override;
        void exitDrop_security_policy(TSqlParser::Drop_security_policyContext * /*ctx*/) override;

        void enterDrop_sequence(TSqlParser::Drop_sequenceContext * /*ctx*/) override;
        void exitDrop_sequence(TSqlParser::Drop_sequenceContext * /*ctx*/) override;

        void enterDrop_server_audit(TSqlParser::Drop_server_auditContext * /*ctx*/) override;
        void exitDrop_server_audit(TSqlParser::Drop_server_auditContext * /*ctx*/) override;

        void enterDrop_server_audit_specification(TSqlParser::Drop_server_audit_specificationContext * /*ctx*/) override;
        void exitDrop_server_audit_specification(TSqlParser::Drop_server_audit_specificationContext * /*ctx*/) override;

        void enterDrop_server_role(TSqlParser::Drop_server_roleContext * /*ctx*/) override;
        void exitDrop_server_role(TSqlParser::Drop_server_roleContext * /*ctx*/) override;

        void enterDrop_service(TSqlParser::Drop_serviceContext * /*ctx*/) override;
        void exitDrop_service(TSqlParser::Drop_serviceContext * /*ctx*/) override;

        void enterDrop_signature(TSqlParser::Drop_signatureContext * /*ctx*/) override;
        void exitDrop_signature(TSqlParser::Drop_signatureContext * /*ctx*/) override;

        void enterDrop_statistics_name_azure_dw_and_pdw(TSqlParser::Drop_statistics_name_azure_dw_and_pdwContext * /*ctx*/) override;
        void exitDrop_statistics_name_azure_dw_and_pdw(TSqlParser::Drop_statistics_name_azure_dw_and_pdwContext * /*ctx*/) override;

        void enterDrop_symmetric_key(TSqlParser::Drop_symmetric_keyContext * /*ctx*/) override;
        void exitDrop_symmetric_key(TSqlParser::Drop_symmetric_keyContext * /*ctx*/) override;

        void enterDrop_synonym(TSqlParser::Drop_synonymContext * /*ctx*/) override;
        void exitDrop_synonym(TSqlParser::Drop_synonymContext * /*ctx*/) override;

        void enterDrop_user(TSqlParser::Drop_userContext * /*ctx*/) override;
        void exitDrop_user(TSqlParser::Drop_userContext * /*ctx*/) override;

        void enterDrop_workload_group(TSqlParser::Drop_workload_groupContext * /*ctx*/) override;
        void exitDrop_workload_group(TSqlParser::Drop_workload_groupContext * /*ctx*/) override;

        void enterDrop_xml_schema_collection(TSqlParser::Drop_xml_schema_collectionContext * /*ctx*/) override;
        void exitDrop_xml_schema_collection(TSqlParser::Drop_xml_schema_collectionContext * /*ctx*/) override;

        void enterDisable_trigger(TSqlParser::Disable_triggerContext * /*ctx*/) override;
        void exitDisable_trigger(TSqlParser::Disable_triggerContext * /*ctx*/) override;

        void enterEnable_trigger(TSqlParser::Enable_triggerContext * /*ctx*/) override;
        void exitEnable_trigger(TSqlParser::Enable_triggerContext * /*ctx*/) override;

        void enterLock_table(TSqlParser::Lock_tableContext * /*ctx*/) override;
        void exitLock_table(TSqlParser::Lock_tableContext * /*ctx*/) override;

        void enterTruncate_table(TSqlParser::Truncate_tableContext * /*ctx*/) override;
        void exitTruncate_table(TSqlParser::Truncate_tableContext * /*ctx*/) override;

        void enterCreate_column_master_key(TSqlParser::Create_column_master_keyContext * /*ctx*/) override;
        void exitCreate_column_master_key(TSqlParser::Create_column_master_keyContext * /*ctx*/) override;

        void enterAlter_credential(TSqlParser::Alter_credentialContext * /*ctx*/) override;
        void exitAlter_credential(TSqlParser::Alter_credentialContext * /*ctx*/) override;

        void enterCreate_credential(TSqlParser::Create_credentialContext * /*ctx*/) override;
        void exitCreate_credential(TSqlParser::Create_credentialContext * /*ctx*/) override;

        void enterAlter_cryptographic_provider(TSqlParser::Alter_cryptographic_providerContext * /*ctx*/) override;
        void exitAlter_cryptographic_provider(TSqlParser::Alter_cryptographic_providerContext * /*ctx*/) override;

        void enterCreate_cryptographic_provider(TSqlParser::Create_cryptographic_providerContext * /*ctx*/) override;
        void exitCreate_cryptographic_provider(TSqlParser::Create_cryptographic_providerContext * /*ctx*/) override;

        void enterCreate_event_notification(TSqlParser::Create_event_notificationContext * /*ctx*/) override;
        void exitCreate_event_notification(TSqlParser::Create_event_notificationContext * /*ctx*/) override;

        void enterCreate_or_alter_event_session(TSqlParser::Create_or_alter_event_sessionContext * /*ctx*/) override;
        void exitCreate_or_alter_event_session(TSqlParser::Create_or_alter_event_sessionContext * /*ctx*/) override;

        void enterEvent_session_predicate_expression(TSqlParser::Event_session_predicate_expressionContext * /*ctx*/) override;
        void exitEvent_session_predicate_expression(TSqlParser::Event_session_predicate_expressionContext * /*ctx*/) override;

        void enterEvent_session_predicate_factor(TSqlParser::Event_session_predicate_factorContext * /*ctx*/) override;
        void exitEvent_session_predicate_factor(TSqlParser::Event_session_predicate_factorContext * /*ctx*/) override;

        void enterEvent_session_predicate_leaf(TSqlParser::Event_session_predicate_leafContext * /*ctx*/) override;
        void exitEvent_session_predicate_leaf(TSqlParser::Event_session_predicate_leafContext * /*ctx*/) override;

        void enterAlter_external_data_source(TSqlParser::Alter_external_data_sourceContext * /*ctx*/) override;
        void exitAlter_external_data_source(TSqlParser::Alter_external_data_sourceContext * /*ctx*/) override;

        void enterAlter_external_library(TSqlParser::Alter_external_libraryContext * /*ctx*/) override;
        void exitAlter_external_library(TSqlParser::Alter_external_libraryContext * /*ctx*/) override;

        void enterCreate_external_library(TSqlParser::Create_external_libraryContext * /*ctx*/) override;
        void exitCreate_external_library(TSqlParser::Create_external_libraryContext * /*ctx*/) override;

        void enterAlter_external_resource_pool(TSqlParser::Alter_external_resource_poolContext * /*ctx*/) override;
        void exitAlter_external_resource_pool(TSqlParser::Alter_external_resource_poolContext * /*ctx*/) override;

        void enterCreate_external_resource_pool(TSqlParser::Create_external_resource_poolContext * /*ctx*/) override;
        void exitCreate_external_resource_pool(TSqlParser::Create_external_resource_poolContext * /*ctx*/) override;

        void enterAlter_fulltext_catalog(TSqlParser::Alter_fulltext_catalogContext * /*ctx*/) override;
        void exitAlter_fulltext_catalog(TSqlParser::Alter_fulltext_catalogContext * /*ctx*/) override;

        void enterCreate_fulltext_catalog(TSqlParser::Create_fulltext_catalogContext * /*ctx*/) override;
        void exitCreate_fulltext_catalog(TSqlParser::Create_fulltext_catalogContext * /*ctx*/) override;

        void enterAlter_fulltext_stoplist(TSqlParser::Alter_fulltext_stoplistContext * /*ctx*/) override;
        void exitAlter_fulltext_stoplist(TSqlParser::Alter_fulltext_stoplistContext * /*ctx*/) override;

        void enterCreate_fulltext_stoplist(TSqlParser::Create_fulltext_stoplistContext * /*ctx*/) override;
        void exitCreate_fulltext_stoplist(TSqlParser::Create_fulltext_stoplistContext * /*ctx*/) override;

        void enterAlter_login_sql_server(TSqlParser::Alter_login_sql_serverContext * /*ctx*/) override;
        void exitAlter_login_sql_server(TSqlParser::Alter_login_sql_serverContext * /*ctx*/) override;

        void enterCreate_login_sql_server(TSqlParser::Create_login_sql_serverContext * /*ctx*/) override;
        void exitCreate_login_sql_server(TSqlParser::Create_login_sql_serverContext * /*ctx*/) override;

        void enterAlter_login_azure_sql(TSqlParser::Alter_login_azure_sqlContext * /*ctx*/) override;
        void exitAlter_login_azure_sql(TSqlParser::Alter_login_azure_sqlContext * /*ctx*/) override;

        void enterCreate_login_azure_sql(TSqlParser::Create_login_azure_sqlContext * /*ctx*/) override;
        void exitCreate_login_azure_sql(TSqlParser::Create_login_azure_sqlContext * /*ctx*/) override;

        void enterAlter_login_azure_sql_dw_and_pdw(TSqlParser::Alter_login_azure_sql_dw_and_pdwContext * /*ctx*/) override;
        void exitAlter_login_azure_sql_dw_and_pdw(TSqlParser::Alter_login_azure_sql_dw_and_pdwContext * /*ctx*/) override;

        void enterCreate_login_pdw(TSqlParser::Create_login_pdwContext * /*ctx*/) override;
        void exitCreate_login_pdw(TSqlParser::Create_login_pdwContext * /*ctx*/) override;

        void enterAlter_master_key_sql_server(TSqlParser::Alter_master_key_sql_serverContext * /*ctx*/) override;
        void exitAlter_master_key_sql_server(TSqlParser::Alter_master_key_sql_serverContext * /*ctx*/) override;

        void enterCreate_master_key_sql_server(TSqlParser::Create_master_key_sql_serverContext * /*ctx*/) override;
        void exitCreate_master_key_sql_server(TSqlParser::Create_master_key_sql_serverContext * /*ctx*/) override;

        void enterAlter_master_key_azure_sql(TSqlParser::Alter_master_key_azure_sqlContext * /*ctx*/) override;
        void exitAlter_master_key_azure_sql(TSqlParser::Alter_master_key_azure_sqlContext * /*ctx*/) override;

        void enterCreate_master_key_azure_sql(TSqlParser::Create_master_key_azure_sqlContext * /*ctx*/) override;
        void exitCreate_master_key_azure_sql(TSqlParser::Create_master_key_azure_sqlContext * /*ctx*/) override;

        void enterAlter_message_type(TSqlParser::Alter_message_typeContext * /*ctx*/) override;
        void exitAlter_message_type(TSqlParser::Alter_message_typeContext * /*ctx*/) override;

        void enterAlter_partition_function(TSqlParser::Alter_partition_functionContext * /*ctx*/) override;
        void exitAlter_partition_function(TSqlParser::Alter_partition_functionContext * /*ctx*/) override;

        void enterAlter_partition_scheme(TSqlParser::Alter_partition_schemeContext * /*ctx*/) override;
        void exitAlter_partition_scheme(TSqlParser::Alter_partition_schemeContext * /*ctx*/) override;

        void enterAlter_remote_service_binding(TSqlParser::Alter_remote_service_bindingContext * /*ctx*/) override;
        void exitAlter_remote_service_binding(TSqlParser::Alter_remote_service_bindingContext * /*ctx*/) override;

        void enterCreate_remote_service_binding(TSqlParser::Create_remote_service_bindingContext * /*ctx*/) override;
        void exitCreate_remote_service_binding(TSqlParser::Create_remote_service_bindingContext * /*ctx*/) override;

        void enterCreate_resource_pool(TSqlParser::Create_resource_poolContext * /*ctx*/) override;
        void exitCreate_resource_pool(TSqlParser::Create_resource_poolContext * /*ctx*/) override;

        void enterAlter_resource_governor(TSqlParser::Alter_resource_governorContext * /*ctx*/) override;
        void exitAlter_resource_governor(TSqlParser::Alter_resource_governorContext * /*ctx*/) override;

        void enterAlter_db_role(TSqlParser::Alter_db_roleContext * /*ctx*/) override;
        void exitAlter_db_role(TSqlParser::Alter_db_roleContext * /*ctx*/) override;

        void enterCreate_db_role(TSqlParser::Create_db_roleContext * /*ctx*/) override;
        void exitCreate_db_role(TSqlParser::Create_db_roleContext * /*ctx*/) override;

        void enterCreate_route(TSqlParser::Create_routeContext * /*ctx*/) override;
        void exitCreate_route(TSqlParser::Create_routeContext * /*ctx*/) override;

        void enterCreate_rule(TSqlParser::Create_ruleContext * /*ctx*/) override;
        void exitCreate_rule(TSqlParser::Create_ruleContext * /*ctx*/) override;

        void enterAlter_schema_sql(TSqlParser::Alter_schema_sqlContext * /*ctx*/) override;
        void exitAlter_schema_sql(TSqlParser::Alter_schema_sqlContext * /*ctx*/) override;

        void enterCreate_schema(TSqlParser::Create_schemaContext * /*ctx*/) override;
        void exitCreate_schema(TSqlParser::Create_schemaContext * /*ctx*/) override;

        void enterCreate_schema_azure_sql_dw_and_pdw(TSqlParser::Create_schema_azure_sql_dw_and_pdwContext * /*ctx*/) override;
        void exitCreate_schema_azure_sql_dw_and_pdw(TSqlParser::Create_schema_azure_sql_dw_and_pdwContext * /*ctx*/) override;

        void enterAlter_schema_azure_sql_dw_and_pdw(TSqlParser::Alter_schema_azure_sql_dw_and_pdwContext * /*ctx*/) override;
        void exitAlter_schema_azure_sql_dw_and_pdw(TSqlParser::Alter_schema_azure_sql_dw_and_pdwContext * /*ctx*/) override;

        void enterCreate_search_property_list(TSqlParser::Create_search_property_listContext * /*ctx*/) override;
        void exitCreate_search_property_list(TSqlParser::Create_search_property_listContext * /*ctx*/) override;

        void enterCreate_security_policy(TSqlParser::Create_security_policyContext * /*ctx*/) override;
        void exitCreate_security_policy(TSqlParser::Create_security_policyContext * /*ctx*/) override;

        void enterAlter_sequence(TSqlParser::Alter_sequenceContext * /*ctx*/) override;
        void exitAlter_sequence(TSqlParser::Alter_sequenceContext * /*ctx*/) override;

        void enterCreate_sequence(TSqlParser::Create_sequenceContext * /*ctx*/) override;
        void exitCreate_sequence(TSqlParser::Create_sequenceContext * /*ctx*/) override;

        void enterAlter_server_audit(TSqlParser::Alter_server_auditContext * /*ctx*/) override;
        void exitAlter_server_audit(TSqlParser::Alter_server_auditContext * /*ctx*/) override;

        void enterCreate_server_audit(TSqlParser::Create_server_auditContext * /*ctx*/) override;
        void exitCreate_server_audit(TSqlParser::Create_server_auditContext * /*ctx*/) override;

        void enterAlter_server_audit_specification(TSqlParser::Alter_server_audit_specificationContext * /*ctx*/) override;
        void exitAlter_server_audit_specification(TSqlParser::Alter_server_audit_specificationContext * /*ctx*/) override;

        void enterCreate_server_audit_specification(TSqlParser::Create_server_audit_specificationContext * /*ctx*/) override;
        void exitCreate_server_audit_specification(TSqlParser::Create_server_audit_specificationContext * /*ctx*/) override;

        void enterAlter_server_configuration(TSqlParser::Alter_server_configurationContext * /*ctx*/) override;
        void exitAlter_server_configuration(TSqlParser::Alter_server_configurationContext * /*ctx*/) override;

        void enterAlter_server_role(TSqlParser::Alter_server_roleContext * /*ctx*/) override;
        void exitAlter_server_role(TSqlParser::Alter_server_roleContext * /*ctx*/) override;

        void enterCreate_server_role(TSqlParser::Create_server_roleContext * /*ctx*/) override;
        void exitCreate_server_role(TSqlParser::Create_server_roleContext * /*ctx*/) override;

        void enterAlter_server_role_pdw(TSqlParser::Alter_server_role_pdwContext * /*ctx*/) override;
        void exitAlter_server_role_pdw(TSqlParser::Alter_server_role_pdwContext * /*ctx*/) override;

        void enterAlter_service(TSqlParser::Alter_serviceContext * /*ctx*/) override;
        void exitAlter_service(TSqlParser::Alter_serviceContext * /*ctx*/) override;

        void enterCreate_service(TSqlParser::Create_serviceContext * /*ctx*/) override;
        void exitCreate_service(TSqlParser::Create_serviceContext * /*ctx*/) override;

        void enterAlter_service_master_key(TSqlParser::Alter_service_master_keyContext * /*ctx*/) override;
        void exitAlter_service_master_key(TSqlParser::Alter_service_master_keyContext * /*ctx*/) override;

        void enterAlter_symmetric_key(TSqlParser::Alter_symmetric_keyContext * /*ctx*/) override;
        void exitAlter_symmetric_key(TSqlParser::Alter_symmetric_keyContext * /*ctx*/) override;

        void enterCreate_symmetric_key(TSqlParser::Create_symmetric_keyContext * /*ctx*/) override;
        void exitCreate_symmetric_key(TSqlParser::Create_symmetric_keyContext * /*ctx*/) override;

        void enterCreate_synonym(TSqlParser::Create_synonymContext * /*ctx*/) override;
        void exitCreate_synonym(TSqlParser::Create_synonymContext * /*ctx*/) override;

        void enterAlter_user(TSqlParser::Alter_userContext * /*ctx*/) override;
        void exitAlter_user(TSqlParser::Alter_userContext * /*ctx*/) override;

        void enterCreate_user(TSqlParser::Create_userContext * /*ctx*/) override;
        void exitCreate_user(TSqlParser::Create_userContext * /*ctx*/) override;

        void enterCreate_user_azure_sql_dw(TSqlParser::Create_user_azure_sql_dwContext * /*ctx*/) override;
        void exitCreate_user_azure_sql_dw(TSqlParser::Create_user_azure_sql_dwContext * /*ctx*/) override;

        void enterAlter_user_azure_sql(TSqlParser::Alter_user_azure_sqlContext * /*ctx*/) override;
        void exitAlter_user_azure_sql(TSqlParser::Alter_user_azure_sqlContext * /*ctx*/) override;

        void enterAlter_workload_group(TSqlParser::Alter_workload_groupContext * /*ctx*/) override;
        void exitAlter_workload_group(TSqlParser::Alter_workload_groupContext * /*ctx*/) override;

        void enterCreate_workload_group(TSqlParser::Create_workload_groupContext * /*ctx*/) override;
        void exitCreate_workload_group(TSqlParser::Create_workload_groupContext * /*ctx*/) override;

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

        void enterCreate_contract(TSqlParser::Create_contractContext * /*ctx*/) override;
        void exitCreate_contract(TSqlParser::Create_contractContext * /*ctx*/) override;

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

        void enterCreate_or_alter_trigger(TSqlParser::Create_or_alter_triggerContext * /*ctx*/) override;
        void exitCreate_or_alter_trigger(TSqlParser::Create_or_alter_triggerContext * /*ctx*/) override;

        void enterCreate_or_alter_dml_trigger(TSqlParser::Create_or_alter_dml_triggerContext * /*ctx*/) override;
        void exitCreate_or_alter_dml_trigger(TSqlParser::Create_or_alter_dml_triggerContext * /*ctx*/) override;

        void enterDml_trigger_option(TSqlParser::Dml_trigger_optionContext * /*ctx*/) override;
        void exitDml_trigger_option(TSqlParser::Dml_trigger_optionContext * /*ctx*/) override;

        void enterDml_trigger_operation(TSqlParser::Dml_trigger_operationContext * /*ctx*/) override;
        void exitDml_trigger_operation(TSqlParser::Dml_trigger_operationContext * /*ctx*/) override;

        void enterCreate_or_alter_ddl_trigger(TSqlParser::Create_or_alter_ddl_triggerContext * /*ctx*/) override;
        void exitCreate_or_alter_ddl_trigger(TSqlParser::Create_or_alter_ddl_triggerContext * /*ctx*/) override;

        void enterDdl_trigger_operation(TSqlParser::Ddl_trigger_operationContext * /*ctx*/) override;
        void exitDdl_trigger_operation(TSqlParser::Ddl_trigger_operationContext * /*ctx*/) override;

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

        void enterCreate_statistics(TSqlParser::Create_statisticsContext * /*ctx*/) override;
        void exitCreate_statistics(TSqlParser::Create_statisticsContext * /*ctx*/) override;

        void enterUpdate_statistics(TSqlParser::Update_statisticsContext * /*ctx*/) override;
        void exitUpdate_statistics(TSqlParser::Update_statisticsContext * /*ctx*/) override;

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

        void enterAlter_database(TSqlParser::Alter_databaseContext * /*ctx*/) override;
        void exitAlter_database(TSqlParser::Alter_databaseContext * /*ctx*/) override;

        void enterDatabase_optionspec(TSqlParser::Database_optionspecContext * /*ctx*/) override;
        void exitDatabase_optionspec(TSqlParser::Database_optionspecContext * /*ctx*/) override;

        void enterAuto_option(TSqlParser::Auto_optionContext * /*ctx*/) override;
        void exitAuto_option(TSqlParser::Auto_optionContext * /*ctx*/) override;

        void enterChange_tracking_option(TSqlParser::Change_tracking_optionContext * /*ctx*/) override;
        void exitChange_tracking_option(TSqlParser::Change_tracking_optionContext * /*ctx*/) override;

        void enterChange_tracking_option_list(TSqlParser::Change_tracking_option_listContext * /*ctx*/) override;
        void exitChange_tracking_option_list(TSqlParser::Change_tracking_option_listContext * /*ctx*/) override;

        void enterContainment_option(TSqlParser::Containment_optionContext * /*ctx*/) override;
        void exitContainment_option(TSqlParser::Containment_optionContext * /*ctx*/) override;

        void enterCursor_option(TSqlParser::Cursor_optionContext * /*ctx*/) override;
        void exitCursor_option(TSqlParser::Cursor_optionContext * /*ctx*/) override;

        void enterAlter_endpoint(TSqlParser::Alter_endpointContext * /*ctx*/) override;
        void exitAlter_endpoint(TSqlParser::Alter_endpointContext * /*ctx*/) override;

        void enterDatabase_mirroring_option(TSqlParser::Database_mirroring_optionContext * /*ctx*/) override;
        void exitDatabase_mirroring_option(TSqlParser::Database_mirroring_optionContext * /*ctx*/) override;

        void enterMirroring_set_option(TSqlParser::Mirroring_set_optionContext * /*ctx*/) override;
        void exitMirroring_set_option(TSqlParser::Mirroring_set_optionContext * /*ctx*/) override;

        void enterMirroring_partner(TSqlParser::Mirroring_partnerContext * /*ctx*/) override;
        void exitMirroring_partner(TSqlParser::Mirroring_partnerContext * /*ctx*/) override;

        void enterMirroring_witness(TSqlParser::Mirroring_witnessContext * /*ctx*/) override;
        void exitMirroring_witness(TSqlParser::Mirroring_witnessContext * /*ctx*/) override;

        void enterWitness_partner_equal(TSqlParser::Witness_partner_equalContext * /*ctx*/) override;
        void exitWitness_partner_equal(TSqlParser::Witness_partner_equalContext * /*ctx*/) override;

        void enterPartner_option(TSqlParser::Partner_optionContext * /*ctx*/) override;
        void exitPartner_option(TSqlParser::Partner_optionContext * /*ctx*/) override;

        void enterWitness_option(TSqlParser::Witness_optionContext * /*ctx*/) override;
        void exitWitness_option(TSqlParser::Witness_optionContext * /*ctx*/) override;

        void enterWitness_server(TSqlParser::Witness_serverContext * /*ctx*/) override;
        void exitWitness_server(TSqlParser::Witness_serverContext * /*ctx*/) override;

        void enterPartner_server(TSqlParser::Partner_serverContext * /*ctx*/) override;
        void exitPartner_server(TSqlParser::Partner_serverContext * /*ctx*/) override;

        void enterMirroring_host_port_seperator(TSqlParser::Mirroring_host_port_seperatorContext * /*ctx*/) override;
        void exitMirroring_host_port_seperator(TSqlParser::Mirroring_host_port_seperatorContext * /*ctx*/) override;

        void enterPartner_server_tcp_prefix(TSqlParser::Partner_server_tcp_prefixContext * /*ctx*/) override;
        void exitPartner_server_tcp_prefix(TSqlParser::Partner_server_tcp_prefixContext * /*ctx*/) override;

        void enterPort_number(TSqlParser::Port_numberContext * /*ctx*/) override;
        void exitPort_number(TSqlParser::Port_numberContext * /*ctx*/) override;

        void enterHost(TSqlParser::HostContext * /*ctx*/) override;
        void exitHost(TSqlParser::HostContext * /*ctx*/) override;

        void enterDate_correlation_optimization_option(TSqlParser::Date_correlation_optimization_optionContext * /*ctx*/) override;
        void exitDate_correlation_optimization_option(TSqlParser::Date_correlation_optimization_optionContext * /*ctx*/) override;

        void enterDb_encryption_option(TSqlParser::Db_encryption_optionContext * /*ctx*/) override;
        void exitDb_encryption_option(TSqlParser::Db_encryption_optionContext * /*ctx*/) override;

        void enterDb_state_option(TSqlParser::Db_state_optionContext * /*ctx*/) override;
        void exitDb_state_option(TSqlParser::Db_state_optionContext * /*ctx*/) override;

        void enterDb_update_option(TSqlParser::Db_update_optionContext * /*ctx*/) override;
        void exitDb_update_option(TSqlParser::Db_update_optionContext * /*ctx*/) override;

        void enterDb_user_access_option(TSqlParser::Db_user_access_optionContext * /*ctx*/) override;
        void exitDb_user_access_option(TSqlParser::Db_user_access_optionContext * /*ctx*/) override;

        void enterDelayed_durability_option(TSqlParser::Delayed_durability_optionContext * /*ctx*/) override;
        void exitDelayed_durability_option(TSqlParser::Delayed_durability_optionContext * /*ctx*/) override;

        void enterExternal_access_option(TSqlParser::External_access_optionContext * /*ctx*/) override;
        void exitExternal_access_option(TSqlParser::External_access_optionContext * /*ctx*/) override;

        void enterHadr_options(TSqlParser::Hadr_optionsContext * /*ctx*/) override;
        void exitHadr_options(TSqlParser::Hadr_optionsContext * /*ctx*/) override;

        void enterMixed_page_allocation_option(TSqlParser::Mixed_page_allocation_optionContext * /*ctx*/) override;
        void exitMixed_page_allocation_option(TSqlParser::Mixed_page_allocation_optionContext * /*ctx*/) override;

        void enterParameterization_option(TSqlParser::Parameterization_optionContext * /*ctx*/) override;
        void exitParameterization_option(TSqlParser::Parameterization_optionContext * /*ctx*/) override;

        void enterRecovery_option(TSqlParser::Recovery_optionContext * /*ctx*/) override;
        void exitRecovery_option(TSqlParser::Recovery_optionContext * /*ctx*/) override;

        void enterService_broker_option(TSqlParser::Service_broker_optionContext * /*ctx*/) override;
        void exitService_broker_option(TSqlParser::Service_broker_optionContext * /*ctx*/) override;

        void enterSnapshot_option(TSqlParser::Snapshot_optionContext * /*ctx*/) override;
        void exitSnapshot_option(TSqlParser::Snapshot_optionContext * /*ctx*/) override;

        void enterSql_option(TSqlParser::Sql_optionContext * /*ctx*/) override;
        void exitSql_option(TSqlParser::Sql_optionContext * /*ctx*/) override;

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

        void enterDrop_trigger(TSqlParser::Drop_triggerContext * /*ctx*/) override;
        void exitDrop_trigger(TSqlParser::Drop_triggerContext * /*ctx*/) override;

        void enterDrop_dml_trigger(TSqlParser::Drop_dml_triggerContext * /*ctx*/) override;
        void exitDrop_dml_trigger(TSqlParser::Drop_dml_triggerContext * /*ctx*/) override;

        void enterDrop_ddl_trigger(TSqlParser::Drop_ddl_triggerContext * /*ctx*/) override;
        void exitDrop_ddl_trigger(TSqlParser::Drop_ddl_triggerContext * /*ctx*/) override;

        void enterDrop_function(TSqlParser::Drop_functionContext * /*ctx*/) override;
        void exitDrop_function(TSqlParser::Drop_functionContext * /*ctx*/) override;

        void enterDrop_statistics(TSqlParser::Drop_statisticsContext * /*ctx*/) override;
        void exitDrop_statistics(TSqlParser::Drop_statisticsContext * /*ctx*/) override;

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

        void enterBackup_database(TSqlParser::Backup_databaseContext * /*ctx*/) override;
        void exitBackup_database(TSqlParser::Backup_databaseContext * /*ctx*/) override;

        void enterBackup_log(TSqlParser::Backup_logContext * /*ctx*/) override;
        void exitBackup_log(TSqlParser::Backup_logContext * /*ctx*/) override;

        void enterBackup_certificate(TSqlParser::Backup_certificateContext * /*ctx*/) override;
        void exitBackup_certificate(TSqlParser::Backup_certificateContext * /*ctx*/) override;

        void enterBackup_master_key(TSqlParser::Backup_master_keyContext * /*ctx*/) override;
        void exitBackup_master_key(TSqlParser::Backup_master_keyContext * /*ctx*/) override;

        void enterBackup_service_master_key(TSqlParser::Backup_service_master_keyContext * /*ctx*/) override;
        void exitBackup_service_master_key(TSqlParser::Backup_service_master_keyContext * /*ctx*/) override;

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

        void enterSecurity_statement(TSqlParser::Security_statementContext * /*ctx*/) override;
        void exitSecurity_statement(TSqlParser::Security_statementContext * /*ctx*/) override;

        void enterCreate_certificate(TSqlParser::Create_certificateContext * /*ctx*/) override;
        void exitCreate_certificate(TSqlParser::Create_certificateContext * /*ctx*/) override;

        void enterExisting_keys(TSqlParser::Existing_keysContext * /*ctx*/) override;
        void exitExisting_keys(TSqlParser::Existing_keysContext * /*ctx*/) override;

        void enterPrivate_key_options(TSqlParser::Private_key_optionsContext * /*ctx*/) override;
        void exitPrivate_key_options(TSqlParser::Private_key_optionsContext * /*ctx*/) override;

        void enterGenerate_new_keys(TSqlParser::Generate_new_keysContext * /*ctx*/) override;
        void exitGenerate_new_keys(TSqlParser::Generate_new_keysContext * /*ctx*/) override;

        void enterDate_options(TSqlParser::Date_optionsContext * /*ctx*/) override;
        void exitDate_options(TSqlParser::Date_optionsContext * /*ctx*/) override;

        void enterOpen_key(TSqlParser::Open_keyContext * /*ctx*/) override;
        void exitOpen_key(TSqlParser::Open_keyContext * /*ctx*/) override;

        void enterClose_key(TSqlParser::Close_keyContext * /*ctx*/) override;
        void exitClose_key(TSqlParser::Close_keyContext * /*ctx*/) override;

        void enterCreate_key(TSqlParser::Create_keyContext * /*ctx*/) override;
        void exitCreate_key(TSqlParser::Create_keyContext * /*ctx*/) override;

        void enterKey_options(TSqlParser::Key_optionsContext * /*ctx*/) override;
        void exitKey_options(TSqlParser::Key_optionsContext * /*ctx*/) override;

        void enterAlgorithm(TSqlParser::AlgorithmContext * /*ctx*/) override;
        void exitAlgorithm(TSqlParser::AlgorithmContext * /*ctx*/) override;

        void enterEncryption_mechanism(TSqlParser::Encryption_mechanismContext * /*ctx*/) override;
        void exitEncryption_mechanism(TSqlParser::Encryption_mechanismContext * /*ctx*/) override;

        void enterDecryption_mechanism(TSqlParser::Decryption_mechanismContext * /*ctx*/) override;
        void exitDecryption_mechanism(TSqlParser::Decryption_mechanismContext * /*ctx*/) override;

        void enterGrant_permission(TSqlParser::Grant_permissionContext * /*ctx*/) override;
        void exitGrant_permission(TSqlParser::Grant_permissionContext * /*ctx*/) override;

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
