
// Generated from TSqlParser.g4 by ANTLR 4.9.2

#pragma once


#include "antlr4-runtime.h"




class  TSqlParser : public antlr4::Parser {
public:
  enum {
    ABS = 1, ABSENT = 2, ADD = 3, AES = 4, ALL = 5, ALLOW_CONNECTIONS = 6, 
    ALLOW_MULTIPLE_EVENT_LOSS = 7, ALLOW_SINGLE_EVENT_LOSS = 8, ALTER = 9, 
    AND = 10, ANONYMOUS = 11, ANY = 12, APPEND = 13, APPLICATION = 14, AS = 15, 
    ASC = 16, ASCII = 17, ASYMMETRIC = 18, ASYNCHRONOUS_COMMIT = 19, AUTHORIZATION = 20, 
    AUTHENTICATION = 21, AUTOMATED_BACKUP_PREFERENCE = 22, AUTOMATIC = 23, 
    AVAILABILITY_MODE = 24, BACKSLASH = 25, BACKUP = 26, BEFORE = 27, BEGIN = 28, 
    BETWEEN = 29, BLOCK = 30, BLOCKSIZE = 31, BLOCKING_HIERARCHY = 32, BREAK = 33, 
    BROWSE = 34, BUFFER = 35, BUFFERCOUNT = 36, BULK = 37, BY = 38, CACHE = 39, 
    CALLED = 40, CASCADE = 41, CASE = 42, CEILING = 43, CERTIFICATE = 44, 
    CHANGETABLE = 45, CHANGES = 46, CHAR = 47, CHARINDEX = 48, CHECK = 49, 
    CHECKPOINT = 50, CHECK_POLICY = 51, CHECK_EXPIRATION = 52, CLASSIFIER_FUNCTION = 53, 
    CLOSE = 54, CLUSTER = 55, CLUSTERED = 56, COALESCE = 57, COLLATE = 58, 
    COLUMN = 59, COMPRESSION = 60, COMMIT = 61, COMPUTE = 62, CONFIGURATION = 63, 
    CONSTRAINT = 64, CONTAINMENT = 65, CONTAINS = 66, CONTAINSTABLE = 67, 
    CONTEXT = 68, CONTINUE = 69, CONTINUE_AFTER_ERROR = 70, CONTRACT = 71, 
    CONTRACT_NAME = 72, CONVERSATION = 73, CONVERT = 74, COPY_ONLY = 75, 
    CREATE = 76, CROSS = 77, CURRENT = 78, CURRENT_DATE = 79, CURRENT_TIME = 80, 
    CURRENT_TIMESTAMP = 81, CURRENT_USER = 82, CURSOR = 83, CYCLE = 84, 
    DATA_COMPRESSION = 85, DATA_SOURCE = 86, DATABASE = 87, DATABASE_MIRRORING = 88, 
    DBCC = 89, DEALLOCATE = 90, DECLARE = 91, DEFAULT = 92, DEFAULT_DATABASE = 93, 
    DEFAULT_SCHEMA = 94, DELETE = 95, DENY = 96, DESC = 97, DIAGNOSTICS = 98, 
    DIFFERENTIAL = 99, DISK = 100, DISTINCT = 101, DISTRIBUTED = 102, DOUBLE = 103, 
    DOUBLE_BACK_SLASH = 104, DOUBLE_FORWARD_SLASH = 105, DROP = 106, DTC_SUPPORT = 107, 
    DUMP = 108, ELSE = 109, ENABLED = 110, END = 111, ENDPOINT = 112, ERRLVL = 113, 
    ESCAPE = 114, ERROR = 115, EVENT = 116, EVENTDATA = 117, EVENT_RETENTION_MODE = 118, 
    EXCEPT = 119, EXECUTABLE_FILE = 120, EXECUTE = 121, EXISTS = 122, EXPIREDATE = 123, 
    EXIT = 124, EXTENSION = 125, EXTERNAL = 126, EXTERNAL_ACCESS = 127, 
    FAILOVER = 128, FAILURECONDITIONLEVEL = 129, FAN_IN = 130, FETCH = 131, 
    FILE = 132, FILENAME = 133, FILLFACTOR = 134, FILE_SNAPSHOT = 135, FLOOR = 136, 
    FOR = 137, FORCESEEK = 138, FORCE_SERVICE_ALLOW_DATA_LOSS = 139, FOREIGN = 140, 
    FREETEXT = 141, FREETEXTTABLE = 142, FROM = 143, FULL = 144, FUNCTION = 145, 
    GET = 146, GOTO = 147, GOVERNOR = 148, GRANT = 149, GROUP = 150, HAVING = 151, 
    HASHED = 152, HEALTHCHECKTIMEOUT = 153, IDENTITY = 154, IDENTITYCOL = 155, 
    IDENTITY_INSERT = 156, IF = 157, IIF = 158, IN = 159, INCLUDE = 160, 
    INCREMENT = 161, INDEX = 162, INFINITE = 163, INIT = 164, INNER = 165, 
    INSERT = 166, INSTEAD = 167, INTERSECT = 168, INTO = 169, IPV4_ADDR = 170, 
    IPV6_ADDR = 171, IS = 172, ISDATE = 173, ISNULL = 174, ISNUMERIC = 175, 
    JOIN = 176, KERBEROS = 177, KEY = 178, KEY_PATH = 179, KEY_STORE_PROVIDER_NAME = 180, 
    KILL = 181, LANGUAGE = 182, LEFT = 183, LEN = 184, LIBRARY = 185, LIFETIME = 186, 
    LIKE = 187, LINENO = 188, LINUX = 189, LISTENER_IP = 190, LISTENER_PORT = 191, 
    LOAD = 192, LOCAL_SERVICE_NAME = 193, LOG = 194, LOWER = 195, LTRIM = 196, 
    MATCHED = 197, MASTER = 198, MAX_MEMORY = 199, MAXTRANSFER = 200, MAXVALUE = 201, 
    MAX_DISPATCH_LATENCY = 202, MAX_EVENT_SIZE = 203, MAX_SIZE = 204, MAX_OUTSTANDING_IO_PER_VOLUME = 205, 
    MEDIADESCRIPTION = 206, MEDIANAME = 207, MEMBER = 208, MEMORY_PARTITION_MODE = 209, 
    MERGE = 210, MESSAGE_FORWARDING = 211, MESSAGE_FORWARD_SIZE = 212, MINVALUE = 213, 
    MIRROR = 214, MONTH = 215, MUST_CHANGE = 216, NATIONAL = 217, NCHAR = 218, 
    NEGOTIATE = 219, NOCHECK = 220, NOFORMAT = 221, NOINIT = 222, NONCLUSTERED = 223, 
    NONE = 224, NOREWIND = 225, NOSKIP = 226, NOUNLOAD = 227, NO_CHECKSUM = 228, 
    NO_COMPRESSION = 229, NO_EVENT_LOSS = 230, NOT = 231, NOTIFICATION = 232, 
    NTLM = 233, null = 234, NULLIF = 235, OF = 236, OFF = 237, OFFSETS = 238, 
    OLD_PASSWORD = 239, ON = 240, ON_FAILURE = 241, OPEN = 242, OPENDATASOURCE = 243, 
    OPENQUERY = 244, OPENROWSET = 245, OPENXML = 246, OPTION = 247, OR = 248, 
    ORDER = 249, OUTER = 250, OVER = 251, PAGE = 252, PARAM_NODE = 253, 
    PARTIAL = 254, PASSWORD = 255, PATINDEX = 256, PERCENT = 257, PERMISSION_SET = 258, 
    PER_CPU = 259, PER_DB = 260, PER_NODE = 261, PIVOT = 262, PLAN = 263, 
    PLATFORM = 264, POLICY = 265, PRECISION = 266, PREDICATE = 267, PRIMARY = 268, 
    PRINT = 269, PROC = 270, PROCEDURE = 271, PROCESS = 272, PUBLIC = 273, 
    PYTHON = 274, R = 275, RAISERROR = 276, RAND = 277, RAW = 278, READ = 279, 
    READTEXT = 280, READ_WRITE_FILEGROUPS = 281, RECONFIGURE = 282, REFERENCES = 283, 
    REGENERATE = 284, RELATED_CONVERSATION = 285, RELATED_CONVERSATION_GROUP = 286, 
    REPLACE = 287, REPLICATION = 288, REQUIRED = 289, RESET = 290, RESTART = 291, 
    RESTORE = 292, RESTRICT = 293, RESUME = 294, RETAINDAYS = 295, RETURN = 296, 
    RETURNS = 297, REVERT = 298, REVOKE = 299, REWIND = 300, RIGHT = 301, 
    ROLLBACK = 302, ROLE = 303, ROUND = 304, ROWCOUNT = 305, ROWGUIDCOL = 306, 
    RSA_512 = 307, RSA_1024 = 308, RSA_2048 = 309, RSA_3072 = 310, RSA_4096 = 311, 
    RTRIM = 312, SAFETY = 313, RULE = 314, SAFE = 315, SAVE = 316, SCHEDULER = 317, 
    SCHEMA = 318, SCHEME = 319, SECURITYAUDIT = 320, SELECT = 321, SEMANTICKEYPHRASETABLE = 322, 
    SEMANTICSIMILARITYDETAILSTABLE = 323, SEMANTICSIMILARITYTABLE = 324, 
    SERVER = 325, SERVICE = 326, SERVICE_BROKER = 327, SERVICE_NAME = 328, 
    SESSION = 329, SESSION_USER = 330, SESSIONPROPERTY = 331, SET = 332, 
    SETUSER = 333, SIGN = 334, SHUTDOWN = 335, SID = 336, SKIP_KEYWORD = 337, 
    SOFTNUMA = 338, SOME = 339, SOURCE = 340, SPACE = 341, SPECIFICATION = 342, 
    SPLIT = 343, SQLDUMPERFLAGS = 344, SQLDUMPERPATH = 345, SQLDUMPERTIMEOUT = 346, 
    STATISTICS = 347, STATE = 348, STATS = 349, START = 350, STARTED = 351, 
    STARTUP_STATE = 352, STOP = 353, STOPPED = 354, STOP_ON_ERROR = 355, 
    STR = 356, SUPPORTED = 357, SYSTEM_USER = 358, TABLE = 359, TABLESAMPLE = 360, 
    TAPE = 361, TARGET = 362, TCP = 363, TEXTSIZE = 364, THEN = 365, TO = 366, 
    TOP = 367, TRACK_CAUSALITY = 368, TRAN = 369, TRANSACTION = 370, TRANSFER = 371, 
    TRIGGER = 372, TRUNCATE = 373, TSEQUAL = 374, UNCHECKED = 375, UNION = 376, 
    UNIQUE = 377, UNLOCK = 378, UNPIVOT = 379, UNSAFE = 380, UPDATE = 381, 
    UPDATETEXT = 382, UPPER = 383, URL = 384, USE = 385, USED = 386, USER = 387, 
    USER_NAME = 388, VALUES = 389, VARYING = 390, VERBOSELOGGING = 391, 
    VIEW = 392, VISIBILITY = 393, WAITFOR = 394, WHEN = 395, WHERE = 396, 
    WHILE = 397, WINDOWS = 398, WITH = 399, WITHIN = 400, WITHOUT = 401, 
    WITNESS = 402, WRITETEXT = 403, YEAR = 404, ABSOLUTE = 405, ACCENT_SENSITIVITY = 406, 
    ACTION = 407, ACTIVATION = 408, ACTIVE = 409, ADDRESS = 410, AES_128 = 411, 
    AES_192 = 412, AES_256 = 413, AFFINITY = 414, AFTER = 415, AGGREGATE = 416, 
    ALGORITHM = 417, ALLOW_ENCRYPTED_VALUE_MODIFICATIONS = 418, ALLOW_SNAPSHOT_ISOLATION = 419, 
    ALLOWED = 420, ANSI_NULL_DEFAULT = 421, ANSI_NULLS = 422, ANSI_PADDING = 423, 
    ANSI_WARNINGS = 424, APPLICATION_LOG = 425, APPLY = 426, ARITHABORT = 427, 
    ASSEMBLY = 428, AUDIT = 429, AUDIT_GUID = 430, AUTO = 431, AUTO_CLEANUP = 432, 
    AUTO_CLOSE = 433, AUTO_CREATE_STATISTICS = 434, AUTO_SHRINK = 435, AUTO_UPDATE_STATISTICS = 436, 
    AUTO_UPDATE_STATISTICS_ASYNC = 437, AVAILABILITY = 438, AVG = 439, BACKUP_PRIORITY = 440, 
    BEGIN_DIALOG = 441, BIGINT = 442, BINARY_BASE64 = 443, BINARY_CHECKSUM = 444, 
    BINDING = 445, BLOB_STORAGE = 446, BROKER = 447, BROKER_INSTANCE = 448, 
    BULK_LOGGED = 449, CALLER = 450, CAP_CPU_PERCENT = 451, CAST = 452, 
    CATALOG = 453, CATCH = 454, CHANGE_RETENTION = 455, CHANGE_TRACKING = 456, 
    CHECKSUM = 457, CHECKSUM_AGG = 458, CLEANUP = 459, COLLECTION = 460, 
    COLUMN_MASTER_KEY = 461, COMMITTED = 462, COMPATIBILITY_LEVEL = 463, 
    CONCAT = 464, CONCAT_NULL_YIELDS_NULL = 465, CONTENT = 466, CONTROL = 467, 
    COOKIE = 468, COUNT = 469, COUNT_BIG = 470, COUNTER = 471, CPU = 472, 
    CREATE_NEW = 473, CREATION_DISPOSITION = 474, CREDENTIAL = 475, CRYPTOGRAPHIC = 476, 
    CURSOR_CLOSE_ON_COMMIT = 477, CURSOR_DEFAULT = 478, DATA = 479, DATALENGTH = 480, 
    DATE_CORRELATION_OPTIMIZATION = 481, DATEADD = 482, DATEDIFF = 483, 
    DATENAME = 484, DATEPART = 485, DAY = 486, DAYS = 487, DB_CHAINING = 488, 
    DB_FAILOVER = 489, DECRYPTION = 490, DEFAULT_DOUBLE_QUOTE = 491, DEFAULT_FULLTEXT_LANGUAGE = 492, 
    DEFAULT_LANGUAGE = 493, DELAY = 494, DELAYED_DURABILITY = 495, DELETED = 496, 
    DENSE_RANK = 497, DEPENDENTS = 498, DES = 499, DESCRIPTION = 500, DESX = 501, 
    DHCP = 502, DIALOG = 503, DIRECTORY_NAME = 504, DISABLE = 505, DISABLE_BROKER = 506, 
    DISABLED = 507, DISK_DRIVE = 508, DOCUMENT = 509, DYNAMIC = 510, ELEMENTS = 511, 
    EMERGENCY = 512, EMPTY = 513, ENABLE = 514, ENABLE_BROKER = 515, ENCRYPTED_VALUE = 516, 
    ENCRYPTION = 517, ENDPOINT_URL = 518, ERROR_BROKER_CONVERSATIONS = 519, 
    EXCLUSIVE = 520, EXECUTABLE = 521, EXIST = 522, EXPAND = 523, EXPIRY_DATE = 524, 
    EXPLICIT = 525, FAIL_OPERATION = 526, FAILOVER_MODE = 527, FAILURE = 528, 
    FAILURE_CONDITION_LEVEL = 529, FAST = 530, FAST_FORWARD = 531, FILEGROUP = 532, 
    FILEGROWTH = 533, FILEPATH = 534, FILESTREAM = 535, FILTER = 536, FIRST = 537, 
    FIRST_VALUE = 538, FOLLOWING = 539, FORCE = 540, FORCE_FAILOVER_ALLOW_DATA_LOSS = 541, 
    FORCED = 542, FORMAT = 543, FORWARD_ONLY = 544, FULLSCAN = 545, FULLTEXT = 546, 
    GB = 547, GETDATE = 548, GETUTCDATE = 549, GLOBAL = 550, GO = 551, GROUP_MAX_REQUESTS = 552, 
    GROUPING = 553, GROUPING_ID = 554, HADR = 555, HASH = 556, HEALTH_CHECK_TIMEOUT = 557, 
    HIGH = 558, HONOR_BROKER_PRIORITY = 559, HOURS = 560, IDENTITY_VALUE = 561, 
    IGNORE_NONCLUSTERED_COLUMNSTORE_INDEX = 562, IMMEDIATE = 563, IMPERSONATE = 564, 
    IMPORTANCE = 565, INCLUDE_NULL_VALUES = 566, INCREMENTAL = 567, INITIATOR = 568, 
    INPUT = 569, INSENSITIVE = 570, INSERTED = 571, INT = 572, IP = 573, 
    ISOLATION = 574, JOB = 575, JSON = 576, KB = 577, KEEP = 578, KEEPFIXED = 579, 
    KEY_SOURCE = 580, KEYS = 581, KEYSET = 582, LAG = 583, LAST = 584, LAST_VALUE = 585, 
    LEAD = 586, LEVEL = 587, LIST = 588, LISTENER = 589, LISTENER_URL = 590, 
    LOB_COMPACTION = 591, LOCAL = 592, LOCATION = 593, LOCK = 594, LOCK_ESCALATION = 595, 
    LOGIN = 596, LOOP = 597, LOW = 598, MANUAL = 599, MARK = 600, MATERIALIZED = 601, 
    MAX = 602, MAX_CPU_PERCENT = 603, MAX_DOP = 604, MAX_FILES = 605, MAX_IOPS_PER_VOLUME = 606, 
    MAX_MEMORY_PERCENT = 607, MAX_PROCESSES = 608, MAX_QUEUE_READERS = 609, 
    MAX_ROLLOVER_FILES = 610, MAXDOP = 611, MAXRECURSION = 612, MAXSIZE = 613, 
    MB = 614, MEDIUM = 615, MEMORY_OPTIMIZED_DATA = 616, MESSAGE = 617, 
    MIN = 618, MIN_ACTIVE_ROWVERSION = 619, MIN_CPU_PERCENT = 620, MIN_IOPS_PER_VOLUME = 621, 
    MIN_MEMORY_PERCENT = 622, MINUTES = 623, MIRROR_ADDRESS = 624, MIXED_PAGE_ALLOCATION = 625, 
    MODE = 626, MODIFY = 627, MOVE = 628, MULTI_USER = 629, NAME = 630, 
    NESTED_TRIGGERS = 631, NEW_ACCOUNT = 632, NEW_BROKER = 633, NEW_PASSWORD = 634, 
    NEXT = 635, NO = 636, NO_TRUNCATE = 637, NO_WAIT = 638, NOCOUNT = 639, 
    NODES = 640, NOEXPAND = 641, NON_TRANSACTED_ACCESS = 642, NORECOMPUTE = 643, 
    NORECOVERY = 644, NOWAIT = 645, NTILE = 646, NUMANODE = 647, NUMBER = 648, 
    NUMERIC_ROUNDABORT = 649, OBJECT = 650, OFFLINE = 651, OFFSET = 652, 
    OLD_ACCOUNT = 653, ONLINE = 654, ONLY = 655, OPEN_EXISTING = 656, OPTIMISTIC = 657, 
    OPTIMIZE = 658, OUT = 659, OUTPUT = 660, OVERRIDE = 661, OWNER = 662, 
    PAGE_VERIFY = 663, PARAMETERIZATION = 664, PARTITION = 665, PARTITIONS = 666, 
    PARTNER = 667, PATH = 668, POISON_MESSAGE_HANDLING = 669, POOL = 670, 
    PORT = 671, PRECEDING = 672, PRIMARY_ROLE = 673, PRIOR = 674, PRIORITY = 675, 
    PRIORITY_LEVEL = 676, PRIVATE = 677, PRIVATE_KEY = 678, PRIVILEGES = 679, 
    PROCEDURE_NAME = 680, PROPERTY = 681, PROVIDER = 682, PROVIDER_KEY_NAME = 683, 
    QUERY = 684, QUEUE = 685, QUEUE_DELAY = 686, QUOTED_IDENTIFIER = 687, 
    RANGE = 688, RANK = 689, RC2 = 690, RC4 = 691, RC4_128 = 692, READ_COMMITTED_SNAPSHOT = 693, 
    READ_ONLY = 694, READ_ONLY_ROUTING_LIST = 695, READ_WRITE = 696, READONLY = 697, 
    REBUILD = 698, RECEIVE = 699, RECOMPILE = 700, RECOVERY = 701, RECURSIVE_TRIGGERS = 702, 
    RELATIVE = 703, REMOTE = 704, REMOTE_SERVICE_NAME = 705, REMOVE = 706, 
    REORGANIZE = 707, REPEATABLE = 708, REPLICA = 709, REQUEST_MAX_CPU_TIME_SEC = 710, 
    REQUEST_MAX_MEMORY_GRANT_PERCENT = 711, REQUEST_MEMORY_GRANT_TIMEOUT_SEC = 712, 
    REQUIRED_SYNCHRONIZED_SECONDARIES_TO_COMMIT = 713, RESERVE_DISK_SPACE = 714, 
    RESOURCE = 715, RESOURCE_MANAGER_LOCATION = 716, RESTRICTED_USER = 717, 
    RETENTION = 718, ROBUST = 719, ROOT = 720, ROUTE = 721, ROW = 722, ROW_NUMBER = 723, 
    ROWGUID = 724, ROWS = 725, SAMPLE = 726, SCHEMABINDING = 727, SCOPED = 728, 
    SCROLL = 729, SCROLL_LOCKS = 730, SEARCH = 731, SECONDARY = 732, SECONDARY_ONLY = 733, 
    SECONDARY_ROLE = 734, SECONDS = 735, SECRET = 736, SECURITY = 737, SECURITY_LOG = 738, 
    SEEDING_MODE = 739, SELF = 740, SEMI_SENSITIVE = 741, SEND = 742, SENT = 743, 
    SEQUENCE = 744, SERIALIZABLE = 745, SESSION_TIMEOUT = 746, SETERROR = 747, 
    SHARE = 748, SHOWPLAN = 749, SIGNATURE = 750, SIMPLE = 751, SINGLE_USER = 752, 
    SIZE = 753, SMALLINT = 754, SNAPSHOT = 755, SPATIAL_WINDOW_MAX_CELLS = 756, 
    STANDBY = 757, START_DATE = 758, STATIC = 759, STATS_STREAM = 760, STATUS = 761, 
    STATUSONLY = 762, STDEV = 763, STDEVP = 764, STOPLIST = 765, STRING_AGG = 766, 
    STUFF = 767, SUBJECT = 768, SUBSCRIPTION = 769, SUBSTRING = 770, SUM = 771, 
    SUSPEND = 772, SYMMETRIC = 773, SYNCHRONOUS_COMMIT = 774, SYNONYM = 775, 
    SYSTEM = 776, TAKE = 777, TARGET_RECOVERY_TIME = 778, TB = 779, TEXTIMAGE_ON = 780, 
    THROW = 781, TIES = 782, TIME = 783, TIMEOUT = 784, TIMER = 785, TINYINT = 786, 
    TORN_PAGE_DETECTION = 787, TRANSFORM_NOISE_WORDS = 788, TRIPLE_DES = 789, 
    TRIPLE_DES_3KEY = 790, TRUSTWORTHY = 791, TRY = 792, TSQL = 793, TWO_DIGIT_YEAR_CUTOFF = 794, 
    TYPE = 795, TYPE_WARNING = 796, UNBOUNDED = 797, UNCOMMITTED = 798, 
    UNKNOWN = 799, UNLIMITED = 800, UOW = 801, USING = 802, VALID_XML = 803, 
    VALIDATION = 804, VALUE = 805, VAR = 806, VARP = 807, VIEW_METADATA = 808, 
    VIEWS = 809, WAIT = 810, WELL_FORMED_XML = 811, WITHOUT_ARRAY_WRAPPER = 812, 
    WORK = 813, WORKLOAD = 814, XML = 815, XMLDATA = 816, XMLNAMESPACES = 817, 
    XMLSCHEMA = 818, XSINIL = 819, DOLLAR_ACTION = 820, WHITE_SPACE = 821, 
    COMMENT = 822, LINE_COMMENT = 823, DOUBLE_QUOTE_ID = 824, SINGLE_QUOTE = 825, 
    SQUARE_BRACKET_ID = 826, LOCAL_ID = 827, DECIMAL = 828, ID = 829, QUOTED_URL = 830, 
    QUOTED_HOST_AND_PORT = 831, STRING = 832, BINARY = 833, FLOAT = 834, 
    REAL = 835, EQUAL = 836, GREATER = 837, LESS = 838, EXCLAMATION = 839, 
    PLUS_ASSIGN = 840, MINUS_ASSIGN = 841, MULT_ASSIGN = 842, DIV_ASSIGN = 843, 
    MOD_ASSIGN = 844, AND_ASSIGN = 845, XOR_ASSIGN = 846, OR_ASSIGN = 847, 
    DOUBLE_BAR = 848, DOT = 849, UNDERLINE = 850, AT = 851, SHARP = 852, 
    DOLLAR = 853, LR_BRACKET = 854, RR_BRACKET = 855, COMMA = 856, SEMI = 857, 
    COLON = 858, STAR = 859, DIVIDE = 860, MODULE = 861, PLUS = 862, MINUS = 863, 
    BIT_NOT = 864, BIT_OR = 865, BIT_AND = 866, BIT_XOR = 867, IPV4_OCTECT = 868
  };

