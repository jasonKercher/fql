#ifndef TABLE_H
#define TABLE_H


#ifdef __cplusplus
extern "C" {
#endif

#include "util/queue.h"
#include "expression.h"
#include "reader.h"


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
void table_add_column(table_t* table, expression_t* expr);


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

column_t* column_new();


#ifdef __cplusplus
}
#endif

#endif /* TABLE_H */


