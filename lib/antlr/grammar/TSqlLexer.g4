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

lexer grammar TSqlLexer;

// Basic keywords (from https://msdn.microsoft.com/en-us/library/ms189822.aspx)

ADD:                                   'ADD';
ALL:                                   'ALL';
ALTER:                                 'ALTER';
AND:                                   'AND';
ANY:                                   'ANY';
AS:                                    'AS';
ASC:                                   'ASC';
AUTHORIZATION:                         'AUTHORIZATION';
BACKSLASH:                             '\\';
BEGIN:                                 'BEGIN';
BETWEEN:                               'BETWEEN';
BREAK:                                 'BREAK';
BY:                                    'BY';
CASE:                                  'CASE';
CHECK:                                 'CHECK';
CLOSE:                                 'CLOSE';
COALESCE:                              'COALESCE';
COLLATE:                               'COLLATE';
COLUMN:                                'COLUMN';
COMMIT:                                'COMMIT';
COMPUTE:                               'COMPUTE';
CONSTRAINT:                            'CONSTRAINT';
CONTINUE:                              'CONTINUE';
CONVERT:                               'TRY_'? 'CONVERT';
CREATE:                                'CREATE';
CROSS:                                 'CROSS';
CURRENT:                               'CURRENT';
CURRENT_DATE:                          'CURRENT_DATE';
CURRENT_TIME:                          'CURRENT_TIME';
CURRENT_TIMESTAMP:                     'CURRENT_TIMESTAMP';
CURRENT_USER:                          'CURRENT_USER';
CURSOR:                                'CURSOR';
DATABASE:                              'DATABASE';
DBCC:                                  'DBCC';
DEALLOCATE:                            'DEALLOCATE';
DECLARE:                               'DECLARE';
DEFAULT:                               'DEFAULT';
DELETE:                                'DELETE';
DESC:                                  'DESC';
DISTINCT:                              'DISTINCT';
DISTRIBUTED:                           'DISTRIBUTED';
DOUBLE:                                'DOUBLE';
DOUBLE_BACK_SLASH:                     '\\\\';
DOUBLE_FORWARD_SLASH:                  '//';
DROP:                                  'DROP';
ELSE:                                  'ELSE';
END:                                   'END';
ESCAPE:                                'ESCAPE';
EXCEPT:                                'EXCEPT';
EXECUTE:                               'EXEC' 'UTE'?;
EXISTS:                                'EXISTS';
FETCH:                                 'FETCH';
FOR:                                   'FOR';
FOREIGN:                               'FOREIGN';
FROM:                                  'FROM';
FULL:                                  'FULL';
FUNCTION:                              'FUNCTION';
GOTO:                                  'GOTO';
GROUP:                                 'GROUP';
HAVING:                                'HAVING';
HOLDLOCK:                              'HOLDLOCK';
IDENTITY:                              'IDENTITY';
IDENTITY_INSERT:                       'IDENTITY_INSERT';
IF:                                    'IF';
IN:                                    'IN';
INDEX:                                 'INDEX';
INNER:                                 'INNER';
INSERT:                                'INSERT';
INTERSECT:                             'INTERSECT';
INTO:                                  'INTO';
IS:                                    'IS';
JOIN:                                  'JOIN';
KEY:                                   'KEY';
KILL:                                  'KILL';
LEFT:                                  'LEFT';
LIKE:                                  'LIKE';
MERGE:                                 'MERGE';
NOT:                                   'NOT';
NULL_:                                 'NULL';
NULLIF:                                'NULLIF';
OF:                                    'OF';
OFF:                                   'OFF';
ON:                                    'ON';
OPEN:                                  'OPEN';
OPTION:                                'OPTION';
OR:                                    'OR';
ORDER:                                 'ORDER';
OUTER:                                 'OUTER';
OVER:                                  'OVER';
PERCENT:                               'PERCENT';
PIVOT:                                 'PIVOT';
PLAN:                                  'PLAN';
PRECISION:                             'PRECISION';
PRIMARY:                               'PRIMARY';
PRINT:                                 'PRINT';
PROC:                                  'PROC';
PROCEDURE:                             'PROCEDURE';
RAISERROR:                             'RAISERROR';
READ:                                  'READ';
REFERENCES:                            'REFERENCES';
REPLICATION:                           'REPLICATION';
RETURN:                                'RETURN';
REVERT:                                'REVERT';
RIGHT:                                 'RIGHT';
ROLLBACK:                              'ROLLBACK';
ROWCOUNT:                              'ROWCOUNT';
ROWGUIDCOL:                            'ROWGUIDCOL';
SAVE:                                  'SAVE';
SCHEMA:                                'SCHEMA';
SELECT:                                'SELECT';
SESSION_USER:                          'SESSION_USER';
SET:                                   'SET';
SHUTDOWN:                              'SHUTDOWN';
SOME:                                  'SOME';
SYSTEM_USER:                           'SYSTEM_USER';
TABLE:                                 'TABLE';
THEN:                                  'THEN';
TO:                                    'TO';
TOP:                                   'TOP';
TRAN:                                  'TRAN';
TRANSACTION:                           'TRANSACTION';
TRUNCATE:                              'TRUNCATE';
UNION:                                 'UNION';
UNIQUE:                                'UNIQUE';
UNPIVOT:                               'UNPIVOT';
UPDATE:                                'UPDATE';
USE:                                   'USE';
USER:                                  'USER';
VALUES:                                'VALUES';
VARYING:                               'VARYING';
WHEN:                                  'WHEN';
WHERE:                                 'WHERE';
WHILE:                                 'WHILE';
WITH:                                  'WITH';
WITHIN:                                'WITHIN';
DOLLAR_PARTITION:                      '$PARTITION';