  enum {
    RuleTsql_file = 0, RuleBatch = 1, RuleSql_clauses = 2, RuleSql_clause = 3, 
    RuleDml_clause = 4, RuleDdl_clause = 5, RuleCfl_statement = 6, RuleBlock_statement = 7, 
    RuleBreak_statement = 8, RuleContinue_statement = 9, RuleGoto_statement = 10, 
    RuleReturn_statement = 11, RuleIf_statement = 12, RuleThrow_statement = 13, 
    RuleThrow_error_number = 14, RuleThrow_message = 15, RuleThrow_state = 16, 
    RuleTry_catch_statement = 17, RuleWaitfor_statement = 18, RuleWhile_statement = 19, 
    RulePrint_statement = 20, RuleRaiseerror_statement = 21, RuleEmpty_statement = 22, 
    RuleAnother_statement = 23, RuleDrop_aggregate = 24, RuleEntity_to = 25, 
    RuleColon_colon = 26, RuleClass_type = 27, RuleClass_type_for_sql_database = 28, 
    RuleClass_type_for_parallel_dw = 29, RuleDrop_schema = 30, RuleLock_table = 31, 
    RuleTruncate_table = 32, RuleEvent_session_predicate_expression = 33, 
    RuleEvent_session_predicate_factor = 34, RuleEvent_session_predicate_leaf = 35, 
    RuleAlter_schema_sql = 36, RuleCreate_schema = 37, RuleCreate_xml_schema_collection = 38, 
    RuleCreate_queue = 39, RuleQueue_settings = 40, RuleAlter_queue = 41, 
    RuleQueue_action = 42, RuleQueue_rebuild_options = 43, RuleConversation_statement = 44, 
    RuleMessage_statement = 45, RuleMerge_statement = 46, RuleMerge_matched = 47, 
    RuleMerge_not_matched = 48, RuleDelete_statement = 49, RuleDelete_statement_from = 50, 
    RuleInsert_statement = 51, RuleInsert_statement_value = 52, RuleReceive_statement = 53, 
    RuleSelect_statement = 54, RuleTime = 55, RuleUpdate_statement = 56, 
    RuleOutput_clause = 57, RuleOutput_dml_list_elem = 58, RuleOutput_column_name = 59, 
    RuleCreate_database = 60, RuleCreate_index = 61, RuleCreate_or_alter_procedure = 62, 
    RuleCreate_or_alter_function = 63, RuleFunc_body_returns_select = 64, 
    RuleFunc_body_returns_table = 65, RuleFunc_body_returns_scalar = 66, 
    RuleProcedure_param = 67, RuleProcedure_option = 68, RuleFunction_option = 69, 
    RuleCreate_table = 70, RuleTable_options = 71, RuleCreate_view = 72, 
    RuleView_attribute = 73, RuleAlter_table = 74, RuleCursor_option = 75, 
    RuleTarget_recovery_time_option = 76, RuleTermination = 77, RuleDrop_index = 78, 
    RuleDrop_relational_or_xml_or_spatial_index = 79, RuleDrop_backward_compatible_index = 80, 
    RuleDrop_procedure = 81, RuleDrop_function = 82, RuleDrop_table = 83, 
    RuleDrop_view = 84, RuleCreate_type = 85, RuleDrop_type = 86, RuleRowset_function_limited = 87, 
    RuleOpenquery = 88, RuleOpendatasource = 89, RuleDeclare_statement = 90, 
    RuleCursor_statement = 91, RuleKill_statement = 92, RuleKill_process = 93, 
    RuleKill_query_notification = 94, RuleKill_stats_job = 95, RuleExecute_statement = 96, 
    RuleExecute_body = 97, RuleExecute_statement_arg = 98, RuleExecute_var_string = 99, 
    RuleSet_statement = 100, RuleTransaction_statement = 101, RuleGo_statement = 102, 
    RuleUse_statement = 103, RuleSetuser_statement = 104, RuleReconfigure_statement = 105, 
    RuleShutdown_statement = 106, RuleDbcc_clause = 107, RuleDbcc_options = 108, 
    RuleExecute_clause = 109, RuleDeclare_local = 110, RuleTable_type_definition = 111, 
    RuleXml_type_definition = 112, RuleXml_schema_collection = 113, RuleColumn_def_table_constraints = 114, 
    RuleColumn_def_table_constraint = 115, RuleColumn_definition = 116, 
    RuleMaterialized_column_definition = 117, RuleColumn_constraint = 118, 
    RuleTable_constraint = 119, RuleOn_delete = 120, RuleOn_update = 121, 
    RuleIndex_options = 122, RuleIndex_option = 123, RuleDeclare_cursor = 124, 
    RuleDeclare_set_cursor_common = 125, RuleDeclare_set_cursor_common_partial = 126, 
    RuleFetch_cursor = 127, RuleSet_special = 128, RuleConstant_LOCAL_ID = 129, 
    RuleExpression = 130, RulePrimitive_expression = 131, RuleCase_expression = 132, 
    RuleUnary_operator_expression = 133, RuleBracket_expression = 134, RuleConstant_expression = 135, 
    RuleSubquery = 136, RuleWith_expression = 137, RuleCommon_table_expression = 138, 
    RuleUpdate_elem = 139, RuleSearch_condition_list = 140, RuleSearch_condition = 141, 
    RuleSearch_condition_and = 142, RuleSearch_condition_not = 143, RulePredicate = 144, 
    RuleQuery_expression = 145, RuleSql_union = 146, RuleQuery_specification = 147, 
    RuleTop_clause = 148, RuleTop_percent = 149, RuleTop_count = 150, RuleOrder_by_clause = 151, 
    RuleFor_clause = 152, RuleXml_common_directives = 153, RuleOrder_by_expression = 154, 
    RuleGroup_by_item = 155, RuleOption_clause = 156, RuleOption = 157, 
    RuleOptimize_for_arg = 158, RuleSelect_list = 159, RuleUdt_method_arguments = 160, 
    RuleAsterisk = 161, RuleColumn_elem = 162, RuleUdt_elem = 163, RuleExpression_elem = 164, 
    RuleSelect_list_elem = 165, RuleTable_sources = 166, RuleTable_source = 167, 
    RuleTable_source_item_joined = 168, RuleTable_source_item = 169, RuleOpen_xml = 170, 
    RuleSchema_declaration = 171, RuleColumn_declaration = 172, RuleChange_table = 173, 
    RuleJoin_part = 174, RulePivot_clause = 175, RuleUnpivot_clause = 176, 
    RuleFull_column_name_list = 177, RuleTable_name_with_hint = 178, RuleRowset_function = 179, 
    RuleBulk_option = 180, RuleDerived_table = 181, RuleFunction_call = 182, 
    RuleXml_data_type_methods = 183, RuleValue_method = 184, RuleQuery_method = 185, 
    RuleExist_method = 186, RuleModify_method = 187, RuleNodes_method = 188, 
    RuleSwitch_section = 189, RuleSwitch_search_condition_section = 190, 
    RuleAs_column_alias = 191, RuleAs_table_alias = 192, RuleTable_alias = 193, 
    RuleWith_table_hints = 194, RuleInsert_with_table_hints = 195, RuleTable_hint = 196, 
    RuleIndex_value = 197, RuleColumn_alias_list = 198, RuleColumn_alias = 199, 
    RuleTable_value_constructor = 200, RuleExpression_list = 201, RuleRanking_windowed_function = 202, 
    RuleAggregate_windowed_function = 203, RuleAnalytic_windowed_function = 204, 
    RuleAll_distinct_expression = 205, RuleOver_clause = 206, RuleRow_or_range_clause = 207, 
    RuleWindow_frame_extent = 208, RuleWindow_frame_bound = 209, RuleWindow_frame_preceding = 210, 
    RuleWindow_frame_following = 211, RuleCreate_database_option = 212, 
    RuleDatabase_filestream_option = 213, RuleDatabase_file_spec = 214, 
    RuleFile_group = 215, RuleFile_spec = 216, RuleEntity_name = 217, RuleEntity_name_for_azure_dw = 218, 
    RuleEntity_name_for_parallel_dw = 219, RuleFull_table_name = 220, RuleTable_name = 221, 
    RuleSimple_name = 222, RuleFunc_proc_name_schema = 223, RuleFunc_proc_name_database_schema = 224, 
    RuleFunc_proc_name_server_database_schema = 225, RuleDdl_object = 226, 
    RuleFull_column_name = 227, RuleColumn_name_list_with_order = 228, RuleColumn_name_list = 229, 
    RuleCursor_name = 230, RuleOn_off = 231, RuleClustered = 232, RuleNull_notnull = 233, 
    RuleNull_or_default = 234, RuleScalar_function_name = 235, RuleBegin_conversation_timer = 236, 
    RuleBegin_conversation_dialog = 237, RuleContract_name = 238, RuleService_name = 239, 
    RuleEnd_conversation = 240, RuleWaitfor_conversation = 241, RuleGet_conversation = 242, 
    RuleQueue_id = 243, RuleSend_conversation = 244, RuleData_type = 245, 
    RuleDefault_value = 246, RuleConstant = 247, RuleSign = 248, RuleId = 249, 
    RuleSimple_id = 250, RuleComparison_operator = 251, RuleAssignment_operator = 252, 
    RuleFile_size = 253
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
  class Sql_clausesContext;
  class Sql_clauseContext;
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
  class Waitfor_statementContext;
  class While_statementContext;
  class Print_statementContext;
  class Raiseerror_statementContext;
  class Empty_statementContext;
  class Another_statementContext;
  class Drop_aggregateContext;
  class Entity_toContext;
  class Colon_colonContext;
  class Class_typeContext;
  class Class_type_for_sql_databaseContext;
  class Class_type_for_parallel_dwContext;
  class Drop_schemaContext;
  class Lock_tableContext;
  class Truncate_tableContext;
  class Event_session_predicate_expressionContext;
  class Event_session_predicate_factorContext;
  class Event_session_predicate_leafContext;
  class Alter_schema_sqlContext;
  class Create_schemaContext;
  class Create_xml_schema_collectionContext;
  class Create_queueContext;
  class Queue_settingsContext;
  class Alter_queueContext;
  class Queue_actionContext;
  class Queue_rebuild_optionsContext;
  class Conversation_statementContext;
  class Message_statementContext;
  class Merge_statementContext;
  class Merge_matchedContext;
  class Merge_not_matchedContext;
  class Delete_statementContext;
  class Delete_statement_fromContext;
  class Insert_statementContext;
  class Insert_statement_valueContext;
  class Receive_statementContext;
  class Select_statementContext;
  class TimeContext;
  class Update_statementContext;
  class Output_clauseContext;
  class Output_dml_list_elemContext;
  class Output_column_nameContext;
  class Create_databaseContext;
  class Create_indexContext;
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
  class Create_viewContext;
  class View_attributeContext;
  class Alter_tableContext;
  class Cursor_optionContext;
  class Target_recovery_time_optionContext;
  class TerminationContext;
  class Drop_indexContext;
  class Drop_relational_or_xml_or_spatial_indexContext;
  class Drop_backward_compatible_indexContext;
  class Drop_procedureContext;
  class Drop_functionContext;
  class Drop_tableContext;
  class Drop_viewContext;
  class Create_typeContext;
  class Drop_typeContext;
  class Rowset_function_limitedContext;
  class OpenqueryContext;
  class OpendatasourceContext;
  class Declare_statementContext;
  class Cursor_statementContext;
  class Kill_statementContext;
  class Kill_processContext;
  class Kill_query_notificationContext;
  class Kill_stats_jobContext;
  class Execute_statementContext;
  class Execute_bodyContext;
  class Execute_statement_argContext;
  class Execute_var_stringContext;
  class Set_statementContext;
  class Transaction_statementContext;
  class Go_statementContext;
  class Use_statementContext;
  class Setuser_statementContext;
  class Reconfigure_statementContext;
  class Shutdown_statementContext;
  class Dbcc_clauseContext;
  class Dbcc_optionsContext;
  class Execute_clauseContext;
  class Declare_localContext;
  class Table_type_definitionContext;
  class Xml_type_definitionContext;
  class Xml_schema_collectionContext;
  class Column_def_table_constraintsContext;
  class Column_def_table_constraintContext;
  class Column_definitionContext;
  class Materialized_column_definitionContext;
  class Column_constraintContext;
  class Table_constraintContext;
  class On_deleteContext;
  class On_updateContext;
  class Index_optionsContext;
  class Index_optionContext;
  class Declare_cursorContext;
  class Declare_set_cursor_commonContext;
  class Declare_set_cursor_common_partialContext;
  class Fetch_cursorContext;
  class Set_specialContext;
  class Constant_LOCAL_IDContext;
  class ExpressionContext;
  class Primitive_expressionContext;
  class Case_expressionContext;
  class Unary_operator_expressionContext;
  class Bracket_expressionContext;
  class Constant_expressionContext;
  class SubqueryContext;
  class With_expressionContext;
  class Common_table_expressionContext;
  class Update_elemContext;
  class Search_condition_listContext;
  class Search_conditionContext;
  class Search_condition_andContext;
  class Search_condition_notContext;
  class PredicateContext;
  class Query_expressionContext;
  class Sql_unionContext;
  class Query_specificationContext;
  class Top_clauseContext;
  class Top_percentContext;
  class Top_countContext;
  class Order_by_clauseContext;
  class For_clauseContext;
  class Xml_common_directivesContext;
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
  class Open_xmlContext;
  class Schema_declarationContext;
  class Column_declarationContext;
  class Change_tableContext;
  class Join_partContext;
  class Pivot_clauseContext;
  class Unpivot_clauseContext;
  class Full_column_name_listContext;
  class Table_name_with_hintContext;
  class Rowset_functionContext;
  class Bulk_optionContext;
  class Derived_tableContext;
  class Function_callContext;
  class Xml_data_type_methodsContext;
  class Value_methodContext;
  class Query_methodContext;
  class Exist_methodContext;
  class Modify_methodContext;
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
  class Create_database_optionContext;
  class Database_filestream_optionContext;
  class Database_file_specContext;
  class File_groupContext;
  class File_specContext;
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
  class Column_name_listContext;
  class Cursor_nameContext;
  class On_offContext;
  class ClusteredContext;
  class Null_notnullContext;
  class Null_or_defaultContext;
  class Scalar_function_nameContext;
  class Begin_conversation_timerContext;
  class Begin_conversation_dialogContext;
  class Contract_nameContext;
  class Service_nameContext;
  class End_conversationContext;
  class Waitfor_conversationContext;
  class Get_conversationContext;
  class Queue_idContext;
  class Send_conversationContext;
  class Data_typeContext;
  class Default_valueContext;
  class ConstantContext;
  class SignContext;
  class IdContext;
  class Simple_idContext;
  class Comparison_operatorContext;
  class Assignment_operatorContext;
  class File_sizeContext; 

