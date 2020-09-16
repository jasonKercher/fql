#ifndef TABLE_H
#define TABLE_H


#ifdef __cplusplus
extern "C" {
#endif

#include "util/stack.h"
#include "expression.h"
#include "schema.h"
#include "reader.h"
#include "fqlimits.h"


/** Table **/

struct table_t {
        reader_t* reader;
        schema_t* schema;
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



#ifdef __cplusplus
}
#endif

#endif /* TABLE_H */


