#ifndef QUERY_H
#define QUERY_H


#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "fqlplan.h"
#include "table.h"
#include "function.h"
#include "operation.h"
#include "logic.h"
#include "util/queue.h"
#include "util/vec.h"

enum mode {
	MODE_UNDEFINED,
	MODE_SELECT,
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

/** Query **/
struct query {
	struct fql_plan* plan;          /* execution plan */
	struct vec* sources;            /* struct source */
	struct logicgroup* where;       /* struct logicgroup */
	struct vec* groups;             /* struct expression */
	struct queue* having;           /* struct expression */
	struct expression* limit;       /* TOP */
	void* op;                       /* Operation structure */
	int query_id;			/* Query ID */
	int query_total;                /* Query Total */

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
void query_add_subquery_source(struct query*, struct query*, const char*);
void query_add_source(struct query*, struct stack**, const char*);
void query_apply_table_alias(struct query*, const char*);
//void query_select_finalize(struct query*);

void query_enter_function(struct query*, const char*);
void query_exit_function(struct query*);
void query_enter_operator(struct query*, enum expr_operator op);

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

