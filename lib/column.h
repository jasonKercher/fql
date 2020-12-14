#ifndef COLUMN_H
#define COLUMN_H

#include "fqlimits.h"
#include "table.h"
#include "util/fqlstring.h"

/** data type **/
enum col_type {
        COL_UNDEFINED,
        COL_STRING,
        COL_INT,
        COL_FLOAT,
};

/** Expression **/
enum expr_type {
        EXPR_NONE,
        EXPR_COLUMN_NAME,
        EXPR_CONST,
        EXPR_FUNCTION,
        EXPR_SOURCE,
        EXPR_SUBQUERY,
        EXPR_SUBQUERY_CONST,
};

/** Column **/
struct column {
        enum col_type type;
        struct table* table;
        struct column* data_source;
        enum expr_type expr;
        void* data;
        char alias[COLUMN_NAME_MAX];
        char table_name[TABLE_NAME_MAX];
        unsigned location;
        unsigned width;
};
typedef struct column Column;

struct column* column_new(enum expr_type, void*, const char* table_id);
struct column* column_init(struct column*, enum expr_type, void*, const char*);
void column_free(void*);

void column_cat_description(struct column* col, String*);
int column_try_assign_source(struct column*, struct source*);

#endif /* COLUMN_H */
