#ifndef QUERY_H
#define QUERY_H


#ifdef __cplusplus
extern "C" {
#endif

#include "util/queue.h"
#include "table.h"
#include "expression.h"
#include "operation.h"
#include "reader.h"

enum mode {
        MODE_UNDEFINED,
        MODE_SELECT,
        MODE_UPDATE,
        MODE_SOURCES,
        MODE_SEARCH,
};

/** Query **/
struct query_t {
        table_t* table;         /* output table */
        stack_t* sources;       /* source_t */
        queue_t* conditions;    /* expression_t */
        queue_t* groups;        /* expression_t */
        queue_t* having;        /* expression_t */
        expression_t* limit;    /* TOP */
        enum oper oper;         /* type of operation */

        /* temps used when traversing */
        expression_t* expr;
        enum mode mode;
        enum join join;
};
typedef struct query_t query_t;

query_t* query_new();
void query_add_source(query_t*, stack_t*);



#ifdef __cplusplus
}
#endif

#endif /* QUERY_H */

