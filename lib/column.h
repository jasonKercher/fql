#ifndef COLUMN_H
#define COLUMN_H

#include "fqlimits.h"
#include "table.h"
#include "expression.h"

/** Column **/
struct column_t {
        table_t* table;
        struct column_t* data_source;
        expression_t* expr;
        char alias[COLUMN_NAME_MAX];
        char table_name[TABLE_NAME_MAX];
        unsigned location;
        unsigned width;
};
typedef struct column_t column_t;

column_t* column_new(expression_t*, const char*);
void column_free(void*);

int column_try_assign_source(column_t*, source_t*);



#endif
