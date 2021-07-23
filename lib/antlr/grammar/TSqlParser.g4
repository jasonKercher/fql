/*
T-SQL (Transact-SQL, MSSQL) grammar.
The MIT License (MIT).
Copyright (c) 2017, Mark Adams (madams51703@gmail.com)
Copyright (c) 2015-2017, Ivan Kochurkin (kvanttt@gmail.com), Positive Technologies.
Copyright (c) 2016, Scott Ure (scott@redstormsoftware.com).
Copyright (c) 2016, Rui Zhang (ruizhang.ccs@gmail.com).
Copyright (c) 2016, Marcus Henriksson (kuseman80@gmail.com).
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

parser grammar TSqlParser;

options { tokenVocab=TSqlLexer; }

tsql_file
    : batch* EOF
    | execute_body_batch go_batch_statement* EOF
    ;

batch
    : go_batch_statement
    | execute_body_batch? (go_batch_statement | sql_clauses+) go_statement*
    | batch_level_statement go_statement*
    | go_statement
    ;

batch_level_statement
    : create_or_alter_function
    | create_or_alter_procedure
    ;
sql_clauses
    : dml_clause SEMI?
    | cfl_statement SEMI?
    | another_statement SEMI?
    | ddl_clause SEMI?
    | dbcc_special SEMI?
    | dbcc_clause SEMI?
    | SEMI
    ;

// Data Manipulation Language: https://msdn.microsoft.com/en-us/library/ff848766(v=sql.120).aspx
dml_clause
    : merge_statement
    | delete_statement
    | insert_statement
    | select_statement_standalone
    | update_statement
    ;

// Data Definition Language: https://msdn.microsoft.com/en-us/library/ff848799.aspx)
ddl_clause
    : alter_sequence
    | alter_table
    | create_sequence
    | create_table
    | drop_function
    | drop_procedure
    | drop_table
    | lock_table
    | truncate_table
    ;

// Control-of-Flow Language: https://docs.microsoft.com/en-us/sql/t-sql/language-elements/control-of-flow
cfl_statement
    : block_statement
    | break_statement
    | continue_statement
    | goto_statement
    | if_statement
    | return_statement
    | throw_statement
    | try_catch_statement
    | while_statement
    | print_statement
    | raiseerror_statement
    ;

// https://docs.microsoft.com/en-us/sql/t-sql/language-elements/begin-end-transact-sql
block_statement
    : BEGIN ';'? sql_clauses* END ';'?
    ;

// https://docs.microsoft.com/en-us/sql/t-sql/language-elements/break-transact-sql
break_statement
    : BREAK ';'?
    ;

// https://docs.microsoft.com/en-us/sql/t-sql/language-elements/continue-transact-sql
continue_statement
    : CONTINUE ';'?
    ;

// https://docs.microsoft.com/en-us/sql/t-sql/language-elements/goto-transact-sql
goto_statement
    : GOTO id_ ';'?
    | id_ ':' ';'?
    ;

// https://docs.microsoft.com/en-us/sql/t-sql/language-elements/return-transact-sql
return_statement
    : RETURN expression? ';'?
    ;

// https://docs.microsoft.com/en-us/sql/t-sql/language-elements/if-else-transact-sql
if_statement
    : IF search_condition sql_clauses (ELSE sql_clauses)? ';'?
    ;

// https://docs.microsoft.com/en-us/sql/t-sql/language-elements/throw-transact-sql
throw_statement
    : THROW (throw_error_number ',' throw_message ',' throw_state)? ';'?
    ;

throw_error_number
    : DECIMAL | LOCAL_ID
    ;

throw_message
    : STRING | LOCAL_ID
    ;

throw_state
    : DECIMAL | LOCAL_ID
    ;

// https://docs.microsoft.com/en-us/sql/t-sql/language-elements/try-catch-transact-sql
try_catch_statement
    : BEGIN TRY ';'? try_clauses=sql_clauses+ END TRY ';'? BEGIN CATCH ';'? catch_clauses=sql_clauses* END CATCH ';'?
    ;

// https://docs.microsoft.com/en-us/sql/t-sql/language-elements/while-transact-sql
while_statement
    : WHILE search_condition (sql_clauses | BREAK ';'? | CONTINUE ';'?)
    ;

// https://docs.microsoft.com/en-us/sql/t-sql/language-elements/print-transact-sql
print_statement
    : PRINT (expression | DOUBLE_QUOTE_ID) (',' LOCAL_ID)* ';'?
    ;

// https://docs.microsoft.com/en-us/sql/t-sql/language-elements/raiserror-transact-sql
raiseerror_statement
    : RAISERROR '(' msg=(DECIMAL | STRING | LOCAL_ID) ',' severity=constant_LOCAL_ID ','
    state=constant_LOCAL_ID (',' constant_LOCAL_ID)* ')' (WITH (LOG | SETERROR | NOWAIT))? ';'?
    | RAISERROR DECIMAL formatstring=(STRING | LOCAL_ID | DOUBLE_QUOTE_ID) (',' argument=(DECIMAL | STRING | LOCAL_ID))*
    ;

empty_statement
    : ';'
    ;

another_statement
    : declare_statement
    | execute_statement
    | cursor_statement
    | kill_statement
    | set_statement
    | transaction_statement
    | use_statement
    | shutdown_statement
    ;

entity_to
    : TO
    ;

colon_colon
    : DOUBLE_COLON
    ;

class_type
    : OBJECT
    | TYPE
    | SCHEMA
    ;

class_type_for_sql_database
    : OBJECT
    | TYPE
    | SCHEMA
    ;

class_type_for_azure_dw
    : SCHEMA
    | OBJECT
    ;

class_type_for_parallel_dw
    : DATABASE
    | SCHEMA
    | OBJECT
    ;

lock_table
    : LOCK TABLE table_name IN (SHARE | EXCLUSIVE) MODE (WAIT seconds=DECIMAL | NOWAIT)? ';'?
    ;

// https://docs.microsoft.com/en-us/sql/t-sql/statements/truncate-table-transact-sql
truncate_table
    : TRUNCATE TABLE table_name
          ( WITH LR_BRACKET
              PARTITIONS LR_BRACKET
                                (COMMA? (DECIMAL|DECIMAL TO DECIMAL) )+
                         RR_BRACKET

                 RR_BRACKET
          )?
    ;

// https://docs.microsoft.com/en-us/sql/t-sql/statements/alter-sequence-transact-sql
alter_sequence
    : ALTER SEQUENCE (schema_name=id_ DOT)? sequence_name=id_ ( RESTART (WITH DECIMAL)? )? (INCREMENT BY sequnce_increment=DECIMAL )? ( MINVALUE DECIMAL| NO MINVALUE)? (MAXVALUE DECIMAL| NO MAXVALUE)? (CYCLE|NO CYCLE)? (CACHE DECIMAL | NO CACHE)?
    ;

// https://docs.microsoft.com/en-us/sql/t-sql/statements/create-sequence-transact-sql
create_sequence
    : CREATE SEQUENCE (schema_name=id_ DOT)? sequence_name=id_
        (AS data_type  )?
        (START WITH DECIMAL)?
        (INCREMENT BY MINUS? DECIMAL)?
        (MINVALUE DECIMAL? | NO MINVALUE)?
        (MAXVALUE DECIMAL? | NO MAXVALUE)?
        (CYCLE|NO CYCLE)?
        (CACHE DECIMAL? | NO CACHE)?
    ;

// DML

// https://docs.microsoft.com/en-us/sql/t-sql/statements/merge-transact-sql
// note that there's a limit on number of when_matches but it has to be done runtime due to different ordering of statements allowed
merge_statement
    : with_expression?
      MERGE (TOP '(' expression ')' PERCENT?)?
      INTO? ddl_object insert_with_table_hints? as_table_alias?
      USING table_sources
      ON search_condition
      when_matches+
      output_clause?
      option_clause? ';'
    ;

when_matches
    : (WHEN MATCHED (AND search_condition)?
          THEN merge_matched)+
    | (WHEN NOT MATCHED (BY TARGET)? (AND search_condition)?
          THEN merge_not_matched)
    | (WHEN NOT MATCHED BY SOURCE (AND search_condition)?
          THEN merge_matched)+
    ;

merge_matched
    : UPDATE SET update_elem_merge (',' update_elem_merge)*
    | DELETE
    ;

merge_not_matched
    : INSERT ('(' column_name_list ')')?
      (table_value_constructor | DEFAULT VALUES)
    ;

// https://msdn.microsoft.com/en-us/library/ms189835.aspx
delete_statement
    : with_expression?
      DELETE (TOP '(' expression ')' PERCENT? | TOP DECIMAL)?
      FROM? delete_statement_from
      insert_with_table_hints?
      output_clause?
      (FROM table_sources)?
      (WHERE (search_condition | CURRENT OF (GLOBAL? cursor_name | cursor_var=LOCAL_ID)))?
      option_clause? ';'?
    ;

delete_statement_from
    : ddl_object
    | table_alias
    | table_var=LOCAL_ID
    ;

// https://msdn.microsoft.com/en-us/library/ms174335.aspx
insert_statement
    : with_expression?
      INSERT (TOP '(' expression ')' PERCENT?)?
      INTO? (ddl_object)
      insert_with_table_hints?
      ('(' insert_column_name_list ')')?
      output_clause?
      insert_statement_value
    ;

insert_statement_value
    : table_value_constructor
    | derived_table
    | execute_statement
    | DEFAULT VALUES
    ;


// https://msdn.microsoft.com/en-us/library/ms189499.aspx
select_statement_standalone
    : with_expression? select_statement
    ;

select_statement
    : query_expression order_by_clause? option_clause? ';'?
    ;

time
    : (LOCAL_ID | constant)
    ;

// https://msdn.microsoft.com/en-us/library/ms177523.aspx
update_statement
    : with_expression?
      UPDATE (TOP '(' expression ')' PERCENT?)?
      (ddl_object)
      with_table_hints?
      SET update_elem (',' update_elem)*
      output_clause?
      (FROM table_sources)?
      (WHERE (search_condition | CURRENT OF (GLOBAL? cursor_name | cursor_var=LOCAL_ID)))?
      option_clause? ';'?
    ;

// https://msdn.microsoft.com/en-us/library/ms177564.aspx
output_clause
    : OUTPUT output_dml_list_elem (',' output_dml_list_elem)*
      (INTO (LOCAL_ID | table_name) ('(' column_name_list ')')? )?
    ;

output_dml_list_elem
    : (expression | asterisk) as_column_alias?
    ;

// DDL

// https://msdn.microsoft.com/en-us/library/ms187926(v=sql.120).aspx
create_or_alter_procedure
    : ((CREATE (OR ALTER)?) | ALTER) proc=(PROC | PROCEDURE) procName=func_proc_name_schema (';' DECIMAL)?
      ('('? procedure_param (',' procedure_param)* ')'?)?
      (WITH procedure_option (',' procedure_option)*)?
      (FOR REPLICATION)? AS (sql_clauses*)
    ;

// https://msdn.microsoft.com/en-us/library/ms186755.aspx
create_or_alter_function
    : ((CREATE (OR ALTER)?) | ALTER) FUNCTION funcName=func_proc_name_schema
        (('(' procedure_param (',' procedure_param)* ')') | '(' ')') //must have (), but can be empty
        (func_body_returns_select | func_body_returns_table | func_body_returns_scalar) ';'?
    ;

func_body_returns_select
    : RETURNS TABLE
        (WITH function_option (',' function_option)*)?
        AS?
        RETURN ('(' select_statement_standalone ')' | select_statement_standalone)
    ;

func_body_returns_table
    : RETURNS LOCAL_ID table_type_definition
        (WITH function_option (',' function_option)*)?
        AS?
        BEGIN
           sql_clauses*
           RETURN ';'?
        END ';'?
    ;

func_body_returns_scalar
    : RETURNS data_type
        (WITH function_option (',' function_option)*)?
        AS?
        BEGIN
           sql_clauses*
           RETURN ret=expression ';'?
       END
    ;

procedure_param
    : LOCAL_ID AS? (type_schema=id_ '.')? data_type VARYING? ('=' default_val=default_value)? (OUT | OUTPUT | READONLY)?
    ;

procedure_option
    : ENCRYPTION
    | RECOMPILE
    | execute_clause
    ;

function_option
    : ENCRYPTION
    | SCHEMABINDING
    | RETURNS NULL_ ON NULL_ INPUT
    | CALLED ON NULL_ INPUT
    | execute_clause
    ;

// https://msdn.microsoft.com/en-us/library/ms174979.aspx
create_table
    : CREATE TABLE table_name '(' column_def_table_constraints    ','? ')' (LOCK simple_id)? table_options* (ON id_ | DEFAULT)? (TEXTIMAGE_ON id_ | DEFAULT)?';'?
    ;

table_options
    : WITH ('(' index_option (',' index_option)* ')' | index_option (',' index_option)*)
    ;

// https://msdn.microsoft.com/en-us/library/ms190273.aspx
alter_table
    : ALTER TABLE table_name (SET '(' LOCK_ESCALATION '=' (AUTO | TABLE | DISABLE) ')'
                             | ADD column_def_table_constraints
                             | ALTER COLUMN column_definition
                             | DROP COLUMN id_ (',' id_)*
                             | DROP CONSTRAINT constraint=id_
                             | WITH CHECK ADD CONSTRAINT constraint=id_ FOREIGN KEY '(' fk = column_name_list ')' REFERENCES table_name '(' pk = column_name_list')'
                             | CHECK CONSTRAINT constraint=id_
                             | REBUILD table_options)
                             ';'?
    ;

// https://msdn.microsoft.com/en-us/library/bb522682.aspx
// Runtime check.
database_optionspec
    : cursor_option
    ;

cursor_option
    : CURSOR_CLOSE_ON_COMMIT on_off
    | CURSOR_DEFAULT ( LOCAL | GLOBAL )
    ;

// https://msdn.microsoft.com/en-us/library/ms174969.aspx
drop_procedure
    : DROP proc=(PROC | PROCEDURE) (IF EXISTS)? func_proc_name_schema (',' func_proc_name_schema)* ';'?
    ;

// https://msdn.microsoft.com/en-us/library/ms190290.aspx
drop_function
    : DROP FUNCTION (IF EXISTS)? func_proc_name_schema (',' func_proc_name_schema)* ';'?
    ;

// https://msdn.microsoft.com/en-us/library/ms173790.aspx
drop_table
    : DROP TABLE (IF EXISTS)? table_name ';'?
    ;

// Other statements.

// https://msdn.microsoft.com/en-us/library/ms188927.aspx
declare_statement
    : DECLARE LOCAL_ID AS? table_type_definition ';'?
    | DECLARE loc+=declare_local (',' loc+=declare_local)* ';'?
    ;

// https://msdn.microsoft.com/en-us/library/ms181441(v=sql.120).aspx
cursor_statement
    // https://msdn.microsoft.com/en-us/library/ms175035(v=sql.120).aspx
    : CLOSE GLOBAL? cursor_name ';'?
    // https://msdn.microsoft.com/en-us/library/ms188782(v=sql.120).aspx
    | DEALLOCATE GLOBAL? CURSOR? cursor_name ';'?
    // https://msdn.microsoft.com/en-us/library/ms180169(v=sql.120).aspx
    | declare_cursor
    // https://msdn.microsoft.com/en-us/library/ms180152(v=sql.120).aspx
    | fetch_cursor
    // https://msdn.microsoft.com/en-us/library/ms190500(v=sql.120).aspx
    | OPEN GLOBAL? cursor_name ';'?
    ;

kill_statement
    : KILL (kill_process | kill_query_notification)
    ;

// https://docs.microsoft.com/en-us/sql/t-sql/language-elements/kill-transact-sql
kill_process
    : (session_id=(DECIMAL|STRING) | UOW) (WITH STATUSONLY)?
    ;

// https://docs.microsoft.com/en-us/sql/t-sql/language-elements/kill-query-notification-subscription-transact-sql
kill_query_notification
    : QUERY NOTIFICATION SUBSCRIPTION (ALL | subscription_id=DECIMAL)
    ;

// https://msdn.microsoft.com/en-us/library/ms188332.aspx
execute_statement
    : EXECUTE execute_body ';'?
    ;

execute_body_batch
    : func_proc_name_server_database_schema (execute_statement_arg (',' execute_statement_arg)*)? ';'?
    ;

execute_body
    : (return_status=LOCAL_ID '=')? (func_proc_name_server_database_schema | execute_var_string)  execute_statement_arg?
    | '(' execute_var_string ('+' execute_var_string)* ')'
    ;

execute_statement_arg
    :
    execute_statement_arg_unnamed (',' execute_statement_arg) *    //Unnamed params can continue unnamed
    |
    execute_statement_arg_named (',' execute_statement_arg_named)* //Named can only be continued by unnamed
    ;

execute_statement_arg_named
    : name=LOCAL_ID '=' value=execute_parameter
    ;

execute_statement_arg_unnamed
    : value=execute_parameter
    ;

execute_parameter
    : (constant | LOCAL_ID (OUTPUT | OUT)? | id_ | DEFAULT | NULL_)
    ;

execute_var_string
    : LOCAL_ID
    | STRING
    ;

algorithm
    : DES
    | TRIPLE_DES
    | TRIPLE_DES_3KEY
    | RC2
    | RC4
    | RC4_128
    | DESX
    | AES_128
    | AES_192
    | AES_256
    ;

// https://msdn.microsoft.com/en-us/library/ms190356.aspx
// https://msdn.microsoft.com/en-us/library/ms189484.aspx
set_statement
    : SET LOCAL_ID ('.' member_name=id_)? '=' expression ';'?
    | SET LOCAL_ID assignment_operator expression ';'?
    | SET LOCAL_ID '='
      CURSOR declare_set_cursor_common (FOR (READ ONLY | UPDATE (OF column_name_list)?))? ';'?
    // https://msdn.microsoft.com/en-us/library/ms189837.aspx
    | set_special
    ;

// https://msdn.microsoft.com/en-us/library/ms174377.aspx
transaction_statement
    // https://msdn.microsoft.com/en-us/library/ms188386.aspx
    : BEGIN DISTRIBUTED (TRAN | TRANSACTION) (id_ | LOCAL_ID)? ';'?
    // https://msdn.microsoft.com/en-us/library/ms188929.aspx
    | BEGIN (TRAN | TRANSACTION) ((id_ | LOCAL_ID) (WITH MARK STRING)?)? ';'?
    // https://msdn.microsoft.com/en-us/library/ms190295.aspx
    | COMMIT (TRAN | TRANSACTION) ((id_ | LOCAL_ID) (WITH '(' DELAYED_DURABILITY EQUAL (OFF | ON) ')')?)? ';'?
    // https://msdn.microsoft.com/en-us/library/ms178628.aspx
    | COMMIT WORK? ';'?
    | COMMIT id_
    | ROLLBACK id_
    // https://msdn.microsoft.com/en-us/library/ms181299.aspx
    | ROLLBACK (TRAN | TRANSACTION) (id_ | LOCAL_ID)? ';'?
    // https://msdn.microsoft.com/en-us/library/ms174973.aspx
    | ROLLBACK WORK? ';'?
    // https://msdn.microsoft.com/en-us/library/ms188378.aspx
    | SAVE (TRAN | TRANSACTION) (id_ | LOCAL_ID)? ';'?
    ;

// https://msdn.microsoft.com/en-us/library/ms188037.aspx
go_batch_statement
    : GO_BATCH (count=DECIMAL)?
    ;

go_statement
    : GO (count=DECIMAL)?
    ;

// https://msdn.microsoft.com/en-us/library/ms188366.aspx
use_statement
    : USE database=id_ ';'?
    ;

// https://docs.microsoft.com/en-us/sql/t-sql/language-elements/shutdown-transact-sql
shutdown_statement
    : SHUTDOWN (WITH NOWAIT)?
    ;

//These are dbcc commands with strange syntax that doesn't fit the regular dbcc syntax
dbcc_special
    : DBCC SHRINKLOG ('(' SIZE '='  (constant_expression| id_ | DEFAULT) ')')? ';'?
    ;

dbcc_clause
    : DBCC name=dbcc_command ('(' expression_list ')')? (WITH dbcc_options)? ';'?
    ;

dbcc_command
    : simple_id | keyword
    ;

dbcc_options
    :  simple_id (',' simple_id)?
    ;

execute_clause
    : EXECUTE AS clause=(CALLER | SELF | OWNER | STRING)
    ;

declare_local
    : LOCAL_ID AS? data_type ('=' expression)?
    ;

table_type_definition
    : TABLE '(' column_def_table_constraints   ')'
    ;

column_def_table_constraints
    : column_def_table_constraint (','? column_def_table_constraint)*
    ;

column_def_table_constraint
    : column_definition
    | table_constraint
    ;

// https://msdn.microsoft.com/en-us/library/ms187742.aspx
column_definition
    : id_ (data_type | AS expression PERSISTED? ) (COLLATE id_)? null_notnull?
      ((CONSTRAINT constraint=id_)? null_or_default null_or_default?
       | IDENTITY ('(' seed=DECIMAL ',' increment=DECIMAL ')')? (NOT FOR REPLICATION)?)?
      ROWGUIDCOL?
      column_constraint*
    ;

// https://msdn.microsoft.com/en-us/library/ms186712.aspx
column_constraint
    :(CONSTRAINT constraint=id_)?
      ((PRIMARY KEY | UNIQUE) index_options?
      | CHECK (NOT FOR REPLICATION)? '(' search_condition ')'
      | (FOREIGN KEY)? REFERENCES table_name '(' pk = column_name_list')'
      | null_notnull)
    ;

// https://msdn.microsoft.com/en-us/library/ms188066.aspx
table_constraint
    : (CONSTRAINT constraint=id_)?
       ((PRIMARY KEY | UNIQUE) '(' column_name_list_with_order ')' index_options? (ON id_)?
         | CHECK (NOT FOR REPLICATION)? '(' search_condition ')'
         | DEFAULT '('?  (STRING | PLUS | function_call | DECIMAL)+ ')'? FOR id_
         | FOREIGN KEY '(' fk = column_name_list ')' REFERENCES table_name ('(' pk = column_name_list')')?)
    ;

index_options
    : WITH '(' index_option (',' index_option)* ')'
    ;

// https://msdn.microsoft.com/en-us/library/ms186869.aspx
// Id runtime checking. Id in (PAD_INDEX, FILLFACTOR, IGNORE_DUP_KEY, STATISTICS_NORECOMPUTE, ALLOW_ROW_LOCKS,
// ALLOW_PAGE_LOCKS, SORT_IN_TEMPDB, ONLINE, MAXDOP, DATA_COMPRESSION, ONLINE).
index_option
    : (simple_id | keyword) '=' (simple_id | keyword | on_off | DECIMAL)
    ;

// https://msdn.microsoft.com/en-us/library/ms180169.aspx
declare_cursor
    : DECLARE cursor_name
      (CURSOR (declare_set_cursor_common (FOR UPDATE (OF column_name_list)?)?)?
      | (SEMI_SENSITIVE | INSENSITIVE)? SCROLL? CURSOR FOR select_statement_standalone (FOR (READ ONLY | UPDATE | (OF column_name_list)))?
      ) ';'?
    ;

declare_set_cursor_common
    : declare_set_cursor_common_partial*
      FOR select_statement_standalone
    ;

declare_set_cursor_common_partial
    : (LOCAL | GLOBAL)
    | (FORWARD_ONLY | SCROLL)
    | (STATIC | KEYSET | DYNAMIC | FAST_FORWARD)
    | (READ_ONLY | SCROLL_LOCKS | OPTIMISTIC)
    | TYPE_WARNING
    ;

fetch_cursor
    : FETCH ((NEXT | PRIOR | FIRST | LAST | (ABSOLUTE | RELATIVE) expression)? FROM)?
      GLOBAL? cursor_name (INTO LOCAL_ID (',' LOCAL_ID)*)? ';'?
    ;

// https://msdn.microsoft.com/en-us/library/ms190356.aspx
// Runtime check.
set_special
    : SET id_ (id_ | constant_LOCAL_ID | on_off) ';'?
    | SET ROWCOUNT (LOCAL_ID | DECIMAL) ';'?
    // https://msdn.microsoft.com/en-us/library/ms173763.aspx
    | SET TRANSACTION ISOLATION LEVEL
      (READ UNCOMMITTED | READ COMMITTED | REPEATABLE READ | SNAPSHOT | SERIALIZABLE | DECIMAL) ';'?
    // https://msdn.microsoft.com/en-us/library/ms188059.aspx
    | SET IDENTITY_INSERT table_name on_off ';'?
    | SET (ANSI_NULLS | QUOTED_IDENTIFIER | ANSI_PADDING | ANSI_WARNINGS | ANSI_DEFAULTS | ANSI_NULL_DFLT_OFF | ANSI_NULL_DFLT_ON | ARITHABORT | ARITHIGNORE | CONCAT_NULL_YIELDS_NULL | CURSOR_CLOSE_ON_COMMIT | FMTONLY | FORCEPLAN | IMPLICIT_TRANSACTIONS | NOCOUNT | NOEXEC | NUMERIC_ROUNDABORT | PARSEONLY | REMOTE_PROC_TRANSACTIONS | SHOWPLAN_ALL | SHOWPLAN_TEXT | SHOWPLAN_XML | XACT_ABORT) on_off
    ;

constant_LOCAL_ID
    : constant
    | LOCAL_ID
    ;

// Expression.

// https://docs.microsoft.com/en-us/sql/t-sql/language-elements/expressions-transact-sql
// Operator precendence: https://docs.microsoft.com/en-us/sql/t-sql/language-elements/operator-precedence-transact-sql
expression
    : primitive_expression
    | function_call
    | expression COLLATE id_
    | case_expression
    | full_column_name
    | bracket_expression
    | unary_operator_expression
    | expression op=('*' | '/' | '%') expression
    | expression op=('+' | '-' | '&' | '^' | '|' | '||') expression
    | expression time_zone
    | over_clause
    | DOLLAR_ACTION
    ;

time_zone
    : AT_KEYWORD TIME ZONE expression
    ;

primitive_expression
    : DEFAULT | NULL_ | LOCAL_ID | constant
    ;

// https://docs.microsoft.com/en-us/sql/t-sql/language-elements/case-transact-sql
case_expression
    : CASE caseExpr=expression switch_section+ (ELSE elseExpr=expression)? END
    | CASE switch_search_condition_section+ (ELSE elseExpr=expression)? END
    ;

unary_operator_expression
    : '~' expression
    | op=('+' | '-') expression
    ;

bracket_expression
    : '(' expression ')' | '(' subquery ')'
    ;

constant_expression
    : NULL_
    | constant
    // system functions: https://msdn.microsoft.com/en-us/library/ms187786.aspx
    | function_call
    | LOCAL_ID         // TODO: remove.
    | '(' constant_expression ')'
    ;

subquery
    : select_statement
    ;

// https://msdn.microsoft.com/en-us/library/ms175972.aspx
with_expression
    : WITH ctes+=common_table_expression (',' ctes+=common_table_expression)*
    ;

common_table_expression
    : expression_name=id_ ('(' columns=column_name_list ')')? AS '(' cte_query=select_statement ')'
    ;

update_elem
    : LOCAL_ID '=' full_column_name ('=' | assignment_operator) expression //Combined variable and column update
    | (full_column_name | LOCAL_ID) ('=' | assignment_operator) expression
    | udt_column_name=id_ '.' method_name=id_ '(' expression_list ')'
    //| full_column_name '.' WRITE (expression, )
    ;

update_elem_merge
    : (full_column_name | LOCAL_ID) ('=' | assignment_operator) expression
    | udt_column_name=id_ '.' method_name=id_ '(' expression_list ')'
    //| full_column_name '.' WRITE (expression, )
    ;

// https://docs.microsoft.com/en-us/sql/t-sql/queries/search-condition-transact-sql
search_condition
    : NOT* (predicate | '(' search_condition ')')
    | search_condition AND search_condition // AND takes precedence over OR
    | search_condition OR search_condition
    ;

predicate
    : EXISTS '(' subquery ')'
    | expression comparison_operator expression
    | expression comparison_operator (ALL | SOME | ANY) '(' subquery ')'
    | expression NOT* BETWEEN expression AND expression
    | expression NOT* IN '(' (subquery | expression_list) ')'
    | expression NOT* LIKE expression (ESCAPE expression)?
    | expression IS null_notnull
    ;

// Changed union rule to sql_union to avoid union construct with C++ target.  Issue reported by person who generates into C++.  This individual reports change causes generated code to work

query_expression
    : (query_specification | '(' query_expression ')')
    |  query_specification order_by_clause? unions+=sql_union+ //if using top, order by can be on the "top" side of union :/
    ;

sql_union
    : (UNION ALL? | EXCEPT | INTERSECT) (spec=query_specification | ('(' op=query_expression ')'))
    ;

// https://msdn.microsoft.com/en-us/library/ms176104.aspx
query_specification
    : SELECT allOrDistinct=(ALL | DISTINCT)? top=top_clause?
      columns=select_list
      // https://msdn.microsoft.com/en-us/library/ms188029.aspx
      (INTO into=table_name)?
      (FROM from=table_sources)?
      (WHERE where=search_condition)?
      // https://msdn.microsoft.com/en-us/library/ms177673.aspx
      (GROUP BY groupByAll=ALL? groupBys+=group_by_item (',' groupBys+=group_by_item)*)?
      (HAVING having=search_condition)?
    ;

// https://msdn.microsoft.com/en-us/library/ms189463.aspx
top_clause
    : TOP (top_percent | top_count) (WITH TIES)?
    ;

top_percent
    : percent_constant=(REAL | FLOAT | DECIMAL) PERCENT
    | '(' topper_expression=expression ')' PERCENT
    ;

top_count
    : count_constant=DECIMAL
    | '(' topcount_expression=expression ')'
    ;

// https://msdn.microsoft.com/en-us/library/ms188385.aspx
order_by_clause
    : ORDER BY order_bys+=order_by_expression (',' order_bys+=order_by_expression)*
      (OFFSET offset_exp=expression offset_rows=(ROW | ROWS) (FETCH fetch_offset=(FIRST | NEXT) fetch_exp=expression fetch_rows=(ROW | ROWS) ONLY)?)?
    ;

order_by_expression
    : order_by=expression (ascending=ASC | descending=DESC)?
    ;

group_by_item
    : expression
    /*| rollup_spec
    | cube_spec
    | grouping_sets_spec
    | grand_total*/
    ;