//Keywords that can exist in ID etc
ABSOLUTE:                              'ABSOLUTE';
AT_KEYWORD:                            'AT';
ACTION:                                'ACTION';
AES_128:                               'AES_128';
AES_192:                               'AES_192';
AES_256:                               'AES_256';
ALGORITHM:                             'ALGORITHM';
ANSI_NULLS:                            'ANSI_NULLS';
ANSI_PADDING:                          'ANSI_PADDING';
ANSI_WARNINGS:                         'ANSI_WARNINGS';
APPLY:                                 'APPLY';
ARITHABORT:                            'ARITHABORT';
AUTO:                                  'AUTO';
AVG:                                   'AVG';
BIGINT:                                'BIGINT';
BINARY_KEYWORD:                        'BINARY';
BINARY_BASE64:                         'BINARY BASE64';
BINARY_CHECKSUM:                       'BINARY_CHECKSUM';
CALLER:                                'CALLER';
CAST:                                  'CAST';
TRY_CAST:                              'TRY_CAST';
CATCH:                                 'CATCH';
CHECKSUM:                              'CHECKSUM';
CHECKSUM_AGG:                          'CHECKSUM_AGG';
COMMITTED:                             'COMMITTED';
CONCAT:                                'CONCAT';
CONCAT_NULL_YIELDS_NULL:               'CONCAT_NULL_YIELDS_NULL';
COUNT:                                 'COUNT';
COUNT_BIG:                             'COUNT_BIG';
CUME_DIST:                             'CUME_DIST';
CURSOR_CLOSE_ON_COMMIT:                'CURSOR_CLOSE_ON_COMMIT';
CURSOR_DEFAULT:                        'CURSOR_DEFAULT';
DATA:                                  'DATA';
DATEADD:                               'DATEADD';
DATEDIFF:                              'DATEDIFF';
DATENAME:                              'DATENAME';
DATEPART:                              'DATEPART';
DAYS:                                  'DAYS';
DEFAULT_DOUBLE_QUOTE:                  ["]'DEFAULT'["];
DELAYED_DURABILITY:                    'DELAYED_DURABILITY';
DELETED:                               'DELETED';
DENSE_RANK:                            'DENSE_RANK';
DES:                                   'DES';
DESCRIPTION:                           'DESCRIPTION';
DESX:                                  'DESX';
DISABLE:                               'DISABLE';
DYNAMIC:                               'DYNAMIC';
ELEMENTS:                              'ELEMENTS';
EMPTY:                                 'EMPTY';
ENCRYPTION:                            'ENCRYPTION';
EXCLUSIVE:                             'EXCLUSIVE';
FAST:                                  'FAST';
FAST_FORWARD:                          'FAST_FORWARD';
FIRST:                                 'FIRST';
FIRST_VALUE:                           'FIRST_VALUE';
FOLLOWING:                             'FOLLOWING';
FORCE:                                 'FORCE';
FORCED:                                'FORCED';
FORWARD_ONLY:                          'FORWARD_ONLY';
GETDATE:                               'GETDATE';
GETUTCDATE:                            'GETUTCDATE';
GLOBAL:                                'GLOBAL';
GO_BATCH:                              [\r\n]* [\t ]* 'GO'[\t\n\r ];
GO:                                    'GO';
GROUPING:                              'GROUPING';
GROUPING_ID:                           'GROUPING_ID';
HASH:                                  'HASH';
HIGH:                                  'HIGH';
HOURS:                                 'HOURS';
IDENTITY_VALUE:                        'IDENTITY_VALUE';
INCLUDE_NULL_VALUES:                   'INCLUDE_NULL_VALUES';
INPUT:                                 'INPUT';
INSENSITIVE:                           'INSENSITIVE';
INSERTED:                              'INSERTED';
INT:                                   'INT';
ISOLATION:                             'ISOLATION';
KEEP:                                  'KEEP';
KEEPFIXED:                             'KEEPFIXED';
KEYSET:                                'KEYSET';
LAG:                                   'LAG';
LAST:                                  'LAST';
LAST_VALUE:                            'LAST_VALUE';
LEAD:                                  'LEAD';
LEVEL:                                 'LEVEL';
LOCAL:                                 'LOCAL';
LOCATION:                              'LOCATION';
LOCK:                                  'LOCK';
LOCK_ESCALATION:                       'LOCK_ESCALATION';
LOOP:                                  'LOOP';
LOW:                                   'LOW';
MANUAL:                                'MANUAL';
MARK:                                  'MARK';
MAX:                                   'MAX';
MAXDOP:                                'MAXDOP';
MAXRECURSION:                          'MAXRECURSION';
MIN:                                   'MIN';
MIN_ACTIVE_ROWVERSION:                 'MIN_ACTIVE_ROWVERSION';
MINUTES:                               'MINUTES';
MODE:                                  'MODE';
NEXT:                                  'NEXT';
NO:                                    'NO';
NOCOUNT:                               'NOCOUNT';
NODES:                                 'NODES';
NOEXPAND:                              'NOEXPAND';
NOWAIT:                                'NOWAIT';
NTILE:                                 'NTILE';
NUMANODE:                              'NUMANODE';
NUMERIC_ROUNDABORT:                    'NUMERIC_ROUNDABORT';
OBJECT:                                'OBJECT';
OFFSET:                                'OFFSET';
ONLINE:                                'ONLINE';
ONLY:                                  'ONLY';
OPTIMISTIC:                            'OPTIMISTIC';
OPTIMIZE:                              'OPTIMIZE';
OUT:                                   'OUT';
OUTPUT:                                'OUTPUT';
OWNER:                                 'OWNER';
PARAMETERIZATION:                      'PARAMETERIZATION';
PARTITION:                             'PARTITION';
PARTITIONS:                            'PARTITIONS';
PERCENT_RANK:                          'PERCENT_RANK';
PERCENTILE_CONT:                       'PERCENTILE_CONT';
PERCENTILE_DISC:                       'PERCENTILE_DISC';
PRECEDING:                             'PRECEDING';
PRIOR:                                 'PRIOR';
QUERY:                                 'QUERY';
QUOTED_IDENTIFIER:                     'QUOTED_IDENTIFIER';
RANGE:                                 'RANGE';
RANK:                                  'RANK';
RC2:                                   'RC2';
RC4:                                   'RC4';
RC4_128:                               'RC4_128';
READ_ONLY:                             'READ_ONLY';
READONLY:                              'READONLY';
REBUILD:                               'REBUILD';
RECOMPILE:                             'RECOMPILE';
RELATIVE:                              'RELATIVE';
REMOTE:                                'REMOTE';
REPEATABLE:                            'REPEATABLE';
ROBUST:                                'ROBUST';
ROW:                                   'ROW';
ROW_NUMBER:                            'ROW_NUMBER';
ROWGUID:                               'ROWGUID';
ROWS:                                  'ROWS';
SCHEMABINDING:                         'SCHEMABINDING';
SCROLL:                                'SCROLL';
SCROLL_LOCKS:                          'SCROLL_LOCKS';
SECONDS:                               'SECONDS';
SELF:                                  'SELF';
SEMI_SENSITIVE:                        'SEMI_SENSITIVE';
SEQUENCE:                              'SEQUENCE';
SERIALIZABLE:                          'SERIALIZABLE';
SETERROR:                              'SETERROR';
SHARE:                                 'SHARE';
SIMPLE:                                'SIMPLE';
SIZE:                                  'SIZE';
SMALLINT:                              'SMALLINT';
SNAPSHOT:                              'SNAPSHOT';
SPATIAL_WINDOW_MAX_CELLS:              'SPATIAL_WINDOW_MAX_CELLS';
STATIC:                                'STATIC';
STATUSONLY:                            'STATUSONLY';
STDEV:                                 'STDEV';
STDEVP:                                'STDEVP';
STRING_AGG:                            'STRING_AGG';
STUFF:                                 'STUFF';
SUBSCRIPTION:                          'SUBSCRIPTION';
SUM:                                   'SUM';
TEXTIMAGE_ON:                          'TEXTIMAGE_ON';
THROW:                                 'THROW';
TIES:                                  'TIES';
TIME:                                  'TIME';
TINYINT:                               'TINYINT';
TRIPLE_DES:                            'TRIPLE_DES';
TRIPLE_DES_3KEY:                       'TRIPLE_DES_3KEY';
TRY:                                   'TRY';
TWO_DIGIT_YEAR_CUTOFF:                 'TWO_DIGIT_YEAR_CUTOFF';
TYPE:                                  'TYPE';
TYPE_WARNING:                          'TYPE_WARNING';
UNBOUNDED:                             'UNBOUNDED';
UNCOMMITTED:                           'UNCOMMITTED';
UNKNOWN:                               'UNKNOWN';
UOW:                                   'UOW';
USING:                                 'USING';
VAR:                                   'VAR';
VARBINARY_KEYWORD:                     'VARBINARY';
VARP:                                  'VARP';
WAIT:                                  'WAIT';
WORK:                                  'WORK';
ZONE:                                  'ZONE';
DOLLAR_ACTION:                         '$ACTION';
//More keywords
BLOCKING_HIERARCHY:                    'BLOCKING_HIERARCHY';
CACHE:                                 'CACHE';
CALLED:                                'CALLED';
CHANGETABLE:                           'CHANGETABLE';
CHANGES:                               'CHANGES';
CONNECTION:                            'CONNECTION';
CONTRACT:                              'CONTRACT';
CYCLE:                                 'CYCLE';
DATA_COMPRESSION:                      'DATA_COMPRESSION';
DDL:                                   'DDL';
ERROR:                                 'ERROR';
EVENT:                                 'EVENT';
FORCESEEK:                             'FORCESEEK';
IIF:                                   'IIF';
INCREMENT:                             'INCREMENT';
ISNULL:                                'ISNULL';
LOG:                                   'LOG';
MATCHED:                               'MATCHED';
MAXVALUE:                              'MAXVALUE';
MINVALUE:                              'MINVALUE';
NOTIFICATION:                          'NOTIFICATION';
PERSISTED:                             'PERSISTED';
PREDICATE:                             'PREDICATE';
RESTART:                               'RESTART';
RETURNS:                               'RETURNS';
SHRINKLOG:                             'SHRINKLOG';
SOURCE:                                'SOURCE';
SQL:                                   'SQL';
STATE:                                 'STATE';
START:                                 'START';
TARGET:                                'TARGET';
WITHOUT:                               'WITHOUT';

