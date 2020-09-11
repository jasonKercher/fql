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


/** Query **/
struct query_t {
        table_t* table;         /* output table */
        queue_t* sources;       /* table_t */
        queue_t* conditions;    /* expression_t */
        queue_t* groups;        /* expression_t */
        queue_t* having;        /* expression_t */
        expression_t* limit;    /* TOP */
        int opid;               /* type of operation */

        /* temps used when traversing */
        expression_t* expr;
        int mode;
};
typedef struct query_t query_t;

query_t* query_new();



#ifdef __cplusplus
}
#endif

#endif /* QUERY_H */