option_clause
    // https://msdn.microsoft.com/en-us/library/ms181714.aspx
    : OPTION '(' options+=option (',' options+=option)* ')'
    ;

option
    : FAST number_rows=DECIMAL
    | (HASH | ORDER) GROUP
    | (MERGE | HASH | CONCAT) UNION
    | (LOOP | MERGE | HASH) JOIN
    | FORCE ORDER
    | KEEP PLAN
    | KEEPFIXED PLAN
    | MAXDOP number_of_processors=DECIMAL
    | MAXRECURSION number_recursion=DECIMAL
    | OPTIMIZE FOR '(' optimize_for_arg (',' optimize_for_arg)* ')'
    | OPTIMIZE FOR UNKNOWN
    | PARAMETERIZATION (SIMPLE | FORCED)
    | RECOMPILE
    | ROBUST PLAN
    | USE PLAN STRING
    ;

optimize_for_arg
    : LOCAL_ID (UNKNOWN | '=' (constant | NULL_))
    ;

// https://msdn.microsoft.com/en-us/library/ms176104.aspx
select_list
    : selectElement+=select_list_elem (',' selectElement+=select_list_elem)*
    ;

udt_method_arguments
    : '(' argument+=execute_var_string (',' argument+=execute_var_string)* ')'
    ;

