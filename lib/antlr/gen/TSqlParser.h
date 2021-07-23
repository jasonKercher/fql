
// Generated from TSqlParser.g4 by ANTLR 4.9.2

#pragma once


#include "antlr4-runtime.h"




class  TSqlParser : public antlr4::Parser {
public:
  enum {
    ABS = 1, ASCII = 2, CEILING = 3, CHAR = 4, CHARINDEX = 5, DATALENGTH = 6, 
    DAY = 7, FLOOR = 8, ISDATE = 9, ISNUMERIC = 10, LEN = 11, LOWER = 12, 
    LTRIM = 13, MONTH = 14, NCHAR = 15, PATINDEX = 16, RAND = 17, REPLACE = 18, 
    ROUND = 19, RTRIM = 20, SIGN = 21, SPACE_fn = 22, STR = 23, SUBSTRING = 24, 
    UPPER = 25, USER_NAME = 26, YEAR = 27, TEXT = 28, NTEXT = 29, ADD = 30, 
    ALL = 31, ALTER = 32, AND = 33, ANY = 34, AS = 35, ASC = 36, AUTHORIZATION = 37, 
    BACKSLASH = 38, BEGIN = 39, BETWEEN = 40, BREAK = 41, BY = 42, CASE = 43, 
    CHECK = 44, CLOSE = 45, COALESCE = 46, COLLATE = 47, COLUMN = 48, COMMIT = 49, 
    COMPUTE = 50, CONSTRAINT = 51, CONTINUE = 52, CONVERT = 53, CREATE = 54, 
    CROSS = 55, CURRENT = 56, CURRENT_DATE = 57, CURRENT_TIME = 58, CURRENT_TIMESTAMP = 59, 
    CURRENT_USER = 60, CURSOR = 61, DATABASE = 62, DBCC = 63, DEALLOCATE = 64, 
    DECLARE = 65, DEFAULT = 66, DELETE = 67, DESC = 68, DISTINCT = 69, DISTRIBUTED = 70, 
    DOUBLE = 71, DOUBLE_BACK_SLASH = 72, DOUBLE_FORWARD_SLASH = 73, DROP = 74, 
    ELSE = 75, END = 76, ESCAPE = 77, EXCEPT = 78, EXECUTE = 79, EXISTS = 80, 
    FETCH = 81, FOR = 82, FOREIGN = 83, FROM = 84, FULL = 85, FUNCTION = 86, 
    GOTO = 87, GROUP = 88, HAVING = 89, HOLDLOCK = 90, IDENTITY = 91, IDENTITY_INSERT = 92, 
    IF = 93, IN = 94, INDEX = 95, INNER = 96, INSERT = 97, INTERSECT = 98, 
    INTO = 99, IS = 100, JOIN = 101, KEY = 102, KILL = 103, LEFT = 104, 
    LIKE = 105, MERGE = 106, NOT = 107, NULL_ = 108, NULLIF = 109, OF = 110, 
    OFF = 111, ON = 112, OPEN = 113, OPTION = 114, OR = 115, ORDER = 116, 
    OUTER = 117, OVER = 118, PERCENT = 119, PIVOT = 120, PLAN = 121, PRECISION = 122, 
    PRIMARY = 123, PRINT = 124, PROC = 125, PROCEDURE = 126, RAISERROR = 127, 
    READ = 128, REFERENCES = 129, REPLICATION = 130, RETURN = 131, REVERT = 132, 
    RIGHT = 133, ROLLBACK = 134, ROWCOUNT = 135, ROWGUIDCOL = 136, SAVE = 137, 
    SCHEMA = 138, SELECT = 139, SESSION_USER = 140, SET = 141, SHUTDOWN = 142, 
    SOME = 143, SYSTEM_USER = 144, TABLE = 145, THEN = 146, TO = 147, TOP = 148, 
    TRAN = 149, TRANSACTION = 150, TRUNCATE = 151, UNION = 152, UNIQUE = 153, 
    UNPIVOT = 154, UPDATE = 155, USE = 156, USER = 157, VALUES = 158, VARYING = 159, 
    WHEN = 160, WHERE = 161, WHILE = 162, WITH = 163, WITHIN = 164, DOLLAR_PARTITION = 165, 
    ABSOLUTE = 166, AT_KEYWORD = 167, ACTION = 168, AES_128 = 169, AES_192 = 170, 
    AES_256 = 171, ALGORITHM = 172, ANSI_NULLS = 173, ANSI_PADDING = 174, 
    ANSI_WARNINGS = 175, APPLY = 176, ARITHABORT = 177, AUTO = 178, AVG = 179, 
    BIGINT = 180, BINARY_KEYWORD = 181, BINARY_BASE64 = 182, BINARY_CHECKSUM = 183, 
    CALLER = 184, CAST = 185, TRY_CAST = 186, CATCH = 187, CHECKSUM = 188, 
    CHECKSUM_AGG = 189, COMMITTED = 190, CONCAT = 191, CONCAT_NULL_YIELDS_NULL = 192, 
    COUNT = 193, COUNT_BIG = 194, CUME_DIST = 195, CURSOR_CLOSE_ON_COMMIT = 196, 
    CURSOR_DEFAULT = 197, DATA = 198, DATEADD = 199, DATEDIFF = 200, DATENAME = 201, 
    DATEPART = 202, DAYS = 203, DEFAULT_DOUBLE_QUOTE = 204, DELAYED_DURABILITY = 205, 
    DELETED = 206, DENSE_RANK = 207, DES = 208, DESCRIPTION = 209, DESX = 210, 
    DISABLE = 211, DYNAMIC = 212, ELEMENTS = 213, EMPTY = 214, ENCRYPTION = 215, 
    EXCLUSIVE = 216, FAST = 217, FAST_FORWARD = 218, FIRST = 219, FIRST_VALUE = 220, 
    FOLLOWING = 221, FORCE = 222, FORCED = 223, FORWARD_ONLY = 224, GETDATE = 225, 
    GETUTCDATE = 226, GLOBAL = 227, GO_BATCH = 228, GO = 229, GROUPING = 230, 
    GROUPING_ID = 231, HASH = 232, HIGH = 233, HOURS = 234, IDENTITY_VALUE = 235, 
    INCLUDE_NULL_VALUES = 236, INPUT = 237, INSENSITIVE = 238, INSERTED = 239, 
    INT = 240, ISOLATION = 241, KEEP = 242, KEEPFIXED = 243, KEYSET = 244, 
    LAG = 245, LAST = 246, LAST_VALUE = 247, LEAD = 248, LEVEL = 249, LOCAL = 250, 
    LOCATION = 251, LOCK = 252, LOCK_ESCALATION = 253, LOOP = 254, LOW = 255, 
    MANUAL = 256, MARK = 257, MAX = 258, MAXDOP = 259, MAXRECURSION = 260, 
    MIN = 261, MIN_ACTIVE_ROWVERSION = 262, MINUTES = 263, MODE = 264, NEXT = 265, 
    NO = 266, NOCOUNT = 267, NODES = 268, NOEXPAND = 269, NOWAIT = 270, 
    NTILE = 271, NUMANODE = 272, NUMERIC_ROUNDABORT = 273, OBJECT = 274, 
    OFFSET = 275, ONLINE = 276, ONLY = 277, OPTIMISTIC = 278, OPTIMIZE = 279, 
    OUT = 280, OUTPUT = 281, OWNER = 282, PARAMETERIZATION = 283, PARTITION = 284, 
    PARTITIONS = 285, PERCENT_RANK = 286, PERCENTILE_CONT = 287, PERCENTILE_DISC = 288, 
    PRECEDING = 289, PRIOR = 290, QUERY = 291, QUOTED_IDENTIFIER = 292, 
    RANGE = 293, RANK = 294, RC2 = 295, RC4 = 296, RC4_128 = 297, READ_ONLY = 298, 
    READONLY = 299, REBUILD = 300, RECOMPILE = 301, RELATIVE = 302, REMOTE = 303, 
    REPEATABLE = 304, ROBUST = 305, ROW = 306, ROW_NUMBER = 307, ROWGUID = 308, 
    ROWS = 309, SCHEMABINDING = 310, SCROLL = 311, SCROLL_LOCKS = 312, SECONDS = 313, 
    SELF = 314, SEMI_SENSITIVE = 315, SEQUENCE = 316, SERIALIZABLE = 317, 
    SETERROR = 318, SHARE = 319, SIMPLE = 320, SIZE = 321, SMALLINT = 322, 
    SNAPSHOT = 323, SPATIAL_WINDOW_MAX_CELLS = 324, STATIC = 325, STATUSONLY = 326, 
    STDEV = 327, STDEVP = 328, STRING_AGG = 329, STUFF = 330, SUBSCRIPTION = 331, 
    SUM = 332, TEXTIMAGE_ON = 333, THROW = 334, TIES = 335, TIME = 336, 
    TINYINT = 337, TRIPLE_DES = 338, TRIPLE_DES_3KEY = 339, TRY = 340, TWO_DIGIT_YEAR_CUTOFF = 341, 
    TYPE = 342, TYPE_WARNING = 343, UNBOUNDED = 344, UNCOMMITTED = 345, 
    UNKNOWN = 346, UOW = 347, USING = 348, VAR = 349, VARBINARY_KEYWORD = 350, 
    VARP = 351, WAIT = 352, WORK = 353, ZONE = 354, DOLLAR_ACTION = 355, 
    BLOCKING_HIERARCHY = 356, CACHE = 357, CALLED = 358, CHANGETABLE = 359, 
    CHANGES = 360, CONNECTION = 361, CONTRACT = 362, CYCLE = 363, DATA_COMPRESSION = 364, 
    DDL = 365, ERROR = 366, EVENT = 367, FORCESEEK = 368, IIF = 369, INCREMENT = 370, 
    ISNULL = 371, LOG = 372, MATCHED = 373, MAXVALUE = 374, MINVALUE = 375, 
    NOTIFICATION = 376, PERSISTED = 377, PREDICATE = 378, RESTART = 379, 
    RETURNS = 380, SHRINKLOG = 381, SOURCE = 382, SQL = 383, STATE = 384, 
    START = 385, TARGET = 386, WITHOUT = 387, VARCHAR = 388, NVARCHAR = 389, 
    SPACE = 390, COMMENT = 391, LINE_COMMENT = 392, DOUBLE_QUOTE_ID = 393, 
    SINGLE_QUOTE = 394, SQUARE_BRACKET_ID = 395, LOCAL_ID = 396, DECIMAL = 397, 
    ID = 398, STRING = 399, BINARY = 400, FLOAT = 401, REAL = 402, EQUAL = 403, 
    GREATER = 404, LESS = 405, EXCLAMATION = 406, PLUS_ASSIGN = 407, MINUS_ASSIGN = 408, 
    MULT_ASSIGN = 409, DIV_ASSIGN = 410, MOD_ASSIGN = 411, AND_ASSIGN = 412, 
    XOR_ASSIGN = 413, OR_ASSIGN = 414, DOUBLE_BAR = 415, DOT = 416, UNDERLINE = 417, 
    AT = 418, SHARP = 419, DOLLAR = 420, LR_BRACKET = 421, RR_BRACKET = 422, 
    COMMA = 423, SEMI = 424, COLON = 425, DOUBLE_COLON = 426, STAR = 427, 
    DIVIDE = 428, MODULE = 429, PLUS = 430, MINUS = 431, BIT_NOT = 432, 
    BIT_OR = 433, BIT_AND = 434, BIT_XOR = 435, ANSI_DEFAULTS = 436, ANSI_NULL_DFLT_OFF = 437, 
    ANSI_NULL_DFLT_ON = 438, ARITHIGNORE = 439, FMTONLY = 440, FORCEPLAN = 441, 
    IMPLICIT_TRANSACTIONS = 442, NOEXEC = 443, PARSEONLY = 444, REMOTE_PROC_TRANSACTIONS = 445, 
    SHOWPLAN_ALL = 446, SHOWPLAN_TEXT = 447, SHOWPLAN_XML = 448, XACT_ABORT = 449, 
    VERSION = 450
  };

  enum {
    RuleTsql_file = 0, RuleBatch = 1, RuleBatch_level_statement = 2, RuleSql_clauses = 3, 
    RuleDml_clause = 4, RuleDdl_clause = 5, RuleCfl_statement = 6, RuleBlock_statement = 7, 
    RuleBreak_statement = 8, RuleContinue_statement = 9, RuleGoto_statement = 10, 
    RuleReturn_statement = 11, RuleIf_statement = 12, RuleThrow_statement = 13, 
    RuleThrow_error_number = 14, RuleThrow_message = 15, RuleThrow_state = 16, 
    RuleTry_catch_statement = 17, RuleWhile_statement = 18, RulePrint_statement = 19, 
    RuleRaiseerror_statement = 20, RuleEmpty_statement = 21, RuleAnother_statement = 22, 
    RuleEntity_to = 23, RuleColon_colon = 24, RuleClass_type = 25, RuleClass_type_for_sql_database = 26, 
    RuleClass_type_for_azure_dw = 27, RuleClass_type_for_parallel_dw = 28, 
    RuleLock_table = 29, RuleTruncate_table = 30, RuleAlter_sequence = 31, 
    RuleCreate_sequence = 32, RuleMerge_statement = 33, RuleWhen_matches = 34, 
    RuleMerge_matched = 35, RuleMerge_not_matched = 36, RuleDelete_statement = 37, 
    RuleDelete_statement_from = 38, RuleInsert_statement = 39, RuleInsert_statement_value = 40, 
    RuleSelect_statement_standalone = 41, RuleSelect_statement = 42, RuleTime = 43, 
    RuleUpdate_statement = 44, RuleOutput_clause = 45, RuleOutput_dml_list_elem = 46, 
    RuleCreate_or_alter_procedure = 47, RuleCreate_or_alter_function = 48, 
    RuleFunc_body_returns_select = 49, RuleFunc_body_returns_table = 50, 
    RuleFunc_body_returns_scalar = 51, RuleProcedure_param = 52, RuleProcedure_option = 53, 
    RuleFunction_option = 54, RuleCreate_table = 55, RuleTable_options = 56, 
    RuleAlter_table = 57, RuleDatabase_optionspec = 58, RuleCursor_option = 59, 
    RuleDrop_procedure = 60, RuleDrop_function = 61, RuleDrop_table = 62, 
    RuleDeclare_statement = 63, RuleCursor_statement = 64, RuleKill_statement = 65, 
    RuleKill_process = 66, RuleKill_query_notification = 67, RuleExecute_statement = 68, 
    RuleExecute_body_batch = 69, RuleExecute_body = 70, RuleExecute_statement_arg = 71, 
    RuleExecute_statement_arg_named = 72, RuleExecute_statement_arg_unnamed = 73, 
    RuleExecute_parameter = 74, RuleExecute_var_string = 75, RuleAlgorithm = 76, 
    RuleSet_statement = 77, RuleTransaction_statement = 78, RuleGo_batch_statement = 79, 
    RuleGo_statement = 80, RuleUse_statement = 81, RuleShutdown_statement = 82, 
    RuleDbcc_special = 83, RuleDbcc_clause = 84, RuleDbcc_command = 85, 
    RuleDbcc_options = 86, RuleExecute_clause = 87, RuleDeclare_local = 88, 
    RuleTable_type_definition = 89, RuleColumn_def_table_constraints = 90, 
    RuleColumn_def_table_constraint = 91, RuleColumn_definition = 92, RuleColumn_constraint = 93, 
    RuleTable_constraint = 94, RuleIndex_options = 95, RuleIndex_option = 96, 
    RuleDeclare_cursor = 97, RuleDeclare_set_cursor_common = 98, RuleDeclare_set_cursor_common_partial = 99, 
    RuleFetch_cursor = 100, RuleSet_special = 101, RuleConstant_LOCAL_ID = 102, 
    RuleExpression = 103, RuleTime_zone = 104, RulePrimitive_expression = 105, 
    RuleCase_expression = 106, RuleUnary_operator_expression = 107, RuleBracket_expression = 108, 
    RuleConstant_expression = 109, RuleSubquery = 110, RuleWith_expression = 111, 
    RuleCommon_table_expression = 112, RuleUpdate_elem = 113, RuleUpdate_elem_merge = 114, 
    RuleSearch_condition = 115, RulePredicate = 116, RuleQuery_expression = 117, 
    RuleSql_union = 118, RuleQuery_specification = 119, RuleTop_clause = 120, 
    RuleTop_percent = 121, RuleTop_count = 122, RuleOrder_by_clause = 123, 
    RuleOrder_by_expression = 124, RuleGroup_by_item = 125, RuleOption_clause = 126, 
    RuleOption = 127, RuleOptimize_for_arg = 128, RuleSelect_list = 129, 
    RuleUdt_method_arguments = 130, RuleAsterisk = 131, RuleColumn_elem = 132, 
    RuleUdt_elem = 133, RuleExpression_elem = 134, RuleSelect_list_elem = 135, 
    RuleTable_sources = 136, RuleTable_source = 137, RuleTable_source_item_joined = 138, 
    RuleTable_source_item = 139, RuleSchema_declaration = 140, RuleColumn_declaration = 141, 
    RuleChange_table = 142, RuleChange_table_changes = 143, RuleChange_table_version = 144, 
    RuleJoin_part = 145, RuleJoin_on = 146, RuleCross_join = 147, RuleApply_ = 148, 
    RulePivot = 149, RuleUnpivot = 150, RulePivot_clause = 151, RuleUnpivot_clause = 152, 
    RuleFull_column_name_list = 153, RuleTable_name_with_hint = 154, RuleBulk_option = 155, 
    RuleDerived_table = 156, RuleFunction_call = 157, RulePartition_function = 158, 
    RuleBuilt_in_functions = 159, RuleNodes_method = 160, RuleSwitch_section = 161, 
    RuleSwitch_search_condition_section = 162, RuleAs_column_alias = 163, 
    RuleAs_table_alias = 164, RuleTable_alias = 165, RuleWith_table_hints = 166, 
    RuleInsert_with_table_hints = 167, RuleTable_hint = 168, RuleIndex_value = 169, 
    RuleColumn_alias_list = 170, RuleColumn_alias = 171, RuleTable_value_constructor = 172, 
    RuleExpression_list = 173, RuleRanking_windowed_function = 174, RuleAggregate_windowed_function = 175, 
    RuleAnalytic_windowed_function = 176, RuleAll_distinct_expression = 177, 
    RuleOver_clause = 178, RuleRow_or_range_clause = 179, RuleWindow_frame_extent = 180, 
    RuleWindow_frame_bound = 181, RuleWindow_frame_preceding = 182, RuleWindow_frame_following = 183, 
    RuleEntity_name = 184, RuleEntity_name_for_azure_dw = 185, RuleEntity_name_for_parallel_dw = 186, 
    RuleFull_table_name = 187, RuleTable_name = 188, RuleSimple_name = 189, 
    RuleFunc_proc_name_schema = 190, RuleFunc_proc_name_database_schema = 191, 
    RuleFunc_proc_name_server_database_schema = 192, RuleDdl_object = 193, 
    RuleFull_column_name = 194, RuleColumn_name_list_with_order = 195, RuleInsert_column_name_list = 196, 
    RuleInsert_column_id = 197, RuleColumn_name_list = 198, RuleCursor_name = 199, 
    RuleOn_off = 200, RuleNull_notnull = 201, RuleNull_or_default = 202, 
    RuleScalar_function_name = 203, RuleData_type = 204, RuleDefault_value = 205, 
    RuleConstant = 206, RuleSign = 207, RuleKeyword = 208, RuleId_ = 209, 
    RuleSimple_id = 210, RuleId_or_string = 211, RuleComparison_operator = 212, 
    RuleAssignment_operator = 213
  };

  explicit TSqlParser(antlr4::TokenStream *input);
  ~TSqlParser();

  virtual std::string getGrammarFileName() const override;
  virtual const antlr4::atn::ATN& getATN() const override { return _atn; };
  virtual const std::vector<std::string>& getTokenNames() const override { return _tokenNames; }; // deprecated: use vocabulary instead.
  virtual const std::vector<std::string>& getRuleNames() const override;
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;