//Build-ins:
VARCHAR:                               'VARCHAR';
NVARCHAR:                              'NVARCHAR';


SPACE:              [ \t\r\n]+    -> skip;
// https://docs.microsoft.com/en-us/sql/t-sql/language-elements/slash-star-comment-transact-sql
COMMENT:            '/*' (COMMENT | .)*? '*/' -> channel(HIDDEN);
LINE_COMMENT:       '--' ~[\r\n]* -> channel(HIDDEN);

// TODO: ID can be not only Latin.
DOUBLE_QUOTE_ID:    '"' ~'"'+ '"';
SINGLE_QUOTE:       '\'';
SQUARE_BRACKET_ID:  '[' (~']' | ']' ']')* ']';
LOCAL_ID:           '@' ([A-Z_$@#0-9] | FullWidthLetter)*;
DECIMAL:             DEC_DIGIT+;
ID:                  ( [A-Z_#] | FullWidthLetter) ( [A-Z_#$@0-9] | FullWidthLetter )*;
STRING:              'N'? '\'' (~'\'' | '\'\'')* '\'';
BINARY:              '0' 'X' HEX_DIGIT*;
FLOAT:               DEC_DOT_DEC;
REAL:                (DECIMAL | DEC_DOT_DEC) ('E' [+-]? DEC_DIGIT+);

EQUAL:               '=';

GREATER:             '>';
LESS:                '<';
EXCLAMATION:         '!';

PLUS_ASSIGN:         '+=';
MINUS_ASSIGN:        '-=';
MULT_ASSIGN:         '*=';
DIV_ASSIGN:          '/=';
MOD_ASSIGN:          '%=';
AND_ASSIGN:          '&=';
XOR_ASSIGN:          '^=';
OR_ASSIGN:           '|=';

DOUBLE_BAR:          '||';
DOT:                 '.';
UNDERLINE:           '_';
AT:                  '@';
SHARP:               '#';
DOLLAR:              '$';
LR_BRACKET:          '(';
RR_BRACKET:          ')';
COMMA:               ',';
SEMI:                ';';
COLON:               ':';
DOUBLE_COLON:        '::';
STAR:                '*';
DIVIDE:              '/';
MODULE:              '%';
PLUS:                '+';
MINUS:               '-';
BIT_NOT:             '~';
BIT_OR:              '|';
BIT_AND:             '&';
BIT_XOR:             '^';

fragment LETTER:       [A-Z_];
fragment DEC_DOT_DEC:  (DEC_DIGIT+ '.' DEC_DIGIT+ |  DEC_DIGIT+ '.' | '.' DEC_DIGIT+);
fragment HEX_DIGIT:    [0-9A-F];
fragment DEC_DIGIT:    [0-9];


fragment FullWidthLetter
    : '\u00c0'..'\u00d6'
    | '\u00d8'..'\u00f6'
    | '\u00f8'..'\u00ff'
    | '\u0100'..'\u1fff'
    | '\u2c00'..'\u2fff'
    | '\u3040'..'\u318f'
    | '\u3300'..'\u337f'
    | '\u3400'..'\u3fff'
    | '\u4e00'..'\u9fff'
    | '\ua000'..'\ud7ff'
    | '\uf900'..'\ufaff'
    | '\uff00'..'\ufff0'
    // | '\u10000'..'\u1F9FF'  //not support four bytes chars
    // | '\u20000'..'\u2FA1F'
    ;