// https://docs.microsoft.com/ru-ru/sql/t-sql/queries/select-clause-transact-sql
asterisk
    : (table_name '.')? '*'
    | (INSERTED | DELETED) '.' '*'
    ;

column_elem
    : (full_column_name | '$' IDENTITY | '$' ROWGUID | NULL_) as_column_alias?
    ;

udt_elem
    : udt_column_name=id_ '.' non_static_attr=id_ udt_method_arguments as_column_alias?
    | udt_column_name=id_ DOUBLE_COLON static_attr=id_ udt_method_arguments? as_column_alias?
    ;

expression_elem
    : leftAlias=column_alias eq='=' leftAssignment=expression
    | expressionAs=expression as_column_alias?
    ;

select_list_elem
    : asterisk
    | column_elem
    | udt_elem
    | LOCAL_ID (assignment_operator | '=') expression
    | expression_elem
    ;

table_sources
    : source+=table_source (',' source+=table_source)*
    ;

// https://docs.microsoft.com/en-us/sql/t-sql/queries/from-transact-sql
table_source
    : table_source_item_joined
    | '(' table_source ')'
    ;

table_source_item_joined
    : table_source_item joins+=join_part*
    | '(' table_source_item_joined ')' joins+=join_part*
    ;

table_source_item
    : table_name_with_hint        as_table_alias?
    | full_table_name             as_table_alias?
    | '(' derived_table ')'       (as_table_alias column_alias_list?)?
    | change_table                as_table_alias?
    | function_call               (as_table_alias column_alias_list?)?
    | loc_id=LOCAL_ID             as_table_alias?
    | nodes_method                (as_table_alias column_alias_list?)?
    | loc_id_call=LOCAL_ID '.' loc_fcall=function_call (as_table_alias column_alias_list?)?
    | DOUBLE_COLON oldstyle_fcall=function_call       as_table_alias? // Build-in function (old syntax)
    ;