  class Tsql_fileContext;
  class BatchContext;
  class Batch_level_statementContext;
  class Sql_clausesContext;
  class Dml_clauseContext;
  class Ddl_clauseContext;
  class Cfl_statementContext;
  class Block_statementContext;
  class Break_statementContext;
  class Continue_statementContext;
  class Goto_statementContext;
  class Return_statementContext;
  class If_statementContext;
  class Throw_statementContext;
  class Throw_error_numberContext;
  class Throw_messageContext;
  class Throw_stateContext;
  class Try_catch_statementContext;
  class While_statementContext;
  class Print_statementContext;
  class Raiseerror_statementContext;
  class Empty_statementContext;
  class Another_statementContext;
  class Entity_toContext;
  class Colon_colonContext;
  class Class_typeContext;
  class Class_type_for_sql_databaseContext;
  class Class_type_for_azure_dwContext;
  class Class_type_for_parallel_dwContext;
  class Lock_tableContext;
  class Truncate_tableContext;
  class Alter_sequenceContext;
  class Create_sequenceContext;
  class Merge_statementContext;
  class When_matchesContext;
  class Merge_matchedContext;
  class Merge_not_matchedContext;
  class Delete_statementContext;
  class Delete_statement_fromContext;
  class Insert_statementContext;
  class Insert_statement_valueContext;
  class Select_statement_standaloneContext;
  class Select_statementContext;
  class TimeContext;
  class Update_statementContext;
  class Output_clauseContext;
  class Output_dml_list_elemContext;
  class Create_or_alter_procedureContext;
  class Create_or_alter_functionContext;
  class Func_body_returns_selectContext;
  class Func_body_returns_tableContext;
  class Func_body_returns_scalarContext;
  class Procedure_paramContext;
  class Procedure_optionContext;
  class Function_optionContext;
  class Create_tableContext;
  class Table_optionsContext;
  class Alter_tableContext;
  class Database_optionspecContext;
  class Cursor_optionContext;
  class Drop_procedureContext;
  class Drop_functionContext;
  class Drop_tableContext;
  class Declare_statementContext;
  class Cursor_statementContext;
  class Kill_statementContext;
  class Kill_processContext;
  class Kill_query_notificationContext;
  class Execute_statementContext;
  class Execute_body_batchContext;
  class Execute_bodyContext;
  class Execute_statement_argContext;
  class Execute_statement_arg_namedContext;
  class Execute_statement_arg_unnamedContext;
  class Execute_parameterContext;
  class Execute_var_stringContext;
  class AlgorithmContext;
  class Set_statementContext;
  class Transaction_statementContext;
  class Go_batch_statementContext;
  class Go_statementContext;
  class Use_statementContext;
  class Shutdown_statementContext;
  class Dbcc_specialContext;
  class Dbcc_clauseContext;
  class Dbcc_commandContext;
  class Dbcc_optionsContext;
  class Execute_clauseContext;
  class Declare_localContext;
  class Table_type_definitionContext;
  class Column_def_table_constraintsContext;
  class Column_def_table_constraintContext;
  class Column_definitionContext;
  class Column_constraintContext;
  class Table_constraintContext;
  class Index_optionsContext;
  class Index_optionContext;
  class Declare_cursorContext;
  class Declare_set_cursor_commonContext;
  class Declare_set_cursor_common_partialContext;
  class Fetch_cursorContext;
  class Set_specialContext;
  class Constant_LOCAL_IDContext;
  class ExpressionContext;
  class Time_zoneContext;
  class Primitive_expressionContext;
  class Case_expressionContext;
  class Unary_operator_expressionContext;
  class Bracket_expressionContext;
  class Constant_expressionContext;
  class SubqueryContext;
  class With_expressionContext;
  class Common_table_expressionContext;
  class Update_elemContext;
  class Update_elem_mergeContext;
  class Search_conditionContext;
  class PredicateContext;
  class Query_expressionContext;
  class Sql_unionContext;
  class Query_specificationContext;
  class Top_clauseContext;
  class Top_percentContext;
  class Top_countContext;
  class Order_by_clauseContext;
  class Order_by_expressionContext;
  class Group_by_itemContext;
  class Option_clauseContext;
  class OptionContext;
  class Optimize_for_argContext;
  class Select_listContext;
  class Udt_method_argumentsContext;
  class AsteriskContext;
  class Column_elemContext;
  class Udt_elemContext;
  class Expression_elemContext;
  class Select_list_elemContext;
  class Table_sourcesContext;
  class Table_sourceContext;
  class Table_source_item_joinedContext;
  class Table_source_itemContext;
  class Schema_declarationContext;
  class Column_declarationContext;
  class Change_tableContext;
  class Change_table_changesContext;
  class Change_table_versionContext;
  class Join_partContext;
  class Join_onContext;
  class Cross_joinContext;
  class Apply_Context;
  class PivotContext;
  class UnpivotContext;
  class Pivot_clauseContext;
  class Unpivot_clauseContext;
  class Full_column_name_listContext;
  class Table_name_with_hintContext;
  class Bulk_optionContext;
  class Derived_tableContext;
  class Function_callContext;
  class Partition_functionContext;
  class Built_in_functionsContext;
  class Nodes_methodContext;
  class Switch_sectionContext;
  class Switch_search_condition_sectionContext;
  class As_column_aliasContext;
  class As_table_aliasContext;
  class Table_aliasContext;
  class With_table_hintsContext;
  class Insert_with_table_hintsContext;
  class Table_hintContext;
  class Index_valueContext;
  class Column_alias_listContext;
  class Column_aliasContext;
  class Table_value_constructorContext;
  class Expression_listContext;
  class Ranking_windowed_functionContext;
  class Aggregate_windowed_functionContext;
  class Analytic_windowed_functionContext;
  class All_distinct_expressionContext;
  class Over_clauseContext;
  class Row_or_range_clauseContext;
  class Window_frame_extentContext;
  class Window_frame_boundContext;
  class Window_frame_precedingContext;
  class Window_frame_followingContext;
  class Entity_nameContext;
  class Entity_name_for_azure_dwContext;
  class Entity_name_for_parallel_dwContext;
  class Full_table_nameContext;
  class Table_nameContext;
  class Simple_nameContext;
  class Func_proc_name_schemaContext;
  class Func_proc_name_database_schemaContext;
  class Func_proc_name_server_database_schemaContext;
  class Ddl_objectContext;
  class Full_column_nameContext;
  class Column_name_list_with_orderContext;
  class Insert_column_name_listContext;
  class Insert_column_idContext;
  class Column_name_listContext;
  class Cursor_nameContext;
  class On_offContext;
  class Null_notnullContext;
  class Null_or_defaultContext;
  class Scalar_function_nameContext;
  class Data_typeContext;
  class Default_valueContext;
  class ConstantContext;
  class SignContext;
  class KeywordContext;
  class Id_Context;
  class Simple_idContext;
  class Id_or_stringContext;
  class Comparison_operatorContext;
  class Assignment_operatorContext; 

