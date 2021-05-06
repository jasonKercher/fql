#ifndef QUERY_H
#define QUERY_H


#ifdef __cplusplus
extern "C" {
#endif

#include "fqlplan.h"
#include "table.h"
//#include "function.h"
#include "operation.h"
#include "logic.h"
#include "util/queue.h"
#include "util/vec.h"

enum aggregate_function {
	AGG_UNDEFINED,
	AGG_AVG,
	AGG_CHECKSUM_AGG,
	AGG_COUNT,
	AGG_COUNT_BIG,
	AGG_GROUPING,
	AGG_GROUPING_ID,
	//AGG_LR_BRACKET,
	AGG_MAX,
	AGG_MIN,
	//AGG_RR_BRACKET,
	//AGG_STAR,
	AGG_STDEV,
	AGG_STDEVP,
	AGG_SUM,
	AGG_VAR,
	AGG_VARP,
};

enum scalar_function {
	SCALAR_UNDEFINED = -1,
	SCALAR_OP_PLUS,
	SCALAR_OP_MINUS,
	SCALAR_OP_MULTIPY,
	SCALAR_OP_DIVIDE,
	SCALAR_OP_MODULE,
	SCALAR_OP_BIT_OR,
	SCALAR_OP_BIT_AND,
	SCALAR_OP_BIT_XOR,
	SCALAR_OP_UNARY_BIT_NOT,
	SCALAR_OP_UNARY_MINUS,
	SCALAR_ABS,
	SCALAR_ASCII,
	SCALAR_CEILING,
	SCALAR_CHAR,
	SCALAR_CHARINDEX,
	SCALAR_CHECKSUM,
	SCALAR_DATALENGTH,
	SCALAR_DAY,
	SCALAR_FLOOR,
	SCALAR_ISDATE,
	SCALAR_ISNUMERIC,
	SCALAR_LEFT,
	SCALAR_LEN,
	SCALAR_LOWER,
	SCALAR_LTRIM,
	SCALAR_MONTH,
	SCALAR_NCHAR,
	SCALAR_PATINDEX,
	SCALAR_RAND,
	SCALAR_REPLACE,
	SCALAR_RIGHT,
	SCALAR_ROUND,
	SCALAR_RTRIM,
	SCALAR_SIGN,
	SCALAR_SPACE,
	SCALAR_STR,
	SCALAR_SUBSTRING,
	SCALAR_UPPER,
	SCALAR_USER_NAME,
	SCALAR_YEAR,
};

enum mode {
	MODE_UNDEFINED,
	MODE_SELECT,
	MODE_AGGREGATE,
	MODE_UPDATE,
	MODE_SOURCES,
	MODE_SEARCH,
	MODE_GROUPBY,
};

enum logic_mode {
	LOGIC_UNDEFINED,
	LOGIC_WHERE,
	LOGIC_JOIN,
};

struct group;

/** Query **/
struct query {
	struct fql_plan* plan;
	struct vec* sources;
	struct logicgroup* where;
	struct vec* validation_list;    /* for no-source tables */
	struct group* groupby;
	struct group* distinct;
	struct queue* having;
	struct expression* limit;       /* TOP */
	void* op;                       /* Operation structure */
	int query_id;
	int query_total;

	/* All the variables below are temporaries for
	 * tracking the query as antlr traverses it
	 */
	struct stack* logic_stack;      /* used to build logic groups */
	struct vec* joinable;           /* Denotes a joinable condition */
	struct stack* function_stack;   /* used to track function nesting */

	struct expression* expr;
	enum mode mode;
	enum logic_mode logic_mode;
	enum join_type join;
};
typedef struct query Query;

struct query* query_new(int id);
struct query* query_construct(struct query*, int id);
void query_free(void*);

int query_add_constant(Query*, const char*, int);
void query_add_column(struct query*, char*, const char* table);
void query_add_asterisk(struct query*, const char* table);
void query_add_source(struct query*, struct stack**, const char*);
void query_add_subquery_source(struct query*, struct query*, const char*);
void query_apply_table_alias(struct query*, const char*);
void query_set_distinct(struct query*);
int query_add_aggregate(struct query*, enum aggregate_function);

int query_enter_function(struct query*, enum scalar_function, int);
void query_exit_function(struct query*);
void query_enter_operator(struct query*, enum scalar_function op);

/* Search building functions */
void query_set_logic_comparison(struct query*, const char*);

void enter_search(struct query*);
void exit_search(struct query*);
void enter_search_and(struct query*);
void exit_search_and(struct query*);
void enter_search_not(struct query*);
void exit_search_not(struct query*);



#ifdef __cplusplus
}
#endif

#endif /* QUERY_H */

