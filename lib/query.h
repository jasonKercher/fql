#ifndef QUERY_H
#define QUERY_H


#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "util/queue.h"
#include "util/vec.h"
#include "table.h"
#include "expression.h"
#include "operation.h"
#include "reader.h"
#include "logic.h"

enum mode {
        MODE_UNDEFINED,
        MODE_SELECT,
        MODE_UPDATE,
        MODE_SOURCES,
        MODE_SEARCH,
};

enum logic_mode {
        LOGIC_UNDEFINED,
        LOGIC_WHERE,
        LOGIC_JOIN
};

/** Query **/
struct query {
        struct schema* schema;          /* output table */
        struct vec* sources;            /* struct source */
        struct logic_tree* where;       /* struct logic */
        struct queue* groups;           /* struct expression */
        struct queue* having;           /* struct expression */
        struct expression* limit;       /* TOP */
        void* op;                       /* Operation structure */

        /* All the variables below are temporaries for
         * tracking the query as antlr traverses it
         */
        struct stack* logic_stack;      /* stack used to build logic trees */

        struct expression* expr;
        enum mode mode;
        enum logic_mode logic_mode;
        enum join_type join;
};
typedef struct query Query;

struct query* query_new();
struct query* query_init(struct query*);
void query_free(void*);

void query_add_source(struct query*, struct stack**, const char*);
void query_apply_table_alias(struct query*, const char*);

/* Search building functions */

void enter_search(struct query*);
void exit_search(struct query*);
void enter_search_and(struct query*);
void exit_search_and(struct query*);
void enter_search_not(struct query*);
void exit_search_not(struct query*);

void query_add_logic_column(struct query*, struct expression*, const char*);
void query_set_logic_comparison(struct query*, const char*);



#ifdef __cplusplus
}
#endif

#endif /* QUERY_H */

