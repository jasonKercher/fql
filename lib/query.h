#ifndef QUERY_H
#define QUERY_H


#ifdef __cplusplus
extern "C" {
#endif

#include "util/queue.h"
#include "reader.h"

/**
 * Structures
 */


/** Expression **/
#define EXPR_NONE               0
#define EXPR_COLUMN             1
#define EXPR_CONST              2
#define EXPR_SUBQUERY           3
#define EXPR_SUBQUERY_CONST     4

struct expression_t {
        void* data;
        int type;
};
typedef struct expression_t expression_t;

expression_t* expression_new();


/** Table **/
#define TABLE_NAME_MAX 129

struct table_t {
        reader_t* source;
        queue_t* columns;
        char name[TABLE_NAME_MAX];
        char alias[TABLE_NAME_MAX];
};
typedef struct table_t table_t;

table_t* table_new();


/** Column **/
#define COLUMN_NAME_MAX 129

struct column_t {
        table_t* table;
        char name[COLUMN_NAME_MAX];
        char alias[COLUMN_NAME_MAX];
        unsigned location;
        unsigned width;
};
typedef struct column_t column_t;

column_t* column_new();


/** Operation **/
#define OP_NONE         0
#define OP_SELECT       1
#define OP_DELETE       2
#define OP_UPDATE       3

struct operation_t {
        table_t* output_table;
        int type;
};
typedef struct operation_t operation_t;

operation_t* operation_new();


/** Query **/
struct query_t {
        operation_t* operation;
        queue_t* sources;    /* table_t */
        queue_t* conditions; /* expression_t */
        queue_t* groups;     /* expression_t */
        queue_t* having;    /* expression_t */
        expression_t* limit;
};
typedef struct query_t query_t;

query_t* query_new();



#ifdef __cplusplus
}
#endif

#endif /* QUERY_H */

