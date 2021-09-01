#ifndef QUERY_H
#define QUERY_H

#ifdef __cplusplus
extern "C" {
#else
#include <stdbool.h>
#endif

#include <stdlib.h>

/* Try to get avoid includes since this
 * is our interface to c++
 */
struct vec;
struct group;
struct logicgroup;
struct expression;
struct fql_handle;
typedef struct vec string;

enum sql_type {
	SQL_BIT,
	SQL_INT,
	SQL_FLOAT,
	SQL_VARCHAR,
	SQL_TEXT,
	SQL_CHAR,
};

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

/* The order here is important...
 * see function.c
 */
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
	SCALAR_CAST, /* */
	SCALAR_CEILING,
	SCALAR_CHAR,
	SCALAR_CHARINDEX,
	SCALAR_CHECKSUM,
	SCALAR_DATALENGTH,
	SCALAR_DAY,
	SCALAR_FLOOR,
	SCALAR_ISDATE,
	SCALAR_ISNULL,
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
	MODE_IN,
	MODE_TOP,
	MODE_CASE,
	MODE_INTO,
	MODE_UPDATE,
	MODE_SELECT,
	MODE_DELETE,
	MODE_SEARCH,
	MODE_SOURCES,
	MODE_GROUPBY,
	MODE_ORDERBY,
	MODE_AGGREGATE,
};

enum logic_mode {
	LOGIC_UNDEFINED,
	LOGIC_WHERE,
	LOGIC_JOIN,
	LOGIC_HAVING,
	LOGIC_CASE,
};

enum join_type {
	JOIN_FROM,
	JOIN_INNER,
	JOIN_LEFT,
	JOIN_RIGHT,
	JOIN_FULL,
	JOIN_CROSS,
};

/** query **/
struct query {
	struct fql_plan* plan;
	struct vec* sources;
	struct logicgroup* where;
	struct group* groupby;
	struct group* distinct;
	struct logicgroup* having;
	struct order* orderby;
	void* op; /* operation structure */
	struct vec* subquery_const_vec;
	struct expression* top_expr;
	struct vec* unions;
	const char* into_table_name;
	size_t top_count;
	int union_id;
	int query_id;
	int query_total;
	int expect_where; /* Boolean */

	/* all the variables below are temporaries for
	 * tracking the query as antlr traverses it
	 */
	struct vec* joinable;           /* denotes a joinable condition */
	struct node* logic_stack;       /* used to build logic groups */
	struct node* function_stack;    /* track function nesting */
	struct node* switchcase_stack;  /* track casestatement nesting */

	enum mode mode;
	enum mode mode_store;
	enum logic_mode logic_mode;
	enum join_type join;
	bool in_bracket_expression; /* boolean for whether we are in a
	                             * bracket expression or not
	                             */
};
typedef struct query query;

struct query* query_new(int id);
struct query* query_construct(struct query*, int id);
void query_destroy(struct query*);
void query_release_sources(struct query*);
void query_free(void*);

int query_add_constant(struct query*, const char*, int);
int query_add_null_expression(struct query*);
int query_add_expression(struct query*, char*, const char* table);
int query_add_asterisk(struct query*, const char* table);
int query_add_source(struct query*, struct fql_handle*, struct node**, const char*);
void query_add_subquery_source(struct query*, struct query*, const char*);
void query_apply_table_alias(struct query*, const char*);
void query_apply_expression_alias(struct query*, const char*);
int query_set_top_count(struct query*, const char*);
int query_set_into_table(struct query*, const char*);
void query_set_distinct(struct query*);
int query_enter_aggregate(struct query*, enum aggregate_function);
int query_exit_aggregate(struct query*);
int query_init_op(struct query*);
void query_set_op_table(struct query*, const char*);
void query_init_groupby(struct query*);
int query_init_orderby(struct query*);
void query_enter_in_statement(struct query*);
//void query_exit_in_statement(struct query*);
void query_assign_in_subquery(struct query*, struct query*);
void query_add_subquery_const(struct query*, struct query*);
void query_set_order_desc(struct query*);
int query_apply_data_type(struct query*, const char*);
void query_exit_non_select_op(struct query*, struct fql_handle*);

int query_enter_union(struct query*, struct query*);
int query_exit_union(struct query*, struct query*);

int query_enter_function(struct query*, enum scalar_function, int char_as_byte);
void query_exit_function(struct query*);
int query_enter_operator(struct query*, enum scalar_function op);

int query_enter_case_expression(struct query*);
int query_exit_case_expression(struct query*);
int query_enter_switch_section(struct query*);
int query_exit_switch_section(struct query*);
int query_enter_switch_search(struct query*);
int query_exit_switch_search(struct query*);

/* search building functions */
void query_set_logic_comparison(struct query*, const char*);
void query_set_logic_isnull(struct query*, bool negation);
//void query_enter_search(struct query*);
//void query_exit_search(struct query*, bool exit_and);
void query_enter_search_or(struct query*);
void query_enter_search_and(struct query*);
void query_exit_search_item(struct query*);
void query_enter_predicate(struct query*, unsigned negation, bool in);
void query_exit_predicate(struct query*, bool in, bool like);

#ifdef __cplusplus
}
#endif

#endif /* QUERY_H */