schema_declaration
    : xml_col+=column_declaration (',' xml_col+=column_declaration)*
    ;

column_declaration
    : id_ data_type STRING?
    ;

change_table
    : change_table_changes
    | change_table_version
    ;

change_table_changes
    : CHANGETABLE '(' CHANGES changetable=table_name ',' changesid=(NULL_ | DECIMAL | LOCAL_ID) ')'
    ;
change_table_version
    : CHANGETABLE '(' VERSION versiontable=table_name ',' pk_columns=full_column_name_list ',' pk_values=select_list  ')'
    ;

// https://msdn.microsoft.com/en-us/library/ms191472.aspx
join_part
    // https://msdn.microsoft.com/en-us/library/ms173815(v=sql.120).aspx
    : join_on
    | cross_join
    | apply_
    | pivot
    | unpivot
    ;
join_on
    : (inner=INNER? | join_type=(LEFT | RIGHT | FULL) outer=OUTER?) (join_hint=(LOOP | HASH | MERGE | REMOTE))?
       JOIN source=table_source ON cond=search_condition
    ;

cross_join
    : CROSS JOIN table_source
    ;

apply_
    : apply_style=(CROSS | OUTER) APPLY source=table_source
    ;

pivot
    : PIVOT pivot_clause as_table_alias
    ;

