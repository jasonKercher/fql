#ifndef COLUMN_H
#define COLUMN_H

#include "fqlimits.h"
#include "table.h"
#include "expression.h"

enum col_type {
        COL_UNDEFINED,
        COL_STRING,
        COL_INT,
        COL_FLOAT,
};

/** Column **/
struct column {
        enum col_type type;
        struct table* table;
        struct column* data_source;
        struct expression* expr;
        char alias[COLUMN_NAME_MAX];
        char table_name[TABLE_NAME_MAX];
        unsigned location;
        unsigned width;
};
typedef struct column Column;

struct column* column_new(struct expression*, const char* table_id);
struct column* column_init(struct column*, struct expression*, const char*);
void column_free(void*);

void column_cat_description(struct column* col, char* msg);
int column_try_assign_source(struct column*, struct source*);

#endif /* COLUMN_H */
