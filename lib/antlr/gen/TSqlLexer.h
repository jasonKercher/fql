
// Generated from grammar/TSqlLexer.g4 by ANTLR 4.9.1

#pragma once


#include "antlr4-runtime.h"




class  TSqlLexer : public antlr4::Lexer {
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

  explicit TSqlLexer(antlr4::CharStream *input);
  ~TSqlLexer();

  virtual std::string getGrammarFileName() const override;
  virtual const std::vector<std::string>& getRuleNames() const override;

  virtual const std::vector<std::string>& getChannelNames() const override;
  virtual const std::vector<std::string>& getModeNames() const override;
  virtual const std::vector<std::string>& getTokenNames() const override; // deprecated, use vocabulary instead
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;

  virtual const std::vector<uint16_t> getSerializedATN() const override;
  virtual const antlr4::atn::ATN& getATN() const override;

private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;
  static std::vector<std::string> _channelNames;
  static std::vector<std::string> _modeNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};