unpivot
    : UNPIVOT unpivot_clause as_table_alias
    ;

pivot_clause
    : '(' aggregate_windowed_function FOR full_column_name IN column_alias_list ')'
    ;

unpivot_clause
    : '(' unpivot_exp=expression FOR full_column_name IN '(' full_column_name_list ')' ')'
    ;

full_column_name_list
    : column+=full_column_name (',' column+=full_column_name)*
    ;

table_name_with_hint
    : table_name with_table_hints?
    ;

// runtime check.
bulk_option
    : id_ '=' bulk_option_value=(DECIMAL | STRING)
    ;

derived_table
    : subquery
    | '(' subquery ')'
    | table_value_constructor
    | '(' table_value_constructor ')'
    ;

function_call
    : ranking_windowed_function                         #RANKING_WINDOWED_FUNC
    | aggregate_windowed_function                       #AGGREGATE_WINDOWED_FUNC
    | analytic_windowed_function                        #ANALYTIC_WINDOWED_FUNC
    | built_in_functions                                #BUILT_IN_FUNC
    | scalar_function_name '(' expression_list? ')'     #SCALAR_FUNCTION
    | partition_function                                #PARTITION_FUNC
    ;

partition_function
    : (database=id_ '.')? DOLLAR_PARTITION '.' func_name=id_ '(' expression ')'
    ;

