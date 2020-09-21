#ifndef COLUMN_H
#define COLUMN_H

#include "fqlimits.h"
#include "table.h"
#include "expression.h"

/** Column **/
struct column {
        struct table* table;
        struct column* data_source;
        struct expression* expr;
        char alias[COLUMN_NAME_MAX];
        char table_name[TABLE_NAME_MAX];
        unsigned location;
        unsigned width;
};

struct column* column_new(struct expression*, const char*);
void column_free(void*);

int column_try_assign_source(struct column*, struct source*);



#endif