#ifndef QUERY_H
#define QUERY_H


#ifdef __cplusplus
extern "C" {
#endif

#include "util/queue.h"
#include "util/vec.h"
#include "table.h"
#include "expression.h"
#include "operation.h"
#include "reader.h"
#include "search.h"

enum mode {
        MODE_UNDEFINED,
        MODE_SELECT,
        MODE_UPDATE,
        MODE_SOURCES,
        MODE_SEARCH,
};

enum search_mode {
        SEARCH_UNDEFINED,
        SEARCH_WHERE,
        SEARCH_JOIN
};

/** Query **/
struct query {
        struct table* table;         /* output table */
        struct vector* sources;      /* struct source */
        struct search_tree* where;        /* struct search */
        struct queue* groups;        /* struct expression */
        struct queue* having;        /* struct expression */
        struct expression* limit;    /* TOP */
        enum oper oper;              /* type of operation */

        /* temps used when traversing */
        struct search_tree* search_tree;
        struct stack* current_search;
        struct stack* not_stack;
        struct stack* and_stack;

        struct expression* expr;
        enum mode mode;
        enum search_mode search_mode;
        enum join_type join;
};

struct query* query_new();
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

void query_add_search_column(struct query*, struct expression*, const char*);
void query_set_search_comparison(struct query*, const char*);



#ifdef __cplusplus
}
#endif

#endif /* QUERY_H */