built_in_functions
    // https://msdn.microsoft.com/en-us/library/ms173784.aspx
    : BINARY_CHECKSUM '(' '*' ')'                       #BINARY_CHECKSUM
    // https://msdn.microsoft.com/en-us/library/hh231076.aspx
    // https://msdn.microsoft.com/en-us/library/ms187928.aspx
    | CAST '(' expression AS data_type ')'              #CAST
    | TRY_CAST '(' expression AS data_type ')'          #TRY_CAST
    | CONVERT '(' convert_data_type=data_type ','convert_expression=expression (',' style=expression)? ')'                              #CONVERT
    // https://msdn.microsoft.com/en-us/library/ms189788.aspx
    | CHECKSUM '(' '*' ')'                              #CHECKSUM
    // https://msdn.microsoft.com/en-us/library/ms190349.aspx
    | COALESCE '(' expression_list ')'                  #COALESCE
    // https://msdn.microsoft.com/en-us/library/ms188751.aspx
    | CURRENT_TIMESTAMP                                 #CURRENT_TIMESTAMP
    // https://msdn.microsoft.com/en-us/library/ms176050.aspx
    | CURRENT_USER                                      #CURRENT_USER
    // https://msdn.microsoft.com/en-us/library/ms186819.aspx
    | DATEADD '(' datepart=ID ',' number=expression ',' date=expression ')'  #DATEADD
    // https://msdn.microsoft.com/en-us/library/ms189794.aspx
    | DATEDIFF '(' datepart=ID ',' date_first=expression ',' date_second=expression ')' #DATEDIFF
    // https://msdn.microsoft.com/en-us/library/ms174395.aspx
    | DATENAME '(' datepart=ID ',' date=expression ')'                #DATENAME
    // https://msdn.microsoft.com/en-us/library/ms174420.aspx
    | DATEPART '(' datepart=ID ',' date=expression ')'                #DATEPART
    // https://docs.microsoft.com/en-us/sql/t-sql/functions/getdate-transact-sql
    | GETDATE '(' ')'                                   #GETDATE
    // https://docs.microsoft.com/en-us/sql/t-sql/functions/getdate-transact-sql
    | GETUTCDATE '(' ')'                                #GETUTCDATE
    // https://msdn.microsoft.com/en-us/library/ms189838.aspx
    | IDENTITY '(' data_type (',' seed=DECIMAL)? (',' increment=DECIMAL)? ')'                                                           #IDENTITY
    // https://msdn.microsoft.com/en-us/library/bb839514.aspx
    | MIN_ACTIVE_ROWVERSION '(' ')'                     #MIN_ACTIVE_ROWVERSION
    // https://msdn.microsoft.com/en-us/library/ms177562.aspx
    | NULLIF '(' left=expression ',' right=expression ')'          #NULLIF
    // https://msdn.microsoft.com/fr-fr/library/ms188043.aspx
    | STUFF '(' str=expression ',' from=DECIMAL ',' to=DECIMAL ',' str_with=expression ')'                                                                   #STUFF
    // https://msdn.microsoft.com/en-us/library/ms177587.aspx
    | SESSION_USER                                      #SESSION_USER
    // https://msdn.microsoft.com/en-us/library/ms179930.aspx
    | SYSTEM_USER                                       #SYSTEM_USER
    | USER                                              #USER
    // https://msdn.microsoft.com/en-us/library/ms184325.aspx
    | ISNULL '(' left=expression ',' right=expression ')'          #ISNULL
    // https://docs.microsoft.com/en-us/sql/t-sql/xml/xml-data-type-methods
    // https://docs.microsoft.com/en-us/sql/t-sql/functions/logical-functions-iif-transact-sql
    | IIF '(' cond=search_condition ',' left=expression ',' right=expression ')'   #IIF
    | STRING_AGG '(' expr=expression ',' separator=expression ')' (WITHIN GROUP '(' order_by_clause ')')?  #STRINGAGG
    ;

nodes_method
    : (loc_id=LOCAL_ID | value_id=id_ | '(' subquery ')') '.' NODES '(' xquery=STRING ')'
    ;


switch_section
    : WHEN expression THEN expression
    ;

switch_search_condition_section
    : WHEN search_condition THEN expression
    ;

as_column_alias
    : AS? column_alias
    ;

as_table_alias
    : AS? table_alias
    ;

table_alias
    : id_ with_table_hints?
    ;

// https://msdn.microsoft.com/en-us/library/ms187373.aspx
with_table_hints
    : WITH? '(' hint+=table_hint (','? hint+=table_hint)* ')'
    ;

