#ifndef TABLE_H
#define TABLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "util/stack.h"
#include "util/hmap.h"
#include "expression.h"
#include "schema.h"
#include "reader.h"
#include "fqlimits.h"


/** Table **/

struct table {
        struct reader* reader;
        struct schema* schema;
        char name[TABLE_NAME_MAX];
};

struct table* table_new();
void table_free(struct table*);
void table_add_column(struct table*, struct expression*, const char* table_name);
void table_apply_column_alias(struct table*, const char*);


/** Source **/
enum source_type {
        SOURCE_TABLE,
        SOURCE_SUBQUERY,
};

enum join_type {
        JOIN_FROM,
        JOIN_INNER,
        JOIN_LEFT,
        JOIN_RIGHT,
        JOIN_FULL,
        JOIN_CROSS,
};

struct source {
        struct table* table;
        struct search* condition;
        char alias[TABLE_NAME_MAX];
        enum source_type source_type;
        enum join_type join_type;
};

struct source* source_new(struct table*,
                          const char* alias,
                          enum source_type,
                          enum join_type);
void source_free(struct source*);



#ifdef __cplusplus
}
#endif

#endif /* TABLE_H */


