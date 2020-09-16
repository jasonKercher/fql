#ifndef TABLE_H
#define TABLE_H


#ifdef __cplusplus
extern "C" {
#endif

#include "util/stack.h"
#include "expression.h"
#include "reader.h"


/** Table **/
#define TABLE_NAME_MAX 129

struct table_t {
        reader_t* source;
        stack_t* columns;
        char schema[TABLE_NAME_MAX];
        char name[TABLE_NAME_MAX];
};
typedef struct table_t table_t;

table_t* table_new();
void table_free(table_t*);
void table_add_column(table_t*, expression_t*, const char* table_name);
void table_apply_column_alias(table_t*, const char*);


/** Source **/
enum source {
        SOURCE_TABLE,
        SOURCE_SUBQUERY,
};

enum join {
        JOIN_FROM,
        JOIN_INNER,
        JOIN_LEFT,
        JOIN_RIGHT,
        JOIN_FULL,
        JOIN_CROSS,
};

struct source_t {
        table_t* table;
        enum source source_type;
        enum join join_type;
        char alias[TABLE_NAME_MAX];
};
typedef struct source_t source_t;

source_t* source_new(table_t*, enum source);
void source_free(source_t*);


/** Column **/
#define COLUMN_NAME_MAX 129

struct column_t {
        table_t* table;
        expression_t* expr;
        char alias[COLUMN_NAME_MAX];
        char table_name[TABLE_NAME_MAX];
        unsigned location;
        unsigned width;
};
typedef struct column_t column_t;

column_t* column_new(expression_t*, const char*);
void column_free(void*);


#ifdef __cplusplus
}
#endif

#endif /* TABLE_H */