// https://msdn.microsoft.com/en-us/library/ms187373.aspx
insert_with_table_hints
    : WITH '(' hint+=table_hint (','? hint+=table_hint)* ')'
    ;

// Id runtime check. Id can be (FORCESCAN, HOLDLOCK, NOLOCK, NOWAIT, PAGLOCK, READCOMMITTED,
// READCOMMITTEDLOCK, READPAST, READUNCOMMITTED, REPEATABLEREAD, ROWLOCK, TABLOCK, TABLOCKX
// UPDLOCK, XLOCK)
table_hint
    : NOEXPAND? ( INDEX ('(' index_value (',' index_value)* ')' | index_value (',' index_value)*)
                | INDEX '=' index_value
                | FORCESEEK ('(' index_value '(' ID  (',' ID)* ')' ')')?
                | SERIALIZABLE
                | SNAPSHOT
                | SPATIAL_WINDOW_MAX_CELLS '=' DECIMAL
                | HOLDLOCK
                | ID
                )
    ;

index_value
    : id_ | DECIMAL
    ;

column_alias_list
    : '(' alias+=column_alias (',' alias+=column_alias)* ')'
    ;

column_alias
    : id_
    | STRING
    ;

table_value_constructor
    : VALUES '(' exps+=expression_list ')' (',' '(' exps+=expression_list ')')*
    ;

expression_list
    : exp+=expression (',' exp+=expression)*
    ;

// https://msdn.microsoft.com/en-us/library/ms189798.aspx
ranking_windowed_function
    : (RANK | DENSE_RANK | ROW_NUMBER) '(' ')' over_clause
    | NTILE '(' expression ')' over_clause
    ;

// https://msdn.microsoft.com/en-us/library/ms173454.aspx
aggregate_windowed_function
    : agg_func=(AVG | MAX | MIN | SUM | STDEV | STDEVP | VAR | VARP)
      '(' all_distinct_expression ')' over_clause?
    | cnt=(COUNT | COUNT_BIG)
      '(' ('*' | all_distinct_expression) ')' over_clause?
    | CHECKSUM_AGG '(' all_distinct_expression ')'
    | GROUPING '(' expression ')'
    | GROUPING_ID '(' expression_list ')'
    ;

// https://docs.microsoft.com/en-us/sql/t-sql/functions/analytic-functions-transact-sql
analytic_windowed_function
    : (FIRST_VALUE | LAST_VALUE) '(' expression ')' over_clause
    | (LAG | LEAD) '(' expression  (',' expression (',' expression)? )? ')' over_clause
    | (CUME_DIST | PERCENT_RANK) '(' ')' OVER '(' (PARTITION BY expression_list)? order_by_clause ')'
    | (PERCENTILE_CONT | PERCENTILE_DISC) '(' expression ')' WITHIN GROUP '(' ORDER BY expression (ASC | DESC)? ')' OVER '(' (PARTITION BY expression_list)? ')'
    ;

all_distinct_expression
    : (ALL | DISTINCT)? expression
    ;

// https://msdn.microsoft.com/en-us/library/ms189461.aspx
over_clause
    : OVER '(' (PARTITION BY expression_list)? order_by_clause? row_or_range_clause? ')'
    ;

row_or_range_clause
    : (ROWS | RANGE) window_frame_extent
    ;

window_frame_extent
    : window_frame_preceding
    | BETWEEN window_frame_bound AND window_frame_bound
    ;

window_frame_bound
    : window_frame_preceding
    | window_frame_following
    ;

window_frame_preceding
    : UNBOUNDED PRECEDING
    | DECIMAL PRECEDING
    | CURRENT ROW
    ;

window_frame_following
    : UNBOUNDED FOLLOWING
    | DECIMAL FOLLOWING
    ;

// Primitive.
entity_name
    : (server=id_ '.' database=id_ '.'  schema=id_   '.'
    |                database=id_ '.' (schema=id_)? '.'
    |                                 schema=id_   '.')? table=id_
    ;


entity_name_for_azure_dw
    : schema=id_
    | schema=id_ '.' object_name=id_
    ;

entity_name_for_parallel_dw
    : schema_database=id_
    | schema=id_ '.' object_name=id_
    ;

full_table_name
    : (server=id_ '.' database=id_ '.'  schema=id_   '.'
    |                database=id_ '.' (schema=id_)? '.'
    |                                 schema=id_   '.')? table=id_
    ;

table_name
    : (database=id_ '.' (schema=id_)? '.' | schema=id_ '.')? table=id_
    | (database=id_ '.' (schema=id_)? '.' | schema=id_ '.')? blocking_hierarchy=BLOCKING_HIERARCHY
    ;

simple_name
    : (schema=id_ '.')? name=id_
    ;

func_proc_name_schema
    : ((schema=id_) '.')? procedure=id_
    ;

func_proc_name_database_schema
    : database=id_? '.' schema=id_? '.' procedure=id_
    | func_proc_name_schema
    ;

func_proc_name_server_database_schema
    : server=id_? '.' database=id_? '.' schema=id_? '.' procedure=id_
    | func_proc_name_database_schema
    ;

ddl_object
    : full_table_name
    | LOCAL_ID
    ;

full_column_name
    : (DELETED | INSERTED) '.' column_name=id_
    | server=id_? '.' schema=id_? '.' tablename=id_? '.' column_name=id_
    | schema=id_? '.' tablename=id_? '.' column_name=id_
    | tablename=id_? '.' column_name=id_
    | column_name=id_
    ;

column_name_list_with_order
    : id_ (ASC | DESC)? (',' id_ (ASC | DESC)?)*
    ;

//For some reason, sql server allows any number of prefixes:  Here, h is the column: a.b.c.d.e.f.g.h
insert_column_name_list
    : col+=insert_column_id (',' col+=insert_column_id)*
    ;

insert_column_id
    : (ignore+=id_? '.' )* id_
    ;

column_name_list
    : col+=id_ (',' col+=id_)*
    ;

cursor_name
    : id_
    | LOCAL_ID
    ;

on_off
    : ON
    | OFF
    ;

null_notnull
    : NOT? NULL_
    ;

null_or_default
    :(null_notnull | DEFAULT constant_expression (COLLATE id_)? (WITH VALUES)?)
    ;

scalar_function_name
    : func_proc_name_server_database_schema
    | RIGHT
    | LEFT
    | BINARY_CHECKSUM
    | CHECKSUM
    | ABS
    | ASCII
    | CEILING
    | CHAR
    | CHARINDEX
    | DATALENGTH
    | DAY
    | FLOOR
    | ISDATE
    | ISNUMERIC
    | LEN
    | LOWER
    | LTRIM
    | MONTH
    | NCHAR
    | PATINDEX
    | RAND
    | REPLACE
    | ROUND
    | RTRIM
    | SIGN
    | SPACE
    | STR
    | SUBSTRING
    | UPPER
    | USER_NAME
    | YEAR
    ;

// https://msdn.microsoft.com/en-us/library/ms187752.aspx
// TODO: implement runtime check or add new tokens.