  class  Tsql_fileContext : public antlr4::ParserRuleContext {
  public:
    Tsql_fileContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EOF();
    std::vector<BatchContext *> batch();
    BatchContext* batch(size_t i);
    Execute_body_batchContext *execute_body_batch();
    std::vector<Go_batch_statementContext *> go_batch_statement();
    Go_batch_statementContext* go_batch_statement(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Tsql_fileContext* tsql_file();

  class  BatchContext : public antlr4::ParserRuleContext {
  public:
    BatchContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Go_batch_statementContext *go_batch_statement();
    Execute_body_batchContext *execute_body_batch();
    std::vector<Go_statementContext *> go_statement();
    Go_statementContext* go_statement(size_t i);
    std::vector<Sql_clausesContext *> sql_clauses();
    Sql_clausesContext* sql_clauses(size_t i);
    Batch_level_statementContext *batch_level_statement();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  BatchContext* batch();

  class  Batch_level_statementContext : public antlr4::ParserRuleContext {
  public:
    Batch_level_statementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Create_or_alter_functionContext *create_or_alter_function();
    Create_or_alter_procedureContext *create_or_alter_procedure();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Batch_level_statementContext* batch_level_statement();

  class  Sql_clausesContext : public antlr4::ParserRuleContext {
  public:
    Sql_clausesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Dml_clauseContext *dml_clause();
    antlr4::tree::TerminalNode *SEMI();
    Cfl_statementContext *cfl_statement();
    Another_statementContext *another_statement();
    Ddl_clauseContext *ddl_clause();
    Dbcc_specialContext *dbcc_special();
    Dbcc_clauseContext *dbcc_clause();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Sql_clausesContext* sql_clauses();

  class  Dml_clauseContext : public antlr4::ParserRuleContext {
  public:
    Dml_clauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Merge_statementContext *merge_statement();
    Delete_statementContext *delete_statement();
    Insert_statementContext *insert_statement();
    Select_statement_standaloneContext *select_statement_standalone();
    Update_statementContext *update_statement();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Dml_clauseContext* dml_clause();

  class  Ddl_clauseContext : public antlr4::ParserRuleContext {
  public:
    Ddl_clauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Alter_sequenceContext *alter_sequence();
    Alter_tableContext *alter_table();
    Create_sequenceContext *create_sequence();
    Create_tableContext *create_table();
    Drop_functionContext *drop_function();
    Drop_procedureContext *drop_procedure();
    Drop_tableContext *drop_table();
    Lock_tableContext *lock_table();
    Truncate_tableContext *truncate_table();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Ddl_clauseContext* ddl_clause();

  class  Cfl_statementContext : public antlr4::ParserRuleContext {
  public:
    Cfl_statementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Block_statementContext *block_statement();
    Break_statementContext *break_statement();
    Continue_statementContext *continue_statement();
    Goto_statementContext *goto_statement();
    If_statementContext *if_statement();
    Return_statementContext *return_statement();
    Throw_statementContext *throw_statement();
    Try_catch_statementContext *try_catch_statement();
    While_statementContext *while_statement();
    Print_statementContext *print_statement();
    Raiseerror_statementContext *raiseerror_statement();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Cfl_statementContext* cfl_statement();

  class  Block_statementContext : public antlr4::ParserRuleContext {
  public:
    Block_statementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *BEGIN();
    antlr4::tree::TerminalNode *END();
    std::vector<antlr4::tree::TerminalNode *> SEMI();
    antlr4::tree::TerminalNode* SEMI(size_t i);
    std::vector<Sql_clausesContext *> sql_clauses();
    Sql_clausesContext* sql_clauses(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Block_statementContext* block_statement();

  class  Break_statementContext : public antlr4::ParserRuleContext {
  public:
    Break_statementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *BREAK();
    antlr4::tree::TerminalNode *SEMI();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Break_statementContext* break_statement();

  class  Continue_statementContext : public antlr4::ParserRuleContext {
  public:
    Continue_statementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CONTINUE();
    antlr4::tree::TerminalNode *SEMI();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Continue_statementContext* continue_statement();

  class  Goto_statementContext : public antlr4::ParserRuleContext {
  public:
    Goto_statementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *GOTO();
    Id_Context *id_();
    antlr4::tree::TerminalNode *SEMI();
    antlr4::tree::TerminalNode *COLON();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Goto_statementContext* goto_statement();

  class  Return_statementContext : public antlr4::ParserRuleContext {
  public:
    Return_statementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *RETURN();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *SEMI();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Return_statementContext* return_statement();

  class  If_statementContext : public antlr4::ParserRuleContext {
  public:
    If_statementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IF();
    Search_conditionContext *search_condition();
    std::vector<Sql_clausesContext *> sql_clauses();
    Sql_clausesContext* sql_clauses(size_t i);
    antlr4::tree::TerminalNode *ELSE();
    antlr4::tree::TerminalNode *SEMI();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  If_statementContext* if_statement();

  class  Throw_statementContext : public antlr4::ParserRuleContext {
  public:
    Throw_statementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *THROW();
    Throw_error_numberContext *throw_error_number();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    Throw_messageContext *throw_message();
    Throw_stateContext *throw_state();
    antlr4::tree::TerminalNode *SEMI();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Throw_statementContext* throw_statement();

  class  Throw_error_numberContext : public antlr4::ParserRuleContext {
  public:
    Throw_error_numberContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DECIMAL();
    antlr4::tree::TerminalNode *LOCAL_ID();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Throw_error_numberContext* throw_error_number();

  class  Throw_messageContext : public antlr4::ParserRuleContext {
  public:
    Throw_messageContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *STRING();
    antlr4::tree::TerminalNode *LOCAL_ID();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Throw_messageContext* throw_message();

  class  Throw_stateContext : public antlr4::ParserRuleContext {
  public:
    Throw_stateContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DECIMAL();
    antlr4::tree::TerminalNode *LOCAL_ID();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Throw_stateContext* throw_state();

  class  Try_catch_statementContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Sql_clausesContext *try_clauses = nullptr;
    TSqlParser::Sql_clausesContext *catch_clauses = nullptr;
    Try_catch_statementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> BEGIN();
    antlr4::tree::TerminalNode* BEGIN(size_t i);
    std::vector<antlr4::tree::TerminalNode *> TRY();
    antlr4::tree::TerminalNode* TRY(size_t i);
    std::vector<antlr4::tree::TerminalNode *> END();
    antlr4::tree::TerminalNode* END(size_t i);
    std::vector<antlr4::tree::TerminalNode *> CATCH();
    antlr4::tree::TerminalNode* CATCH(size_t i);
    std::vector<antlr4::tree::TerminalNode *> SEMI();
    antlr4::tree::TerminalNode* SEMI(size_t i);
    std::vector<Sql_clausesContext *> sql_clauses();
    Sql_clausesContext* sql_clauses(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Try_catch_statementContext* try_catch_statement();

  class  While_statementContext : public antlr4::ParserRuleContext {
  public:
    While_statementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *WHILE();
    Search_conditionContext *search_condition();
    Sql_clausesContext *sql_clauses();
    antlr4::tree::TerminalNode *BREAK();
    antlr4::tree::TerminalNode *CONTINUE();
    antlr4::tree::TerminalNode *SEMI();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  While_statementContext* while_statement();

  class  Print_statementContext : public antlr4::ParserRuleContext {
  public:
    Print_statementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *PRINT();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *DOUBLE_QUOTE_ID();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    std::vector<antlr4::tree::TerminalNode *> LOCAL_ID();
    antlr4::tree::TerminalNode* LOCAL_ID(size_t i);
    antlr4::tree::TerminalNode *SEMI();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Print_statementContext* print_statement();

  class  Raiseerror_statementContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *msg = nullptr;
    TSqlParser::Constant_LOCAL_IDContext *severity = nullptr;
    TSqlParser::Constant_LOCAL_IDContext *state = nullptr;
    antlr4::Token *formatstring = nullptr;
    antlr4::Token *argument = nullptr;
    Raiseerror_statementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *RAISERROR();
    antlr4::tree::TerminalNode *LR_BRACKET();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    antlr4::tree::TerminalNode *RR_BRACKET();
    std::vector<Constant_LOCAL_IDContext *> constant_LOCAL_ID();
    Constant_LOCAL_IDContext* constant_LOCAL_ID(size_t i);
    std::vector<antlr4::tree::TerminalNode *> DECIMAL();
    antlr4::tree::TerminalNode* DECIMAL(size_t i);
    std::vector<antlr4::tree::TerminalNode *> STRING();
    antlr4::tree::TerminalNode* STRING(size_t i);
    std::vector<antlr4::tree::TerminalNode *> LOCAL_ID();
    antlr4::tree::TerminalNode* LOCAL_ID(size_t i);
    antlr4::tree::TerminalNode *WITH();
    antlr4::tree::TerminalNode *SEMI();
    antlr4::tree::TerminalNode *LOG();
    antlr4::tree::TerminalNode *SETERROR();
    antlr4::tree::TerminalNode *NOWAIT();
    antlr4::tree::TerminalNode *DOUBLE_QUOTE_ID();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Raiseerror_statementContext* raiseerror_statement();

  class  Empty_statementContext : public antlr4::ParserRuleContext {
  public:
    Empty_statementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SEMI();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Empty_statementContext* empty_statement();

  class  Another_statementContext : public antlr4::ParserRuleContext {
  public:
    Another_statementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Declare_statementContext *declare_statement();
    Execute_statementContext *execute_statement();
    Cursor_statementContext *cursor_statement();
    Kill_statementContext *kill_statement();
    Set_statementContext *set_statement();
    Transaction_statementContext *transaction_statement();
    Use_statementContext *use_statement();
    Shutdown_statementContext *shutdown_statement();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Another_statementContext* another_statement();

  class  Entity_toContext : public antlr4::ParserRuleContext {
  public:
    Entity_toContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *TO();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Entity_toContext* entity_to();

  class  Colon_colonContext : public antlr4::ParserRuleContext {
  public:
    Colon_colonContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DOUBLE_COLON();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Colon_colonContext* colon_colon();

  class  Class_typeContext : public antlr4::ParserRuleContext {
  public:
    Class_typeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OBJECT();
    antlr4::tree::TerminalNode *TYPE();
    antlr4::tree::TerminalNode *SCHEMA();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Class_typeContext* class_type();

  class  Class_type_for_sql_databaseContext : public antlr4::ParserRuleContext {
  public:
    Class_type_for_sql_databaseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OBJECT();
    antlr4::tree::TerminalNode *TYPE();
    antlr4::tree::TerminalNode *SCHEMA();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Class_type_for_sql_databaseContext* class_type_for_sql_database();

  class  Class_type_for_azure_dwContext : public antlr4::ParserRuleContext {
  public:
    Class_type_for_azure_dwContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SCHEMA();
    antlr4::tree::TerminalNode *OBJECT();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Class_type_for_azure_dwContext* class_type_for_azure_dw();

  class  Class_type_for_parallel_dwContext : public antlr4::ParserRuleContext {
  public:
    Class_type_for_parallel_dwContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DATABASE();
    antlr4::tree::TerminalNode *SCHEMA();
    antlr4::tree::TerminalNode *OBJECT();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Class_type_for_parallel_dwContext* class_type_for_parallel_dw();

  class  Lock_tableContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *seconds = nullptr;
    Lock_tableContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LOCK();
    antlr4::tree::TerminalNode *TABLE();
    Table_nameContext *table_name();
    antlr4::tree::TerminalNode *IN();
    antlr4::tree::TerminalNode *MODE();
    antlr4::tree::TerminalNode *SHARE();
    antlr4::tree::TerminalNode *EXCLUSIVE();
    antlr4::tree::TerminalNode *WAIT();
    antlr4::tree::TerminalNode *NOWAIT();
    antlr4::tree::TerminalNode *SEMI();
    antlr4::tree::TerminalNode *DECIMAL();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Lock_tableContext* lock_table();

  class  Truncate_tableContext : public antlr4::ParserRuleContext {
  public:
    Truncate_tableContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *TRUNCATE();
    antlr4::tree::TerminalNode *TABLE();
    Table_nameContext *table_name();
    antlr4::tree::TerminalNode *WITH();
    std::vector<antlr4::tree::TerminalNode *> LR_BRACKET();
    antlr4::tree::TerminalNode* LR_BRACKET(size_t i);
    antlr4::tree::TerminalNode *PARTITIONS();
    std::vector<antlr4::tree::TerminalNode *> RR_BRACKET();
    antlr4::tree::TerminalNode* RR_BRACKET(size_t i);
    std::vector<antlr4::tree::TerminalNode *> DECIMAL();
    antlr4::tree::TerminalNode* DECIMAL(size_t i);
    std::vector<antlr4::tree::TerminalNode *> TO();
    antlr4::tree::TerminalNode* TO(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Truncate_tableContext* truncate_table();

  class  Alter_sequenceContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Id_Context *schema_name = nullptr;
    TSqlParser::Id_Context *sequence_name = nullptr;
    antlr4::Token *sequnce_increment = nullptr;
    Alter_sequenceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ALTER();
    antlr4::tree::TerminalNode *SEQUENCE();
    std::vector<Id_Context *> id_();
    Id_Context* id_(size_t i);
    antlr4::tree::TerminalNode *DOT();
    antlr4::tree::TerminalNode *RESTART();
    antlr4::tree::TerminalNode *INCREMENT();
    antlr4::tree::TerminalNode *BY();
    antlr4::tree::TerminalNode *MINVALUE();
    std::vector<antlr4::tree::TerminalNode *> DECIMAL();
    antlr4::tree::TerminalNode* DECIMAL(size_t i);
    std::vector<antlr4::tree::TerminalNode *> NO();
    antlr4::tree::TerminalNode* NO(size_t i);
    antlr4::tree::TerminalNode *MAXVALUE();
    antlr4::tree::TerminalNode *CYCLE();
    antlr4::tree::TerminalNode *CACHE();
    antlr4::tree::TerminalNode *WITH();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Alter_sequenceContext* alter_sequence();

  class  Create_sequenceContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Id_Context *schema_name = nullptr;
    TSqlParser::Id_Context *sequence_name = nullptr;
    Create_sequenceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CREATE();
    antlr4::tree::TerminalNode *SEQUENCE();
    std::vector<Id_Context *> id_();
    Id_Context* id_(size_t i);
    antlr4::tree::TerminalNode *DOT();
    antlr4::tree::TerminalNode *AS();
    Data_typeContext *data_type();
    antlr4::tree::TerminalNode *START();
    antlr4::tree::TerminalNode *WITH();
    std::vector<antlr4::tree::TerminalNode *> DECIMAL();
    antlr4::tree::TerminalNode* DECIMAL(size_t i);
    antlr4::tree::TerminalNode *INCREMENT();
    antlr4::tree::TerminalNode *BY();
    antlr4::tree::TerminalNode *MINVALUE();
    std::vector<antlr4::tree::TerminalNode *> NO();
    antlr4::tree::TerminalNode* NO(size_t i);
    antlr4::tree::TerminalNode *MAXVALUE();
    antlr4::tree::TerminalNode *CYCLE();
    antlr4::tree::TerminalNode *CACHE();
    antlr4::tree::TerminalNode *MINUS();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Create_sequenceContext* create_sequence();

  class  Merge_statementContext : public antlr4::ParserRuleContext {
  public:
    Merge_statementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *MERGE();
    Ddl_objectContext *ddl_object();
    antlr4::tree::TerminalNode *USING();
    Table_sourcesContext *table_sources();
    antlr4::tree::TerminalNode *ON();
    Search_conditionContext *search_condition();
    antlr4::tree::TerminalNode *SEMI();
    With_expressionContext *with_expression();
    antlr4::tree::TerminalNode *TOP();
    antlr4::tree::TerminalNode *LR_BRACKET();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *RR_BRACKET();
    antlr4::tree::TerminalNode *INTO();
    Insert_with_table_hintsContext *insert_with_table_hints();
    As_table_aliasContext *as_table_alias();
    std::vector<When_matchesContext *> when_matches();
    When_matchesContext* when_matches(size_t i);
    Output_clauseContext *output_clause();
    Option_clauseContext *option_clause();
    antlr4::tree::TerminalNode *PERCENT();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Merge_statementContext* merge_statement();

  class  When_matchesContext : public antlr4::ParserRuleContext {
  public:
    When_matchesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> WHEN();
    antlr4::tree::TerminalNode* WHEN(size_t i);
    std::vector<antlr4::tree::TerminalNode *> MATCHED();
    antlr4::tree::TerminalNode* MATCHED(size_t i);
    std::vector<antlr4::tree::TerminalNode *> THEN();
    antlr4::tree::TerminalNode* THEN(size_t i);
    std::vector<Merge_matchedContext *> merge_matched();
    Merge_matchedContext* merge_matched(size_t i);
    std::vector<antlr4::tree::TerminalNode *> AND();
    antlr4::tree::TerminalNode* AND(size_t i);
    std::vector<Search_conditionContext *> search_condition();
    Search_conditionContext* search_condition(size_t i);
    std::vector<antlr4::tree::TerminalNode *> NOT();
    antlr4::tree::TerminalNode* NOT(size_t i);
    Merge_not_matchedContext *merge_not_matched();
    std::vector<antlr4::tree::TerminalNode *> BY();
    antlr4::tree::TerminalNode* BY(size_t i);
    antlr4::tree::TerminalNode *TARGET();
    std::vector<antlr4::tree::TerminalNode *> SOURCE();
    antlr4::tree::TerminalNode* SOURCE(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  When_matchesContext* when_matches();

  class  Merge_matchedContext : public antlr4::ParserRuleContext {
  public:
    Merge_matchedContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *UPDATE();
    antlr4::tree::TerminalNode *SET();
    std::vector<Update_elem_mergeContext *> update_elem_merge();
    Update_elem_mergeContext* update_elem_merge(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    antlr4::tree::TerminalNode *DELETE();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Merge_matchedContext* merge_matched();

  class  Merge_not_matchedContext : public antlr4::ParserRuleContext {
  public:
    Merge_not_matchedContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INSERT();
    Table_value_constructorContext *table_value_constructor();
    antlr4::tree::TerminalNode *DEFAULT();
    antlr4::tree::TerminalNode *VALUES();
    antlr4::tree::TerminalNode *LR_BRACKET();
    Column_name_listContext *column_name_list();
    antlr4::tree::TerminalNode *RR_BRACKET();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Merge_not_matchedContext* merge_not_matched();

  class  Delete_statementContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *cursor_var = nullptr;
    Delete_statementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DELETE();
    Delete_statement_fromContext *delete_statement_from();
    With_expressionContext *with_expression();
    antlr4::tree::TerminalNode *TOP();
    antlr4::tree::TerminalNode *LR_BRACKET();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *RR_BRACKET();
    antlr4::tree::TerminalNode *DECIMAL();
    std::vector<antlr4::tree::TerminalNode *> FROM();
    antlr4::tree::TerminalNode* FROM(size_t i);
    Insert_with_table_hintsContext *insert_with_table_hints();
    Output_clauseContext *output_clause();
    Table_sourcesContext *table_sources();
    antlr4::tree::TerminalNode *WHERE();
    Option_clauseContext *option_clause();
    antlr4::tree::TerminalNode *SEMI();
    Search_conditionContext *search_condition();
    antlr4::tree::TerminalNode *CURRENT();
    antlr4::tree::TerminalNode *OF();
    antlr4::tree::TerminalNode *PERCENT();
    Cursor_nameContext *cursor_name();
    antlr4::tree::TerminalNode *LOCAL_ID();
    antlr4::tree::TerminalNode *GLOBAL();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Delete_statementContext* delete_statement();

  class  Delete_statement_fromContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *table_var = nullptr;
    Delete_statement_fromContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Ddl_objectContext *ddl_object();
    Table_aliasContext *table_alias();
    antlr4::tree::TerminalNode *LOCAL_ID();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Delete_statement_fromContext* delete_statement_from();

  class  Insert_statementContext : public antlr4::ParserRuleContext {
  public:
    Insert_statementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INSERT();
    Insert_statement_valueContext *insert_statement_value();
    Ddl_objectContext *ddl_object();
    With_expressionContext *with_expression();
    antlr4::tree::TerminalNode *TOP();
    std::vector<antlr4::tree::TerminalNode *> LR_BRACKET();
    antlr4::tree::TerminalNode* LR_BRACKET(size_t i);
    ExpressionContext *expression();
    std::vector<antlr4::tree::TerminalNode *> RR_BRACKET();
    antlr4::tree::TerminalNode* RR_BRACKET(size_t i);
    antlr4::tree::TerminalNode *INTO();
    Insert_with_table_hintsContext *insert_with_table_hints();
    Insert_column_name_listContext *insert_column_name_list();
    Output_clauseContext *output_clause();
    antlr4::tree::TerminalNode *PERCENT();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Insert_statementContext* insert_statement();

  class  Insert_statement_valueContext : public antlr4::ParserRuleContext {
  public:
    Insert_statement_valueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Table_value_constructorContext *table_value_constructor();
    Derived_tableContext *derived_table();
    Execute_statementContext *execute_statement();
    antlr4::tree::TerminalNode *DEFAULT();
    antlr4::tree::TerminalNode *VALUES();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Insert_statement_valueContext* insert_statement_value();

  class  Select_statement_standaloneContext : public antlr4::ParserRuleContext {
  public:
    Select_statement_standaloneContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Select_statementContext *select_statement();
    With_expressionContext *with_expression();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Select_statement_standaloneContext* select_statement_standalone();

  class  Select_statementContext : public antlr4::ParserRuleContext {
  public:
    Select_statementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Query_expressionContext *query_expression();
    Order_by_clauseContext *order_by_clause();
    Option_clauseContext *option_clause();
    antlr4::tree::TerminalNode *SEMI();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Select_statementContext* select_statement();

  class  TimeContext : public antlr4::ParserRuleContext {
  public:
    TimeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LOCAL_ID();
    ConstantContext *constant();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  TimeContext* time();

  class  Update_statementContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *cursor_var = nullptr;
    Update_statementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *UPDATE();
    antlr4::tree::TerminalNode *SET();
    std::vector<Update_elemContext *> update_elem();
    Update_elemContext* update_elem(size_t i);
    Ddl_objectContext *ddl_object();
    With_expressionContext *with_expression();
    antlr4::tree::TerminalNode *TOP();
    antlr4::tree::TerminalNode *LR_BRACKET();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *RR_BRACKET();
    With_table_hintsContext *with_table_hints();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    Output_clauseContext *output_clause();
    antlr4::tree::TerminalNode *FROM();
    Table_sourcesContext *table_sources();
    antlr4::tree::TerminalNode *WHERE();
    Option_clauseContext *option_clause();
    antlr4::tree::TerminalNode *SEMI();
    Search_conditionContext *search_condition();
    antlr4::tree::TerminalNode *CURRENT();
    antlr4::tree::TerminalNode *OF();
    antlr4::tree::TerminalNode *PERCENT();
    Cursor_nameContext *cursor_name();
    antlr4::tree::TerminalNode *LOCAL_ID();
    antlr4::tree::TerminalNode *GLOBAL();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Update_statementContext* update_statement();

  class  Output_clauseContext : public antlr4::ParserRuleContext {
  public:
    Output_clauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OUTPUT();
    std::vector<Output_dml_list_elemContext *> output_dml_list_elem();
    Output_dml_list_elemContext* output_dml_list_elem(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    antlr4::tree::TerminalNode *INTO();
    antlr4::tree::TerminalNode *LOCAL_ID();
    Table_nameContext *table_name();
    antlr4::tree::TerminalNode *LR_BRACKET();
    Column_name_listContext *column_name_list();
    antlr4::tree::TerminalNode *RR_BRACKET();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Output_clauseContext* output_clause();

  class  Output_dml_list_elemContext : public antlr4::ParserRuleContext {
  public:
    Output_dml_list_elemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ExpressionContext *expression();
    AsteriskContext *asterisk();
    As_column_aliasContext *as_column_alias();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Output_dml_list_elemContext* output_dml_list_elem();

  class  Create_or_alter_procedureContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *proc = nullptr;
    TSqlParser::Func_proc_name_schemaContext *procName = nullptr;
    Create_or_alter_procedureContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *AS();
    Func_proc_name_schemaContext *func_proc_name_schema();
    antlr4::tree::TerminalNode *ALTER();
    antlr4::tree::TerminalNode *PROC();
    antlr4::tree::TerminalNode *PROCEDURE();
    antlr4::tree::TerminalNode *SEMI();
    antlr4::tree::TerminalNode *DECIMAL();
    std::vector<Procedure_paramContext *> procedure_param();
    Procedure_paramContext* procedure_param(size_t i);
    antlr4::tree::TerminalNode *WITH();
    std::vector<Procedure_optionContext *> procedure_option();
    Procedure_optionContext* procedure_option(size_t i);
    antlr4::tree::TerminalNode *FOR();
    antlr4::tree::TerminalNode *REPLICATION();
    antlr4::tree::TerminalNode *CREATE();
    std::vector<Sql_clausesContext *> sql_clauses();
    Sql_clausesContext* sql_clauses(size_t i);
    antlr4::tree::TerminalNode *LR_BRACKET();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    antlr4::tree::TerminalNode *RR_BRACKET();
    antlr4::tree::TerminalNode *OR();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Create_or_alter_procedureContext* create_or_alter_procedure();

  class  Create_or_alter_functionContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Func_proc_name_schemaContext *funcName = nullptr;
    Create_or_alter_functionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *FUNCTION();
    Func_proc_name_schemaContext *func_proc_name_schema();
    antlr4::tree::TerminalNode *ALTER();
    antlr4::tree::TerminalNode *LR_BRACKET();
    antlr4::tree::TerminalNode *RR_BRACKET();
    Func_body_returns_selectContext *func_body_returns_select();
    Func_body_returns_tableContext *func_body_returns_table();
    Func_body_returns_scalarContext *func_body_returns_scalar();
    antlr4::tree::TerminalNode *SEMI();
    antlr4::tree::TerminalNode *CREATE();
    std::vector<Procedure_paramContext *> procedure_param();
    Procedure_paramContext* procedure_param(size_t i);
    antlr4::tree::TerminalNode *OR();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Create_or_alter_functionContext* create_or_alter_function();

  class  Func_body_returns_selectContext : public antlr4::ParserRuleContext {
  public:
    Func_body_returns_selectContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *RETURNS();
    antlr4::tree::TerminalNode *TABLE();
    antlr4::tree::TerminalNode *RETURN();
    antlr4::tree::TerminalNode *LR_BRACKET();
    Select_statement_standaloneContext *select_statement_standalone();
    antlr4::tree::TerminalNode *RR_BRACKET();
    antlr4::tree::TerminalNode *WITH();
    std::vector<Function_optionContext *> function_option();
    Function_optionContext* function_option(size_t i);
    antlr4::tree::TerminalNode *AS();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Func_body_returns_selectContext* func_body_returns_select();

  class  Func_body_returns_tableContext : public antlr4::ParserRuleContext {
  public:
    Func_body_returns_tableContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *RETURNS();
    antlr4::tree::TerminalNode *LOCAL_ID();
    Table_type_definitionContext *table_type_definition();
    antlr4::tree::TerminalNode *BEGIN();
    antlr4::tree::TerminalNode *RETURN();
    antlr4::tree::TerminalNode *END();
    antlr4::tree::TerminalNode *WITH();
    std::vector<Function_optionContext *> function_option();
    Function_optionContext* function_option(size_t i);
    antlr4::tree::TerminalNode *AS();
    std::vector<Sql_clausesContext *> sql_clauses();
    Sql_clausesContext* sql_clauses(size_t i);
    std::vector<antlr4::tree::TerminalNode *> SEMI();
    antlr4::tree::TerminalNode* SEMI(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Func_body_returns_tableContext* func_body_returns_table();

  class  Func_body_returns_scalarContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::ExpressionContext *ret = nullptr;
    Func_body_returns_scalarContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *RETURNS();
    Data_typeContext *data_type();
    antlr4::tree::TerminalNode *BEGIN();
    antlr4::tree::TerminalNode *RETURN();
    antlr4::tree::TerminalNode *END();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *WITH();
    std::vector<Function_optionContext *> function_option();
    Function_optionContext* function_option(size_t i);
    antlr4::tree::TerminalNode *AS();
    std::vector<Sql_clausesContext *> sql_clauses();
    Sql_clausesContext* sql_clauses(size_t i);
    antlr4::tree::TerminalNode *SEMI();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Func_body_returns_scalarContext* func_body_returns_scalar();

  class  Procedure_paramContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Id_Context *type_schema = nullptr;
    TSqlParser::Default_valueContext *default_val = nullptr;
    Procedure_paramContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LOCAL_ID();
    Data_typeContext *data_type();
    antlr4::tree::TerminalNode *AS();
    antlr4::tree::TerminalNode *DOT();
    antlr4::tree::TerminalNode *VARYING();
    antlr4::tree::TerminalNode *EQUAL();
    Id_Context *id_();
    Default_valueContext *default_value();
    antlr4::tree::TerminalNode *OUT();
    antlr4::tree::TerminalNode *OUTPUT();
    antlr4::tree::TerminalNode *READONLY();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Procedure_paramContext* procedure_param();

  class  Procedure_optionContext : public antlr4::ParserRuleContext {
  public:
    Procedure_optionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ENCRYPTION();
    antlr4::tree::TerminalNode *RECOMPILE();
    Execute_clauseContext *execute_clause();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Procedure_optionContext* procedure_option();

  class  Function_optionContext : public antlr4::ParserRuleContext {
  public:
    Function_optionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ENCRYPTION();
    antlr4::tree::TerminalNode *SCHEMABINDING();
    antlr4::tree::TerminalNode *RETURNS();
    std::vector<antlr4::tree::TerminalNode *> NULL_();
    antlr4::tree::TerminalNode* NULL_(size_t i);
    antlr4::tree::TerminalNode *ON();
    antlr4::tree::TerminalNode *INPUT();
    antlr4::tree::TerminalNode *CALLED();
    Execute_clauseContext *execute_clause();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Function_optionContext* function_option();

  class  Create_tableContext : public antlr4::ParserRuleContext {
  public:
    Create_tableContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CREATE();
    antlr4::tree::TerminalNode *TABLE();
    Table_nameContext *table_name();
    antlr4::tree::TerminalNode *LR_BRACKET();
    Column_def_table_constraintsContext *column_def_table_constraints();
    antlr4::tree::TerminalNode *RR_BRACKET();
    antlr4::tree::TerminalNode *COMMA();
    antlr4::tree::TerminalNode *LOCK();
    Simple_idContext *simple_id();
    std::vector<Table_optionsContext *> table_options();
    Table_optionsContext* table_options(size_t i);
    antlr4::tree::TerminalNode *ON();
    std::vector<Id_Context *> id_();
    Id_Context* id_(size_t i);
    std::vector<antlr4::tree::TerminalNode *> DEFAULT();
    antlr4::tree::TerminalNode* DEFAULT(size_t i);
    antlr4::tree::TerminalNode *TEXTIMAGE_ON();
    antlr4::tree::TerminalNode *SEMI();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Create_tableContext* create_table();

  class  Table_optionsContext : public antlr4::ParserRuleContext {
  public:
    Table_optionsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *WITH();
    antlr4::tree::TerminalNode *LR_BRACKET();
    std::vector<Index_optionContext *> index_option();
    Index_optionContext* index_option(size_t i);
    antlr4::tree::TerminalNode *RR_BRACKET();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Table_optionsContext* table_options();

  class  Alter_tableContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Id_Context *constraint = nullptr;
    TSqlParser::Column_name_listContext *fk = nullptr;
    TSqlParser::Column_name_listContext *pk = nullptr;
    Alter_tableContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> ALTER();
    antlr4::tree::TerminalNode* ALTER(size_t i);
    std::vector<antlr4::tree::TerminalNode *> TABLE();
    antlr4::tree::TerminalNode* TABLE(size_t i);
    std::vector<Table_nameContext *> table_name();
    Table_nameContext* table_name(size_t i);
    antlr4::tree::TerminalNode *SET();
    std::vector<antlr4::tree::TerminalNode *> LR_BRACKET();
    antlr4::tree::TerminalNode* LR_BRACKET(size_t i);
    antlr4::tree::TerminalNode *LOCK_ESCALATION();
    antlr4::tree::TerminalNode *EQUAL();
    std::vector<antlr4::tree::TerminalNode *> RR_BRACKET();
    antlr4::tree::TerminalNode* RR_BRACKET(size_t i);
    antlr4::tree::TerminalNode *ADD();
    Column_def_table_constraintsContext *column_def_table_constraints();
    antlr4::tree::TerminalNode *COLUMN();
    Column_definitionContext *column_definition();
    antlr4::tree::TerminalNode *DROP();
    std::vector<Id_Context *> id_();
    Id_Context* id_(size_t i);
    antlr4::tree::TerminalNode *CONSTRAINT();
    antlr4::tree::TerminalNode *WITH();
    antlr4::tree::TerminalNode *CHECK();
    antlr4::tree::TerminalNode *FOREIGN();
    antlr4::tree::TerminalNode *KEY();
    antlr4::tree::TerminalNode *REFERENCES();
    antlr4::tree::TerminalNode *REBUILD();
    Table_optionsContext *table_options();
    antlr4::tree::TerminalNode *AUTO();
    antlr4::tree::TerminalNode *DISABLE();
    std::vector<Column_name_listContext *> column_name_list();
    Column_name_listContext* column_name_list(size_t i);
    antlr4::tree::TerminalNode *SEMI();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Alter_tableContext* alter_table();

  class  Database_optionspecContext : public antlr4::ParserRuleContext {
  public:
    Database_optionspecContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Cursor_optionContext *cursor_option();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Database_optionspecContext* database_optionspec();

  class  Cursor_optionContext : public antlr4::ParserRuleContext {
  public:
    Cursor_optionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CURSOR_CLOSE_ON_COMMIT();
    On_offContext *on_off();
    antlr4::tree::TerminalNode *CURSOR_DEFAULT();
    antlr4::tree::TerminalNode *LOCAL();
    antlr4::tree::TerminalNode *GLOBAL();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Cursor_optionContext* cursor_option();

  class  Drop_procedureContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *proc = nullptr;
    Drop_procedureContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DROP();
    std::vector<Func_proc_name_schemaContext *> func_proc_name_schema();
    Func_proc_name_schemaContext* func_proc_name_schema(size_t i);
    antlr4::tree::TerminalNode *PROC();
    antlr4::tree::TerminalNode *PROCEDURE();
    antlr4::tree::TerminalNode *IF();
    antlr4::tree::TerminalNode *EXISTS();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    antlr4::tree::TerminalNode *SEMI();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Drop_procedureContext* drop_procedure();

  class  Drop_functionContext : public antlr4::ParserRuleContext {
  public:
    Drop_functionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DROP();
    antlr4::tree::TerminalNode *FUNCTION();
    std::vector<Func_proc_name_schemaContext *> func_proc_name_schema();
    Func_proc_name_schemaContext* func_proc_name_schema(size_t i);
    antlr4::tree::TerminalNode *IF();
    antlr4::tree::TerminalNode *EXISTS();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    antlr4::tree::TerminalNode *SEMI();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Drop_functionContext* drop_function();

  class  Drop_tableContext : public antlr4::ParserRuleContext {
  public:
    Drop_tableContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DROP();
    antlr4::tree::TerminalNode *TABLE();
    Table_nameContext *table_name();
    antlr4::tree::TerminalNode *IF();
    antlr4::tree::TerminalNode *EXISTS();
    antlr4::tree::TerminalNode *SEMI();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Drop_tableContext* drop_table();

  class  Declare_statementContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Declare_localContext *declare_localContext = nullptr;
    std::vector<Declare_localContext *> loc;
    Declare_statementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DECLARE();
    antlr4::tree::TerminalNode *LOCAL_ID();
    Table_type_definitionContext *table_type_definition();
    antlr4::tree::TerminalNode *AS();
    antlr4::tree::TerminalNode *SEMI();
    std::vector<Declare_localContext *> declare_local();
    Declare_localContext* declare_local(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Declare_statementContext* declare_statement();

  class  Cursor_statementContext : public antlr4::ParserRuleContext {
  public:
    Cursor_statementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CLOSE();
    Cursor_nameContext *cursor_name();
    antlr4::tree::TerminalNode *GLOBAL();
    antlr4::tree::TerminalNode *SEMI();
    antlr4::tree::TerminalNode *DEALLOCATE();
    antlr4::tree::TerminalNode *CURSOR();
    Declare_cursorContext *declare_cursor();
    Fetch_cursorContext *fetch_cursor();
    antlr4::tree::TerminalNode *OPEN();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Cursor_statementContext* cursor_statement();

  class  Kill_statementContext : public antlr4::ParserRuleContext {
  public:
    Kill_statementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *KILL();
    Kill_processContext *kill_process();
    Kill_query_notificationContext *kill_query_notification();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Kill_statementContext* kill_statement();

  class  Kill_processContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *session_id = nullptr;
    Kill_processContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *UOW();
    antlr4::tree::TerminalNode *WITH();
    antlr4::tree::TerminalNode *STATUSONLY();
    antlr4::tree::TerminalNode *DECIMAL();
    antlr4::tree::TerminalNode *STRING();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Kill_processContext* kill_process();

  class  Kill_query_notificationContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *subscription_id = nullptr;
    Kill_query_notificationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *QUERY();
    antlr4::tree::TerminalNode *NOTIFICATION();
    antlr4::tree::TerminalNode *SUBSCRIPTION();
    antlr4::tree::TerminalNode *ALL();
    antlr4::tree::TerminalNode *DECIMAL();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Kill_query_notificationContext* kill_query_notification();

  class  Execute_statementContext : public antlr4::ParserRuleContext {
  public:
    Execute_statementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EXECUTE();
    Execute_bodyContext *execute_body();
    antlr4::tree::TerminalNode *SEMI();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Execute_statementContext* execute_statement();

  class  Execute_body_batchContext : public antlr4::ParserRuleContext {
  public:
    Execute_body_batchContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Func_proc_name_server_database_schemaContext *func_proc_name_server_database_schema();
    std::vector<Execute_statement_argContext *> execute_statement_arg();
    Execute_statement_argContext* execute_statement_arg(size_t i);
    antlr4::tree::TerminalNode *SEMI();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Execute_body_batchContext* execute_body_batch();

  class  Execute_bodyContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *return_status = nullptr;
    Execute_bodyContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Func_proc_name_server_database_schemaContext *func_proc_name_server_database_schema();
    std::vector<Execute_var_stringContext *> execute_var_string();
    Execute_var_stringContext* execute_var_string(size_t i);
    antlr4::tree::TerminalNode *EQUAL();
    Execute_statement_argContext *execute_statement_arg();
    antlr4::tree::TerminalNode *LOCAL_ID();
    antlr4::tree::TerminalNode *LR_BRACKET();
    antlr4::tree::TerminalNode *RR_BRACKET();
    std::vector<antlr4::tree::TerminalNode *> PLUS();
    antlr4::tree::TerminalNode* PLUS(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Execute_bodyContext* execute_body();

  class  Execute_statement_argContext : public antlr4::ParserRuleContext {
  public:
    Execute_statement_argContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Execute_statement_arg_unnamedContext *execute_statement_arg_unnamed();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    std::vector<Execute_statement_argContext *> execute_statement_arg();
    Execute_statement_argContext* execute_statement_arg(size_t i);
    std::vector<Execute_statement_arg_namedContext *> execute_statement_arg_named();
    Execute_statement_arg_namedContext* execute_statement_arg_named(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Execute_statement_argContext* execute_statement_arg();

  class  Execute_statement_arg_namedContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *name = nullptr;
    TSqlParser::Execute_parameterContext *value = nullptr;
    Execute_statement_arg_namedContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EQUAL();
    antlr4::tree::TerminalNode *LOCAL_ID();
    Execute_parameterContext *execute_parameter();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Execute_statement_arg_namedContext* execute_statement_arg_named();

  class  Execute_statement_arg_unnamedContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Execute_parameterContext *value = nullptr;
    Execute_statement_arg_unnamedContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Execute_parameterContext *execute_parameter();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Execute_statement_arg_unnamedContext* execute_statement_arg_unnamed();

  class  Execute_parameterContext : public antlr4::ParserRuleContext {
  public:
    Execute_parameterContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ConstantContext *constant();
    antlr4::tree::TerminalNode *LOCAL_ID();
    Id_Context *id_();
    antlr4::tree::TerminalNode *DEFAULT();
    antlr4::tree::TerminalNode *NULL_();
    antlr4::tree::TerminalNode *OUTPUT();
    antlr4::tree::TerminalNode *OUT();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Execute_parameterContext* execute_parameter();

  class  Execute_var_stringContext : public antlr4::ParserRuleContext {
  public:
    Execute_var_stringContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LOCAL_ID();
    antlr4::tree::TerminalNode *STRING();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Execute_var_stringContext* execute_var_string();

  class  AlgorithmContext : public antlr4::ParserRuleContext {
  public:
    AlgorithmContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DES();
    antlr4::tree::TerminalNode *TRIPLE_DES();
    antlr4::tree::TerminalNode *TRIPLE_DES_3KEY();
    antlr4::tree::TerminalNode *RC2();
    antlr4::tree::TerminalNode *RC4();
    antlr4::tree::TerminalNode *RC4_128();
    antlr4::tree::TerminalNode *DESX();
    antlr4::tree::TerminalNode *AES_128();
    antlr4::tree::TerminalNode *AES_192();
    antlr4::tree::TerminalNode *AES_256();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  AlgorithmContext* algorithm();

  class  Set_statementContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Id_Context *member_name = nullptr;
    Set_statementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SET();
    antlr4::tree::TerminalNode *LOCAL_ID();
    antlr4::tree::TerminalNode *EQUAL();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *DOT();
    antlr4::tree::TerminalNode *SEMI();
    Id_Context *id_();
    Assignment_operatorContext *assignment_operator();
    antlr4::tree::TerminalNode *CURSOR();
    Declare_set_cursor_commonContext *declare_set_cursor_common();
    antlr4::tree::TerminalNode *FOR();
    antlr4::tree::TerminalNode *READ();
    antlr4::tree::TerminalNode *ONLY();
    antlr4::tree::TerminalNode *UPDATE();
    antlr4::tree::TerminalNode *OF();
    Column_name_listContext *column_name_list();
    Set_specialContext *set_special();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Set_statementContext* set_statement();

  class  Transaction_statementContext : public antlr4::ParserRuleContext {
  public:
    Transaction_statementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *BEGIN();
    antlr4::tree::TerminalNode *DISTRIBUTED();
    antlr4::tree::TerminalNode *TRAN();
    antlr4::tree::TerminalNode *TRANSACTION();
    Id_Context *id_();
    antlr4::tree::TerminalNode *LOCAL_ID();
    antlr4::tree::TerminalNode *SEMI();
    antlr4::tree::TerminalNode *WITH();
    antlr4::tree::TerminalNode *MARK();
    antlr4::tree::TerminalNode *STRING();
    antlr4::tree::TerminalNode *COMMIT();
    antlr4::tree::TerminalNode *LR_BRACKET();
    antlr4::tree::TerminalNode *DELAYED_DURABILITY();
    antlr4::tree::TerminalNode *EQUAL();
    antlr4::tree::TerminalNode *RR_BRACKET();
    antlr4::tree::TerminalNode *OFF();
    antlr4::tree::TerminalNode *ON();
    antlr4::tree::TerminalNode *WORK();
    antlr4::tree::TerminalNode *ROLLBACK();
    antlr4::tree::TerminalNode *SAVE();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Transaction_statementContext* transaction_statement();

  class  Go_batch_statementContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *count = nullptr;
    Go_batch_statementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *GO_BATCH();
    antlr4::tree::TerminalNode *DECIMAL();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Go_batch_statementContext* go_batch_statement();

  class  Go_statementContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *count = nullptr;
    Go_statementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *GO();
    antlr4::tree::TerminalNode *DECIMAL();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Go_statementContext* go_statement();

  class  Use_statementContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Id_Context *database = nullptr;
    Use_statementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *USE();
    Id_Context *id_();
    antlr4::tree::TerminalNode *SEMI();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Use_statementContext* use_statement();

  class  Shutdown_statementContext : public antlr4::ParserRuleContext {
  public:
    Shutdown_statementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SHUTDOWN();
    antlr4::tree::TerminalNode *WITH();
    antlr4::tree::TerminalNode *NOWAIT();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Shutdown_statementContext* shutdown_statement();

  class  Dbcc_specialContext : public antlr4::ParserRuleContext {
  public:
    Dbcc_specialContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DBCC();
    antlr4::tree::TerminalNode *SHRINKLOG();
    antlr4::tree::TerminalNode *LR_BRACKET();
    antlr4::tree::TerminalNode *SIZE();
    antlr4::tree::TerminalNode *EQUAL();
    antlr4::tree::TerminalNode *RR_BRACKET();
    antlr4::tree::TerminalNode *SEMI();
    Constant_expressionContext *constant_expression();
    Id_Context *id_();
    antlr4::tree::TerminalNode *DEFAULT();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Dbcc_specialContext* dbcc_special();

  class  Dbcc_clauseContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Dbcc_commandContext *name = nullptr;
    Dbcc_clauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DBCC();
    Dbcc_commandContext *dbcc_command();
    antlr4::tree::TerminalNode *LR_BRACKET();
    Expression_listContext *expression_list();
    antlr4::tree::TerminalNode *RR_BRACKET();
    antlr4::tree::TerminalNode *WITH();
    Dbcc_optionsContext *dbcc_options();
    antlr4::tree::TerminalNode *SEMI();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Dbcc_clauseContext* dbcc_clause();

  class  Dbcc_commandContext : public antlr4::ParserRuleContext {
  public:
    Dbcc_commandContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Simple_idContext *simple_id();
    KeywordContext *keyword();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Dbcc_commandContext* dbcc_command();

  class  Dbcc_optionsContext : public antlr4::ParserRuleContext {
  public:
    Dbcc_optionsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Simple_idContext *> simple_id();
    Simple_idContext* simple_id(size_t i);
    antlr4::tree::TerminalNode *COMMA();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Dbcc_optionsContext* dbcc_options();

  class  Execute_clauseContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *clause = nullptr;
    Execute_clauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EXECUTE();
    antlr4::tree::TerminalNode *AS();
    antlr4::tree::TerminalNode *CALLER();
    antlr4::tree::TerminalNode *SELF();
    antlr4::tree::TerminalNode *OWNER();
    antlr4::tree::TerminalNode *STRING();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Execute_clauseContext* execute_clause();

  class  Declare_localContext : public antlr4::ParserRuleContext {
  public:
    Declare_localContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LOCAL_ID();
    Data_typeContext *data_type();
    antlr4::tree::TerminalNode *AS();
    antlr4::tree::TerminalNode *EQUAL();
    ExpressionContext *expression();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Declare_localContext* declare_local();

  class  Table_type_definitionContext : public antlr4::ParserRuleContext {
  public:
    Table_type_definitionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *TABLE();
    antlr4::tree::TerminalNode *LR_BRACKET();
    Column_def_table_constraintsContext *column_def_table_constraints();
    antlr4::tree::TerminalNode *RR_BRACKET();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Table_type_definitionContext* table_type_definition();

  class  Column_def_table_constraintsContext : public antlr4::ParserRuleContext {
  public:
    Column_def_table_constraintsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Column_def_table_constraintContext *> column_def_table_constraint();
    Column_def_table_constraintContext* column_def_table_constraint(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Column_def_table_constraintsContext* column_def_table_constraints();

  class  Column_def_table_constraintContext : public antlr4::ParserRuleContext {
  public:
    Column_def_table_constraintContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Column_definitionContext *column_definition();
    Table_constraintContext *table_constraint();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Column_def_table_constraintContext* column_def_table_constraint();

  class  Column_definitionContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Id_Context *constraint = nullptr;
    antlr4::Token *seed = nullptr;
    antlr4::Token *increment = nullptr;
    Column_definitionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Id_Context *> id_();
    Id_Context* id_(size_t i);
    Data_typeContext *data_type();
    antlr4::tree::TerminalNode *AS();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *COLLATE();
    Null_notnullContext *null_notnull();
    std::vector<Null_or_defaultContext *> null_or_default();
    Null_or_defaultContext* null_or_default(size_t i);
    antlr4::tree::TerminalNode *IDENTITY();
    antlr4::tree::TerminalNode *ROWGUIDCOL();
    std::vector<Column_constraintContext *> column_constraint();
    Column_constraintContext* column_constraint(size_t i);
    antlr4::tree::TerminalNode *PERSISTED();
    antlr4::tree::TerminalNode *CONSTRAINT();
    antlr4::tree::TerminalNode *LR_BRACKET();
    antlr4::tree::TerminalNode *COMMA();
    antlr4::tree::TerminalNode *RR_BRACKET();
    antlr4::tree::TerminalNode *NOT();
    antlr4::tree::TerminalNode *FOR();
    antlr4::tree::TerminalNode *REPLICATION();
    std::vector<antlr4::tree::TerminalNode *> DECIMAL();
    antlr4::tree::TerminalNode* DECIMAL(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Column_definitionContext* column_definition();

  class  Column_constraintContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Id_Context *constraint = nullptr;
    TSqlParser::Column_name_listContext *pk = nullptr;
    Column_constraintContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CHECK();
    antlr4::tree::TerminalNode *LR_BRACKET();
    Search_conditionContext *search_condition();
    antlr4::tree::TerminalNode *RR_BRACKET();
    antlr4::tree::TerminalNode *REFERENCES();
    Table_nameContext *table_name();
    Null_notnullContext *null_notnull();
    antlr4::tree::TerminalNode *CONSTRAINT();
    Column_name_listContext *column_name_list();
    Id_Context *id_();
    antlr4::tree::TerminalNode *PRIMARY();
    antlr4::tree::TerminalNode *KEY();
    antlr4::tree::TerminalNode *UNIQUE();
    Index_optionsContext *index_options();
    antlr4::tree::TerminalNode *NOT();
    antlr4::tree::TerminalNode *FOR();
    antlr4::tree::TerminalNode *REPLICATION();
    antlr4::tree::TerminalNode *FOREIGN();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Column_constraintContext* column_constraint();

  class  Table_constraintContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Id_Context *constraint = nullptr;
    TSqlParser::Column_name_listContext *fk = nullptr;
    TSqlParser::Column_name_listContext *pk = nullptr;
    Table_constraintContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> LR_BRACKET();
    antlr4::tree::TerminalNode* LR_BRACKET(size_t i);
    Column_name_list_with_orderContext *column_name_list_with_order();
    std::vector<antlr4::tree::TerminalNode *> RR_BRACKET();
    antlr4::tree::TerminalNode* RR_BRACKET(size_t i);
    antlr4::tree::TerminalNode *CHECK();
    Search_conditionContext *search_condition();
    antlr4::tree::TerminalNode *DEFAULT();
    antlr4::tree::TerminalNode *FOR();
    std::vector<Id_Context *> id_();
    Id_Context* id_(size_t i);
    antlr4::tree::TerminalNode *FOREIGN();
    antlr4::tree::TerminalNode *KEY();
    antlr4::tree::TerminalNode *REFERENCES();
    Table_nameContext *table_name();
    antlr4::tree::TerminalNode *CONSTRAINT();
    std::vector<Column_name_listContext *> column_name_list();
    Column_name_listContext* column_name_list(size_t i);
    antlr4::tree::TerminalNode *PRIMARY();
    antlr4::tree::TerminalNode *UNIQUE();
    Index_optionsContext *index_options();
    antlr4::tree::TerminalNode *ON();
    antlr4::tree::TerminalNode *NOT();
    antlr4::tree::TerminalNode *REPLICATION();
    std::vector<antlr4::tree::TerminalNode *> STRING();
    antlr4::tree::TerminalNode* STRING(size_t i);
    std::vector<antlr4::tree::TerminalNode *> PLUS();
    antlr4::tree::TerminalNode* PLUS(size_t i);
    std::vector<Function_callContext *> function_call();
    Function_callContext* function_call(size_t i);
    std::vector<antlr4::tree::TerminalNode *> DECIMAL();
    antlr4::tree::TerminalNode* DECIMAL(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Table_constraintContext* table_constraint();

  class  Index_optionsContext : public antlr4::ParserRuleContext {
  public:
    Index_optionsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *WITH();
    antlr4::tree::TerminalNode *LR_BRACKET();
    std::vector<Index_optionContext *> index_option();
    Index_optionContext* index_option(size_t i);
    antlr4::tree::TerminalNode *RR_BRACKET();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Index_optionsContext* index_options();

  class  Index_optionContext : public antlr4::ParserRuleContext {
  public:
    Index_optionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EQUAL();
    std::vector<Simple_idContext *> simple_id();
    Simple_idContext* simple_id(size_t i);
    std::vector<KeywordContext *> keyword();
    KeywordContext* keyword(size_t i);
    On_offContext *on_off();
    antlr4::tree::TerminalNode *DECIMAL();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Index_optionContext* index_option();

  class  Declare_cursorContext : public antlr4::ParserRuleContext {
  public:
    Declare_cursorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DECLARE();
    Cursor_nameContext *cursor_name();
    antlr4::tree::TerminalNode *CURSOR();
    std::vector<antlr4::tree::TerminalNode *> FOR();
    antlr4::tree::TerminalNode* FOR(size_t i);
    Select_statement_standaloneContext *select_statement_standalone();
    antlr4::tree::TerminalNode *SEMI();
    Declare_set_cursor_commonContext *declare_set_cursor_common();
    antlr4::tree::TerminalNode *SCROLL();
    antlr4::tree::TerminalNode *SEMI_SENSITIVE();
    antlr4::tree::TerminalNode *INSENSITIVE();
    antlr4::tree::TerminalNode *READ();
    antlr4::tree::TerminalNode *ONLY();
    antlr4::tree::TerminalNode *UPDATE();
    antlr4::tree::TerminalNode *OF();
    Column_name_listContext *column_name_list();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Declare_cursorContext* declare_cursor();

  class  Declare_set_cursor_commonContext : public antlr4::ParserRuleContext {
  public:
    Declare_set_cursor_commonContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *FOR();
    Select_statement_standaloneContext *select_statement_standalone();
    std::vector<Declare_set_cursor_common_partialContext *> declare_set_cursor_common_partial();
    Declare_set_cursor_common_partialContext* declare_set_cursor_common_partial(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Declare_set_cursor_commonContext* declare_set_cursor_common();

  class  Declare_set_cursor_common_partialContext : public antlr4::ParserRuleContext {
  public:
    Declare_set_cursor_common_partialContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LOCAL();
    antlr4::tree::TerminalNode *GLOBAL();
    antlr4::tree::TerminalNode *FORWARD_ONLY();
    antlr4::tree::TerminalNode *SCROLL();
    antlr4::tree::TerminalNode *STATIC();
    antlr4::tree::TerminalNode *KEYSET();
    antlr4::tree::TerminalNode *DYNAMIC();
    antlr4::tree::TerminalNode *FAST_FORWARD();
    antlr4::tree::TerminalNode *READ_ONLY();
    antlr4::tree::TerminalNode *SCROLL_LOCKS();
    antlr4::tree::TerminalNode *OPTIMISTIC();
    antlr4::tree::TerminalNode *TYPE_WARNING();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Declare_set_cursor_common_partialContext* declare_set_cursor_common_partial();

  class  Fetch_cursorContext : public antlr4::ParserRuleContext {
  public:
    Fetch_cursorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *FETCH();
    Cursor_nameContext *cursor_name();
    antlr4::tree::TerminalNode *FROM();
    antlr4::tree::TerminalNode *GLOBAL();
    antlr4::tree::TerminalNode *INTO();
    std::vector<antlr4::tree::TerminalNode *> LOCAL_ID();
    antlr4::tree::TerminalNode* LOCAL_ID(size_t i);
    antlr4::tree::TerminalNode *SEMI();
    antlr4::tree::TerminalNode *NEXT();
    antlr4::tree::TerminalNode *PRIOR();
    antlr4::tree::TerminalNode *FIRST();
    antlr4::tree::TerminalNode *LAST();
    ExpressionContext *expression();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    antlr4::tree::TerminalNode *ABSOLUTE();
    antlr4::tree::TerminalNode *RELATIVE();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Fetch_cursorContext* fetch_cursor();

  class  Set_specialContext : public antlr4::ParserRuleContext {
  public:
    Set_specialContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SET();
    std::vector<Id_Context *> id_();
    Id_Context* id_(size_t i);
    Constant_LOCAL_IDContext *constant_LOCAL_ID();
    On_offContext *on_off();
    antlr4::tree::TerminalNode *SEMI();
    antlr4::tree::TerminalNode *ROWCOUNT();
    antlr4::tree::TerminalNode *LOCAL_ID();
    antlr4::tree::TerminalNode *DECIMAL();
    antlr4::tree::TerminalNode *TRANSACTION();
    antlr4::tree::TerminalNode *ISOLATION();
    antlr4::tree::TerminalNode *LEVEL();
    antlr4::tree::TerminalNode *READ();
    antlr4::tree::TerminalNode *UNCOMMITTED();
    antlr4::tree::TerminalNode *COMMITTED();
    antlr4::tree::TerminalNode *REPEATABLE();
    antlr4::tree::TerminalNode *SNAPSHOT();
    antlr4::tree::TerminalNode *SERIALIZABLE();
    antlr4::tree::TerminalNode *IDENTITY_INSERT();
    Table_nameContext *table_name();
    antlr4::tree::TerminalNode *ANSI_NULLS();
    antlr4::tree::TerminalNode *QUOTED_IDENTIFIER();
    antlr4::tree::TerminalNode *ANSI_PADDING();
    antlr4::tree::TerminalNode *ANSI_WARNINGS();
    antlr4::tree::TerminalNode *ANSI_DEFAULTS();
    antlr4::tree::TerminalNode *ANSI_NULL_DFLT_OFF();
    antlr4::tree::TerminalNode *ANSI_NULL_DFLT_ON();
    antlr4::tree::TerminalNode *ARITHABORT();
    antlr4::tree::TerminalNode *ARITHIGNORE();
    antlr4::tree::TerminalNode *CONCAT_NULL_YIELDS_NULL();
    antlr4::tree::TerminalNode *CURSOR_CLOSE_ON_COMMIT();
    antlr4::tree::TerminalNode *FMTONLY();
    antlr4::tree::TerminalNode *FORCEPLAN();
    antlr4::tree::TerminalNode *IMPLICIT_TRANSACTIONS();
    antlr4::tree::TerminalNode *NOCOUNT();
    antlr4::tree::TerminalNode *NOEXEC();
    antlr4::tree::TerminalNode *NUMERIC_ROUNDABORT();
    antlr4::tree::TerminalNode *PARSEONLY();
    antlr4::tree::TerminalNode *REMOTE_PROC_TRANSACTIONS();
    antlr4::tree::TerminalNode *SHOWPLAN_ALL();
    antlr4::tree::TerminalNode *SHOWPLAN_TEXT();
    antlr4::tree::TerminalNode *SHOWPLAN_XML();
    antlr4::tree::TerminalNode *XACT_ABORT();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Set_specialContext* set_special();

  class  Constant_LOCAL_IDContext : public antlr4::ParserRuleContext {
  public:
    Constant_LOCAL_IDContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ConstantContext *constant();
    antlr4::tree::TerminalNode *LOCAL_ID();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Constant_LOCAL_IDContext* constant_LOCAL_ID();

  class  ExpressionContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *op = nullptr;
    ExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Primitive_expressionContext *primitive_expression();
    Function_callContext *function_call();
    Case_expressionContext *case_expression();
    Full_column_nameContext *full_column_name();
    Bracket_expressionContext *bracket_expression();
    Unary_operator_expressionContext *unary_operator_expression();
    Over_clauseContext *over_clause();
    antlr4::tree::TerminalNode *DOLLAR_ACTION();
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    antlr4::tree::TerminalNode *STAR();
    antlr4::tree::TerminalNode *DIVIDE();
    antlr4::tree::TerminalNode *MODULE();
    antlr4::tree::TerminalNode *PLUS();
    antlr4::tree::TerminalNode *MINUS();
    antlr4::tree::TerminalNode *BIT_AND();
    antlr4::tree::TerminalNode *BIT_XOR();
    antlr4::tree::TerminalNode *BIT_OR();
    antlr4::tree::TerminalNode *DOUBLE_BAR();
    antlr4::tree::TerminalNode *COLLATE();
    Id_Context *id_();
    Time_zoneContext *time_zone();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ExpressionContext* expression();
  ExpressionContext* expression(int precedence);
  class  Time_zoneContext : public antlr4::ParserRuleContext {
  public:
    Time_zoneContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *AT_KEYWORD();
    antlr4::tree::TerminalNode *TIME();
    antlr4::tree::TerminalNode *ZONE();
    ExpressionContext *expression();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Time_zoneContext* time_zone();

  class  Primitive_expressionContext : public antlr4::ParserRuleContext {
  public:
    Primitive_expressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DEFAULT();
    antlr4::tree::TerminalNode *NULL_();
    antlr4::tree::TerminalNode *LOCAL_ID();
    ConstantContext *constant();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Primitive_expressionContext* primitive_expression();

  class  Case_expressionContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::ExpressionContext *caseExpr = nullptr;
    TSqlParser::ExpressionContext *elseExpr = nullptr;
    Case_expressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CASE();
    antlr4::tree::TerminalNode *END();
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    std::vector<Switch_sectionContext *> switch_section();
    Switch_sectionContext* switch_section(size_t i);
    antlr4::tree::TerminalNode *ELSE();
    std::vector<Switch_search_condition_sectionContext *> switch_search_condition_section();
    Switch_search_condition_sectionContext* switch_search_condition_section(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Case_expressionContext* case_expression();

  class  Unary_operator_expressionContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *op = nullptr;
    Unary_operator_expressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *BIT_NOT();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *PLUS();
    antlr4::tree::TerminalNode *MINUS();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Unary_operator_expressionContext* unary_operator_expression();

  class  Bracket_expressionContext : public antlr4::ParserRuleContext {
  public:
    Bracket_expressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LR_BRACKET();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *RR_BRACKET();
    SubqueryContext *subquery();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Bracket_expressionContext* bracket_expression();

  class  Constant_expressionContext : public antlr4::ParserRuleContext {
  public:
    Constant_expressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NULL_();
    ConstantContext *constant();
    Function_callContext *function_call();
    antlr4::tree::TerminalNode *LOCAL_ID();
    antlr4::tree::TerminalNode *LR_BRACKET();
    Constant_expressionContext *constant_expression();
    antlr4::tree::TerminalNode *RR_BRACKET();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Constant_expressionContext* constant_expression();

  class  SubqueryContext : public antlr4::ParserRuleContext {
  public:
    SubqueryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Select_statementContext *select_statement();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  SubqueryContext* subquery();

  class  With_expressionContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Common_table_expressionContext *common_table_expressionContext = nullptr;
    std::vector<Common_table_expressionContext *> ctes;
    With_expressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *WITH();
    std::vector<Common_table_expressionContext *> common_table_expression();
    Common_table_expressionContext* common_table_expression(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  With_expressionContext* with_expression();

  class  Common_table_expressionContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Id_Context *expression_name = nullptr;
    TSqlParser::Column_name_listContext *columns = nullptr;
    TSqlParser::Select_statementContext *cte_query = nullptr;
    Common_table_expressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *AS();
    std::vector<antlr4::tree::TerminalNode *> LR_BRACKET();
    antlr4::tree::TerminalNode* LR_BRACKET(size_t i);
    std::vector<antlr4::tree::TerminalNode *> RR_BRACKET();
    antlr4::tree::TerminalNode* RR_BRACKET(size_t i);
    Id_Context *id_();
    Select_statementContext *select_statement();
    Column_name_listContext *column_name_list();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Common_table_expressionContext* common_table_expression();

  class  Update_elemContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Id_Context *udt_column_name = nullptr;
    TSqlParser::Id_Context *method_name = nullptr;
    Update_elemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LOCAL_ID();
    std::vector<antlr4::tree::TerminalNode *> EQUAL();
    antlr4::tree::TerminalNode* EQUAL(size_t i);
    Full_column_nameContext *full_column_name();
    ExpressionContext *expression();
    Assignment_operatorContext *assignment_operator();
    antlr4::tree::TerminalNode *DOT();
    antlr4::tree::TerminalNode *LR_BRACKET();
    Expression_listContext *expression_list();
    antlr4::tree::TerminalNode *RR_BRACKET();
    std::vector<Id_Context *> id_();
    Id_Context* id_(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Update_elemContext* update_elem();

  class  Update_elem_mergeContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Id_Context *udt_column_name = nullptr;
    TSqlParser::Id_Context *method_name = nullptr;
    Update_elem_mergeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ExpressionContext *expression();
    Full_column_nameContext *full_column_name();
    antlr4::tree::TerminalNode *LOCAL_ID();
    antlr4::tree::TerminalNode *EQUAL();
    Assignment_operatorContext *assignment_operator();
    antlr4::tree::TerminalNode *DOT();
    antlr4::tree::TerminalNode *LR_BRACKET();
    Expression_listContext *expression_list();
    antlr4::tree::TerminalNode *RR_BRACKET();
    std::vector<Id_Context *> id_();
    Id_Context* id_(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Update_elem_mergeContext* update_elem_merge();

  class  Search_conditionContext : public antlr4::ParserRuleContext {
  public:
    Search_conditionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    PredicateContext *predicate();
    antlr4::tree::TerminalNode *LR_BRACKET();
    std::vector<Search_conditionContext *> search_condition();
    Search_conditionContext* search_condition(size_t i);
    antlr4::tree::TerminalNode *RR_BRACKET();
    std::vector<antlr4::tree::TerminalNode *> NOT();
    antlr4::tree::TerminalNode* NOT(size_t i);
    antlr4::tree::TerminalNode *AND();
    antlr4::tree::TerminalNode *OR();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Search_conditionContext* search_condition();
  Search_conditionContext* search_condition(int precedence);
  class  PredicateContext : public antlr4::ParserRuleContext {
  public:
    PredicateContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EXISTS();
    antlr4::tree::TerminalNode *LR_BRACKET();
    SubqueryContext *subquery();
    antlr4::tree::TerminalNode *RR_BRACKET();
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    Comparison_operatorContext *comparison_operator();
    antlr4::tree::TerminalNode *ALL();
    antlr4::tree::TerminalNode *SOME();
    antlr4::tree::TerminalNode *ANY();
    antlr4::tree::TerminalNode *BETWEEN();
    antlr4::tree::TerminalNode *AND();
    std::vector<antlr4::tree::TerminalNode *> NOT();
    antlr4::tree::TerminalNode* NOT(size_t i);
    antlr4::tree::TerminalNode *IN();
    Expression_listContext *expression_list();
    antlr4::tree::TerminalNode *LIKE();
    antlr4::tree::TerminalNode *ESCAPE();
    antlr4::tree::TerminalNode *IS();
    Null_notnullContext *null_notnull();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  PredicateContext* predicate();

  class  Query_expressionContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Sql_unionContext *sql_unionContext = nullptr;
    std::vector<Sql_unionContext *> unions;
    Query_expressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Query_specificationContext *query_specification();
    antlr4::tree::TerminalNode *LR_BRACKET();
    Query_expressionContext *query_expression();
    antlr4::tree::TerminalNode *RR_BRACKET();
    Order_by_clauseContext *order_by_clause();
    std::vector<Sql_unionContext *> sql_union();
    Sql_unionContext* sql_union(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Query_expressionContext* query_expression();

  class  Sql_unionContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Query_specificationContext *spec = nullptr;
    TSqlParser::Query_expressionContext *op = nullptr;
    Sql_unionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *UNION();
    antlr4::tree::TerminalNode *EXCEPT();
    antlr4::tree::TerminalNode *INTERSECT();
    Query_specificationContext *query_specification();
    antlr4::tree::TerminalNode *LR_BRACKET();
    antlr4::tree::TerminalNode *RR_BRACKET();
    antlr4::tree::TerminalNode *ALL();
    Query_expressionContext *query_expression();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Sql_unionContext* sql_union();

  class  Query_specificationContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *allOrDistinct = nullptr;
    TSqlParser::Top_clauseContext *top = nullptr;
    TSqlParser::Select_listContext *columns = nullptr;
    TSqlParser::Table_nameContext *into = nullptr;
    TSqlParser::Table_sourcesContext *from = nullptr;
    TSqlParser::Search_conditionContext *where = nullptr;
    antlr4::Token *groupByAll = nullptr;
    TSqlParser::Group_by_itemContext *group_by_itemContext = nullptr;
    std::vector<Group_by_itemContext *> groupBys;
    TSqlParser::Search_conditionContext *having = nullptr;
    Query_specificationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SELECT();
    Select_listContext *select_list();
    antlr4::tree::TerminalNode *INTO();
    antlr4::tree::TerminalNode *FROM();
    antlr4::tree::TerminalNode *WHERE();
    antlr4::tree::TerminalNode *GROUP();
    antlr4::tree::TerminalNode *BY();
    antlr4::tree::TerminalNode *HAVING();
    Top_clauseContext *top_clause();
    Table_nameContext *table_name();
    Table_sourcesContext *table_sources();
    std::vector<Search_conditionContext *> search_condition();
    Search_conditionContext* search_condition(size_t i);
    std::vector<Group_by_itemContext *> group_by_item();
    Group_by_itemContext* group_by_item(size_t i);
    std::vector<antlr4::tree::TerminalNode *> ALL();
    antlr4::tree::TerminalNode* ALL(size_t i);
    antlr4::tree::TerminalNode *DISTINCT();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Query_specificationContext* query_specification();

  class  Top_clauseContext : public antlr4::ParserRuleContext {
  public:
    Top_clauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *TOP();
    Top_percentContext *top_percent();
    Top_countContext *top_count();
    antlr4::tree::TerminalNode *WITH();
    antlr4::tree::TerminalNode *TIES();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Top_clauseContext* top_clause();

  class  Top_percentContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *percent_constant = nullptr;
    TSqlParser::ExpressionContext *topper_expression = nullptr;
    Top_percentContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *PERCENT();
    antlr4::tree::TerminalNode *REAL();
    antlr4::tree::TerminalNode *FLOAT();
    antlr4::tree::TerminalNode *DECIMAL();
    antlr4::tree::TerminalNode *LR_BRACKET();
    antlr4::tree::TerminalNode *RR_BRACKET();
    ExpressionContext *expression();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Top_percentContext* top_percent();

  class  Top_countContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *count_constant = nullptr;
    TSqlParser::ExpressionContext *topcount_expression = nullptr;
    Top_countContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DECIMAL();
    antlr4::tree::TerminalNode *LR_BRACKET();
    antlr4::tree::TerminalNode *RR_BRACKET();
    ExpressionContext *expression();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Top_countContext* top_count();

  class  Order_by_clauseContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Order_by_expressionContext *order_by_expressionContext = nullptr;
    std::vector<Order_by_expressionContext *> order_bys;
    TSqlParser::ExpressionContext *offset_exp = nullptr;
    antlr4::Token *offset_rows = nullptr;
    antlr4::Token *fetch_offset = nullptr;
    TSqlParser::ExpressionContext *fetch_exp = nullptr;
    antlr4::Token *fetch_rows = nullptr;
    Order_by_clauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ORDER();
    antlr4::tree::TerminalNode *BY();
    std::vector<Order_by_expressionContext *> order_by_expression();
    Order_by_expressionContext* order_by_expression(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    antlr4::tree::TerminalNode *OFFSET();
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    std::vector<antlr4::tree::TerminalNode *> ROW();
    antlr4::tree::TerminalNode* ROW(size_t i);
    std::vector<antlr4::tree::TerminalNode *> ROWS();
    antlr4::tree::TerminalNode* ROWS(size_t i);
    antlr4::tree::TerminalNode *FETCH();
    antlr4::tree::TerminalNode *ONLY();
    antlr4::tree::TerminalNode *FIRST();
    antlr4::tree::TerminalNode *NEXT();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Order_by_clauseContext* order_by_clause();

  class  Order_by_expressionContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::ExpressionContext *order_by = nullptr;
    antlr4::Token *ascending = nullptr;
    antlr4::Token *descending = nullptr;
    Order_by_expressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *ASC();
    antlr4::tree::TerminalNode *DESC();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Order_by_expressionContext* order_by_expression();

  class  Group_by_itemContext : public antlr4::ParserRuleContext {
  public:
    Group_by_itemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ExpressionContext *expression();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Group_by_itemContext* group_by_item();

  class  Option_clauseContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::OptionContext *optionContext = nullptr;
    std::vector<OptionContext *> options;
    Option_clauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OPTION();
    antlr4::tree::TerminalNode *LR_BRACKET();
    antlr4::tree::TerminalNode *RR_BRACKET();
    std::vector<OptionContext *> option();
    OptionContext* option(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Option_clauseContext* option_clause();

  class  OptionContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *number_rows = nullptr;
    antlr4::Token *number_of_processors = nullptr;
    antlr4::Token *number_recursion = nullptr;
    OptionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *FAST();
    antlr4::tree::TerminalNode *DECIMAL();
    antlr4::tree::TerminalNode *GROUP();
    antlr4::tree::TerminalNode *HASH();
    antlr4::tree::TerminalNode *ORDER();
    antlr4::tree::TerminalNode *UNION();
    antlr4::tree::TerminalNode *MERGE();
    antlr4::tree::TerminalNode *CONCAT();
    antlr4::tree::TerminalNode *JOIN();
    antlr4::tree::TerminalNode *LOOP();
    antlr4::tree::TerminalNode *FORCE();
    antlr4::tree::TerminalNode *KEEP();
    antlr4::tree::TerminalNode *PLAN();
    antlr4::tree::TerminalNode *KEEPFIXED();
    antlr4::tree::TerminalNode *MAXDOP();
    antlr4::tree::TerminalNode *MAXRECURSION();
    antlr4::tree::TerminalNode *OPTIMIZE();
    antlr4::tree::TerminalNode *FOR();
    antlr4::tree::TerminalNode *LR_BRACKET();
    std::vector<Optimize_for_argContext *> optimize_for_arg();
    Optimize_for_argContext* optimize_for_arg(size_t i);
    antlr4::tree::TerminalNode *RR_BRACKET();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    antlr4::tree::TerminalNode *UNKNOWN();
    antlr4::tree::TerminalNode *PARAMETERIZATION();
    antlr4::tree::TerminalNode *SIMPLE();
    antlr4::tree::TerminalNode *FORCED();
    antlr4::tree::TerminalNode *RECOMPILE();
    antlr4::tree::TerminalNode *ROBUST();
    antlr4::tree::TerminalNode *USE();
    antlr4::tree::TerminalNode *STRING();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  OptionContext* option();

  class  Optimize_for_argContext : public antlr4::ParserRuleContext {
  public:
    Optimize_for_argContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LOCAL_ID();
    antlr4::tree::TerminalNode *UNKNOWN();
    antlr4::tree::TerminalNode *EQUAL();
    ConstantContext *constant();
    antlr4::tree::TerminalNode *NULL_();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Optimize_for_argContext* optimize_for_arg();

  class  Select_listContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Select_list_elemContext *select_list_elemContext = nullptr;
    std::vector<Select_list_elemContext *> selectElement;
    Select_listContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Select_list_elemContext *> select_list_elem();
    Select_list_elemContext* select_list_elem(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Select_listContext* select_list();

  class  Udt_method_argumentsContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Execute_var_stringContext *execute_var_stringContext = nullptr;
    std::vector<Execute_var_stringContext *> argument;
    Udt_method_argumentsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LR_BRACKET();
    antlr4::tree::TerminalNode *RR_BRACKET();
    std::vector<Execute_var_stringContext *> execute_var_string();
    Execute_var_stringContext* execute_var_string(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Udt_method_argumentsContext* udt_method_arguments();

  class  AsteriskContext : public antlr4::ParserRuleContext {
  public:
    AsteriskContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *STAR();
    Table_nameContext *table_name();
    antlr4::tree::TerminalNode *DOT();
    antlr4::tree::TerminalNode *INSERTED();
    antlr4::tree::TerminalNode *DELETED();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  AsteriskContext* asterisk();

  class  Column_elemContext : public antlr4::ParserRuleContext {
  public:
    Column_elemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Full_column_nameContext *full_column_name();
    antlr4::tree::TerminalNode *DOLLAR();
    antlr4::tree::TerminalNode *IDENTITY();
    antlr4::tree::TerminalNode *ROWGUID();
    antlr4::tree::TerminalNode *NULL_();
    As_column_aliasContext *as_column_alias();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Column_elemContext* column_elem();

  class  Udt_elemContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Id_Context *udt_column_name = nullptr;
    TSqlParser::Id_Context *non_static_attr = nullptr;
    TSqlParser::Id_Context *static_attr = nullptr;
    Udt_elemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DOT();
    Udt_method_argumentsContext *udt_method_arguments();
    std::vector<Id_Context *> id_();
    Id_Context* id_(size_t i);
    As_column_aliasContext *as_column_alias();
    antlr4::tree::TerminalNode *DOUBLE_COLON();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Udt_elemContext* udt_elem();

  class  Expression_elemContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Column_aliasContext *leftAlias = nullptr;
    antlr4::Token *eq = nullptr;
    TSqlParser::ExpressionContext *leftAssignment = nullptr;
    TSqlParser::ExpressionContext *expressionAs = nullptr;
    Expression_elemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Column_aliasContext *column_alias();
    antlr4::tree::TerminalNode *EQUAL();
    ExpressionContext *expression();
    As_column_aliasContext *as_column_alias();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Expression_elemContext* expression_elem();

  class  Select_list_elemContext : public antlr4::ParserRuleContext {
  public:
    Select_list_elemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    AsteriskContext *asterisk();
    Column_elemContext *column_elem();
    Udt_elemContext *udt_elem();
    antlr4::tree::TerminalNode *LOCAL_ID();
    ExpressionContext *expression();
    Assignment_operatorContext *assignment_operator();
    antlr4::tree::TerminalNode *EQUAL();
    Expression_elemContext *expression_elem();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Select_list_elemContext* select_list_elem();

  class  Table_sourcesContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Table_sourceContext *table_sourceContext = nullptr;
    std::vector<Table_sourceContext *> source;
    Table_sourcesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Table_sourceContext *> table_source();
    Table_sourceContext* table_source(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Table_sourcesContext* table_sources();

  class  Table_sourceContext : public antlr4::ParserRuleContext {
  public:
    Table_sourceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Table_source_item_joinedContext *table_source_item_joined();
    antlr4::tree::TerminalNode *LR_BRACKET();
    Table_sourceContext *table_source();
    antlr4::tree::TerminalNode *RR_BRACKET();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Table_sourceContext* table_source();

  class  Table_source_item_joinedContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Join_partContext *join_partContext = nullptr;
    std::vector<Join_partContext *> joins;
    Table_source_item_joinedContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Table_source_itemContext *table_source_item();
    std::vector<Join_partContext *> join_part();
    Join_partContext* join_part(size_t i);
    antlr4::tree::TerminalNode *LR_BRACKET();
    Table_source_item_joinedContext *table_source_item_joined();
    antlr4::tree::TerminalNode *RR_BRACKET();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Table_source_item_joinedContext* table_source_item_joined();

  class  Table_source_itemContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *loc_id = nullptr;
    antlr4::Token *loc_id_call = nullptr;
    TSqlParser::Function_callContext *loc_fcall = nullptr;
    TSqlParser::Function_callContext *oldstyle_fcall = nullptr;
    Table_source_itemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Table_name_with_hintContext *table_name_with_hint();
    As_table_aliasContext *as_table_alias();
    Full_table_nameContext *full_table_name();
    antlr4::tree::TerminalNode *LR_BRACKET();
    Derived_tableContext *derived_table();
    antlr4::tree::TerminalNode *RR_BRACKET();
    Column_alias_listContext *column_alias_list();
    Change_tableContext *change_table();
    Function_callContext *function_call();
    antlr4::tree::TerminalNode *LOCAL_ID();
    Nodes_methodContext *nodes_method();
    antlr4::tree::TerminalNode *DOT();
    antlr4::tree::TerminalNode *DOUBLE_COLON();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Table_source_itemContext* table_source_item();

  class  Schema_declarationContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Column_declarationContext *column_declarationContext = nullptr;
    std::vector<Column_declarationContext *> xml_col;
    Schema_declarationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Column_declarationContext *> column_declaration();
    Column_declarationContext* column_declaration(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Schema_declarationContext* schema_declaration();

  class  Column_declarationContext : public antlr4::ParserRuleContext {
  public:
    Column_declarationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Id_Context *id_();
    Data_typeContext *data_type();
    antlr4::tree::TerminalNode *STRING();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Column_declarationContext* column_declaration();

  class  Change_tableContext : public antlr4::ParserRuleContext {
  public:
    Change_tableContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Change_table_changesContext *change_table_changes();
    Change_table_versionContext *change_table_version();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Change_tableContext* change_table();

  class  Change_table_changesContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Table_nameContext *changetable = nullptr;
    antlr4::Token *changesid = nullptr;
    Change_table_changesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CHANGETABLE();
    antlr4::tree::TerminalNode *LR_BRACKET();
    antlr4::tree::TerminalNode *CHANGES();
    antlr4::tree::TerminalNode *COMMA();
    antlr4::tree::TerminalNode *RR_BRACKET();
    Table_nameContext *table_name();
    antlr4::tree::TerminalNode *NULL_();
    antlr4::tree::TerminalNode *DECIMAL();
    antlr4::tree::TerminalNode *LOCAL_ID();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Change_table_changesContext* change_table_changes();

  class  Change_table_versionContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Table_nameContext *versiontable = nullptr;
    TSqlParser::Full_column_name_listContext *pk_columns = nullptr;
    TSqlParser::Select_listContext *pk_values = nullptr;
    Change_table_versionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CHANGETABLE();
    antlr4::tree::TerminalNode *LR_BRACKET();
    antlr4::tree::TerminalNode *VERSION();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    antlr4::tree::TerminalNode *RR_BRACKET();
    Table_nameContext *table_name();
    Full_column_name_listContext *full_column_name_list();
    Select_listContext *select_list();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Change_table_versionContext* change_table_version();

  class  Join_partContext : public antlr4::ParserRuleContext {
  public:
    Join_partContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Join_onContext *join_on();
    Cross_joinContext *cross_join();
    Apply_Context *apply_();
    PivotContext *pivot();
    UnpivotContext *unpivot();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Join_partContext* join_part();

  class  Join_onContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *inner = nullptr;
    antlr4::Token *join_type = nullptr;
    antlr4::Token *outer = nullptr;
    antlr4::Token *join_hint = nullptr;
    TSqlParser::Table_sourceContext *source = nullptr;
    TSqlParser::Search_conditionContext *cond = nullptr;
    Join_onContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *JOIN();
    antlr4::tree::TerminalNode *ON();
    Table_sourceContext *table_source();
    Search_conditionContext *search_condition();
    antlr4::tree::TerminalNode *LEFT();
    antlr4::tree::TerminalNode *RIGHT();
    antlr4::tree::TerminalNode *FULL();
    antlr4::tree::TerminalNode *LOOP();
    antlr4::tree::TerminalNode *HASH();
    antlr4::tree::TerminalNode *MERGE();
    antlr4::tree::TerminalNode *REMOTE();
    antlr4::tree::TerminalNode *INNER();
    antlr4::tree::TerminalNode *OUTER();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Join_onContext* join_on();

  class  Cross_joinContext : public antlr4::ParserRuleContext {
  public:
    Cross_joinContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CROSS();
    antlr4::tree::TerminalNode *JOIN();
    Table_sourceContext *table_source();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Cross_joinContext* cross_join();

  class  Apply_Context : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *apply_style = nullptr;
    TSqlParser::Table_sourceContext *source = nullptr;
    Apply_Context(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *APPLY();
    Table_sourceContext *table_source();
    antlr4::tree::TerminalNode *CROSS();
    antlr4::tree::TerminalNode *OUTER();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Apply_Context* apply_();

  class  PivotContext : public antlr4::ParserRuleContext {
  public:
    PivotContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *PIVOT();
    Pivot_clauseContext *pivot_clause();
    As_table_aliasContext *as_table_alias();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  PivotContext* pivot();

  class  UnpivotContext : public antlr4::ParserRuleContext {
  public:
    UnpivotContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *UNPIVOT();
    Unpivot_clauseContext *unpivot_clause();
    As_table_aliasContext *as_table_alias();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  UnpivotContext* unpivot();

  class  Pivot_clauseContext : public antlr4::ParserRuleContext {
  public:
    Pivot_clauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LR_BRACKET();
    Aggregate_windowed_functionContext *aggregate_windowed_function();
    antlr4::tree::TerminalNode *FOR();
    Full_column_nameContext *full_column_name();
    antlr4::tree::TerminalNode *IN();
    Column_alias_listContext *column_alias_list();
    antlr4::tree::TerminalNode *RR_BRACKET();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Pivot_clauseContext* pivot_clause();

  class  Unpivot_clauseContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::ExpressionContext *unpivot_exp = nullptr;
    Unpivot_clauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> LR_BRACKET();
    antlr4::tree::TerminalNode* LR_BRACKET(size_t i);
    antlr4::tree::TerminalNode *FOR();
    Full_column_nameContext *full_column_name();
    antlr4::tree::TerminalNode *IN();
    Full_column_name_listContext *full_column_name_list();
    std::vector<antlr4::tree::TerminalNode *> RR_BRACKET();
    antlr4::tree::TerminalNode* RR_BRACKET(size_t i);
    ExpressionContext *expression();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Unpivot_clauseContext* unpivot_clause();

  class  Full_column_name_listContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Full_column_nameContext *full_column_nameContext = nullptr;
    std::vector<Full_column_nameContext *> column;
    Full_column_name_listContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Full_column_nameContext *> full_column_name();
    Full_column_nameContext* full_column_name(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Full_column_name_listContext* full_column_name_list();

  class  Table_name_with_hintContext : public antlr4::ParserRuleContext {
  public:
    Table_name_with_hintContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Table_nameContext *table_name();
    With_table_hintsContext *with_table_hints();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Table_name_with_hintContext* table_name_with_hint();

  class  Bulk_optionContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *bulk_option_value = nullptr;
    Bulk_optionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Id_Context *id_();
    antlr4::tree::TerminalNode *EQUAL();
    antlr4::tree::TerminalNode *DECIMAL();
    antlr4::tree::TerminalNode *STRING();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Bulk_optionContext* bulk_option();

  class  Derived_tableContext : public antlr4::ParserRuleContext {
  public:
    Derived_tableContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SubqueryContext *subquery();
    antlr4::tree::TerminalNode *LR_BRACKET();
    antlr4::tree::TerminalNode *RR_BRACKET();
    Table_value_constructorContext *table_value_constructor();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Derived_tableContext* derived_table();

  class  Function_callContext : public antlr4::ParserRuleContext {
  public:
    Function_callContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    Function_callContext() = default;
    void copyFrom(Function_callContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  RANKING_WINDOWED_FUNCContext : public Function_callContext {
  public:
    RANKING_WINDOWED_FUNCContext(Function_callContext *ctx);

    Ranking_windowed_functionContext *ranking_windowed_function();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  BUILT_IN_FUNCContext : public Function_callContext {
  public:
    BUILT_IN_FUNCContext(Function_callContext *ctx);

    Built_in_functionsContext *built_in_functions();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  ANALYTIC_WINDOWED_FUNCContext : public Function_callContext {
  public:
    ANALYTIC_WINDOWED_FUNCContext(Function_callContext *ctx);

    Analytic_windowed_functionContext *analytic_windowed_function();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  SCALAR_FUNCTIONContext : public Function_callContext {
  public:
    SCALAR_FUNCTIONContext(Function_callContext *ctx);

    Scalar_function_nameContext *scalar_function_name();
    antlr4::tree::TerminalNode *LR_BRACKET();
    antlr4::tree::TerminalNode *RR_BRACKET();
    Expression_listContext *expression_list();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  PARTITION_FUNCContext : public Function_callContext {
  public:
    PARTITION_FUNCContext(Function_callContext *ctx);

    Partition_functionContext *partition_function();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  AGGREGATE_WINDOWED_FUNCContext : public Function_callContext {
  public:
    AGGREGATE_WINDOWED_FUNCContext(Function_callContext *ctx);

    Aggregate_windowed_functionContext *aggregate_windowed_function();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  Function_callContext* function_call();

  class  Partition_functionContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Id_Context *database = nullptr;
    TSqlParser::Id_Context *func_name = nullptr;
    Partition_functionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DOLLAR_PARTITION();
    std::vector<antlr4::tree::TerminalNode *> DOT();
    antlr4::tree::TerminalNode* DOT(size_t i);
    antlr4::tree::TerminalNode *LR_BRACKET();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *RR_BRACKET();
    std::vector<Id_Context *> id_();
    Id_Context* id_(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Partition_functionContext* partition_function();

  class  Built_in_functionsContext : public antlr4::ParserRuleContext {
  public:
    Built_in_functionsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    Built_in_functionsContext() = default;
    void copyFrom(Built_in_functionsContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  CASTContext : public Built_in_functionsContext {
  public:
    CASTContext(Built_in_functionsContext *ctx);

    antlr4::tree::TerminalNode *CAST();
    antlr4::tree::TerminalNode *LR_BRACKET();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *AS();
    Data_typeContext *data_type();
    antlr4::tree::TerminalNode *RR_BRACKET();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  CURRENT_USERContext : public Built_in_functionsContext {
  public:
    CURRENT_USERContext(Built_in_functionsContext *ctx);

    antlr4::tree::TerminalNode *CURRENT_USER();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  DATEADDContext : public Built_in_functionsContext {
  public:
    DATEADDContext(Built_in_functionsContext *ctx);

    antlr4::Token *datepart = nullptr;
    TSqlParser::ExpressionContext *number = nullptr;
    TSqlParser::ExpressionContext *date = nullptr;
    antlr4::tree::TerminalNode *DATEADD();
    antlr4::tree::TerminalNode *LR_BRACKET();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    antlr4::tree::TerminalNode *RR_BRACKET();
    antlr4::tree::TerminalNode *ID();
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  CHECKSUMContext : public Built_in_functionsContext {
  public:
    CHECKSUMContext(Built_in_functionsContext *ctx);

    antlr4::tree::TerminalNode *CHECKSUM();
    antlr4::tree::TerminalNode *LR_BRACKET();
    antlr4::tree::TerminalNode *STAR();
    antlr4::tree::TerminalNode *RR_BRACKET();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  CURRENT_TIMESTAMPContext : public Built_in_functionsContext {
  public:
    CURRENT_TIMESTAMPContext(Built_in_functionsContext *ctx);

    antlr4::tree::TerminalNode *CURRENT_TIMESTAMP();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  MIN_ACTIVE_ROWVERSIONContext : public Built_in_functionsContext {
  public:
    MIN_ACTIVE_ROWVERSIONContext(Built_in_functionsContext *ctx);

    antlr4::tree::TerminalNode *MIN_ACTIVE_ROWVERSION();
    antlr4::tree::TerminalNode *LR_BRACKET();
    antlr4::tree::TerminalNode *RR_BRACKET();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  BINARY_CHECKSUMContext : public Built_in_functionsContext {
  public:
    BINARY_CHECKSUMContext(Built_in_functionsContext *ctx);

    antlr4::tree::TerminalNode *BINARY_CHECKSUM();
    antlr4::tree::TerminalNode *LR_BRACKET();
    antlr4::tree::TerminalNode *STAR();
    antlr4::tree::TerminalNode *RR_BRACKET();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  IIFContext : public Built_in_functionsContext {
  public:
    IIFContext(Built_in_functionsContext *ctx);

    TSqlParser::Search_conditionContext *cond = nullptr;
    TSqlParser::ExpressionContext *left = nullptr;
    TSqlParser::ExpressionContext *right = nullptr;
    antlr4::tree::TerminalNode *IIF();
    antlr4::tree::TerminalNode *LR_BRACKET();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    antlr4::tree::TerminalNode *RR_BRACKET();
    Search_conditionContext *search_condition();
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  DATEPARTContext : public Built_in_functionsContext {
  public:
    DATEPARTContext(Built_in_functionsContext *ctx);

    antlr4::Token *datepart = nullptr;
    TSqlParser::ExpressionContext *date = nullptr;
    antlr4::tree::TerminalNode *DATEPART();
    antlr4::tree::TerminalNode *LR_BRACKET();
    antlr4::tree::TerminalNode *COMMA();
    antlr4::tree::TerminalNode *RR_BRACKET();
    antlr4::tree::TerminalNode *ID();
    ExpressionContext *expression();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  USERContext : public Built_in_functionsContext {
  public:
    USERContext(Built_in_functionsContext *ctx);

    antlr4::tree::TerminalNode *USER();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  STUFFContext : public Built_in_functionsContext {
  public:
    STUFFContext(Built_in_functionsContext *ctx);

    TSqlParser::ExpressionContext *str = nullptr;
    antlr4::Token *from = nullptr;
    antlr4::Token *to = nullptr;
    TSqlParser::ExpressionContext *str_with = nullptr;
    antlr4::tree::TerminalNode *STUFF();
    antlr4::tree::TerminalNode *LR_BRACKET();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    antlr4::tree::TerminalNode *RR_BRACKET();
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    std::vector<antlr4::tree::TerminalNode *> DECIMAL();
    antlr4::tree::TerminalNode* DECIMAL(size_t i);
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  SYSTEM_USERContext : public Built_in_functionsContext {
  public:
    SYSTEM_USERContext(Built_in_functionsContext *ctx);

    antlr4::tree::TerminalNode *SYSTEM_USER();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  NULLIFContext : public Built_in_functionsContext {
  public:
    NULLIFContext(Built_in_functionsContext *ctx);

    TSqlParser::ExpressionContext *left = nullptr;
    TSqlParser::ExpressionContext *right = nullptr;
    antlr4::tree::TerminalNode *NULLIF();
    antlr4::tree::TerminalNode *LR_BRACKET();
    antlr4::tree::TerminalNode *COMMA();
    antlr4::tree::TerminalNode *RR_BRACKET();
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  SESSION_USERContext : public Built_in_functionsContext {
  public:
    SESSION_USERContext(Built_in_functionsContext *ctx);

    antlr4::tree::TerminalNode *SESSION_USER();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  CONVERTContext : public Built_in_functionsContext {
  public:
    CONVERTContext(Built_in_functionsContext *ctx);

    TSqlParser::Data_typeContext *convert_data_type = nullptr;
    TSqlParser::ExpressionContext *convert_expression = nullptr;
    TSqlParser::ExpressionContext *style = nullptr;
    antlr4::tree::TerminalNode *CONVERT();
    antlr4::tree::TerminalNode *LR_BRACKET();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    antlr4::tree::TerminalNode *RR_BRACKET();
    Data_typeContext *data_type();
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  IDENTITYContext : public Built_in_functionsContext {
  public:
    IDENTITYContext(Built_in_functionsContext *ctx);

    antlr4::Token *seed = nullptr;
    antlr4::Token *increment = nullptr;
    antlr4::tree::TerminalNode *IDENTITY();
    antlr4::tree::TerminalNode *LR_BRACKET();
    Data_typeContext *data_type();
    antlr4::tree::TerminalNode *RR_BRACKET();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    std::vector<antlr4::tree::TerminalNode *> DECIMAL();
    antlr4::tree::TerminalNode* DECIMAL(size_t i);
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  COALESCEContext : public Built_in_functionsContext {
  public:
    COALESCEContext(Built_in_functionsContext *ctx);

    antlr4::tree::TerminalNode *COALESCE();
    antlr4::tree::TerminalNode *LR_BRACKET();
    Expression_listContext *expression_list();
    antlr4::tree::TerminalNode *RR_BRACKET();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  DATENAMEContext : public Built_in_functionsContext {
  public:
    DATENAMEContext(Built_in_functionsContext *ctx);

    antlr4::Token *datepart = nullptr;
    TSqlParser::ExpressionContext *date = nullptr;
    antlr4::tree::TerminalNode *DATENAME();
    antlr4::tree::TerminalNode *LR_BRACKET();
    antlr4::tree::TerminalNode *COMMA();
    antlr4::tree::TerminalNode *RR_BRACKET();
    antlr4::tree::TerminalNode *ID();
    ExpressionContext *expression();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  GETUTCDATEContext : public Built_in_functionsContext {
  public:
    GETUTCDATEContext(Built_in_functionsContext *ctx);

    antlr4::tree::TerminalNode *GETUTCDATE();
    antlr4::tree::TerminalNode *LR_BRACKET();
    antlr4::tree::TerminalNode *RR_BRACKET();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  TRY_CASTContext : public Built_in_functionsContext {
  public:
    TRY_CASTContext(Built_in_functionsContext *ctx);

    antlr4::tree::TerminalNode *TRY_CAST();
    antlr4::tree::TerminalNode *LR_BRACKET();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *AS();
    Data_typeContext *data_type();
    antlr4::tree::TerminalNode *RR_BRACKET();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  ISNULLContext : public Built_in_functionsContext {
  public:
    ISNULLContext(Built_in_functionsContext *ctx);

    TSqlParser::ExpressionContext *left = nullptr;
    TSqlParser::ExpressionContext *right = nullptr;
    antlr4::tree::TerminalNode *ISNULL();
    antlr4::tree::TerminalNode *LR_BRACKET();
    antlr4::tree::TerminalNode *COMMA();
    antlr4::tree::TerminalNode *RR_BRACKET();
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  DATEDIFFContext : public Built_in_functionsContext {
  public:
    DATEDIFFContext(Built_in_functionsContext *ctx);

    antlr4::Token *datepart = nullptr;
    TSqlParser::ExpressionContext *date_first = nullptr;
    TSqlParser::ExpressionContext *date_second = nullptr;
    antlr4::tree::TerminalNode *DATEDIFF();
    antlr4::tree::TerminalNode *LR_BRACKET();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    antlr4::tree::TerminalNode *RR_BRACKET();
    antlr4::tree::TerminalNode *ID();
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  GETDATEContext : public Built_in_functionsContext {
  public:
    GETDATEContext(Built_in_functionsContext *ctx);

    antlr4::tree::TerminalNode *GETDATE();
    antlr4::tree::TerminalNode *LR_BRACKET();
    antlr4::tree::TerminalNode *RR_BRACKET();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  STRINGAGGContext : public Built_in_functionsContext {
  public:
    STRINGAGGContext(Built_in_functionsContext *ctx);

    TSqlParser::ExpressionContext *expr = nullptr;
    TSqlParser::ExpressionContext *separator = nullptr;
    antlr4::tree::TerminalNode *STRING_AGG();
    std::vector<antlr4::tree::TerminalNode *> LR_BRACKET();
    antlr4::tree::TerminalNode* LR_BRACKET(size_t i);
    antlr4::tree::TerminalNode *COMMA();
    std::vector<antlr4::tree::TerminalNode *> RR_BRACKET();
    antlr4::tree::TerminalNode* RR_BRACKET(size_t i);
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    antlr4::tree::TerminalNode *WITHIN();
    antlr4::tree::TerminalNode *GROUP();
    Order_by_clauseContext *order_by_clause();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  Built_in_functionsContext* built_in_functions();

  class  Nodes_methodContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *loc_id = nullptr;
    TSqlParser::Id_Context *value_id = nullptr;
    antlr4::Token *xquery = nullptr;
    Nodes_methodContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DOT();
    antlr4::tree::TerminalNode *NODES();
    std::vector<antlr4::tree::TerminalNode *> LR_BRACKET();
    antlr4::tree::TerminalNode* LR_BRACKET(size_t i);
    std::vector<antlr4::tree::TerminalNode *> RR_BRACKET();
    antlr4::tree::TerminalNode* RR_BRACKET(size_t i);
    antlr4::tree::TerminalNode *STRING();
    SubqueryContext *subquery();
    antlr4::tree::TerminalNode *LOCAL_ID();
    Id_Context *id_();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Nodes_methodContext* nodes_method();

  class  Switch_sectionContext : public antlr4::ParserRuleContext {
  public:
    Switch_sectionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *WHEN();
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    antlr4::tree::TerminalNode *THEN();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Switch_sectionContext* switch_section();

  class  Switch_search_condition_sectionContext : public antlr4::ParserRuleContext {
  public:
    Switch_search_condition_sectionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *WHEN();
    Search_conditionContext *search_condition();
    antlr4::tree::TerminalNode *THEN();
    ExpressionContext *expression();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Switch_search_condition_sectionContext* switch_search_condition_section();

  class  As_column_aliasContext : public antlr4::ParserRuleContext {
  public:
    As_column_aliasContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Column_aliasContext *column_alias();
    antlr4::tree::TerminalNode *AS();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  As_column_aliasContext* as_column_alias();

  class  As_table_aliasContext : public antlr4::ParserRuleContext {
  public:
    As_table_aliasContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Table_aliasContext *table_alias();
    antlr4::tree::TerminalNode *AS();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  As_table_aliasContext* as_table_alias();

  class  Table_aliasContext : public antlr4::ParserRuleContext {
  public:
    Table_aliasContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Id_Context *id_();
    With_table_hintsContext *with_table_hints();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Table_aliasContext* table_alias();

  class  With_table_hintsContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Table_hintContext *table_hintContext = nullptr;
    std::vector<Table_hintContext *> hint;
    With_table_hintsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LR_BRACKET();
    antlr4::tree::TerminalNode *RR_BRACKET();
    std::vector<Table_hintContext *> table_hint();
    Table_hintContext* table_hint(size_t i);
    antlr4::tree::TerminalNode *WITH();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  With_table_hintsContext* with_table_hints();

  class  Insert_with_table_hintsContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Table_hintContext *table_hintContext = nullptr;
    std::vector<Table_hintContext *> hint;
    Insert_with_table_hintsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *WITH();
    antlr4::tree::TerminalNode *LR_BRACKET();
    antlr4::tree::TerminalNode *RR_BRACKET();
    std::vector<Table_hintContext *> table_hint();
    Table_hintContext* table_hint(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Insert_with_table_hintsContext* insert_with_table_hints();

  class  Table_hintContext : public antlr4::ParserRuleContext {
  public:
    Table_hintContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INDEX();
    antlr4::tree::TerminalNode *EQUAL();
    std::vector<Index_valueContext *> index_value();
    Index_valueContext* index_value(size_t i);
    antlr4::tree::TerminalNode *FORCESEEK();
    antlr4::tree::TerminalNode *SERIALIZABLE();
    antlr4::tree::TerminalNode *SNAPSHOT();
    antlr4::tree::TerminalNode *SPATIAL_WINDOW_MAX_CELLS();
    antlr4::tree::TerminalNode *DECIMAL();
    antlr4::tree::TerminalNode *HOLDLOCK();
    std::vector<antlr4::tree::TerminalNode *> ID();
    antlr4::tree::TerminalNode* ID(size_t i);
    antlr4::tree::TerminalNode *NOEXPAND();
    std::vector<antlr4::tree::TerminalNode *> LR_BRACKET();
    antlr4::tree::TerminalNode* LR_BRACKET(size_t i);
    std::vector<antlr4::tree::TerminalNode *> RR_BRACKET();
    antlr4::tree::TerminalNode* RR_BRACKET(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Table_hintContext* table_hint();

  class  Index_valueContext : public antlr4::ParserRuleContext {
  public:
    Index_valueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Id_Context *id_();
    antlr4::tree::TerminalNode *DECIMAL();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Index_valueContext* index_value();

  class  Column_alias_listContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Column_aliasContext *column_aliasContext = nullptr;
    std::vector<Column_aliasContext *> alias;
    Column_alias_listContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LR_BRACKET();
    antlr4::tree::TerminalNode *RR_BRACKET();
    std::vector<Column_aliasContext *> column_alias();
    Column_aliasContext* column_alias(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Column_alias_listContext* column_alias_list();

  class  Column_aliasContext : public antlr4::ParserRuleContext {
  public:
    Column_aliasContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Id_Context *id_();
    antlr4::tree::TerminalNode *STRING();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Column_aliasContext* column_alias();

  class  Table_value_constructorContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Expression_listContext *expression_listContext = nullptr;
    std::vector<Expression_listContext *> exps;
    Table_value_constructorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *VALUES();
    std::vector<antlr4::tree::TerminalNode *> LR_BRACKET();
    antlr4::tree::TerminalNode* LR_BRACKET(size_t i);
    std::vector<antlr4::tree::TerminalNode *> RR_BRACKET();
    antlr4::tree::TerminalNode* RR_BRACKET(size_t i);
    std::vector<Expression_listContext *> expression_list();
    Expression_listContext* expression_list(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Table_value_constructorContext* table_value_constructor();

  class  Expression_listContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::ExpressionContext *expressionContext = nullptr;
    std::vector<ExpressionContext *> exp;
    Expression_listContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Expression_listContext* expression_list();

  class  Ranking_windowed_functionContext : public antlr4::ParserRuleContext {
  public:
    Ranking_windowed_functionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LR_BRACKET();
    antlr4::tree::TerminalNode *RR_BRACKET();
    Over_clauseContext *over_clause();
    antlr4::tree::TerminalNode *RANK();
    antlr4::tree::TerminalNode *DENSE_RANK();
    antlr4::tree::TerminalNode *ROW_NUMBER();
    antlr4::tree::TerminalNode *NTILE();
    ExpressionContext *expression();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Ranking_windowed_functionContext* ranking_windowed_function();

  class  Aggregate_windowed_functionContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *agg_func = nullptr;
    antlr4::Token *cnt = nullptr;
    Aggregate_windowed_functionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LR_BRACKET();
    All_distinct_expressionContext *all_distinct_expression();
    antlr4::tree::TerminalNode *RR_BRACKET();
    antlr4::tree::TerminalNode *AVG();
    antlr4::tree::TerminalNode *MAX();
    antlr4::tree::TerminalNode *MIN();
    antlr4::tree::TerminalNode *SUM();
    antlr4::tree::TerminalNode *STDEV();
    antlr4::tree::TerminalNode *STDEVP();
    antlr4::tree::TerminalNode *VAR();
    antlr4::tree::TerminalNode *VARP();
    Over_clauseContext *over_clause();
    antlr4::tree::TerminalNode *COUNT();
    antlr4::tree::TerminalNode *COUNT_BIG();
    antlr4::tree::TerminalNode *STAR();
    antlr4::tree::TerminalNode *CHECKSUM_AGG();
    antlr4::tree::TerminalNode *GROUPING();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *GROUPING_ID();
    Expression_listContext *expression_list();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Aggregate_windowed_functionContext* aggregate_windowed_function();

  class  Analytic_windowed_functionContext : public antlr4::ParserRuleContext {
  public:
    Analytic_windowed_functionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> LR_BRACKET();
    antlr4::tree::TerminalNode* LR_BRACKET(size_t i);
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    std::vector<antlr4::tree::TerminalNode *> RR_BRACKET();
    antlr4::tree::TerminalNode* RR_BRACKET(size_t i);
    Over_clauseContext *over_clause();
    antlr4::tree::TerminalNode *FIRST_VALUE();
    antlr4::tree::TerminalNode *LAST_VALUE();
    antlr4::tree::TerminalNode *LAG();
    antlr4::tree::TerminalNode *LEAD();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    antlr4::tree::TerminalNode *OVER();
    Order_by_clauseContext *order_by_clause();
    antlr4::tree::TerminalNode *CUME_DIST();
    antlr4::tree::TerminalNode *PERCENT_RANK();
    antlr4::tree::TerminalNode *PARTITION();
    std::vector<antlr4::tree::TerminalNode *> BY();
    antlr4::tree::TerminalNode* BY(size_t i);
    Expression_listContext *expression_list();
    antlr4::tree::TerminalNode *WITHIN();
    antlr4::tree::TerminalNode *GROUP();
    antlr4::tree::TerminalNode *ORDER();
    antlr4::tree::TerminalNode *PERCENTILE_CONT();
    antlr4::tree::TerminalNode *PERCENTILE_DISC();
    antlr4::tree::TerminalNode *ASC();
    antlr4::tree::TerminalNode *DESC();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Analytic_windowed_functionContext* analytic_windowed_function();

  class  All_distinct_expressionContext : public antlr4::ParserRuleContext {
  public:
    All_distinct_expressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *ALL();
    antlr4::tree::TerminalNode *DISTINCT();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  All_distinct_expressionContext* all_distinct_expression();

  class  Over_clauseContext : public antlr4::ParserRuleContext {
  public:
    Over_clauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OVER();
    antlr4::tree::TerminalNode *LR_BRACKET();
    antlr4::tree::TerminalNode *RR_BRACKET();
    antlr4::tree::TerminalNode *PARTITION();
    antlr4::tree::TerminalNode *BY();
    Expression_listContext *expression_list();
    Order_by_clauseContext *order_by_clause();
    Row_or_range_clauseContext *row_or_range_clause();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Over_clauseContext* over_clause();

  class  Row_or_range_clauseContext : public antlr4::ParserRuleContext {
  public:
    Row_or_range_clauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Window_frame_extentContext *window_frame_extent();
    antlr4::tree::TerminalNode *ROWS();
    antlr4::tree::TerminalNode *RANGE();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Row_or_range_clauseContext* row_or_range_clause();

  class  Window_frame_extentContext : public antlr4::ParserRuleContext {
  public:
    Window_frame_extentContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Window_frame_precedingContext *window_frame_preceding();
    antlr4::tree::TerminalNode *BETWEEN();
    std::vector<Window_frame_boundContext *> window_frame_bound();
    Window_frame_boundContext* window_frame_bound(size_t i);
    antlr4::tree::TerminalNode *AND();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Window_frame_extentContext* window_frame_extent();

  class  Window_frame_boundContext : public antlr4::ParserRuleContext {
  public:
    Window_frame_boundContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Window_frame_precedingContext *window_frame_preceding();
    Window_frame_followingContext *window_frame_following();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Window_frame_boundContext* window_frame_bound();

  class  Window_frame_precedingContext : public antlr4::ParserRuleContext {
  public:
    Window_frame_precedingContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *UNBOUNDED();
    antlr4::tree::TerminalNode *PRECEDING();
    antlr4::tree::TerminalNode *DECIMAL();
    antlr4::tree::TerminalNode *CURRENT();
    antlr4::tree::TerminalNode *ROW();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Window_frame_precedingContext* window_frame_preceding();

  class  Window_frame_followingContext : public antlr4::ParserRuleContext {
  public:
    Window_frame_followingContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *UNBOUNDED();
    antlr4::tree::TerminalNode *FOLLOWING();
    antlr4::tree::TerminalNode *DECIMAL();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Window_frame_followingContext* window_frame_following();

  class  Entity_nameContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Id_Context *server = nullptr;
    TSqlParser::Id_Context *database = nullptr;
    TSqlParser::Id_Context *schema = nullptr;
    TSqlParser::Id_Context *table = nullptr;
    Entity_nameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Id_Context *> id_();
    Id_Context* id_(size_t i);
    std::vector<antlr4::tree::TerminalNode *> DOT();
    antlr4::tree::TerminalNode* DOT(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Entity_nameContext* entity_name();

  class  Entity_name_for_azure_dwContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Id_Context *schema = nullptr;
    TSqlParser::Id_Context *object_name = nullptr;
    Entity_name_for_azure_dwContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Id_Context *> id_();
    Id_Context* id_(size_t i);
    antlr4::tree::TerminalNode *DOT();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Entity_name_for_azure_dwContext* entity_name_for_azure_dw();

  class  Entity_name_for_parallel_dwContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Id_Context *schema_database = nullptr;
    TSqlParser::Id_Context *schema = nullptr;
    TSqlParser::Id_Context *object_name = nullptr;
    Entity_name_for_parallel_dwContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Id_Context *> id_();
    Id_Context* id_(size_t i);
    antlr4::tree::TerminalNode *DOT();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Entity_name_for_parallel_dwContext* entity_name_for_parallel_dw();

  class  Full_table_nameContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Id_Context *server = nullptr;
    TSqlParser::Id_Context *database = nullptr;
    TSqlParser::Id_Context *schema = nullptr;
    TSqlParser::Id_Context *table = nullptr;
    Full_table_nameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Id_Context *> id_();
    Id_Context* id_(size_t i);
    std::vector<antlr4::tree::TerminalNode *> DOT();
    antlr4::tree::TerminalNode* DOT(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Full_table_nameContext* full_table_name();

  class  Table_nameContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Id_Context *database = nullptr;
    TSqlParser::Id_Context *schema = nullptr;
    TSqlParser::Id_Context *table = nullptr;
    antlr4::Token *blocking_hierarchy = nullptr;
    Table_nameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Id_Context *> id_();
    Id_Context* id_(size_t i);
    std::vector<antlr4::tree::TerminalNode *> DOT();
    antlr4::tree::TerminalNode* DOT(size_t i);
    antlr4::tree::TerminalNode *BLOCKING_HIERARCHY();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Table_nameContext* table_name();

  class  Simple_nameContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Id_Context *schema = nullptr;
    TSqlParser::Id_Context *name = nullptr;
    Simple_nameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Id_Context *> id_();
    Id_Context* id_(size_t i);
    antlr4::tree::TerminalNode *DOT();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Simple_nameContext* simple_name();

  class  Func_proc_name_schemaContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Id_Context *schema = nullptr;
    TSqlParser::Id_Context *procedure = nullptr;
    Func_proc_name_schemaContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Id_Context *> id_();
    Id_Context* id_(size_t i);
    antlr4::tree::TerminalNode *DOT();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Func_proc_name_schemaContext* func_proc_name_schema();

  class  Func_proc_name_database_schemaContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Id_Context *database = nullptr;
    TSqlParser::Id_Context *schema = nullptr;
    TSqlParser::Id_Context *procedure = nullptr;
    Func_proc_name_database_schemaContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> DOT();
    antlr4::tree::TerminalNode* DOT(size_t i);
    std::vector<Id_Context *> id_();
    Id_Context* id_(size_t i);
    Func_proc_name_schemaContext *func_proc_name_schema();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Func_proc_name_database_schemaContext* func_proc_name_database_schema();

  class  Func_proc_name_server_database_schemaContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Id_Context *server = nullptr;
    TSqlParser::Id_Context *database = nullptr;
    TSqlParser::Id_Context *schema = nullptr;
    TSqlParser::Id_Context *procedure = nullptr;
    Func_proc_name_server_database_schemaContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> DOT();
    antlr4::tree::TerminalNode* DOT(size_t i);
    std::vector<Id_Context *> id_();
    Id_Context* id_(size_t i);
    Func_proc_name_database_schemaContext *func_proc_name_database_schema();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Func_proc_name_server_database_schemaContext* func_proc_name_server_database_schema();

  class  Ddl_objectContext : public antlr4::ParserRuleContext {
  public:
    Ddl_objectContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Full_table_nameContext *full_table_name();
    antlr4::tree::TerminalNode *LOCAL_ID();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Ddl_objectContext* ddl_object();

  class  Full_column_nameContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Id_Context *column_name = nullptr;
    TSqlParser::Id_Context *server = nullptr;
    TSqlParser::Id_Context *schema = nullptr;
    TSqlParser::Id_Context *tablename = nullptr;
    Full_column_nameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> DOT();
    antlr4::tree::TerminalNode* DOT(size_t i);
    antlr4::tree::TerminalNode *DELETED();
    antlr4::tree::TerminalNode *INSERTED();
    std::vector<Id_Context *> id_();
    Id_Context* id_(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Full_column_nameContext* full_column_name();

  class  Column_name_list_with_orderContext : public antlr4::ParserRuleContext {
  public:
    Column_name_list_with_orderContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Id_Context *> id_();
    Id_Context* id_(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    std::vector<antlr4::tree::TerminalNode *> ASC();
    antlr4::tree::TerminalNode* ASC(size_t i);
    std::vector<antlr4::tree::TerminalNode *> DESC();
    antlr4::tree::TerminalNode* DESC(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Column_name_list_with_orderContext* column_name_list_with_order();

  class  Insert_column_name_listContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Insert_column_idContext *insert_column_idContext = nullptr;
    std::vector<Insert_column_idContext *> col;
    Insert_column_name_listContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Insert_column_idContext *> insert_column_id();
    Insert_column_idContext* insert_column_id(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Insert_column_name_listContext* insert_column_name_list();

  class  Insert_column_idContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Id_Context *id_Context = nullptr;
    std::vector<Id_Context *> ignore;
    Insert_column_idContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Id_Context *> id_();
    Id_Context* id_(size_t i);
    std::vector<antlr4::tree::TerminalNode *> DOT();
    antlr4::tree::TerminalNode* DOT(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Insert_column_idContext* insert_column_id();

  class  Column_name_listContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Id_Context *id_Context = nullptr;
    std::vector<Id_Context *> col;
    Column_name_listContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Id_Context *> id_();
    Id_Context* id_(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Column_name_listContext* column_name_list();

  class  Cursor_nameContext : public antlr4::ParserRuleContext {
  public:
    Cursor_nameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Id_Context *id_();
    antlr4::tree::TerminalNode *LOCAL_ID();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Cursor_nameContext* cursor_name();

  class  On_offContext : public antlr4::ParserRuleContext {
  public:
    On_offContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ON();
    antlr4::tree::TerminalNode *OFF();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  On_offContext* on_off();

  class  Null_notnullContext : public antlr4::ParserRuleContext {
  public:
    Null_notnullContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NULL_();
    antlr4::tree::TerminalNode *NOT();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Null_notnullContext* null_notnull();

  class  Null_or_defaultContext : public antlr4::ParserRuleContext {
  public:
    Null_or_defaultContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Null_notnullContext *null_notnull();
    antlr4::tree::TerminalNode *DEFAULT();
    Constant_expressionContext *constant_expression();
    antlr4::tree::TerminalNode *COLLATE();
    Id_Context *id_();
    antlr4::tree::TerminalNode *WITH();
    antlr4::tree::TerminalNode *VALUES();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Null_or_defaultContext* null_or_default();

  class  Scalar_function_nameContext : public antlr4::ParserRuleContext {
  public:
    Scalar_function_nameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Func_proc_name_server_database_schemaContext *func_proc_name_server_database_schema();
    antlr4::tree::TerminalNode *RIGHT();
    antlr4::tree::TerminalNode *LEFT();
    antlr4::tree::TerminalNode *BINARY_CHECKSUM();
    antlr4::tree::TerminalNode *CHECKSUM();
    antlr4::tree::TerminalNode *ABS();
    antlr4::tree::TerminalNode *ASCII();
    antlr4::tree::TerminalNode *CEILING();
    antlr4::tree::TerminalNode *CHAR();
    antlr4::tree::TerminalNode *CHARINDEX();
    antlr4::tree::TerminalNode *DATALENGTH();
    antlr4::tree::TerminalNode *DAY();
    antlr4::tree::TerminalNode *FLOOR();
    antlr4::tree::TerminalNode *ISDATE();
    antlr4::tree::TerminalNode *ISNUMERIC();
    antlr4::tree::TerminalNode *LEN();
    antlr4::tree::TerminalNode *LOWER();
    antlr4::tree::TerminalNode *LTRIM();
    antlr4::tree::TerminalNode *MONTH();
    antlr4::tree::TerminalNode *NCHAR();
    antlr4::tree::TerminalNode *PATINDEX();
    antlr4::tree::TerminalNode *RAND();
    antlr4::tree::TerminalNode *REPLACE();
    antlr4::tree::TerminalNode *ROUND();
    antlr4::tree::TerminalNode *RTRIM();
    antlr4::tree::TerminalNode *SIGN();
    antlr4::tree::TerminalNode *SPACE();
    antlr4::tree::TerminalNode *STR();
    antlr4::tree::TerminalNode *SUBSTRING();
    antlr4::tree::TerminalNode *UPPER();
    antlr4::tree::TerminalNode *USER_NAME();
    antlr4::tree::TerminalNode *YEAR();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Scalar_function_nameContext* scalar_function_name();

  class  Data_typeContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *scaled = nullptr;
    TSqlParser::Id_Context *ext_type = nullptr;
    antlr4::Token *scale = nullptr;
    antlr4::Token *prec = nullptr;
    antlr4::Token *seed = nullptr;
    antlr4::Token *inc = nullptr;
    antlr4::Token *double_prec = nullptr;
    TSqlParser::Id_Context *unscaled_type = nullptr;
    Data_typeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LR_BRACKET();
    antlr4::tree::TerminalNode *MAX();
    antlr4::tree::TerminalNode *RR_BRACKET();
    antlr4::tree::TerminalNode *NTEXT();
    antlr4::tree::TerminalNode *TEXT();
    antlr4::tree::TerminalNode *NCHAR();
    antlr4::tree::TerminalNode *CHAR();
    antlr4::tree::TerminalNode *VARCHAR();
    antlr4::tree::TerminalNode *NVARCHAR();
    antlr4::tree::TerminalNode *BINARY_KEYWORD();
    antlr4::tree::TerminalNode *VARBINARY_KEYWORD();
    antlr4::tree::TerminalNode *COMMA();
    Id_Context *id_();
    std::vector<antlr4::tree::TerminalNode *> DECIMAL();
    antlr4::tree::TerminalNode* DECIMAL(size_t i);
    antlr4::tree::TerminalNode *IDENTITY();
    antlr4::tree::TerminalNode *DOUBLE();
    antlr4::tree::TerminalNode *PRECISION();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Data_typeContext* data_type();

  class  Default_valueContext : public antlr4::ParserRuleContext {
  public:
    Default_valueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NULL_();
    antlr4::tree::TerminalNode *DEFAULT();
    ConstantContext *constant();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Default_valueContext* default_value();

  class  ConstantContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *dollar = nullptr;
    ConstantContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *STRING();
    antlr4::tree::TerminalNode *BINARY();
    antlr4::tree::TerminalNode *DECIMAL();
    SignContext *sign();
    antlr4::tree::TerminalNode *REAL();
    antlr4::tree::TerminalNode *FLOAT();
    antlr4::tree::TerminalNode *DOLLAR();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ConstantContext* constant();

  class  SignContext : public antlr4::ParserRuleContext {
  public:
    SignContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *PLUS();
    antlr4::tree::TerminalNode *MINUS();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  SignContext* sign();

  class  KeywordContext : public antlr4::ParserRuleContext {
  public:
    KeywordContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ABSOLUTE();
    antlr4::tree::TerminalNode *ACTION();
    antlr4::tree::TerminalNode *AES_128();
    antlr4::tree::TerminalNode *AES_192();
    antlr4::tree::TerminalNode *AES_256();
    antlr4::tree::TerminalNode *ALGORITHM();
    antlr4::tree::TerminalNode *ANSI_NULLS();
    antlr4::tree::TerminalNode *ANSI_PADDING();
    antlr4::tree::TerminalNode *ANSI_WARNINGS();
    antlr4::tree::TerminalNode *APPLY();
    antlr4::tree::TerminalNode *ARITHABORT();
    antlr4::tree::TerminalNode *AT_KEYWORD();
    antlr4::tree::TerminalNode *AUTO();
    antlr4::tree::TerminalNode *AVG();
    antlr4::tree::TerminalNode *BIGINT();
    antlr4::tree::TerminalNode *BINARY_BASE64();
    antlr4::tree::TerminalNode *BINARY_CHECKSUM();
    antlr4::tree::TerminalNode *CALLER();
    antlr4::tree::TerminalNode *CAST();
    antlr4::tree::TerminalNode *CATCH();
    antlr4::tree::TerminalNode *CHECKSUM();
    antlr4::tree::TerminalNode *CHECKSUM_AGG();
    antlr4::tree::TerminalNode *COMMITTED();
    antlr4::tree::TerminalNode *CONCAT();
    antlr4::tree::TerminalNode *CONCAT_NULL_YIELDS_NULL();
    antlr4::tree::TerminalNode *COUNT();
    antlr4::tree::TerminalNode *COUNT_BIG();
    antlr4::tree::TerminalNode *CURSOR_CLOSE_ON_COMMIT();
    antlr4::tree::TerminalNode *CURSOR_DEFAULT();
    antlr4::tree::TerminalNode *DATA();
    antlr4::tree::TerminalNode *DATEADD();
    antlr4::tree::TerminalNode *DATEDIFF();
    antlr4::tree::TerminalNode *DATENAME();
    antlr4::tree::TerminalNode *DATEPART();
    antlr4::tree::TerminalNode *DAYS();
    antlr4::tree::TerminalNode *DEFAULT_DOUBLE_QUOTE();
    antlr4::tree::TerminalNode *DELAYED_DURABILITY();
    antlr4::tree::TerminalNode *DELETED();
    antlr4::tree::TerminalNode *DENSE_RANK();
    antlr4::tree::TerminalNode *DES();
    antlr4::tree::TerminalNode *DESCRIPTION();
    antlr4::tree::TerminalNode *DESX();
    antlr4::tree::TerminalNode *DISABLE();
    antlr4::tree::TerminalNode *DYNAMIC();
    antlr4::tree::TerminalNode *ELEMENTS();
    antlr4::tree::TerminalNode *EMPTY();
    antlr4::tree::TerminalNode *ENCRYPTION();
    antlr4::tree::TerminalNode *EXCLUSIVE();
    antlr4::tree::TerminalNode *FAST();
    antlr4::tree::TerminalNode *FAST_FORWARD();
    antlr4::tree::TerminalNode *FIRST();
    antlr4::tree::TerminalNode *FIRST_VALUE();
    antlr4::tree::TerminalNode *FOLLOWING();
    antlr4::tree::TerminalNode *FORCE();
    antlr4::tree::TerminalNode *FORCED();
    antlr4::tree::TerminalNode *FORWARD_ONLY();
    antlr4::tree::TerminalNode *GETDATE();
    antlr4::tree::TerminalNode *GETUTCDATE();
    antlr4::tree::TerminalNode *GLOBAL();
    antlr4::tree::TerminalNode *GO();
    antlr4::tree::TerminalNode *GO_BATCH();
    antlr4::tree::TerminalNode *GROUPING();
    antlr4::tree::TerminalNode *GROUPING_ID();
    antlr4::tree::TerminalNode *HASH();
    antlr4::tree::TerminalNode *HIGH();
    antlr4::tree::TerminalNode *HOURS();
    antlr4::tree::TerminalNode *IDENTITY_VALUE();
    antlr4::tree::TerminalNode *INCLUDE_NULL_VALUES();
    antlr4::tree::TerminalNode *INPUT();
    antlr4::tree::TerminalNode *INSENSITIVE();
    antlr4::tree::TerminalNode *INSERTED();
    antlr4::tree::TerminalNode *INT();
    antlr4::tree::TerminalNode *ISOLATION();
    antlr4::tree::TerminalNode *KEEP();
    antlr4::tree::TerminalNode *KEEPFIXED();
    antlr4::tree::TerminalNode *KEYSET();
    antlr4::tree::TerminalNode *LAG();
    antlr4::tree::TerminalNode *LAST();
    antlr4::tree::TerminalNode *LAST_VALUE();
    antlr4::tree::TerminalNode *LEAD();
    antlr4::tree::TerminalNode *LEVEL();
    antlr4::tree::TerminalNode *LOCAL();
    antlr4::tree::TerminalNode *LOCATION();
    antlr4::tree::TerminalNode *LOCK();
    antlr4::tree::TerminalNode *LOCK_ESCALATION();
    antlr4::tree::TerminalNode *LOOP();
    antlr4::tree::TerminalNode *LOW();
    antlr4::tree::TerminalNode *MANUAL();
    antlr4::tree::TerminalNode *MARK();
    antlr4::tree::TerminalNode *MAX();
    antlr4::tree::TerminalNode *MAXDOP();
    antlr4::tree::TerminalNode *MAXRECURSION();
    antlr4::tree::TerminalNode *MIN();
    antlr4::tree::TerminalNode *MINUTES();
    antlr4::tree::TerminalNode *MODE();
    antlr4::tree::TerminalNode *NEXT();
    antlr4::tree::TerminalNode *NO();
    antlr4::tree::TerminalNode *NOCOUNT();
    antlr4::tree::TerminalNode *NODES();
    antlr4::tree::TerminalNode *NOEXPAND();
    antlr4::tree::TerminalNode *NOWAIT();
    antlr4::tree::TerminalNode *NTILE();
    antlr4::tree::TerminalNode *NUMANODE();
    antlr4::tree::TerminalNode *NUMERIC_ROUNDABORT();
    antlr4::tree::TerminalNode *OBJECT();
    antlr4::tree::TerminalNode *OFFSET();
    antlr4::tree::TerminalNode *ONLINE();
    antlr4::tree::TerminalNode *ONLY();
    antlr4::tree::TerminalNode *OPTIMISTIC();
    antlr4::tree::TerminalNode *OPTIMIZE();
    antlr4::tree::TerminalNode *OUT();
    antlr4::tree::TerminalNode *OUTPUT();
    antlr4::tree::TerminalNode *OWNER();
    antlr4::tree::TerminalNode *PARAMETERIZATION();
    antlr4::tree::TerminalNode *PARTITION();
    antlr4::tree::TerminalNode *PARTITIONS();
    antlr4::tree::TerminalNode *PRECEDING();
    antlr4::tree::TerminalNode *PRIOR();
    antlr4::tree::TerminalNode *QUERY();
    antlr4::tree::TerminalNode *QUOTED_IDENTIFIER();
    antlr4::tree::TerminalNode *RANGE();
    antlr4::tree::TerminalNode *RANK();
    antlr4::tree::TerminalNode *RC2();
    antlr4::tree::TerminalNode *RC4();
    antlr4::tree::TerminalNode *RC4_128();
    antlr4::tree::TerminalNode *READ_ONLY();
    antlr4::tree::TerminalNode *READONLY();
    antlr4::tree::TerminalNode *REBUILD();
    antlr4::tree::TerminalNode *RECOMPILE();
    antlr4::tree::TerminalNode *RELATIVE();
    antlr4::tree::TerminalNode *REMOTE();
    antlr4::tree::TerminalNode *REPEATABLE();
    antlr4::tree::TerminalNode *ROBUST();
    antlr4::tree::TerminalNode *ROW();
    antlr4::tree::TerminalNode *ROW_NUMBER();
    antlr4::tree::TerminalNode *ROWGUID();
    antlr4::tree::TerminalNode *ROWS();
    antlr4::tree::TerminalNode *SCHEMABINDING();
    antlr4::tree::TerminalNode *SCROLL();
    antlr4::tree::TerminalNode *SCROLL_LOCKS();
    antlr4::tree::TerminalNode *SECONDS();
    antlr4::tree::TerminalNode *SELF();
    antlr4::tree::TerminalNode *SEMI_SENSITIVE();
    antlr4::tree::TerminalNode *SEQUENCE();
    antlr4::tree::TerminalNode *SERIALIZABLE();
    antlr4::tree::TerminalNode *SETERROR();
    antlr4::tree::TerminalNode *SHARE();
    antlr4::tree::TerminalNode *SIMPLE();
    antlr4::tree::TerminalNode *SIZE();
    antlr4::tree::TerminalNode *SMALLINT();
    antlr4::tree::TerminalNode *SNAPSHOT();
    antlr4::tree::TerminalNode *SPATIAL_WINDOW_MAX_CELLS();
    antlr4::tree::TerminalNode *STATIC();
    antlr4::tree::TerminalNode *STATUSONLY();
    antlr4::tree::TerminalNode *STDEV();
    antlr4::tree::TerminalNode *STDEVP();
    antlr4::tree::TerminalNode *STRING_AGG();
    antlr4::tree::TerminalNode *STUFF();
    antlr4::tree::TerminalNode *SUBSCRIPTION();
    antlr4::tree::TerminalNode *SUM();
    antlr4::tree::TerminalNode *TEXTIMAGE_ON();
    antlr4::tree::TerminalNode *THROW();
    antlr4::tree::TerminalNode *TIES();
    antlr4::tree::TerminalNode *TIME();
    antlr4::tree::TerminalNode *TINYINT();
    antlr4::tree::TerminalNode *TRIPLE_DES();
    antlr4::tree::TerminalNode *TRIPLE_DES_3KEY();
    antlr4::tree::TerminalNode *TRY();
    antlr4::tree::TerminalNode *TWO_DIGIT_YEAR_CUTOFF();
    antlr4::tree::TerminalNode *TYPE();
    antlr4::tree::TerminalNode *TYPE_WARNING();
    antlr4::tree::TerminalNode *UNBOUNDED();
    antlr4::tree::TerminalNode *UNCOMMITTED();
    antlr4::tree::TerminalNode *UNKNOWN();
    antlr4::tree::TerminalNode *UOW();
    antlr4::tree::TerminalNode *USING();
    antlr4::tree::TerminalNode *VAR();
    antlr4::tree::TerminalNode *VARP();
    antlr4::tree::TerminalNode *WAIT();
    antlr4::tree::TerminalNode *WORK();
    antlr4::tree::TerminalNode *BLOCKING_HIERARCHY();
    antlr4::tree::TerminalNode *CACHE();
    antlr4::tree::TerminalNode *CALLED();
    antlr4::tree::TerminalNode *CHANGETABLE();
    antlr4::tree::TerminalNode *CHANGES();
    antlr4::tree::TerminalNode *CUME_DIST();
    antlr4::tree::TerminalNode *CYCLE();
    antlr4::tree::TerminalNode *DATA_COMPRESSION();
    antlr4::tree::TerminalNode *ERROR();
    antlr4::tree::TerminalNode *FORCESEEK();
    antlr4::tree::TerminalNode *IIF();
    antlr4::tree::TerminalNode *INCREMENT();
    antlr4::tree::TerminalNode *ISNULL();
    antlr4::tree::TerminalNode *LOG();
    antlr4::tree::TerminalNode *MATCHED();
    antlr4::tree::TerminalNode *MAXVALUE();
    antlr4::tree::TerminalNode *MINVALUE();
    antlr4::tree::TerminalNode *NOTIFICATION();
    antlr4::tree::TerminalNode *PERCENTILE_CONT();
    antlr4::tree::TerminalNode *PERCENTILE_DISC();
    antlr4::tree::TerminalNode *PERCENT_RANK();
    antlr4::tree::TerminalNode *PERSISTED();
    antlr4::tree::TerminalNode *PREDICATE();
    antlr4::tree::TerminalNode *RESTART();
    antlr4::tree::TerminalNode *RETURNS();
    antlr4::tree::TerminalNode *SOURCE();
    antlr4::tree::TerminalNode *STATE();
    antlr4::tree::TerminalNode *START();
    antlr4::tree::TerminalNode *TARGET();
    antlr4::tree::TerminalNode *TRY_CAST();
    antlr4::tree::TerminalNode *ZONE();
    antlr4::tree::TerminalNode *TEXT();
    antlr4::tree::TerminalNode *NTEXT();
    antlr4::tree::TerminalNode *CHAR();
    antlr4::tree::TerminalNode *NCHAR();
    antlr4::tree::TerminalNode *VARCHAR();
    antlr4::tree::TerminalNode *NVARCHAR();
    antlr4::tree::TerminalNode *BINARY_KEYWORD();
    antlr4::tree::TerminalNode *VARBINARY_KEYWORD();
    antlr4::tree::TerminalNode *PRECISION();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  KeywordContext* keyword();

  class  Id_Context : public antlr4::ParserRuleContext {
  public:
    Id_Context(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ID();
    antlr4::tree::TerminalNode *DOUBLE_QUOTE_ID();
    antlr4::tree::TerminalNode *SQUARE_BRACKET_ID();
    KeywordContext *keyword();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Id_Context* id_();

  class  Simple_idContext : public antlr4::ParserRuleContext {
  public:
    Simple_idContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ID();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Simple_idContext* simple_id();

  class  Id_or_stringContext : public antlr4::ParserRuleContext {
  public:
    Id_or_stringContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Id_Context *id_();
    antlr4::tree::TerminalNode *STRING();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Id_or_stringContext* id_or_string();

  class  Comparison_operatorContext : public antlr4::ParserRuleContext {
  public:
    Comparison_operatorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EQUAL();
    antlr4::tree::TerminalNode *GREATER();
    antlr4::tree::TerminalNode *LESS();
    antlr4::tree::TerminalNode *EXCLAMATION();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Comparison_operatorContext* comparison_operator();

  class  Assignment_operatorContext : public antlr4::ParserRuleContext {
  public:
    Assignment_operatorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *PLUS_ASSIGN();
    antlr4::tree::TerminalNode *MINUS_ASSIGN();
    antlr4::tree::TerminalNode *MULT_ASSIGN();
    antlr4::tree::TerminalNode *DIV_ASSIGN();
    antlr4::tree::TerminalNode *MOD_ASSIGN();
    antlr4::tree::TerminalNode *AND_ASSIGN();
    antlr4::tree::TerminalNode *XOR_ASSIGN();
    antlr4::tree::TerminalNode *OR_ASSIGN();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Assignment_operatorContext* assignment_operator();


  virtual bool sempred(antlr4::RuleContext *_localctx, size_t ruleIndex, size_t predicateIndex) override;
  bool expressionSempred(ExpressionContext *_localctx, size_t predicateIndex);
  bool search_conditionSempred(Search_conditionContext *_localctx, size_t predicateIndex);

private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};