  class  Tsql_fileContext : public antlr4::ParserRuleContext {
  public:
    Tsql_fileContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EOF();
    std::vector<BatchContext *> batch();
    BatchContext* batch(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Tsql_fileContext* tsql_file();

  class  BatchContext : public antlr4::ParserRuleContext {
  public:
    BatchContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Execute_bodyContext *execute_body();
    std::vector<Go_statementContext *> go_statement();
    Go_statementContext* go_statement(size_t i);
    Sql_clausesContext *sql_clauses();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  BatchContext* batch();

  class  Sql_clausesContext : public antlr4::ParserRuleContext {
  public:
    Sql_clausesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Sql_clauseContext *> sql_clause();
    Sql_clauseContext* sql_clause(size_t i);
    std::vector<antlr4::tree::TerminalNode *> SEMI();
    antlr4::tree::TerminalNode* SEMI(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Sql_clausesContext* sql_clauses();

  class  Sql_clauseContext : public antlr4::ParserRuleContext {
  public:
    Sql_clauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Dml_clauseContext *dml_clause();
    Ddl_clauseContext *ddl_clause();
    Cfl_statementContext *cfl_statement();
    Dbcc_clauseContext *dbcc_clause();
    Empty_statementContext *empty_statement();
    Another_statementContext *another_statement();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Sql_clauseContext* sql_clause();

  class  Dml_clauseContext : public antlr4::ParserRuleContext {
  public:
    Dml_clauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Merge_statementContext *merge_statement();
    Delete_statementContext *delete_statement();
    Insert_statementContext *insert_statement();
    Select_statementContext *select_statement();
    Update_statementContext *update_statement();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Dml_clauseContext* dml_clause();

  class  Ddl_clauseContext : public antlr4::ParserRuleContext {
  public:
    Ddl_clauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Alter_schema_sqlContext *alter_schema_sql();
    Alter_tableContext *alter_table();
    Create_indexContext *create_index();
    Create_or_alter_functionContext *create_or_alter_function();
    Create_or_alter_procedureContext *create_or_alter_procedure();
    Create_schemaContext *create_schema();
    Create_tableContext *create_table();
    Create_typeContext *create_type();
    Create_viewContext *create_view();
    Drop_aggregateContext *drop_aggregate();
    Drop_functionContext *drop_function();
    Drop_indexContext *drop_index();
    Drop_procedureContext *drop_procedure();
    Drop_schemaContext *drop_schema();
    Drop_tableContext *drop_table();
    Drop_typeContext *drop_type();
    Drop_viewContext *drop_view();
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
    Waitfor_statementContext *waitfor_statement();
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
    Sql_clausesContext *sql_clauses();

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
    IdContext *id();
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
    std::vector<Sql_clauseContext *> sql_clause();
    Sql_clauseContext* sql_clause(size_t i);
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

  class  Waitfor_statementContext : public antlr4::ParserRuleContext {
  public:
    Waitfor_statementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *WAITFOR();
    Receive_statementContext *receive_statement();
    antlr4::tree::TerminalNode *COMMA();
    TimeContext *time();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *SEMI();
    antlr4::tree::TerminalNode *DELAY();
    antlr4::tree::TerminalNode *TIME();
    antlr4::tree::TerminalNode *TIMEOUT();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Waitfor_statementContext* waitfor_statement();

  class  While_statementContext : public antlr4::ParserRuleContext {
  public:
    While_statementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *WHILE();
    Search_conditionContext *search_condition();
    Sql_clauseContext *sql_clause();
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
    Cursor_statementContext *cursor_statement();
    Conversation_statementContext *conversation_statement();
    Create_queueContext *create_queue();
    Alter_queueContext *alter_queue();
    Execute_statementContext *execute_statement();
    Kill_statementContext *kill_statement();
    Message_statementContext *message_statement();
    Set_statementContext *set_statement();
    Transaction_statementContext *transaction_statement();
    Use_statementContext *use_statement();
    Setuser_statementContext *setuser_statement();
    Reconfigure_statementContext *reconfigure_statement();
    Shutdown_statementContext *shutdown_statement();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Another_statementContext* another_statement();

  class  Drop_aggregateContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::IdContext *schema_name = nullptr;
    TSqlParser::IdContext *aggregate_name = nullptr;
    Drop_aggregateContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DROP();
    antlr4::tree::TerminalNode *AGGREGATE();
    std::vector<IdContext *> id();
    IdContext* id(size_t i);
    antlr4::tree::TerminalNode *IF();
    antlr4::tree::TerminalNode *EXISTS();
    antlr4::tree::TerminalNode *DOT();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Drop_aggregateContext* drop_aggregate();

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
    std::vector<antlr4::tree::TerminalNode *> COLON();
    antlr4::tree::TerminalNode* COLON(size_t i);

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

  class  Drop_schemaContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::IdContext *schema_name = nullptr;
    Drop_schemaContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DROP();
    antlr4::tree::TerminalNode *SCHEMA();
    IdContext *id();
    antlr4::tree::TerminalNode *IF();
    antlr4::tree::TerminalNode *EXISTS();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Drop_schemaContext* drop_schema();

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

  class  Event_session_predicate_expressionContext : public antlr4::ParserRuleContext {
  public:
    Event_session_predicate_expressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Event_session_predicate_factorContext *> event_session_predicate_factor();
    Event_session_predicate_factorContext* event_session_predicate_factor(size_t i);
    std::vector<antlr4::tree::TerminalNode *> LR_BRACKET();
    antlr4::tree::TerminalNode* LR_BRACKET(size_t i);
    std::vector<Event_session_predicate_expressionContext *> event_session_predicate_expression();
    Event_session_predicate_expressionContext* event_session_predicate_expression(size_t i);
    std::vector<antlr4::tree::TerminalNode *> RR_BRACKET();
    antlr4::tree::TerminalNode* RR_BRACKET(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    std::vector<antlr4::tree::TerminalNode *> NOT();
    antlr4::tree::TerminalNode* NOT(size_t i);
    std::vector<antlr4::tree::TerminalNode *> AND();
    antlr4::tree::TerminalNode* AND(size_t i);
    std::vector<antlr4::tree::TerminalNode *> OR();
    antlr4::tree::TerminalNode* OR(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Event_session_predicate_expressionContext* event_session_predicate_expression();

  class  Event_session_predicate_factorContext : public antlr4::ParserRuleContext {
  public:
    Event_session_predicate_factorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Event_session_predicate_leafContext *event_session_predicate_leaf();
    antlr4::tree::TerminalNode *LR_BRACKET();
    Event_session_predicate_expressionContext *event_session_predicate_expression();
    antlr4::tree::TerminalNode *RR_BRACKET();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Event_session_predicate_factorContext* event_session_predicate_factor();

  class  Event_session_predicate_leafContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::IdContext *event_field_name = nullptr;
    TSqlParser::IdContext *event_module_guid = nullptr;
    TSqlParser::IdContext *event_package_name = nullptr;
    TSqlParser::IdContext *predicate_source_name = nullptr;
    TSqlParser::IdContext *predicate_compare_name = nullptr;
    Event_session_predicate_leafContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<IdContext *> id();
    IdContext* id(size_t i);
    antlr4::tree::TerminalNode *DECIMAL();
    antlr4::tree::TerminalNode *STRING();
    antlr4::tree::TerminalNode *EQUAL();
    antlr4::tree::TerminalNode *GREATER();
    antlr4::tree::TerminalNode *LESS();
    std::vector<antlr4::tree::TerminalNode *> DOT();
    antlr4::tree::TerminalNode* DOT(size_t i);
    antlr4::tree::TerminalNode *EXCLAMATION();
    antlr4::tree::TerminalNode *LR_BRACKET();
    antlr4::tree::TerminalNode *RR_BRACKET();
    antlr4::tree::TerminalNode *COMMA();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Event_session_predicate_leafContext* event_session_predicate_leaf();

  class  Alter_schema_sqlContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::IdContext *schema_name = nullptr;
    Alter_schema_sqlContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ALTER();
    std::vector<antlr4::tree::TerminalNode *> SCHEMA();
    antlr4::tree::TerminalNode* SCHEMA(size_t i);
    antlr4::tree::TerminalNode *TRANSFER();
    std::vector<IdContext *> id();
    IdContext* id(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COLON();
    antlr4::tree::TerminalNode* COLON(size_t i);
    antlr4::tree::TerminalNode *DOT();
    antlr4::tree::TerminalNode *OBJECT();
    antlr4::tree::TerminalNode *TYPE();
    antlr4::tree::TerminalNode *XML();
    antlr4::tree::TerminalNode *COLLECTION();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Alter_schema_sqlContext* alter_schema_sql();

  class  Create_schemaContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::IdContext *schema_name = nullptr;
    TSqlParser::IdContext *owner_name = nullptr;
    TSqlParser::IdContext *object_name = nullptr;
    Create_schemaContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CREATE();
    std::vector<antlr4::tree::TerminalNode *> SCHEMA();
    antlr4::tree::TerminalNode* SCHEMA(size_t i);
    antlr4::tree::TerminalNode *AUTHORIZATION();
    std::vector<IdContext *> id();
    IdContext* id(size_t i);
    std::vector<Create_tableContext *> create_table();
    Create_tableContext* create_table(size_t i);
    std::vector<Create_viewContext *> create_view();
    Create_viewContext* create_view(size_t i);
    std::vector<antlr4::tree::TerminalNode *> ON();
    antlr4::tree::TerminalNode* ON(size_t i);
    std::vector<antlr4::tree::TerminalNode *> TO();
    antlr4::tree::TerminalNode* TO(size_t i);
    std::vector<antlr4::tree::TerminalNode *> REVOKE();
    antlr4::tree::TerminalNode* REVOKE(size_t i);
    std::vector<antlr4::tree::TerminalNode *> FROM();
    antlr4::tree::TerminalNode* FROM(size_t i);
    std::vector<antlr4::tree::TerminalNode *> GRANT();
    antlr4::tree::TerminalNode* GRANT(size_t i);
    std::vector<antlr4::tree::TerminalNode *> DENY();
    antlr4::tree::TerminalNode* DENY(size_t i);
    std::vector<antlr4::tree::TerminalNode *> SELECT();
    antlr4::tree::TerminalNode* SELECT(size_t i);
    std::vector<antlr4::tree::TerminalNode *> INSERT();
    antlr4::tree::TerminalNode* INSERT(size_t i);
    std::vector<antlr4::tree::TerminalNode *> DELETE();
    antlr4::tree::TerminalNode* DELETE(size_t i);
    std::vector<antlr4::tree::TerminalNode *> UPDATE();
    antlr4::tree::TerminalNode* UPDATE(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COLON();
    antlr4::tree::TerminalNode* COLON(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Create_schemaContext* create_schema();

  class  Create_xml_schema_collectionContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::IdContext *relational_schema = nullptr;
    TSqlParser::IdContext *sql_identifier = nullptr;
    Create_xml_schema_collectionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CREATE();
    antlr4::tree::TerminalNode *XML();
    antlr4::tree::TerminalNode *SCHEMA();
    antlr4::tree::TerminalNode *COLLECTION();
    antlr4::tree::TerminalNode *AS();
    std::vector<IdContext *> id();
    IdContext* id(size_t i);
    antlr4::tree::TerminalNode *STRING();
    antlr4::tree::TerminalNode *LOCAL_ID();
    antlr4::tree::TerminalNode *DOT();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Create_xml_schema_collectionContext* create_xml_schema_collection();

  class  Create_queueContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::IdContext *queue_name = nullptr;
    TSqlParser::IdContext *filegroup = nullptr;
    Create_queueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CREATE();
    antlr4::tree::TerminalNode *QUEUE();
    Full_table_nameContext *full_table_name();
    std::vector<IdContext *> id();
    IdContext* id(size_t i);
    Queue_settingsContext *queue_settings();
    antlr4::tree::TerminalNode *ON();
    antlr4::tree::TerminalNode *DEFAULT();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Create_queueContext* create_queue();

  class  Queue_settingsContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *max_readers = nullptr;
    antlr4::Token *user_name = nullptr;
    Queue_settingsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *WITH();
    std::vector<antlr4::tree::TerminalNode *> STATUS();
    antlr4::tree::TerminalNode* STATUS(size_t i);
    std::vector<antlr4::tree::TerminalNode *> EQUAL();
    antlr4::tree::TerminalNode* EQUAL(size_t i);
    antlr4::tree::TerminalNode *RETENTION();
    antlr4::tree::TerminalNode *ACTIVATION();
    std::vector<antlr4::tree::TerminalNode *> LR_BRACKET();
    antlr4::tree::TerminalNode* LR_BRACKET(size_t i);
    std::vector<antlr4::tree::TerminalNode *> RR_BRACKET();
    antlr4::tree::TerminalNode* RR_BRACKET(size_t i);
    antlr4::tree::TerminalNode *POISON_MESSAGE_HANDLING();
    std::vector<antlr4::tree::TerminalNode *> ON();
    antlr4::tree::TerminalNode* ON(size_t i);
    std::vector<antlr4::tree::TerminalNode *> OFF();
    antlr4::tree::TerminalNode* OFF(size_t i);
    antlr4::tree::TerminalNode *DROP();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    antlr4::tree::TerminalNode *PROCEDURE_NAME();
    Func_proc_name_database_schemaContext *func_proc_name_database_schema();
    antlr4::tree::TerminalNode *MAX_QUEUE_READERS();
    antlr4::tree::TerminalNode *EXECUTE();
    antlr4::tree::TerminalNode *AS();
    antlr4::tree::TerminalNode *DECIMAL();
    antlr4::tree::TerminalNode *SELF();
    antlr4::tree::TerminalNode *OWNER();
    antlr4::tree::TerminalNode *STRING();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Queue_settingsContext* queue_settings();

  class  Alter_queueContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::IdContext *queue_name = nullptr;
    Alter_queueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ALTER();
    antlr4::tree::TerminalNode *QUEUE();
    Full_table_nameContext *full_table_name();
    Queue_settingsContext *queue_settings();
    Queue_actionContext *queue_action();
    IdContext *id();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Alter_queueContext* alter_queue();

  class  Queue_actionContext : public antlr4::ParserRuleContext {
  public:
    Queue_actionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *REBUILD();
    antlr4::tree::TerminalNode *WITH();
    antlr4::tree::TerminalNode *LR_BRACKET();
    Queue_rebuild_optionsContext *queue_rebuild_options();
    antlr4::tree::TerminalNode *RR_BRACKET();
    antlr4::tree::TerminalNode *REORGANIZE();
    antlr4::tree::TerminalNode *LOB_COMPACTION();
    antlr4::tree::TerminalNode *EQUAL();
    antlr4::tree::TerminalNode *ON();
    antlr4::tree::TerminalNode *OFF();
    antlr4::tree::TerminalNode *MOVE();
    antlr4::tree::TerminalNode *TO();
    IdContext *id();
    antlr4::tree::TerminalNode *DEFAULT();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Queue_actionContext* queue_action();

  class  Queue_rebuild_optionsContext : public antlr4::ParserRuleContext {
  public:
    Queue_rebuild_optionsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *MAXDOP();
    antlr4::tree::TerminalNode *EQUAL();
    antlr4::tree::TerminalNode *DECIMAL();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Queue_rebuild_optionsContext* queue_rebuild_options();

  class  Conversation_statementContext : public antlr4::ParserRuleContext {
  public:
    Conversation_statementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Begin_conversation_timerContext *begin_conversation_timer();
    Begin_conversation_dialogContext *begin_conversation_dialog();
    End_conversationContext *end_conversation();
    Get_conversationContext *get_conversation();
    Send_conversationContext *send_conversation();
    Waitfor_conversationContext *waitfor_conversation();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Conversation_statementContext* conversation_statement();

  class  Message_statementContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::IdContext *message_type_name = nullptr;
    TSqlParser::IdContext *owner_name = nullptr;
    TSqlParser::IdContext *schema_collection_name = nullptr;
    Message_statementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CREATE();
    antlr4::tree::TerminalNode *MESSAGE();
    antlr4::tree::TerminalNode *TYPE();
    std::vector<IdContext *> id();
    IdContext* id(size_t i);
    antlr4::tree::TerminalNode *VALIDATION();
    antlr4::tree::TerminalNode *EQUAL();
    antlr4::tree::TerminalNode *AUTHORIZATION();
    antlr4::tree::TerminalNode *NONE();
    antlr4::tree::TerminalNode *EMPTY();
    antlr4::tree::TerminalNode *WELL_FORMED_XML();
    antlr4::tree::TerminalNode *VALID_XML();
    antlr4::tree::TerminalNode *WITH();
    antlr4::tree::TerminalNode *SCHEMA();
    antlr4::tree::TerminalNode *COLLECTION();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Message_statementContext* message_statement();

  class  Merge_statementContext : public antlr4::ParserRuleContext {
  public:
    Merge_statementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *MERGE();
    Ddl_objectContext *ddl_object();
    antlr4::tree::TerminalNode *USING();
    Table_sourcesContext *table_sources();
    antlr4::tree::TerminalNode *ON();
    std::vector<Search_conditionContext *> search_condition();
    Search_conditionContext* search_condition(size_t i);
    antlr4::tree::TerminalNode *SEMI();
    With_expressionContext *with_expression();
    antlr4::tree::TerminalNode *TOP();
    antlr4::tree::TerminalNode *LR_BRACKET();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *RR_BRACKET();
    antlr4::tree::TerminalNode *INTO();
    Insert_with_table_hintsContext *insert_with_table_hints();
    As_table_aliasContext *as_table_alias();
    std::vector<antlr4::tree::TerminalNode *> WHEN();
    antlr4::tree::TerminalNode* WHEN(size_t i);
    std::vector<antlr4::tree::TerminalNode *> MATCHED();
    antlr4::tree::TerminalNode* MATCHED(size_t i);
    std::vector<antlr4::tree::TerminalNode *> THEN();
    antlr4::tree::TerminalNode* THEN(size_t i);
    std::vector<Merge_matchedContext *> merge_matched();
    Merge_matchedContext* merge_matched(size_t i);
    std::vector<antlr4::tree::TerminalNode *> NOT();
    antlr4::tree::TerminalNode* NOT(size_t i);
    Merge_not_matchedContext *merge_not_matched();
    std::vector<antlr4::tree::TerminalNode *> BY();
    antlr4::tree::TerminalNode* BY(size_t i);
    std::vector<antlr4::tree::TerminalNode *> SOURCE();
    antlr4::tree::TerminalNode* SOURCE(size_t i);
    Output_clauseContext *output_clause();
    Option_clauseContext *option_clause();
    antlr4::tree::TerminalNode *PERCENT();
    std::vector<antlr4::tree::TerminalNode *> AND();
    antlr4::tree::TerminalNode* AND(size_t i);
    antlr4::tree::TerminalNode *TARGET();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Merge_statementContext* merge_statement();

  class  Merge_matchedContext : public antlr4::ParserRuleContext {
  public:
    Merge_matchedContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *UPDATE();
    antlr4::tree::TerminalNode *SET();
    std::vector<Update_elemContext *> update_elem();
    Update_elemContext* update_elem(size_t i);
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
    For_clauseContext *for_clause();
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
    Rowset_function_limitedContext *rowset_function_limited();
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
    Rowset_function_limitedContext *rowset_function_limited();
    With_expressionContext *with_expression();
    antlr4::tree::TerminalNode *TOP();
    std::vector<antlr4::tree::TerminalNode *> LR_BRACKET();
    antlr4::tree::TerminalNode* LR_BRACKET(size_t i);
    ExpressionContext *expression();
    std::vector<antlr4::tree::TerminalNode *> RR_BRACKET();
    antlr4::tree::TerminalNode* RR_BRACKET(size_t i);
    antlr4::tree::TerminalNode *INTO();
    Insert_with_table_hintsContext *insert_with_table_hints();
    Column_name_listContext *column_name_list();
    Output_clauseContext *output_clause();
    For_clauseContext *for_clause();
    Option_clauseContext *option_clause();
    antlr4::tree::TerminalNode *SEMI();
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

  class  Receive_statementContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::IdContext *table_variable = nullptr;
    TSqlParser::Search_conditionContext *where = nullptr;
    Receive_statementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *RECEIVE();
    antlr4::tree::TerminalNode *FROM();
    Full_table_nameContext *full_table_name();
    antlr4::tree::TerminalNode *ALL();
    antlr4::tree::TerminalNode *DISTINCT();
    Top_clauseContext *top_clause();
    antlr4::tree::TerminalNode *STAR();
    antlr4::tree::TerminalNode *LR_BRACKET();
    std::vector<antlr4::tree::TerminalNode *> LOCAL_ID();
    antlr4::tree::TerminalNode* LOCAL_ID(size_t i);
    std::vector<antlr4::tree::TerminalNode *> EQUAL();
    antlr4::tree::TerminalNode* EQUAL(size_t i);
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    antlr4::tree::TerminalNode *INTO();
    antlr4::tree::TerminalNode *RR_BRACKET();
    IdContext *id();
    antlr4::tree::TerminalNode *WHERE();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    Search_conditionContext *search_condition();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Receive_statementContext* receive_statement();

  class  Select_statementContext : public antlr4::ParserRuleContext {
  public:
    Select_statementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Query_expressionContext *query_expression();
    With_expressionContext *with_expression();
    Order_by_clauseContext *order_by_clause();
    For_clauseContext *for_clause();
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
    Rowset_function_limitedContext *rowset_function_limited();
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
    For_clauseContext *for_clause();
    Option_clauseContext *option_clause();
    antlr4::tree::TerminalNode *SEMI();
    Search_condition_listContext *search_condition_list();
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
    Output_column_nameContext *output_column_name();
    ExpressionContext *expression();
    As_column_aliasContext *as_column_alias();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Output_dml_list_elemContext* output_dml_list_elem();

  class  Output_column_nameContext : public antlr4::ParserRuleContext {
  public:
    Output_column_nameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DOT();
    antlr4::tree::TerminalNode *DELETED();
    antlr4::tree::TerminalNode *INSERTED();
    Table_nameContext *table_name();
    antlr4::tree::TerminalNode *STAR();
    IdContext *id();
    antlr4::tree::TerminalNode *DOLLAR_ACTION();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Output_column_nameContext* output_column_name();

  class  Create_databaseContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::IdContext *database = nullptr;
    TSqlParser::IdContext *collation_name = nullptr;
    Create_databaseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CREATE();
    antlr4::tree::TerminalNode *DATABASE();
    std::vector<IdContext *> id();
    IdContext* id(size_t i);
    antlr4::tree::TerminalNode *CONTAINMENT();
    antlr4::tree::TerminalNode *EQUAL();
    std::vector<antlr4::tree::TerminalNode *> ON();
    antlr4::tree::TerminalNode* ON(size_t i);
    std::vector<Database_file_specContext *> database_file_spec();
    Database_file_specContext* database_file_spec(size_t i);
    antlr4::tree::TerminalNode *LOG();
    antlr4::tree::TerminalNode *COLLATE();
    antlr4::tree::TerminalNode *WITH();
    std::vector<Create_database_optionContext *> create_database_option();
    Create_database_optionContext* create_database_option(size_t i);
    antlr4::tree::TerminalNode *NONE();
    antlr4::tree::TerminalNode *PARTIAL();
    antlr4::tree::TerminalNode *PRIMARY();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Create_databaseContext* create_database();

  class  Create_indexContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Search_conditionContext *where = nullptr;
    Create_indexContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CREATE();
    antlr4::tree::TerminalNode *INDEX();
    std::vector<IdContext *> id();
    IdContext* id(size_t i);
    std::vector<antlr4::tree::TerminalNode *> ON();
    antlr4::tree::TerminalNode* ON(size_t i);
    Table_name_with_hintContext *table_name_with_hint();
    std::vector<antlr4::tree::TerminalNode *> LR_BRACKET();
    antlr4::tree::TerminalNode* LR_BRACKET(size_t i);
    Column_name_list_with_orderContext *column_name_list_with_order();
    std::vector<antlr4::tree::TerminalNode *> RR_BRACKET();
    antlr4::tree::TerminalNode* RR_BRACKET(size_t i);
    antlr4::tree::TerminalNode *UNIQUE();
    ClusteredContext *clustered();
    antlr4::tree::TerminalNode *INCLUDE();
    Column_name_listContext *column_name_list();
    antlr4::tree::TerminalNode *WHERE();
    Index_optionsContext *index_options();
    antlr4::tree::TerminalNode *SEMI();
    Search_conditionContext *search_condition();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Create_indexContext* create_index();

  class  Create_or_alter_procedureContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *proc = nullptr;
    Create_or_alter_procedureContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Func_proc_name_schemaContext *func_proc_name_schema();
    antlr4::tree::TerminalNode *AS();
    Sql_clausesContext *sql_clauses();
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
    Select_statementContext *select_statement();
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
    std::vector<Sql_clauseContext *> sql_clause();
    Sql_clauseContext* sql_clause(size_t i);
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
    std::vector<Sql_clauseContext *> sql_clause();
    Sql_clauseContext* sql_clause(size_t i);
    antlr4::tree::TerminalNode *SEMI();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Func_body_returns_scalarContext* func_body_returns_scalar();

  class  Procedure_paramContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Default_valueContext *default_val = nullptr;
    Procedure_paramContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LOCAL_ID();
    Data_typeContext *data_type();
    IdContext *id();
    antlr4::tree::TerminalNode *DOT();
    antlr4::tree::TerminalNode *AS();
    antlr4::tree::TerminalNode *VARYING();
    antlr4::tree::TerminalNode *EQUAL();
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
    std::vector<antlr4::tree::TerminalNode *> null();
    antlr4::tree::TerminalNode* null(size_t i);
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
    std::vector<IdContext *> id();
    IdContext* id(size_t i);
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

  class  Create_viewContext : public antlr4::ParserRuleContext {
  public:
    Create_viewContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CREATE();
    antlr4::tree::TerminalNode *VIEW();
    Simple_nameContext *simple_name();
    antlr4::tree::TerminalNode *AS();
    Select_statementContext *select_statement();
    antlr4::tree::TerminalNode *LR_BRACKET();
    Column_name_listContext *column_name_list();
    antlr4::tree::TerminalNode *RR_BRACKET();
    std::vector<antlr4::tree::TerminalNode *> WITH();
    antlr4::tree::TerminalNode* WITH(size_t i);
    std::vector<View_attributeContext *> view_attribute();
    View_attributeContext* view_attribute(size_t i);
    antlr4::tree::TerminalNode *CHECK();
    antlr4::tree::TerminalNode *OPTION();
    antlr4::tree::TerminalNode *SEMI();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Create_viewContext* create_view();

  class  View_attributeContext : public antlr4::ParserRuleContext {
  public:
    View_attributeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ENCRYPTION();
    antlr4::tree::TerminalNode *SCHEMABINDING();
    antlr4::tree::TerminalNode *VIEW_METADATA();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  View_attributeContext* view_attribute();

  class  Alter_tableContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::IdContext *constraint = nullptr;
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
    Column_def_table_constraintContext *column_def_table_constraint();
    antlr4::tree::TerminalNode *COLUMN();
    Column_definitionContext *column_definition();
    antlr4::tree::TerminalNode *DROP();
    IdContext *id();
    antlr4::tree::TerminalNode *CONSTRAINT();
    antlr4::tree::TerminalNode *WITH();
    antlr4::tree::TerminalNode *CHECK();
    antlr4::tree::TerminalNode *FOREIGN();
    antlr4::tree::TerminalNode *KEY();
    antlr4::tree::TerminalNode *REFERENCES();
    antlr4::tree::TerminalNode *TRIGGER();
    antlr4::tree::TerminalNode *REBUILD();
    Table_optionsContext *table_options();
    antlr4::tree::TerminalNode *AUTO();
    antlr4::tree::TerminalNode *DISABLE();
    std::vector<Column_name_listContext *> column_name_list();
    Column_name_listContext* column_name_list(size_t i);
    antlr4::tree::TerminalNode *ENABLE();
    antlr4::tree::TerminalNode *SEMI();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Alter_tableContext* alter_table();

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

  class  Target_recovery_time_optionContext : public antlr4::ParserRuleContext {
  public:
    Target_recovery_time_optionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *TARGET_RECOVERY_TIME();
    antlr4::tree::TerminalNode *EQUAL();
    antlr4::tree::TerminalNode *DECIMAL();
    antlr4::tree::TerminalNode *SECONDS();
    antlr4::tree::TerminalNode *MINUTES();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Target_recovery_time_optionContext* target_recovery_time_option();

  class  TerminationContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *seconds = nullptr;
    TerminationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ROLLBACK();
    antlr4::tree::TerminalNode *AFTER();
    antlr4::tree::TerminalNode *DECIMAL();
    antlr4::tree::TerminalNode *IMMEDIATE();
    antlr4::tree::TerminalNode *NO_WAIT();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  TerminationContext* termination();

  class  Drop_indexContext : public antlr4::ParserRuleContext {
  public:
    Drop_indexContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DROP();
    antlr4::tree::TerminalNode *INDEX();
    std::vector<Drop_relational_or_xml_or_spatial_indexContext *> drop_relational_or_xml_or_spatial_index();
    Drop_relational_or_xml_or_spatial_indexContext* drop_relational_or_xml_or_spatial_index(size_t i);
    std::vector<Drop_backward_compatible_indexContext *> drop_backward_compatible_index();
    Drop_backward_compatible_indexContext* drop_backward_compatible_index(size_t i);
    antlr4::tree::TerminalNode *IF();
    antlr4::tree::TerminalNode *EXISTS();
    antlr4::tree::TerminalNode *SEMI();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Drop_indexContext* drop_index();

  class  Drop_relational_or_xml_or_spatial_indexContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::IdContext *index_name = nullptr;
    Drop_relational_or_xml_or_spatial_indexContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ON();
    Full_table_nameContext *full_table_name();
    IdContext *id();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Drop_relational_or_xml_or_spatial_indexContext* drop_relational_or_xml_or_spatial_index();

  class  Drop_backward_compatible_indexContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::IdContext *owner_name = nullptr;
    TSqlParser::IdContext *table_or_view_name = nullptr;
    TSqlParser::IdContext *index_name = nullptr;
    Drop_backward_compatible_indexContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> DOT();
    antlr4::tree::TerminalNode* DOT(size_t i);
    std::vector<IdContext *> id();
    IdContext* id(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Drop_backward_compatible_indexContext* drop_backward_compatible_index();

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

  class  Drop_viewContext : public antlr4::ParserRuleContext {
  public:
    Drop_viewContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DROP();
    antlr4::tree::TerminalNode *VIEW();
    std::vector<Simple_nameContext *> simple_name();
    Simple_nameContext* simple_name(size_t i);
    antlr4::tree::TerminalNode *IF();
    antlr4::tree::TerminalNode *EXISTS();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    antlr4::tree::TerminalNode *SEMI();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Drop_viewContext* drop_view();

  class  Create_typeContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Simple_nameContext *name = nullptr;
    Create_typeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CREATE();
    antlr4::tree::TerminalNode *TYPE();
    Simple_nameContext *simple_name();
    antlr4::tree::TerminalNode *FROM();
    Data_typeContext *data_type();
    Default_valueContext *default_value();
    antlr4::tree::TerminalNode *AS();
    antlr4::tree::TerminalNode *TABLE();
    antlr4::tree::TerminalNode *LR_BRACKET();
    Column_def_table_constraintsContext *column_def_table_constraints();
    antlr4::tree::TerminalNode *RR_BRACKET();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Create_typeContext* create_type();

  class  Drop_typeContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Simple_nameContext *name = nullptr;
    Drop_typeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DROP();
    antlr4::tree::TerminalNode *TYPE();
    Simple_nameContext *simple_name();
    antlr4::tree::TerminalNode *IF();
    antlr4::tree::TerminalNode *EXISTS();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Drop_typeContext* drop_type();

  class  Rowset_function_limitedContext : public antlr4::ParserRuleContext {
  public:
    Rowset_function_limitedContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    OpenqueryContext *openquery();
    OpendatasourceContext *opendatasource();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Rowset_function_limitedContext* rowset_function_limited();

  class  OpenqueryContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::IdContext *linked_server = nullptr;
    antlr4::Token *query = nullptr;
    OpenqueryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OPENQUERY();
    antlr4::tree::TerminalNode *LR_BRACKET();
    antlr4::tree::TerminalNode *COMMA();
    antlr4::tree::TerminalNode *RR_BRACKET();
    IdContext *id();
    antlr4::tree::TerminalNode *STRING();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  OpenqueryContext* openquery();

  class  OpendatasourceContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *provider = nullptr;
    antlr4::Token *init = nullptr;
    TSqlParser::IdContext *database = nullptr;
    TSqlParser::IdContext *scheme = nullptr;
    TSqlParser::IdContext *table = nullptr;
    OpendatasourceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OPENDATASOURCE();
    antlr4::tree::TerminalNode *LR_BRACKET();
    antlr4::tree::TerminalNode *COMMA();
    antlr4::tree::TerminalNode *RR_BRACKET();
    std::vector<antlr4::tree::TerminalNode *> DOT();
    antlr4::tree::TerminalNode* DOT(size_t i);
    std::vector<antlr4::tree::TerminalNode *> STRING();
    antlr4::tree::TerminalNode* STRING(size_t i);
    std::vector<IdContext *> id();
    IdContext* id(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  OpendatasourceContext* opendatasource();

  class  Declare_statementContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *xml_namespace_uri = nullptr;
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
    Xml_type_definitionContext *xml_type_definition();
    antlr4::tree::TerminalNode *WITH();
    antlr4::tree::TerminalNode *XMLNAMESPACES();
    antlr4::tree::TerminalNode *LR_BRACKET();
    IdContext *id();
    antlr4::tree::TerminalNode *RR_BRACKET();
    antlr4::tree::TerminalNode *STRING();

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
    Kill_stats_jobContext *kill_stats_job();

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

  class  Kill_stats_jobContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *job_id = nullptr;
    Kill_stats_jobContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *STATS();
    antlr4::tree::TerminalNode *JOB();
    antlr4::tree::TerminalNode *DECIMAL();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Kill_stats_jobContext* kill_stats_job();

  class  Execute_statementContext : public antlr4::ParserRuleContext {
  public:
    Execute_statementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EXECUTE();
    Execute_bodyContext *execute_body();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Execute_statementContext* execute_statement();

  class  Execute_bodyContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *return_status = nullptr;
    Execute_bodyContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Func_proc_name_server_database_schemaContext *func_proc_name_server_database_schema();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *EQUAL();
    std::vector<Execute_statement_argContext *> execute_statement_arg();
    Execute_statement_argContext* execute_statement_arg(size_t i);
    antlr4::tree::TerminalNode *SEMI();
    antlr4::tree::TerminalNode *LOCAL_ID();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    antlr4::tree::TerminalNode *LR_BRACKET();
    std::vector<Execute_var_stringContext *> execute_var_string();
    Execute_var_stringContext* execute_var_string(size_t i);
    antlr4::tree::TerminalNode *RR_BRACKET();
    std::vector<antlr4::tree::TerminalNode *> PLUS();
    antlr4::tree::TerminalNode* PLUS(size_t i);
    antlr4::tree::TerminalNode *STRING();
    antlr4::tree::TerminalNode *LOGIN();
    antlr4::tree::TerminalNode *USER();
    antlr4::tree::TerminalNode *AS();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Execute_bodyContext* execute_body();

  class  Execute_statement_argContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *parameter = nullptr;
    Execute_statement_argContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DEFAULT();
    antlr4::tree::TerminalNode *null();
    antlr4::tree::TerminalNode *EQUAL();
    antlr4::tree::TerminalNode *LOCAL_ID();
    Constant_LOCAL_IDContext *constant_LOCAL_ID();
    IdContext *id();
    antlr4::tree::TerminalNode *OUTPUT();
    antlr4::tree::TerminalNode *OUT();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Execute_statement_argContext* execute_statement_arg();

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

  class  Set_statementContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::IdContext *member_name = nullptr;
    Set_statementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SET();
    antlr4::tree::TerminalNode *LOCAL_ID();
    antlr4::tree::TerminalNode *EQUAL();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *DOT();
    antlr4::tree::TerminalNode *SEMI();
    IdContext *id();
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
    IdContext *id();
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
    TSqlParser::IdContext *database = nullptr;
    Use_statementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *USE();
    IdContext *id();
    antlr4::tree::TerminalNode *SEMI();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Use_statementContext* use_statement();

  class  Setuser_statementContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *user = nullptr;
    Setuser_statementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SETUSER();
    antlr4::tree::TerminalNode *STRING();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Setuser_statementContext* setuser_statement();

  class  Reconfigure_statementContext : public antlr4::ParserRuleContext {
  public:
    Reconfigure_statementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *RECONFIGURE();
    antlr4::tree::TerminalNode *WITH();
    antlr4::tree::TerminalNode *OVERRIDE();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Reconfigure_statementContext* reconfigure_statement();

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

  class  Dbcc_clauseContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Simple_idContext *name = nullptr;
    Dbcc_clauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DBCC();
    Simple_idContext *simple_id();
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

  class  Xml_type_definitionContext : public antlr4::ParserRuleContext {
  public:
    Xml_type_definitionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *XML();
    antlr4::tree::TerminalNode *LR_BRACKET();
    Xml_schema_collectionContext *xml_schema_collection();
    antlr4::tree::TerminalNode *RR_BRACKET();
    antlr4::tree::TerminalNode *CONTENT();
    antlr4::tree::TerminalNode *DOCUMENT();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Xml_type_definitionContext* xml_type_definition();

  class  Xml_schema_collectionContext : public antlr4::ParserRuleContext {
  public:
    Xml_schema_collectionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> ID();
    antlr4::tree::TerminalNode* ID(size_t i);
    antlr4::tree::TerminalNode *DOT();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Xml_schema_collectionContext* xml_schema_collection();

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
    Materialized_column_definitionContext *materialized_column_definition();
    Table_constraintContext *table_constraint();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Column_def_table_constraintContext* column_def_table_constraint();

  class  Column_definitionContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::IdContext *constraint = nullptr;
    antlr4::Token *seed = nullptr;
    antlr4::Token *increment = nullptr;
    Column_definitionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<IdContext *> id();
    IdContext* id(size_t i);
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

  class  Materialized_column_definitionContext : public antlr4::ParserRuleContext {
  public:
    Materialized_column_definitionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdContext *id();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *COMPUTE();
    antlr4::tree::TerminalNode *AS();
    antlr4::tree::TerminalNode *MATERIALIZED();
    antlr4::tree::TerminalNode *NOT();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Materialized_column_definitionContext* materialized_column_definition();

  class  Column_constraintContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::IdContext *constraint = nullptr;
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
    IdContext *id();
    antlr4::tree::TerminalNode *PRIMARY();
    antlr4::tree::TerminalNode *KEY();
    antlr4::tree::TerminalNode *UNIQUE();
    ClusteredContext *clustered();
    Index_optionsContext *index_options();
    antlr4::tree::TerminalNode *NOT();
    antlr4::tree::TerminalNode *FOR();
    antlr4::tree::TerminalNode *REPLICATION();
    antlr4::tree::TerminalNode *FOREIGN();
    On_deleteContext *on_delete();
    On_updateContext *on_update();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Column_constraintContext* column_constraint();

  class  Table_constraintContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::IdContext *constraint = nullptr;
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
    std::vector<IdContext *> id();
    IdContext* id(size_t i);
    antlr4::tree::TerminalNode *FOREIGN();
    antlr4::tree::TerminalNode *KEY();
    antlr4::tree::TerminalNode *REFERENCES();
    Table_nameContext *table_name();
    antlr4::tree::TerminalNode *CONSTRAINT();
    std::vector<Column_name_listContext *> column_name_list();
    Column_name_listContext* column_name_list(size_t i);
    antlr4::tree::TerminalNode *PRIMARY();
    antlr4::tree::TerminalNode *UNIQUE();
    ClusteredContext *clustered();
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
    On_deleteContext *on_delete();
    On_updateContext *on_update();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Table_constraintContext* table_constraint();

  class  On_deleteContext : public antlr4::ParserRuleContext {
  public:
    On_deleteContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ON();
    antlr4::tree::TerminalNode *DELETE();
    antlr4::tree::TerminalNode *NO();
    antlr4::tree::TerminalNode *ACTION();
    antlr4::tree::TerminalNode *CASCADE();
    antlr4::tree::TerminalNode *SET();
    antlr4::tree::TerminalNode *null();
    antlr4::tree::TerminalNode *DEFAULT();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  On_deleteContext* on_delete();

  class  On_updateContext : public antlr4::ParserRuleContext {
  public:
    On_updateContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ON();
    antlr4::tree::TerminalNode *UPDATE();
    antlr4::tree::TerminalNode *NO();
    antlr4::tree::TerminalNode *ACTION();
    antlr4::tree::TerminalNode *CASCADE();
    antlr4::tree::TerminalNode *SET();
    antlr4::tree::TerminalNode *null();
    antlr4::tree::TerminalNode *DEFAULT();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  On_updateContext* on_update();

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
    std::vector<Simple_idContext *> simple_id();
    Simple_idContext* simple_id(size_t i);
    antlr4::tree::TerminalNode *EQUAL();
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
    Select_statementContext *select_statement();
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
    Select_statementContext *select_statement();
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
    std::vector<IdContext *> id();
    IdContext* id(size_t i);
    Constant_LOCAL_IDContext *constant_LOCAL_ID();
    On_offContext *on_off();
    antlr4::tree::TerminalNode *SEMI();
    antlr4::tree::TerminalNode *TRANSACTION();
    antlr4::tree::TerminalNode *ISOLATION();
    antlr4::tree::TerminalNode *LEVEL();
    antlr4::tree::TerminalNode *READ();
    antlr4::tree::TerminalNode *UNCOMMITTED();
    antlr4::tree::TerminalNode *COMMITTED();
    antlr4::tree::TerminalNode *REPEATABLE();
    antlr4::tree::TerminalNode *SNAPSHOT();
    antlr4::tree::TerminalNode *SERIALIZABLE();
    antlr4::tree::TerminalNode *DECIMAL();
    antlr4::tree::TerminalNode *IDENTITY_INSERT();
    Table_nameContext *table_name();
    antlr4::tree::TerminalNode *ANSI_NULLS();
    antlr4::tree::TerminalNode *QUOTED_IDENTIFIER();
    antlr4::tree::TerminalNode *ANSI_PADDING();
    antlr4::tree::TerminalNode *ANSI_WARNINGS();
    Modify_methodContext *modify_method();

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
    Comparison_operatorContext *comparison_operator();
    Assignment_operatorContext *assignment_operator();
    antlr4::tree::TerminalNode *COLLATE();
    IdContext *id();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ExpressionContext* expression();
  ExpressionContext* expression(int precedence);
  class  Primitive_expressionContext : public antlr4::ParserRuleContext {
  public:
    Primitive_expressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DEFAULT();
    antlr4::tree::TerminalNode *null();
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
    antlr4::tree::TerminalNode *null();
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
    With_expressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *WITH();
    std::vector<Common_table_expressionContext *> common_table_expression();
    Common_table_expressionContext* common_table_expression(size_t i);
    antlr4::tree::TerminalNode *XMLNAMESPACES();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    antlr4::tree::TerminalNode *BLOCKING_HIERARCHY();
    antlr4::tree::TerminalNode *AS();
    std::vector<antlr4::tree::TerminalNode *> LR_BRACKET();
    antlr4::tree::TerminalNode* LR_BRACKET(size_t i);
    Select_statementContext *select_statement();
    std::vector<antlr4::tree::TerminalNode *> RR_BRACKET();
    antlr4::tree::TerminalNode* RR_BRACKET(size_t i);
    Full_column_name_listContext *full_column_name_list();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  With_expressionContext* with_expression();

  class  Common_table_expressionContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::IdContext *expression_name = nullptr;
    Common_table_expressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *AS();
    std::vector<antlr4::tree::TerminalNode *> LR_BRACKET();
    antlr4::tree::TerminalNode* LR_BRACKET(size_t i);
    Select_statementContext *select_statement();
    std::vector<antlr4::tree::TerminalNode *> RR_BRACKET();
    antlr4::tree::TerminalNode* RR_BRACKET(size_t i);
    IdContext *id();
    Column_name_listContext *column_name_list();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Common_table_expressionContext* common_table_expression();

  class  Update_elemContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::IdContext *udt_column_name = nullptr;
    TSqlParser::IdContext *method_name = nullptr;
    Update_elemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
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
    std::vector<IdContext *> id();
    IdContext* id(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Update_elemContext* update_elem();

  class  Search_condition_listContext : public antlr4::ParserRuleContext {
  public:
    Search_condition_listContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Search_conditionContext *> search_condition();
    Search_conditionContext* search_condition(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Search_condition_listContext* search_condition_list();

  class  Search_conditionContext : public antlr4::ParserRuleContext {
  public:
    Search_conditionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Search_condition_andContext *> search_condition_and();
    Search_condition_andContext* search_condition_and(size_t i);
    std::vector<antlr4::tree::TerminalNode *> OR();
    antlr4::tree::TerminalNode* OR(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Search_conditionContext* search_condition();

  class  Search_condition_andContext : public antlr4::ParserRuleContext {
  public:
    Search_condition_andContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Search_condition_notContext *> search_condition_not();
    Search_condition_notContext* search_condition_not(size_t i);
    std::vector<antlr4::tree::TerminalNode *> AND();
    antlr4::tree::TerminalNode* AND(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Search_condition_andContext* search_condition_and();

  class  Search_condition_notContext : public antlr4::ParserRuleContext {
  public:
    Search_condition_notContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    PredicateContext *predicate();
    antlr4::tree::TerminalNode *NOT();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Search_condition_notContext* search_condition_not();

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
    antlr4::tree::TerminalNode *NOT();
    antlr4::tree::TerminalNode *IN();
    Expression_listContext *expression_list();
    antlr4::tree::TerminalNode *LIKE();
    antlr4::tree::TerminalNode *ESCAPE();
    antlr4::tree::TerminalNode *IS();
    Null_notnullContext *null_notnull();
    Search_conditionContext *search_condition();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  PredicateContext* predicate();

  class  Query_expressionContext : public antlr4::ParserRuleContext {
  public:
    Query_expressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Query_specificationContext *query_specification();
    antlr4::tree::TerminalNode *LR_BRACKET();
    Query_expressionContext *query_expression();
    antlr4::tree::TerminalNode *RR_BRACKET();
    std::vector<Sql_unionContext *> sql_union();
    Sql_unionContext* sql_union(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Query_expressionContext* query_expression();

  class  Sql_unionContext : public antlr4::ParserRuleContext {
  public:
    Sql_unionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *UNION();
    antlr4::tree::TerminalNode *EXCEPT();
    antlr4::tree::TerminalNode *INTERSECT();
    Query_specificationContext *query_specification();
    antlr4::tree::TerminalNode *LR_BRACKET();
    Query_expressionContext *query_expression();
    antlr4::tree::TerminalNode *RR_BRACKET();
    antlr4::tree::TerminalNode *ALL();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Sql_unionContext* sql_union();

  class  Query_specificationContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::Search_conditionContext *where = nullptr;
    TSqlParser::Search_conditionContext *having = nullptr;
    Query_specificationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SELECT();
    Select_listContext *select_list();
    Top_clauseContext *top_clause();
    antlr4::tree::TerminalNode *INTO();
    Table_nameContext *table_name();
    antlr4::tree::TerminalNode *FROM();
    Table_sourcesContext *table_sources();
    antlr4::tree::TerminalNode *WHERE();
    antlr4::tree::TerminalNode *GROUP();
    antlr4::tree::TerminalNode *BY();
    std::vector<Group_by_itemContext *> group_by_item();
    Group_by_itemContext* group_by_item(size_t i);
    antlr4::tree::TerminalNode *HAVING();
    std::vector<antlr4::tree::TerminalNode *> ALL();
    antlr4::tree::TerminalNode* ALL(size_t i);
    antlr4::tree::TerminalNode *DISTINCT();
    std::vector<Search_conditionContext *> search_condition();
    Search_conditionContext* search_condition(size_t i);
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
    Top_percentContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *PERCENT();
    antlr4::tree::TerminalNode *REAL();
    antlr4::tree::TerminalNode *FLOAT();
    antlr4::tree::TerminalNode *LR_BRACKET();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *RR_BRACKET();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Top_percentContext* top_percent();

  class  Top_countContext : public antlr4::ParserRuleContext {
  public:
    Top_countContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DECIMAL();
    antlr4::tree::TerminalNode *LR_BRACKET();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *RR_BRACKET();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Top_countContext* top_count();

  class  Order_by_clauseContext : public antlr4::ParserRuleContext {
  public:
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

  class  For_clauseContext : public antlr4::ParserRuleContext {
  public:
    For_clauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *FOR();
    antlr4::tree::TerminalNode *BROWSE();
    antlr4::tree::TerminalNode *XML();
    antlr4::tree::TerminalNode *RAW();
    antlr4::tree::TerminalNode *AUTO();
    std::vector<Xml_common_directivesContext *> xml_common_directives();
    Xml_common_directivesContext* xml_common_directives(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    antlr4::tree::TerminalNode *ELEMENTS();
    antlr4::tree::TerminalNode *XSINIL();
    antlr4::tree::TerminalNode *ABSENT();
    std::vector<antlr4::tree::TerminalNode *> LR_BRACKET();
    antlr4::tree::TerminalNode* LR_BRACKET(size_t i);
    std::vector<antlr4::tree::TerminalNode *> STRING();
    antlr4::tree::TerminalNode* STRING(size_t i);
    std::vector<antlr4::tree::TerminalNode *> RR_BRACKET();
    antlr4::tree::TerminalNode* RR_BRACKET(size_t i);
    antlr4::tree::TerminalNode *XMLDATA();
    antlr4::tree::TerminalNode *XMLSCHEMA();
    antlr4::tree::TerminalNode *EXPLICIT();
    antlr4::tree::TerminalNode *PATH();
    antlr4::tree::TerminalNode *JSON();
    antlr4::tree::TerminalNode *ROOT();
    antlr4::tree::TerminalNode *INCLUDE_NULL_VALUES();
    antlr4::tree::TerminalNode *WITHOUT_ARRAY_WRAPPER();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  For_clauseContext* for_clause();

  class  Xml_common_directivesContext : public antlr4::ParserRuleContext {
  public:
    Xml_common_directivesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *COMMA();
    antlr4::tree::TerminalNode *BINARY_BASE64();
    antlr4::tree::TerminalNode *TYPE();
    antlr4::tree::TerminalNode *ROOT();
    antlr4::tree::TerminalNode *LR_BRACKET();
    antlr4::tree::TerminalNode *STRING();
    antlr4::tree::TerminalNode *RR_BRACKET();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Xml_common_directivesContext* xml_common_directives();

  class  Order_by_expressionContext : public antlr4::ParserRuleContext {
  public:
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
    Option_clauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OPTION();
    antlr4::tree::TerminalNode *LR_BRACKET();
    std::vector<OptionContext *> option();
    OptionContext* option(size_t i);
    antlr4::tree::TerminalNode *RR_BRACKET();
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
    antlr4::tree::TerminalNode *EXPAND();
    antlr4::tree::TerminalNode *VIEWS();
    antlr4::tree::TerminalNode *FORCE();
    antlr4::tree::TerminalNode *IGNORE_NONCLUSTERED_COLUMNSTORE_INDEX();
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
    antlr4::tree::TerminalNode *null();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Optimize_for_argContext* optimize_for_arg();

  class  Select_listContext : public antlr4::ParserRuleContext {
  public:
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
    Udt_method_argumentsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LR_BRACKET();
    std::vector<Execute_var_stringContext *> execute_var_string();
    Execute_var_stringContext* execute_var_string(size_t i);
    antlr4::tree::TerminalNode *RR_BRACKET();
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

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  AsteriskContext* asterisk();

  class  Column_elemContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::IdContext *column_name = nullptr;
    Column_elemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *null();
    As_column_aliasContext *as_column_alias();
    antlr4::tree::TerminalNode *DOLLAR();
    antlr4::tree::TerminalNode *IDENTITY();
    antlr4::tree::TerminalNode *ROWGUID();
    Table_nameContext *table_name();
    antlr4::tree::TerminalNode *DOT();
    IdContext *id();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Column_elemContext* column_elem();

  class  Udt_elemContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::IdContext *udt_column_name = nullptr;
    TSqlParser::IdContext *non_static_attr = nullptr;
    TSqlParser::IdContext *static_attr = nullptr;
    Udt_elemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DOT();
    Udt_method_argumentsContext *udt_method_arguments();
    std::vector<IdContext *> id();
    IdContext* id(size_t i);
    As_column_aliasContext *as_column_alias();
    std::vector<antlr4::tree::TerminalNode *> COLON();
    antlr4::tree::TerminalNode* COLON(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Udt_elemContext* udt_elem();

  class  Expression_elemContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *eq = nullptr;
    Expression_elemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Column_aliasContext *column_alias();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *EQUAL();
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
    Expression_elemContext *expression_elem();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Select_list_elemContext* select_list_elem();

  class  Table_sourcesContext : public antlr4::ParserRuleContext {
  public:
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
    antlr4::tree::TerminalNode *RR_BRACKET();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Table_sourceContext* table_source();

  class  Table_source_item_joinedContext : public antlr4::ParserRuleContext {
  public:
    Table_source_item_joinedContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Table_source_itemContext *table_source_item();
    std::vector<Join_partContext *> join_part();
    Join_partContext* join_part(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Table_source_item_joinedContext* table_source_item_joined();

  class  Table_source_itemContext : public antlr4::ParserRuleContext {
  public:
    Table_source_itemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Table_name_with_hintContext *table_name_with_hint();
    As_table_aliasContext *as_table_alias();
    Full_table_nameContext *full_table_name();
    Rowset_functionContext *rowset_function();
    Derived_tableContext *derived_table();
    Column_alias_listContext *column_alias_list();
    Change_tableContext *change_table();
    Function_callContext *function_call();
    antlr4::tree::TerminalNode *LOCAL_ID();
    antlr4::tree::TerminalNode *DOT();
    Open_xmlContext *open_xml();
    std::vector<antlr4::tree::TerminalNode *> COLON();
    antlr4::tree::TerminalNode* COLON(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Table_source_itemContext* table_source_item();

  class  Open_xmlContext : public antlr4::ParserRuleContext {
  public:
    Open_xmlContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OPENXML();
    std::vector<antlr4::tree::TerminalNode *> LR_BRACKET();
    antlr4::tree::TerminalNode* LR_BRACKET(size_t i);
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    std::vector<antlr4::tree::TerminalNode *> RR_BRACKET();
    antlr4::tree::TerminalNode* RR_BRACKET(size_t i);
    antlr4::tree::TerminalNode *WITH();
    Schema_declarationContext *schema_declaration();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Open_xmlContext* open_xml();

  class  Schema_declarationContext : public antlr4::ParserRuleContext {
  public:
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
    antlr4::tree::TerminalNode *ID();
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
    antlr4::tree::TerminalNode *CHANGETABLE();
    antlr4::tree::TerminalNode *LR_BRACKET();
    antlr4::tree::TerminalNode *CHANGES();
    Table_nameContext *table_name();
    antlr4::tree::TerminalNode *COMMA();
    antlr4::tree::TerminalNode *RR_BRACKET();
    antlr4::tree::TerminalNode *null();
    antlr4::tree::TerminalNode *DECIMAL();
    antlr4::tree::TerminalNode *LOCAL_ID();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Change_tableContext* change_table();

  class  Join_partContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *join_type = nullptr;
    antlr4::Token *join_hint = nullptr;
    Join_partContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *JOIN();
    Table_sourceContext *table_source();
    antlr4::tree::TerminalNode *ON();
    Search_conditionContext *search_condition();
    antlr4::tree::TerminalNode *LEFT();
    antlr4::tree::TerminalNode *RIGHT();
    antlr4::tree::TerminalNode *FULL();
    antlr4::tree::TerminalNode *INNER();
    antlr4::tree::TerminalNode *OUTER();
    antlr4::tree::TerminalNode *LOOP();
    antlr4::tree::TerminalNode *HASH();
    antlr4::tree::TerminalNode *MERGE();
    antlr4::tree::TerminalNode *REMOTE();
    antlr4::tree::TerminalNode *CROSS();
    antlr4::tree::TerminalNode *APPLY();
    antlr4::tree::TerminalNode *PIVOT();
    Pivot_clauseContext *pivot_clause();
    As_table_aliasContext *as_table_alias();
    antlr4::tree::TerminalNode *UNPIVOT();
    Unpivot_clauseContext *unpivot_clause();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Join_partContext* join_part();

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
    Unpivot_clauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> LR_BRACKET();
    antlr4::tree::TerminalNode* LR_BRACKET(size_t i);
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *FOR();
    Full_column_nameContext *full_column_name();
    antlr4::tree::TerminalNode *IN();
    Full_column_name_listContext *full_column_name_list();
    std::vector<antlr4::tree::TerminalNode *> RR_BRACKET();
    antlr4::tree::TerminalNode* RR_BRACKET(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Unpivot_clauseContext* unpivot_clause();

  class  Full_column_name_listContext : public antlr4::ParserRuleContext {
  public:
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

  class  Rowset_functionContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *provider_name = nullptr;
    antlr4::Token *connectionString = nullptr;
    antlr4::Token *sql = nullptr;
    antlr4::Token *data_file = nullptr;
    Rowset_functionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OPENROWSET();
    antlr4::tree::TerminalNode *LR_BRACKET();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    antlr4::tree::TerminalNode *RR_BRACKET();
    std::vector<antlr4::tree::TerminalNode *> STRING();
    antlr4::tree::TerminalNode* STRING(size_t i);
    antlr4::tree::TerminalNode *BULK();
    std::vector<Bulk_optionContext *> bulk_option();
    Bulk_optionContext* bulk_option(size_t i);
    IdContext *id();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Rowset_functionContext* rowset_function();

  class  Bulk_optionContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *bulk_option_value = nullptr;
    Bulk_optionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdContext *id();
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

  class  CURRENT_USERContext : public Function_callContext {
  public:
    CURRENT_USERContext(Function_callContext *ctx);

    antlr4::tree::TerminalNode *CURRENT_USER();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  DATEADDContext : public Function_callContext {
  public:
    DATEADDContext(Function_callContext *ctx);

    antlr4::tree::TerminalNode *DATEADD();
    antlr4::tree::TerminalNode *LR_BRACKET();
    antlr4::tree::TerminalNode *ID();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    antlr4::tree::TerminalNode *RR_BRACKET();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  CHECKSUMContext : public Function_callContext {
  public:
    CHECKSUMContext(Function_callContext *ctx);

    antlr4::tree::TerminalNode *CHECKSUM();
    antlr4::tree::TerminalNode *LR_BRACKET();
    antlr4::tree::TerminalNode *STAR();
    antlr4::tree::TerminalNode *RR_BRACKET();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  CURRENT_TIMESTAMPContext : public Function_callContext {
  public:
    CURRENT_TIMESTAMPContext(Function_callContext *ctx);

    antlr4::tree::TerminalNode *CURRENT_TIMESTAMP();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  BINARY_CHECKSUMContext : public Function_callContext {
  public:
    BINARY_CHECKSUMContext(Function_callContext *ctx);

    antlr4::tree::TerminalNode *BINARY_CHECKSUM();
    antlr4::tree::TerminalNode *LR_BRACKET();
    antlr4::tree::TerminalNode *STAR();
    antlr4::tree::TerminalNode *RR_BRACKET();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  IFFContext : public Function_callContext {
  public:
    IFFContext(Function_callContext *ctx);

    antlr4::tree::TerminalNode *IIF();
    antlr4::tree::TerminalNode *LR_BRACKET();
    Search_conditionContext *search_condition();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    antlr4::tree::TerminalNode *RR_BRACKET();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  SYSTEM_USERContext : public Function_callContext {
  public:
    SYSTEM_USERContext(Function_callContext *ctx);

    antlr4::tree::TerminalNode *SYSTEM_USER();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  NULLIFContext : public Function_callContext {
  public:
    NULLIFContext(Function_callContext *ctx);

    antlr4::tree::TerminalNode *NULLIF();
    antlr4::tree::TerminalNode *LR_BRACKET();
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    antlr4::tree::TerminalNode *COMMA();
    antlr4::tree::TerminalNode *RR_BRACKET();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  SESSION_USERContext : public Function_callContext {
  public:
    SESSION_USERContext(Function_callContext *ctx);

    antlr4::tree::TerminalNode *SESSION_USER();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  CONVERTContext : public Function_callContext {
  public:
    CONVERTContext(Function_callContext *ctx);

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

  class  XML_DATA_TYPE_FUNCContext : public Function_callContext {
  public:
    XML_DATA_TYPE_FUNCContext(Function_callContext *ctx);

    Xml_data_type_methodsContext *xml_data_type_methods();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  COALESCEContext : public Function_callContext {
  public:
    COALESCEContext(Function_callContext *ctx);

    antlr4::tree::TerminalNode *COALESCE();
    antlr4::tree::TerminalNode *LR_BRACKET();
    Expression_listContext *expression_list();
    antlr4::tree::TerminalNode *RR_BRACKET();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  CASTContext : public Function_callContext {
  public:
    CASTContext(Function_callContext *ctx);

    antlr4::tree::TerminalNode *CAST();
    antlr4::tree::TerminalNode *LR_BRACKET();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *AS();
    Data_typeContext *data_type();
    antlr4::tree::TerminalNode *RR_BRACKET();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  MIN_ACTIVE_ROWVERSIONContext : public Function_callContext {
  public:
    MIN_ACTIVE_ROWVERSIONContext(Function_callContext *ctx);

    antlr4::tree::TerminalNode *MIN_ACTIVE_ROWVERSION();
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

  class  DATEPARTContext : public Function_callContext {
  public:
    DATEPARTContext(Function_callContext *ctx);

    antlr4::tree::TerminalNode *DATEPART();
    antlr4::tree::TerminalNode *LR_BRACKET();
    antlr4::tree::TerminalNode *ID();
    antlr4::tree::TerminalNode *COMMA();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *RR_BRACKET();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  STUFFContext : public Function_callContext {
  public:
    STUFFContext(Function_callContext *ctx);

    antlr4::tree::TerminalNode *STUFF();
    antlr4::tree::TerminalNode *LR_BRACKET();
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    std::vector<antlr4::tree::TerminalNode *> DECIMAL();
    antlr4::tree::TerminalNode* DECIMAL(size_t i);
    antlr4::tree::TerminalNode *RR_BRACKET();
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

  class  IDENTITYContext : public Function_callContext {
  public:
    IDENTITYContext(Function_callContext *ctx);

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

  class  RANKING_WINDOWED_FUNCContext : public Function_callContext {
  public:
    RANKING_WINDOWED_FUNCContext(Function_callContext *ctx);

    Ranking_windowed_functionContext *ranking_windowed_function();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  DATENAMEContext : public Function_callContext {
  public:
    DATENAMEContext(Function_callContext *ctx);

    antlr4::tree::TerminalNode *DATENAME();
    antlr4::tree::TerminalNode *LR_BRACKET();
    antlr4::tree::TerminalNode *ID();
    antlr4::tree::TerminalNode *COMMA();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *RR_BRACKET();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  GETUTCDATEContext : public Function_callContext {
  public:
    GETUTCDATEContext(Function_callContext *ctx);

    antlr4::tree::TerminalNode *GETUTCDATE();
    antlr4::tree::TerminalNode *LR_BRACKET();
    antlr4::tree::TerminalNode *RR_BRACKET();
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

  class  ISNULLContext : public Function_callContext {
  public:
    ISNULLContext(Function_callContext *ctx);

    antlr4::tree::TerminalNode *ISNULL();
    antlr4::tree::TerminalNode *LR_BRACKET();
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    antlr4::tree::TerminalNode *COMMA();
    antlr4::tree::TerminalNode *RR_BRACKET();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  DATEDIFFContext : public Function_callContext {
  public:
    DATEDIFFContext(Function_callContext *ctx);

    antlr4::tree::TerminalNode *DATEDIFF();
    antlr4::tree::TerminalNode *LR_BRACKET();
    antlr4::tree::TerminalNode *ID();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    antlr4::tree::TerminalNode *RR_BRACKET();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  GETDATEContext : public Function_callContext {
  public:
    GETDATEContext(Function_callContext *ctx);

    antlr4::tree::TerminalNode *GETDATE();
    antlr4::tree::TerminalNode *LR_BRACKET();
    antlr4::tree::TerminalNode *RR_BRACKET();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  STRINGAGGContext : public Function_callContext {
  public:
    STRINGAGGContext(Function_callContext *ctx);

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

  Function_callContext* function_call();

  class  Xml_data_type_methodsContext : public antlr4::ParserRuleContext {
  public:
    Xml_data_type_methodsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Value_methodContext *value_method();
    Query_methodContext *query_method();
    Exist_methodContext *exist_method();
    Modify_methodContext *modify_method();
    Nodes_methodContext *nodes_method();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Xml_data_type_methodsContext* xml_data_type_methods();

  class  Value_methodContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *xquery = nullptr;
    antlr4::Token *sqltype = nullptr;
    Value_methodContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> DOT();
    antlr4::tree::TerminalNode* DOT(size_t i);
    antlr4::tree::TerminalNode *VALUE();
    antlr4::tree::TerminalNode *LR_BRACKET();
    antlr4::tree::TerminalNode *COMMA();
    antlr4::tree::TerminalNode *RR_BRACKET();
    std::vector<antlr4::tree::TerminalNode *> STRING();
    antlr4::tree::TerminalNode* STRING(size_t i);
    antlr4::tree::TerminalNode *LOCAL_ID();
    antlr4::tree::TerminalNode *ID();
    antlr4::tree::TerminalNode *EVENTDATA();
    Query_methodContext *query_method();
    antlr4::tree::TerminalNode *ROW();
    antlr4::tree::TerminalNode *PARAM_NODE();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Value_methodContext* value_method();

  class  Query_methodContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *xquery = nullptr;
    Query_methodContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> DOT();
    antlr4::tree::TerminalNode* DOT(size_t i);
    antlr4::tree::TerminalNode *QUERY();
    antlr4::tree::TerminalNode *LR_BRACKET();
    antlr4::tree::TerminalNode *RR_BRACKET();
    antlr4::tree::TerminalNode *STRING();
    antlr4::tree::TerminalNode *LOCAL_ID();
    antlr4::tree::TerminalNode *ID();
    Full_table_nameContext *full_table_name();
    antlr4::tree::TerminalNode *ROW();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Query_methodContext* query_method();

  class  Exist_methodContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *xquery = nullptr;
    Exist_methodContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DOT();
    antlr4::tree::TerminalNode *EXIST();
    antlr4::tree::TerminalNode *LR_BRACKET();
    antlr4::tree::TerminalNode *RR_BRACKET();
    antlr4::tree::TerminalNode *LOCAL_ID();
    antlr4::tree::TerminalNode *ID();
    antlr4::tree::TerminalNode *STRING();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Exist_methodContext* exist_method();

  class  Modify_methodContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *xml_dml = nullptr;
    Modify_methodContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DOT();
    antlr4::tree::TerminalNode *MODIFY();
    antlr4::tree::TerminalNode *LR_BRACKET();
    antlr4::tree::TerminalNode *RR_BRACKET();
    antlr4::tree::TerminalNode *LOCAL_ID();
    antlr4::tree::TerminalNode *ID();
    antlr4::tree::TerminalNode *STRING();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Modify_methodContext* modify_method();

  class  Nodes_methodContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *xquery = nullptr;
    Nodes_methodContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DOT();
    antlr4::tree::TerminalNode *NODES();
    antlr4::tree::TerminalNode *LR_BRACKET();
    antlr4::tree::TerminalNode *RR_BRACKET();
    antlr4::tree::TerminalNode *LOCAL_ID();
    antlr4::tree::TerminalNode *ID();
    antlr4::tree::TerminalNode *STRING();

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
    IdContext *id();
    With_table_hintsContext *with_table_hints();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Table_aliasContext* table_alias();

  class  With_table_hintsContext : public antlr4::ParserRuleContext {
  public:
    With_table_hintsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LR_BRACKET();
    std::vector<Table_hintContext *> table_hint();
    Table_hintContext* table_hint(size_t i);
    antlr4::tree::TerminalNode *RR_BRACKET();
    antlr4::tree::TerminalNode *WITH();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  With_table_hintsContext* with_table_hints();

  class  Insert_with_table_hintsContext : public antlr4::ParserRuleContext {
  public:
    Insert_with_table_hintsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *WITH();
    antlr4::tree::TerminalNode *LR_BRACKET();
    std::vector<Table_hintContext *> table_hint();
    Table_hintContext* table_hint(size_t i);
    antlr4::tree::TerminalNode *RR_BRACKET();
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
    IdContext *id();
    antlr4::tree::TerminalNode *DECIMAL();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Index_valueContext* index_value();

  class  Column_alias_listContext : public antlr4::ParserRuleContext {
  public:
    Column_alias_listContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LR_BRACKET();
    std::vector<Column_aliasContext *> column_alias();
    Column_aliasContext* column_alias(size_t i);
    antlr4::tree::TerminalNode *RR_BRACKET();
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
    IdContext *id();
    antlr4::tree::TerminalNode *STRING();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Column_aliasContext* column_alias();

  class  Table_value_constructorContext : public antlr4::ParserRuleContext {
  public:
    Table_value_constructorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *VALUES();
    std::vector<antlr4::tree::TerminalNode *> LR_BRACKET();
    antlr4::tree::TerminalNode* LR_BRACKET(size_t i);
    std::vector<Expression_listContext *> expression_list();
    Expression_listContext* expression_list(size_t i);
    std::vector<antlr4::tree::TerminalNode *> RR_BRACKET();
    antlr4::tree::TerminalNode* RR_BRACKET(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Table_value_constructorContext* table_value_constructor();

  class  Expression_listContext : public antlr4::ParserRuleContext {
  public:
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
    antlr4::tree::TerminalNode *LR_BRACKET();
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    antlr4::tree::TerminalNode *RR_BRACKET();
    Over_clauseContext *over_clause();
    antlr4::tree::TerminalNode *FIRST_VALUE();
    antlr4::tree::TerminalNode *LAST_VALUE();
    antlr4::tree::TerminalNode *LAG();
    antlr4::tree::TerminalNode *LEAD();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

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

  class  Create_database_optionContext : public antlr4::ParserRuleContext {
  public:
    Create_database_optionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *FILESTREAM();
    std::vector<Database_filestream_optionContext *> database_filestream_option();
    Database_filestream_optionContext* database_filestream_option(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    antlr4::tree::TerminalNode *DEFAULT_LANGUAGE();
    antlr4::tree::TerminalNode *EQUAL();
    IdContext *id();
    antlr4::tree::TerminalNode *STRING();
    antlr4::tree::TerminalNode *DEFAULT_FULLTEXT_LANGUAGE();
    antlr4::tree::TerminalNode *NESTED_TRIGGERS();
    antlr4::tree::TerminalNode *OFF();
    antlr4::tree::TerminalNode *ON();
    antlr4::tree::TerminalNode *TRANSFORM_NOISE_WORDS();
    antlr4::tree::TerminalNode *TWO_DIGIT_YEAR_CUTOFF();
    antlr4::tree::TerminalNode *DECIMAL();
    antlr4::tree::TerminalNode *DB_CHAINING();
    antlr4::tree::TerminalNode *TRUSTWORTHY();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Create_database_optionContext* create_database_option();

  class  Database_filestream_optionContext : public antlr4::ParserRuleContext {
  public:
    Database_filestream_optionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LR_BRACKET();
    antlr4::tree::TerminalNode *RR_BRACKET();
    antlr4::tree::TerminalNode *NON_TRANSACTED_ACCESS();
    antlr4::tree::TerminalNode *EQUAL();
    antlr4::tree::TerminalNode *DIRECTORY_NAME();
    antlr4::tree::TerminalNode *STRING();
    antlr4::tree::TerminalNode *OFF();
    antlr4::tree::TerminalNode *READ_ONLY();
    antlr4::tree::TerminalNode *FULL();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Database_filestream_optionContext* database_filestream_option();

  class  Database_file_specContext : public antlr4::ParserRuleContext {
  public:
    Database_file_specContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    File_groupContext *file_group();
    File_specContext *file_spec();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Database_file_specContext* database_file_spec();

  class  File_groupContext : public antlr4::ParserRuleContext {
  public:
    File_groupContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *FILEGROUP();
    IdContext *id();
    std::vector<File_specContext *> file_spec();
    File_specContext* file_spec(size_t i);
    std::vector<antlr4::tree::TerminalNode *> CONTAINS();
    antlr4::tree::TerminalNode* CONTAINS(size_t i);
    antlr4::tree::TerminalNode *FILESTREAM();
    antlr4::tree::TerminalNode *DEFAULT();
    antlr4::tree::TerminalNode *MEMORY_OPTIMIZED_DATA();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  File_groupContext* file_group();

  class  File_specContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *file = nullptr;
    File_specContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LR_BRACKET();
    antlr4::tree::TerminalNode *NAME();
    std::vector<antlr4::tree::TerminalNode *> EQUAL();
    antlr4::tree::TerminalNode* EQUAL(size_t i);
    antlr4::tree::TerminalNode *FILENAME();
    antlr4::tree::TerminalNode *RR_BRACKET();
    std::vector<antlr4::tree::TerminalNode *> STRING();
    antlr4::tree::TerminalNode* STRING(size_t i);
    IdContext *id();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    antlr4::tree::TerminalNode *SIZE();
    std::vector<File_sizeContext *> file_size();
    File_sizeContext* file_size(size_t i);
    antlr4::tree::TerminalNode *MAXSIZE();
    antlr4::tree::TerminalNode *FILEGROWTH();
    antlr4::tree::TerminalNode *UNLIMITED();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  File_specContext* file_spec();

  class  Entity_nameContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::IdContext *server = nullptr;
    TSqlParser::IdContext *database = nullptr;
    TSqlParser::IdContext *schema = nullptr;
    TSqlParser::IdContext *table = nullptr;
    Entity_nameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<IdContext *> id();
    IdContext* id(size_t i);
    std::vector<antlr4::tree::TerminalNode *> DOT();
    antlr4::tree::TerminalNode* DOT(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Entity_nameContext* entity_name();

  class  Entity_name_for_azure_dwContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::IdContext *schema = nullptr;
    TSqlParser::IdContext *object_name = nullptr;
    Entity_name_for_azure_dwContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<IdContext *> id();
    IdContext* id(size_t i);
    antlr4::tree::TerminalNode *DOT();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Entity_name_for_azure_dwContext* entity_name_for_azure_dw();

  class  Entity_name_for_parallel_dwContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::IdContext *schema_database = nullptr;
    TSqlParser::IdContext *schema = nullptr;
    TSqlParser::IdContext *object_name = nullptr;
    Entity_name_for_parallel_dwContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<IdContext *> id();
    IdContext* id(size_t i);
    antlr4::tree::TerminalNode *DOT();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Entity_name_for_parallel_dwContext* entity_name_for_parallel_dw();

  class  Full_table_nameContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::IdContext *server = nullptr;
    TSqlParser::IdContext *database = nullptr;
    TSqlParser::IdContext *schema = nullptr;
    TSqlParser::IdContext *table = nullptr;
    Full_table_nameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<IdContext *> id();
    IdContext* id(size_t i);
    std::vector<antlr4::tree::TerminalNode *> DOT();
    antlr4::tree::TerminalNode* DOT(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Full_table_nameContext* full_table_name();

  class  Table_nameContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::IdContext *database = nullptr;
    TSqlParser::IdContext *schema = nullptr;
    TSqlParser::IdContext *table = nullptr;
    Table_nameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<IdContext *> id();
    IdContext* id(size_t i);
    std::vector<antlr4::tree::TerminalNode *> DOT();
    antlr4::tree::TerminalNode* DOT(size_t i);
    antlr4::tree::TerminalNode *BLOCKING_HIERARCHY();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Table_nameContext* table_name();

  class  Simple_nameContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::IdContext *schema = nullptr;
    TSqlParser::IdContext *name = nullptr;
    Simple_nameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<IdContext *> id();
    IdContext* id(size_t i);
    antlr4::tree::TerminalNode *DOT();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Simple_nameContext* simple_name();

  class  Func_proc_name_schemaContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::IdContext *schema = nullptr;
    TSqlParser::IdContext *procedure = nullptr;
    Func_proc_name_schemaContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<IdContext *> id();
    IdContext* id(size_t i);
    antlr4::tree::TerminalNode *DOT();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Func_proc_name_schemaContext* func_proc_name_schema();

  class  Func_proc_name_database_schemaContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::IdContext *database = nullptr;
    TSqlParser::IdContext *schema = nullptr;
    TSqlParser::IdContext *procedure = nullptr;
    Func_proc_name_database_schemaContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Func_proc_name_schemaContext *func_proc_name_schema();
    std::vector<IdContext *> id();
    IdContext* id(size_t i);
    std::vector<antlr4::tree::TerminalNode *> DOT();
    antlr4::tree::TerminalNode* DOT(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Func_proc_name_database_schemaContext* func_proc_name_database_schema();

  class  Func_proc_name_server_database_schemaContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::IdContext *server = nullptr;
    TSqlParser::IdContext *database = nullptr;
    TSqlParser::IdContext *schema = nullptr;
    TSqlParser::IdContext *procedure = nullptr;
    Func_proc_name_server_database_schemaContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Func_proc_name_database_schemaContext *func_proc_name_database_schema();
    std::vector<IdContext *> id();
    IdContext* id(size_t i);
    std::vector<antlr4::tree::TerminalNode *> DOT();
    antlr4::tree::TerminalNode* DOT(size_t i);

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
    TSqlParser::IdContext *column_name = nullptr;
    Full_column_nameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdContext *id();
    Table_nameContext *table_name();
    antlr4::tree::TerminalNode *DOT();
    antlr4::tree::TerminalNode *COMPATIBILITY_LEVEL();
    antlr4::tree::TerminalNode *STATUS();
    antlr4::tree::TerminalNode *QUOTED_IDENTIFIER();
    antlr4::tree::TerminalNode *ARITHABORT();
    antlr4::tree::TerminalNode *ANSI_WARNINGS();
    antlr4::tree::TerminalNode *ANSI_PADDING();
    antlr4::tree::TerminalNode *ANSI_NULLS();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Full_column_nameContext* full_column_name();

  class  Column_name_list_with_orderContext : public antlr4::ParserRuleContext {
  public:
    Column_name_list_with_orderContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<IdContext *> id();
    IdContext* id(size_t i);
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

  class  Column_name_listContext : public antlr4::ParserRuleContext {
  public:
    Column_name_listContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<IdContext *> id();
    IdContext* id(size_t i);
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
    IdContext *id();
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

  class  ClusteredContext : public antlr4::ParserRuleContext {
  public:
    ClusteredContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CLUSTERED();
    antlr4::tree::TerminalNode *NONCLUSTERED();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ClusteredContext* clustered();

  class  Null_notnullContext : public antlr4::ParserRuleContext {
  public:
    Null_notnullContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *null();
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

  class  Begin_conversation_timerContext : public antlr4::ParserRuleContext {
  public:
    Begin_conversation_timerContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *BEGIN();
    antlr4::tree::TerminalNode *CONVERSATION();
    antlr4::tree::TerminalNode *TIMER();
    antlr4::tree::TerminalNode *LR_BRACKET();
    antlr4::tree::TerminalNode *LOCAL_ID();
    antlr4::tree::TerminalNode *RR_BRACKET();
    antlr4::tree::TerminalNode *TIMEOUT();
    antlr4::tree::TerminalNode *EQUAL();
    TimeContext *time();
    antlr4::tree::TerminalNode *SEMI();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Begin_conversation_timerContext* begin_conversation_timer();

  class  Begin_conversation_dialogContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *dialog_handle = nullptr;
    TSqlParser::Service_nameContext *initiator_service_name = nullptr;
    TSqlParser::Service_nameContext *target_service_name = nullptr;
    antlr4::Token *service_broker_guid = nullptr;
    Begin_conversation_dialogContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *BEGIN();
    antlr4::tree::TerminalNode *DIALOG();
    antlr4::tree::TerminalNode *FROM();
    std::vector<antlr4::tree::TerminalNode *> SERVICE();
    antlr4::tree::TerminalNode* SERVICE(size_t i);
    antlr4::tree::TerminalNode *TO();
    std::vector<antlr4::tree::TerminalNode *> ON();
    antlr4::tree::TerminalNode* ON(size_t i);
    antlr4::tree::TerminalNode *CONTRACT();
    Contract_nameContext *contract_name();
    std::vector<antlr4::tree::TerminalNode *> LOCAL_ID();
    antlr4::tree::TerminalNode* LOCAL_ID(size_t i);
    std::vector<Service_nameContext *> service_name();
    Service_nameContext* service_name(size_t i);
    antlr4::tree::TerminalNode *CONVERSATION();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    antlr4::tree::TerminalNode *WITH();
    antlr4::tree::TerminalNode *SEMI();
    antlr4::tree::TerminalNode *STRING();
    std::vector<antlr4::tree::TerminalNode *> EQUAL();
    antlr4::tree::TerminalNode* EQUAL(size_t i);
    antlr4::tree::TerminalNode *LIFETIME();
    antlr4::tree::TerminalNode *ENCRYPTION();
    antlr4::tree::TerminalNode *RELATED_CONVERSATION();
    antlr4::tree::TerminalNode *RELATED_CONVERSATION_GROUP();
    antlr4::tree::TerminalNode *DECIMAL();
    antlr4::tree::TerminalNode *OFF();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Begin_conversation_dialogContext* begin_conversation_dialog();

  class  Contract_nameContext : public antlr4::ParserRuleContext {
  public:
    Contract_nameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdContext *id();
    ExpressionContext *expression();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Contract_nameContext* contract_name();

  class  Service_nameContext : public antlr4::ParserRuleContext {
  public:
    Service_nameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdContext *id();
    ExpressionContext *expression();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Service_nameContext* service_name();

  class  End_conversationContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *conversation_handle = nullptr;
    antlr4::Token *faliure_code = nullptr;
    antlr4::Token *failure_text = nullptr;
    End_conversationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *END();
    antlr4::tree::TerminalNode *CONVERSATION();
    std::vector<antlr4::tree::TerminalNode *> LOCAL_ID();
    antlr4::tree::TerminalNode* LOCAL_ID(size_t i);
    antlr4::tree::TerminalNode *SEMI();
    antlr4::tree::TerminalNode *WITH();
    antlr4::tree::TerminalNode *ERROR();
    std::vector<antlr4::tree::TerminalNode *> EQUAL();
    antlr4::tree::TerminalNode* EQUAL(size_t i);
    antlr4::tree::TerminalNode *DESCRIPTION();
    antlr4::tree::TerminalNode *CLEANUP();
    std::vector<antlr4::tree::TerminalNode *> STRING();
    antlr4::tree::TerminalNode* STRING(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  End_conversationContext* end_conversation();

  class  Waitfor_conversationContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::TimeContext *timeout = nullptr;
    Waitfor_conversationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LR_BRACKET();
    Get_conversationContext *get_conversation();
    antlr4::tree::TerminalNode *RR_BRACKET();
    antlr4::tree::TerminalNode *WAITFOR();
    antlr4::tree::TerminalNode *TIMEOUT();
    antlr4::tree::TerminalNode *SEMI();
    TimeContext *time();
    antlr4::tree::TerminalNode *COMMA();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Waitfor_conversationContext* waitfor_conversation();

  class  Get_conversationContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *conversation_group_id = nullptr;
    TSqlParser::Queue_idContext *queue = nullptr;
    Get_conversationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *GET();
    antlr4::tree::TerminalNode *CONVERSATION();
    antlr4::tree::TerminalNode *GROUP();
    antlr4::tree::TerminalNode *FROM();
    Queue_idContext *queue_id();
    antlr4::tree::TerminalNode *STRING();
    antlr4::tree::TerminalNode *LOCAL_ID();
    antlr4::tree::TerminalNode *SEMI();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Get_conversationContext* get_conversation();

  class  Queue_idContext : public antlr4::ParserRuleContext {
  public:
    TSqlParser::IdContext *database_name = nullptr;
    TSqlParser::IdContext *schema_name = nullptr;
    TSqlParser::IdContext *name = nullptr;
    Queue_idContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> DOT();
    antlr4::tree::TerminalNode* DOT(size_t i);
    std::vector<IdContext *> id();
    IdContext* id(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Queue_idContext* queue_id();

  class  Send_conversationContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *conversation_handle = nullptr;
    TSqlParser::ExpressionContext *message_type_name = nullptr;
    antlr4::Token *message_body_expression = nullptr;
    Send_conversationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SEND();
    antlr4::tree::TerminalNode *ON();
    antlr4::tree::TerminalNode *CONVERSATION();
    antlr4::tree::TerminalNode *MESSAGE();
    antlr4::tree::TerminalNode *TYPE();
    ExpressionContext *expression();
    std::vector<antlr4::tree::TerminalNode *> STRING();
    antlr4::tree::TerminalNode* STRING(size_t i);
    std::vector<antlr4::tree::TerminalNode *> LOCAL_ID();
    antlr4::tree::TerminalNode* LOCAL_ID(size_t i);
    antlr4::tree::TerminalNode *LR_BRACKET();
    antlr4::tree::TerminalNode *RR_BRACKET();
    antlr4::tree::TerminalNode *SEMI();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Send_conversationContext* send_conversation();

  class  Data_typeContext : public antlr4::ParserRuleContext {
  public:
    Data_typeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdContext *id();
    antlr4::tree::TerminalNode *IDENTITY();
    antlr4::tree::TerminalNode *LR_BRACKET();
    antlr4::tree::TerminalNode *RR_BRACKET();
    std::vector<antlr4::tree::TerminalNode *> DECIMAL();
    antlr4::tree::TerminalNode* DECIMAL(size_t i);
    antlr4::tree::TerminalNode *MAX();
    antlr4::tree::TerminalNode *COMMA();
    antlr4::tree::TerminalNode *DOUBLE();
    antlr4::tree::TerminalNode *PRECISION();
    antlr4::tree::TerminalNode *INT();
    antlr4::tree::TerminalNode *TINYINT();
    antlr4::tree::TerminalNode *SMALLINT();
    antlr4::tree::TerminalNode *BIGINT();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Data_typeContext* data_type();

  class  Default_valueContext : public antlr4::ParserRuleContext {
  public:
    Default_valueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *null();
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

  class  IdContext : public antlr4::ParserRuleContext {
  public:
    IdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Simple_idContext *simple_id();
    antlr4::tree::TerminalNode *DOUBLE_QUOTE_ID();
    antlr4::tree::TerminalNode *SQUARE_BRACKET_ID();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  IdContext* id();

  class  Simple_idContext : public antlr4::ParserRuleContext {
  public:
    Simple_idContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ID();
    antlr4::tree::TerminalNode *ABSOLUTE();
    antlr4::tree::TerminalNode *ACCENT_SENSITIVITY();
    antlr4::tree::TerminalNode *ACTION();
    antlr4::tree::TerminalNode *ACTIVATION();
    antlr4::tree::TerminalNode *ACTIVE();
    antlr4::tree::TerminalNode *ADDRESS();
    antlr4::tree::TerminalNode *AES_128();
    antlr4::tree::TerminalNode *AES_192();
    antlr4::tree::TerminalNode *AES_256();
    antlr4::tree::TerminalNode *AFFINITY();
    antlr4::tree::TerminalNode *AFTER();
    antlr4::tree::TerminalNode *AGGREGATE();
    antlr4::tree::TerminalNode *ALGORITHM();
    antlr4::tree::TerminalNode *ALLOW_ENCRYPTED_VALUE_MODIFICATIONS();
    antlr4::tree::TerminalNode *ALLOW_SNAPSHOT_ISOLATION();
    antlr4::tree::TerminalNode *ALLOWED();
    antlr4::tree::TerminalNode *ANSI_NULL_DEFAULT();
    antlr4::tree::TerminalNode *ANSI_NULLS();
    antlr4::tree::TerminalNode *ANSI_PADDING();
    antlr4::tree::TerminalNode *ANSI_WARNINGS();
    antlr4::tree::TerminalNode *APPLICATION_LOG();
    antlr4::tree::TerminalNode *APPLY();
    antlr4::tree::TerminalNode *ARITHABORT();
    antlr4::tree::TerminalNode *ASSEMBLY();
    antlr4::tree::TerminalNode *AUDIT();
    antlr4::tree::TerminalNode *AUDIT_GUID();
    antlr4::tree::TerminalNode *AUTO();
    antlr4::tree::TerminalNode *AUTO_CLEANUP();
    antlr4::tree::TerminalNode *AUTO_CLOSE();
    antlr4::tree::TerminalNode *AUTO_CREATE_STATISTICS();
    antlr4::tree::TerminalNode *AUTO_SHRINK();
    antlr4::tree::TerminalNode *AUTO_UPDATE_STATISTICS();
    antlr4::tree::TerminalNode *AUTO_UPDATE_STATISTICS_ASYNC();
    antlr4::tree::TerminalNode *AVAILABILITY();
    antlr4::tree::TerminalNode *AVG();
    antlr4::tree::TerminalNode *BACKUP_PRIORITY();
    antlr4::tree::TerminalNode *BEGIN_DIALOG();
    antlr4::tree::TerminalNode *BIGINT();
    antlr4::tree::TerminalNode *BINARY_BASE64();
    antlr4::tree::TerminalNode *BINARY_CHECKSUM();
    antlr4::tree::TerminalNode *BINDING();
    antlr4::tree::TerminalNode *BLOB_STORAGE();
    antlr4::tree::TerminalNode *BROKER();
    antlr4::tree::TerminalNode *BROKER_INSTANCE();
    antlr4::tree::TerminalNode *BULK_LOGGED();
    antlr4::tree::TerminalNode *CALLED();
    antlr4::tree::TerminalNode *CALLER();
    antlr4::tree::TerminalNode *CAP_CPU_PERCENT();
    antlr4::tree::TerminalNode *CAST();
    antlr4::tree::TerminalNode *CATALOG();
    antlr4::tree::TerminalNode *CATCH();
    antlr4::tree::TerminalNode *CHANGE_RETENTION();
    antlr4::tree::TerminalNode *CHANGE_TRACKING();
    antlr4::tree::TerminalNode *CHECKSUM();
    antlr4::tree::TerminalNode *CHECKSUM_AGG();
    antlr4::tree::TerminalNode *CLEANUP();
    antlr4::tree::TerminalNode *COLLECTION();
    antlr4::tree::TerminalNode *COLUMN_MASTER_KEY();
    antlr4::tree::TerminalNode *COMMITTED();
    antlr4::tree::TerminalNode *COMPATIBILITY_LEVEL();
    antlr4::tree::TerminalNode *CONCAT();
    antlr4::tree::TerminalNode *CONCAT_NULL_YIELDS_NULL();
    antlr4::tree::TerminalNode *CONTENT();
    antlr4::tree::TerminalNode *CONTROL();
    antlr4::tree::TerminalNode *COOKIE();
    antlr4::tree::TerminalNode *COUNT();
    antlr4::tree::TerminalNode *COUNT_BIG();
    antlr4::tree::TerminalNode *COUNTER();
    antlr4::tree::TerminalNode *CPU();
    antlr4::tree::TerminalNode *CREATE_NEW();
    antlr4::tree::TerminalNode *CREATION_DISPOSITION();
    antlr4::tree::TerminalNode *CREDENTIAL();
    antlr4::tree::TerminalNode *CRYPTOGRAPHIC();
    antlr4::tree::TerminalNode *CURSOR_CLOSE_ON_COMMIT();
    antlr4::tree::TerminalNode *CURSOR_DEFAULT();
    antlr4::tree::TerminalNode *DATA();
    antlr4::tree::TerminalNode *DATA_COMPRESSION();
    antlr4::tree::TerminalNode *DATE_CORRELATION_OPTIMIZATION();
    antlr4::tree::TerminalNode *DATEADD();
    antlr4::tree::TerminalNode *DATEDIFF();
    antlr4::tree::TerminalNode *DATENAME();
    antlr4::tree::TerminalNode *DATEPART();
    antlr4::tree::TerminalNode *DAYS();
    antlr4::tree::TerminalNode *DB_CHAINING();
    antlr4::tree::TerminalNode *DB_FAILOVER();
    antlr4::tree::TerminalNode *DECRYPTION();
    antlr4::tree::TerminalNode *DEFAULT_DOUBLE_QUOTE();
    antlr4::tree::TerminalNode *DEFAULT_FULLTEXT_LANGUAGE();
    antlr4::tree::TerminalNode *DEFAULT_LANGUAGE();
    antlr4::tree::TerminalNode *DELAY();
    antlr4::tree::TerminalNode *DELAYED_DURABILITY();
    antlr4::tree::TerminalNode *DELETED();
    antlr4::tree::TerminalNode *DENSE_RANK();
    antlr4::tree::TerminalNode *DEPENDENTS();
    antlr4::tree::TerminalNode *DES();
    antlr4::tree::TerminalNode *DESCRIPTION();
    antlr4::tree::TerminalNode *DESX();
    antlr4::tree::TerminalNode *DHCP();
    antlr4::tree::TerminalNode *DIALOG();
    antlr4::tree::TerminalNode *DIRECTORY_NAME();
    antlr4::tree::TerminalNode *DISABLE();
    antlr4::tree::TerminalNode *DISABLE_BROKER();
    antlr4::tree::TerminalNode *DISABLED();
    antlr4::tree::TerminalNode *DISK_DRIVE();
    antlr4::tree::TerminalNode *DOCUMENT();
    antlr4::tree::TerminalNode *DYNAMIC();
    antlr4::tree::TerminalNode *EMERGENCY();
    antlr4::tree::TerminalNode *EMPTY();
    antlr4::tree::TerminalNode *ENABLE();
    antlr4::tree::TerminalNode *ENABLE_BROKER();
    antlr4::tree::TerminalNode *ENCRYPTED_VALUE();
    antlr4::tree::TerminalNode *ENCRYPTION();
    antlr4::tree::TerminalNode *ENDPOINT_URL();
    antlr4::tree::TerminalNode *ERROR_BROKER_CONVERSATIONS();
    antlr4::tree::TerminalNode *EVENTDATA();
    antlr4::tree::TerminalNode *EXCLUSIVE();
    antlr4::tree::TerminalNode *EXECUTABLE();
    antlr4::tree::TerminalNode *EXIST();
    antlr4::tree::TerminalNode *EXPAND();
    antlr4::tree::TerminalNode *EXPIRY_DATE();
    antlr4::tree::TerminalNode *EXPLICIT();
    antlr4::tree::TerminalNode *FAIL_OPERATION();
    antlr4::tree::TerminalNode *FAILOVER_MODE();
    antlr4::tree::TerminalNode *FAILURE();
    antlr4::tree::TerminalNode *FAILURE_CONDITION_LEVEL();
    antlr4::tree::TerminalNode *FAST();
    antlr4::tree::TerminalNode *FAST_FORWARD();
    antlr4::tree::TerminalNode *FILEGROUP();
    antlr4::tree::TerminalNode *FILEGROWTH();
    antlr4::tree::TerminalNode *FILENAME();
    antlr4::tree::TerminalNode *FILEPATH();
    antlr4::tree::TerminalNode *FILESTREAM();
    antlr4::tree::TerminalNode *FILLFACTOR();
    antlr4::tree::TerminalNode *FILTER();
    antlr4::tree::TerminalNode *FIRST();
    antlr4::tree::TerminalNode *FIRST_VALUE();
    antlr4::tree::TerminalNode *FOLLOWING();
    antlr4::tree::TerminalNode *FORCE();
    antlr4::tree::TerminalNode *FORCE_FAILOVER_ALLOW_DATA_LOSS();
    antlr4::tree::TerminalNode *FORCED();
    antlr4::tree::TerminalNode *FORCESEEK();
    antlr4::tree::TerminalNode *FORMAT();
    antlr4::tree::TerminalNode *FORWARD_ONLY();
    antlr4::tree::TerminalNode *FULLSCAN();
    antlr4::tree::TerminalNode *FULLTEXT();
    antlr4::tree::TerminalNode *GB();
    antlr4::tree::TerminalNode *GETDATE();
    antlr4::tree::TerminalNode *GETUTCDATE();
    antlr4::tree::TerminalNode *GLOBAL();
    antlr4::tree::TerminalNode *GO();
    antlr4::tree::TerminalNode *GROUP_MAX_REQUESTS();
    antlr4::tree::TerminalNode *GROUPING();
    antlr4::tree::TerminalNode *GROUPING_ID();
    antlr4::tree::TerminalNode *HADR();
    antlr4::tree::TerminalNode *HASH();
    antlr4::tree::TerminalNode *HEALTH_CHECK_TIMEOUT();
    antlr4::tree::TerminalNode *HIGH();
    antlr4::tree::TerminalNode *HONOR_BROKER_PRIORITY();
    antlr4::tree::TerminalNode *HOURS();
    antlr4::tree::TerminalNode *IDENTITY_VALUE();
    antlr4::tree::TerminalNode *IGNORE_NONCLUSTERED_COLUMNSTORE_INDEX();
    antlr4::tree::TerminalNode *IMMEDIATE();
    antlr4::tree::TerminalNode *IMPERSONATE();
    antlr4::tree::TerminalNode *IMPORTANCE();
    antlr4::tree::TerminalNode *INCREMENTAL();
    antlr4::tree::TerminalNode *INIT();
    antlr4::tree::TerminalNode *INITIATOR();
    antlr4::tree::TerminalNode *INPUT();
    antlr4::tree::TerminalNode *INSENSITIVE();
    antlr4::tree::TerminalNode *INSERTED();
    antlr4::tree::TerminalNode *INT();
    antlr4::tree::TerminalNode *IP();
    antlr4::tree::TerminalNode *ISOLATION();
    antlr4::tree::TerminalNode *KB();
    antlr4::tree::TerminalNode *KEEP();
    antlr4::tree::TerminalNode *KEEPFIXED();
    antlr4::tree::TerminalNode *KEY();
    antlr4::tree::TerminalNode *KEY_SOURCE();
    antlr4::tree::TerminalNode *KEYS();
    antlr4::tree::TerminalNode *KEYSET();
    antlr4::tree::TerminalNode *LAG();
    antlr4::tree::TerminalNode *LAST();
    antlr4::tree::TerminalNode *LAST_VALUE();
    antlr4::tree::TerminalNode *LEAD();
    antlr4::tree::TerminalNode *LEVEL();
    antlr4::tree::TerminalNode *LIST();
    antlr4::tree::TerminalNode *LISTENER();
    antlr4::tree::TerminalNode *LISTENER_URL();
    antlr4::tree::TerminalNode *LOB_COMPACTION();
    antlr4::tree::TerminalNode *LOCAL();
    antlr4::tree::TerminalNode *LOCATION();
    antlr4::tree::TerminalNode *LOCK();
    antlr4::tree::TerminalNode *LOCK_ESCALATION();
    antlr4::tree::TerminalNode *LOGIN();
    antlr4::tree::TerminalNode *LOOP();
    antlr4::tree::TerminalNode *LOW();
    antlr4::tree::TerminalNode *MANUAL();
    antlr4::tree::TerminalNode *MARK();
    antlr4::tree::TerminalNode *MASTER();
    antlr4::tree::TerminalNode *MATERIALIZED();
    antlr4::tree::TerminalNode *MAX();
    antlr4::tree::TerminalNode *MAX_CPU_PERCENT();
    antlr4::tree::TerminalNode *MAX_DOP();
    antlr4::tree::TerminalNode *MAX_FILES();
    antlr4::tree::TerminalNode *MAX_IOPS_PER_VOLUME();
    antlr4::tree::TerminalNode *MAX_MEMORY();
    antlr4::tree::TerminalNode *MAX_MEMORY_PERCENT();
    antlr4::tree::TerminalNode *MAX_PROCESSES();
    antlr4::tree::TerminalNode *MAX_QUEUE_READERS();
    antlr4::tree::TerminalNode *MAX_ROLLOVER_FILES();
    antlr4::tree::TerminalNode *MAXDOP();
    antlr4::tree::TerminalNode *MAXRECURSION();
    antlr4::tree::TerminalNode *MAXSIZE();
    antlr4::tree::TerminalNode *MB();
    antlr4::tree::TerminalNode *MEDIUM();
    antlr4::tree::TerminalNode *MEMORY_OPTIMIZED_DATA();
    antlr4::tree::TerminalNode *MESSAGE();
    antlr4::tree::TerminalNode *MIN();
    antlr4::tree::TerminalNode *MIN_ACTIVE_ROWVERSION();
    antlr4::tree::TerminalNode *MIN_CPU_PERCENT();
    antlr4::tree::TerminalNode *MIN_IOPS_PER_VOLUME();
    antlr4::tree::TerminalNode *MIN_MEMORY_PERCENT();
    antlr4::tree::TerminalNode *MINUTES();
    antlr4::tree::TerminalNode *MIRROR_ADDRESS();
    antlr4::tree::TerminalNode *MIXED_PAGE_ALLOCATION();
    antlr4::tree::TerminalNode *MODE();
    antlr4::tree::TerminalNode *MODIFY();
    antlr4::tree::TerminalNode *MOVE();
    antlr4::tree::TerminalNode *MULTI_USER();
    antlr4::tree::TerminalNode *NAME();
    antlr4::tree::TerminalNode *NESTED_TRIGGERS();
    antlr4::tree::TerminalNode *NEW_ACCOUNT();
    antlr4::tree::TerminalNode *NEW_BROKER();
    antlr4::tree::TerminalNode *NEW_PASSWORD();
    antlr4::tree::TerminalNode *NEXT();
    antlr4::tree::TerminalNode *NO();
    antlr4::tree::TerminalNode *NO_TRUNCATE();
    antlr4::tree::TerminalNode *NO_WAIT();
    antlr4::tree::TerminalNode *NOCOUNT();
    antlr4::tree::TerminalNode *NODES();
    antlr4::tree::TerminalNode *NOEXPAND();
    antlr4::tree::TerminalNode *NON_TRANSACTED_ACCESS();
    antlr4::tree::TerminalNode *NORECOMPUTE();
    antlr4::tree::TerminalNode *NORECOVERY();
    antlr4::tree::TerminalNode *NOWAIT();
    antlr4::tree::TerminalNode *NTILE();
    antlr4::tree::TerminalNode *NUMANODE();
    antlr4::tree::TerminalNode *NUMBER();
    antlr4::tree::TerminalNode *NUMERIC_ROUNDABORT();
    antlr4::tree::TerminalNode *OBJECT();
    antlr4::tree::TerminalNode *OFFLINE();
    antlr4::tree::TerminalNode *OFFSET();
    antlr4::tree::TerminalNode *OFFSETS();
    antlr4::tree::TerminalNode *OLD_ACCOUNT();
    antlr4::tree::TerminalNode *ONLINE();
    antlr4::tree::TerminalNode *ONLY();
    antlr4::tree::TerminalNode *OPEN_EXISTING();
    antlr4::tree::TerminalNode *OPTIMISTIC();
    antlr4::tree::TerminalNode *OPTIMIZE();
    antlr4::tree::TerminalNode *OUT();
    antlr4::tree::TerminalNode *OUTPUT();
    antlr4::tree::TerminalNode *OWNER();
    antlr4::tree::TerminalNode *PAGE();
    antlr4::tree::TerminalNode *PAGE_VERIFY();
    antlr4::tree::TerminalNode *PARAMETERIZATION();
    antlr4::tree::TerminalNode *PARTITION();
    antlr4::tree::TerminalNode *PARTITIONS();
    antlr4::tree::TerminalNode *PARTNER();
    antlr4::tree::TerminalNode *PATH();
    antlr4::tree::TerminalNode *POISON_MESSAGE_HANDLING();
    antlr4::tree::TerminalNode *POOL();
    antlr4::tree::TerminalNode *PORT();
    antlr4::tree::TerminalNode *PRECEDING();
    antlr4::tree::TerminalNode *PRIMARY_ROLE();
    antlr4::tree::TerminalNode *PRIOR();
    antlr4::tree::TerminalNode *PRIORITY();
    antlr4::tree::TerminalNode *PRIORITY_LEVEL();
    antlr4::tree::TerminalNode *PRIVATE();
    antlr4::tree::TerminalNode *PRIVATE_KEY();
    antlr4::tree::TerminalNode *PRIVILEGES();
    antlr4::tree::TerminalNode *PROCEDURE_NAME();
    antlr4::tree::TerminalNode *PROPERTY();
    antlr4::tree::TerminalNode *PROVIDER();
    antlr4::tree::TerminalNode *PROVIDER_KEY_NAME();
    antlr4::tree::TerminalNode *PUBLIC();
    antlr4::tree::TerminalNode *QUERY();
    antlr4::tree::TerminalNode *QUEUE();
    antlr4::tree::TerminalNode *QUEUE_DELAY();
    antlr4::tree::TerminalNode *QUOTED_IDENTIFIER();
    antlr4::tree::TerminalNode *R();
    antlr4::tree::TerminalNode *RANGE();
    antlr4::tree::TerminalNode *RANK();
    antlr4::tree::TerminalNode *RAW();
    antlr4::tree::TerminalNode *RC2();
    antlr4::tree::TerminalNode *RC4();
    antlr4::tree::TerminalNode *RC4_128();
    antlr4::tree::TerminalNode *READ_COMMITTED_SNAPSHOT();
    antlr4::tree::TerminalNode *READ_ONLY();
    antlr4::tree::TerminalNode *READ_ONLY_ROUTING_LIST();
    antlr4::tree::TerminalNode *READ_WRITE();
    antlr4::tree::TerminalNode *READONLY();
    antlr4::tree::TerminalNode *REBUILD();
    antlr4::tree::TerminalNode *RECEIVE();
    antlr4::tree::TerminalNode *RECOMPILE();
    antlr4::tree::TerminalNode *RECOVERY();
    antlr4::tree::TerminalNode *RECURSIVE_TRIGGERS();
    antlr4::tree::TerminalNode *RELATIVE();
    antlr4::tree::TerminalNode *REMOTE();
    antlr4::tree::TerminalNode *REMOTE_SERVICE_NAME();
    antlr4::tree::TerminalNode *REMOVE();
    antlr4::tree::TerminalNode *REORGANIZE();
    antlr4::tree::TerminalNode *REPEATABLE();
    antlr4::tree::TerminalNode *REPLICA();
    antlr4::tree::TerminalNode *REQUEST_MAX_CPU_TIME_SEC();
    antlr4::tree::TerminalNode *REQUEST_MAX_MEMORY_GRANT_PERCENT();
    antlr4::tree::TerminalNode *REQUEST_MEMORY_GRANT_TIMEOUT_SEC();
    antlr4::tree::TerminalNode *REQUIRED_SYNCHRONIZED_SECONDARIES_TO_COMMIT();
    antlr4::tree::TerminalNode *RESERVE_DISK_SPACE();
    antlr4::tree::TerminalNode *RESOURCE();
    antlr4::tree::TerminalNode *RESOURCE_MANAGER_LOCATION();
    antlr4::tree::TerminalNode *RESTRICTED_USER();
    antlr4::tree::TerminalNode *RETENTION();
    antlr4::tree::TerminalNode *RETURN();
    antlr4::tree::TerminalNode *RETURNS();
    antlr4::tree::TerminalNode *ROBUST();
    antlr4::tree::TerminalNode *ROOT();
    antlr4::tree::TerminalNode *ROUTE();
    antlr4::tree::TerminalNode *ROW();
    antlr4::tree::TerminalNode *ROW_NUMBER();
    antlr4::tree::TerminalNode *ROWCOUNT();
    antlr4::tree::TerminalNode *ROWGUID();
    antlr4::tree::TerminalNode *ROWS();
    antlr4::tree::TerminalNode *SAFETY();
    antlr4::tree::TerminalNode *SAMPLE();
    antlr4::tree::TerminalNode *SCHEMABINDING();
    antlr4::tree::TerminalNode *SCOPED();
    antlr4::tree::TerminalNode *SCROLL();
    antlr4::tree::TerminalNode *SCROLL_LOCKS();
    antlr4::tree::TerminalNode *SEARCH();
    antlr4::tree::TerminalNode *SECONDARY();
    antlr4::tree::TerminalNode *SECONDARY_ONLY();
    antlr4::tree::TerminalNode *SECONDARY_ROLE();
    antlr4::tree::TerminalNode *SECONDS();
    antlr4::tree::TerminalNode *SECRET();
    antlr4::tree::TerminalNode *SECURITY();
    antlr4::tree::TerminalNode *SECURITY_LOG();
    antlr4::tree::TerminalNode *SEEDING_MODE();
    antlr4::tree::TerminalNode *SELF();
    antlr4::tree::TerminalNode *SEMI_SENSITIVE();
    antlr4::tree::TerminalNode *SEND();
    antlr4::tree::TerminalNode *SENT();
    antlr4::tree::TerminalNode *SEQUENCE();
    antlr4::tree::TerminalNode *SERIALIZABLE();
    antlr4::tree::TerminalNode *SERVER();
    antlr4::tree::TerminalNode *SESSION_TIMEOUT();
    antlr4::tree::TerminalNode *SETERROR();
    antlr4::tree::TerminalNode *SHARE();
    antlr4::tree::TerminalNode *SHOWPLAN();
    antlr4::tree::TerminalNode *SID();
    antlr4::tree::TerminalNode *SIGNATURE();
    antlr4::tree::TerminalNode *SIMPLE();
    antlr4::tree::TerminalNode *SINGLE_USER();
    antlr4::tree::TerminalNode *SIZE();
    antlr4::tree::TerminalNode *SMALLINT();
    antlr4::tree::TerminalNode *SNAPSHOT();
    antlr4::tree::TerminalNode *SOURCE();
    antlr4::tree::TerminalNode *SPATIAL_WINDOW_MAX_CELLS();
    antlr4::tree::TerminalNode *SPLIT();
    antlr4::tree::TerminalNode *STANDBY();
    antlr4::tree::TerminalNode *START();
    antlr4::tree::TerminalNode *START_DATE();
    antlr4::tree::TerminalNode *STATE();
    antlr4::tree::TerminalNode *STATIC();
    antlr4::tree::TerminalNode *STATS_STREAM();
    antlr4::tree::TerminalNode *STATUS();
    antlr4::tree::TerminalNode *STDEV();
    antlr4::tree::TerminalNode *STDEVP();
    antlr4::tree::TerminalNode *STOPLIST();
    antlr4::tree::TerminalNode *STRING_AGG();
    antlr4::tree::TerminalNode *STUFF();
    antlr4::tree::TerminalNode *SUBJECT();
    antlr4::tree::TerminalNode *SUM();
    antlr4::tree::TerminalNode *SUSPEND();
    antlr4::tree::TerminalNode *SYMMETRIC();
    antlr4::tree::TerminalNode *SYNCHRONOUS_COMMIT();
    antlr4::tree::TerminalNode *SYNONYM();
    antlr4::tree::TerminalNode *SYSTEM();
    antlr4::tree::TerminalNode *TAKE();
    antlr4::tree::TerminalNode *TARGET();
    antlr4::tree::TerminalNode *TARGET_RECOVERY_TIME();
    antlr4::tree::TerminalNode *TB();
    antlr4::tree::TerminalNode *TEXTIMAGE_ON();
    antlr4::tree::TerminalNode *THROW();
    antlr4::tree::TerminalNode *TIES();
    antlr4::tree::TerminalNode *TIME();
    antlr4::tree::TerminalNode *TIMEOUT();
    antlr4::tree::TerminalNode *TIMER();
    antlr4::tree::TerminalNode *TINYINT();
    antlr4::tree::TerminalNode *TORN_PAGE_DETECTION();
    antlr4::tree::TerminalNode *TRANSFORM_NOISE_WORDS();
    antlr4::tree::TerminalNode *TRIPLE_DES();
    antlr4::tree::TerminalNode *TRIPLE_DES_3KEY();
    antlr4::tree::TerminalNode *TRUSTWORTHY();
    antlr4::tree::TerminalNode *TRY();
    antlr4::tree::TerminalNode *TSQL();
    antlr4::tree::TerminalNode *TWO_DIGIT_YEAR_CUTOFF();
    antlr4::tree::TerminalNode *TYPE();
    antlr4::tree::TerminalNode *TYPE_WARNING();
    antlr4::tree::TerminalNode *UNBOUNDED();
    antlr4::tree::TerminalNode *UNCOMMITTED();
    antlr4::tree::TerminalNode *UNKNOWN();
    antlr4::tree::TerminalNode *UNLIMITED();
    antlr4::tree::TerminalNode *USING();
    antlr4::tree::TerminalNode *VALID_XML();
    antlr4::tree::TerminalNode *VALIDATION();
    antlr4::tree::TerminalNode *VALUE();
    antlr4::tree::TerminalNode *VAR();
    antlr4::tree::TerminalNode *VARP();
    antlr4::tree::TerminalNode *VIEW_METADATA();
    antlr4::tree::TerminalNode *VIEWS();
    antlr4::tree::TerminalNode *WAIT();
    antlr4::tree::TerminalNode *WELL_FORMED_XML();
    antlr4::tree::TerminalNode *WORK();
    antlr4::tree::TerminalNode *WORKLOAD();
    antlr4::tree::TerminalNode *XML();
    antlr4::tree::TerminalNode *XMLNAMESPACES();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  Simple_idContext* simple_id();

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

  class  File_sizeContext : public antlr4::ParserRuleContext {
  public:
    File_sizeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DECIMAL();
    antlr4::tree::TerminalNode *KB();
    antlr4::tree::TerminalNode *MB();
    antlr4::tree::TerminalNode *GB();
    antlr4::tree::TerminalNode *TB();
    antlr4::tree::TerminalNode *MODULE();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  File_sizeContext* file_size();


  virtual bool sempred(antlr4::RuleContext *_localctx, size_t ruleIndex, size_t predicateIndex) override;
  bool expressionSempred(ExpressionContext *_localctx, size_t predicateIndex);

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