data_type
    : scaled=(NTEXT | TEXT | NCHAR | CHAR | VARCHAR | NVARCHAR | BINARY_KEYWORD | VARBINARY_KEYWORD) '(' MAX ')'
    | ext_type=id_ '(' scale=DECIMAL ',' prec=DECIMAL ')'
    | ext_type=id_ '(' scale=DECIMAL ')'
    | ext_type=id_ IDENTITY ('(' seed=DECIMAL ',' inc=DECIMAL ')')?
    | double_prec=DOUBLE PRECISION?
    | unscaled_type=id_
    ;

default_value
    : NULL_
    | DEFAULT
    | constant
    ;

// https://msdn.microsoft.com/en-us/library/ms179899.aspx
constant
    : STRING // string, datetime or uniqueidentifier
    | BINARY
    | sign? DECIMAL
    | sign? (REAL | FLOAT)  // float or decimal
    | sign? dollar='$' (DECIMAL | FLOAT)       // money
    ;

sign
    : '+'
    | '-'
    ;

keyword
    : ABSOLUTE
    | ACTION
    | AES_128
    | AES_192
    | AES_256
    | ALGORITHM
    | ANSI_NULLS
    | ANSI_PADDING
    | ANSI_WARNINGS
    | APPLY
    | ARITHABORT
    | AT_KEYWORD
    | AUTO
    | AVG
    | BIGINT
    | BINARY_BASE64
    | BINARY_CHECKSUM
    | CALLER
    | CAST
    | CATCH
    | CHECKSUM
    | CHECKSUM_AGG
    | COMMITTED
    | CONCAT
    | CONCAT_NULL_YIELDS_NULL
    | COUNT
    | COUNT_BIG
    | CURSOR_CLOSE_ON_COMMIT
    | CURSOR_DEFAULT
    | DATA
    | DATEADD
    | DATEDIFF
    | DATENAME
    | DATEPART
    | DAYS
    | DEFAULT_DOUBLE_QUOTE
    | DELAYED_DURABILITY
    | DELETED
    | DENSE_RANK
    | DES
    | DESCRIPTION
    | DESX
    | DISABLE
    | DYNAMIC
    | ELEMENTS
    | EMPTY
    | ENCRYPTION
    | EXCLUSIVE
    | FAST
    | FAST_FORWARD
    | FIRST
    | FIRST_VALUE
    | FOLLOWING
    | FORCE
    | FORCED
    | FORWARD_ONLY
    | GETDATE
    | GETUTCDATE
    | GLOBAL
    | GO
    | GO_BATCH
    | GROUPING
    | GROUPING_ID
    | HASH
    | HIGH
    | HOURS
    | IDENTITY_VALUE
    | INCLUDE_NULL_VALUES
    | INPUT
    | INSENSITIVE
    | INSERTED
    | INT
    | ISOLATION
    | KEEP
    | KEEPFIXED
    | KEYSET
    | LAG
    | LAST
    | LAST_VALUE
    | LEAD
    | LEVEL
    | LOCAL
    | LOCATION
    | LOCK
    | LOCK_ESCALATION
    | LOOP
    | LOW
    | MANUAL
    | MARK
    | MAX
    | MAXDOP
    | MAXRECURSION
    | MIN
    | MINUTES
    | MODE
    | NEXT
    | NO
    | NOCOUNT
    | NODES
    | NOEXPAND
    | NOWAIT
    | NTILE
    | NUMANODE
    | NUMERIC_ROUNDABORT
    | OBJECT
    | OFFSET
    | ONLINE
    | ONLY
    | OPTIMISTIC
    | OPTIMIZE
    | OUT
    | OUTPUT
    | OWNER
    | PARAMETERIZATION
    | PARTITION
    | PARTITIONS
    | PRECEDING
    | PRIOR
    | QUERY
    | QUOTED_IDENTIFIER
    | RANGE
    | RANK
    | RC2
    | RC4
    | RC4_128
    | READ_ONLY
    | READONLY
    | REBUILD
    | RECOMPILE
    | RELATIVE
    | REMOTE
    | REPEATABLE
    | ROBUST
    | ROW
    | ROW_NUMBER
    | ROWGUID
    | ROWS
    | SCHEMABINDING
    | SCROLL
    | SCROLL_LOCKS
    | SECONDS
    | SELF
    | SEMI_SENSITIVE
    | SEQUENCE
    | SERIALIZABLE
    | SETERROR
    | SHARE
    | SIMPLE
    | SIZE
    | SMALLINT
    | SNAPSHOT
    | SPATIAL_WINDOW_MAX_CELLS
    | STATIC
    | STATUSONLY
    | STDEV
    | STDEVP
    | STRING_AGG
    | STUFF
    | SUBSCRIPTION
    | SUM
    | TEXTIMAGE_ON
    | THROW
    | TIES
    | TIME
    | TINYINT
    | TRIPLE_DES
    | TRIPLE_DES_3KEY
    | TRY
    | TWO_DIGIT_YEAR_CUTOFF
    | TYPE
    | TYPE_WARNING
    | UNBOUNDED
    | UNCOMMITTED
    | UNKNOWN
    | UOW
    | USING
    | VAR
    | VARP
    | WAIT
    | WORK
    | BLOCKING_HIERARCHY
    | CACHE
    | CALLED
    | CHANGETABLE
    | CHANGES
    | CUME_DIST
    | CYCLE
    | DATA_COMPRESSION
    | ERROR
    | FORCESEEK
    | IIF
    | INCREMENT
    | ISNULL
    | LOG
    | MATCHED
    | MAXVALUE
    | MINVALUE
    | NOTIFICATION
    | PERCENTILE_CONT
    | PERCENTILE_DISC
    | PERCENT_RANK
    | PERSISTED
    | PREDICATE
    | RESTART
    | RETURNS
    | SOURCE
    | STATE
    | START
    | TARGET
    | TRY_CAST
    | ZONE
    //Build-ins:
    | TEXT
    | NTEXT
    | CHAR
    | NCHAR
    | VARCHAR
    | NVARCHAR
    | BINARY_KEYWORD
    | VARBINARY_KEYWORD
    | PRECISION //For some reason this is possible to use as ID
    ;

// https://msdn.microsoft.com/en-us/library/ms175874.aspx
id_
    : ID
    | DOUBLE_QUOTE_ID
    | SQUARE_BRACKET_ID
    | keyword
    ;

simple_id
    : ID
    ;

id_or_string
    : id_
    | STRING
    ;

// https://msdn.microsoft.com/en-us/library/ms188074.aspx
// Spaces are allowed for comparison operators.
comparison_operator
    : '=' | '>' | '<' | '<' '=' | '>' '=' | '<' '>' | '!' '=' | '!' '>' | '!' '<'
    ;

assignment_operator
    : '+=' | '-=' | '*=' | '/=' | '%=' | '&=' | '^=' | '|='
    ;
