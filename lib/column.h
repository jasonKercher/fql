#ifndef COLUMN_H
#define COLUMN_H

#include "fqlimits.h"
#include "table.h"
#include "field.h"
#include "util/stringview.h"
#include "util/fqlstring.h"

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
        enum expr_type expr;
        struct table* table;
        struct column* data_source;
        char alias[COLUMN_NAME_MAX];
        char table_name[TABLE_NAME_MAX];
        enum field_type field_type;
        union field field;
        unsigned location;
        unsigned width;
        int src_idx;
};
typedef struct column Column;

struct column* column_new(enum expr_type, void*, const char* table_id);
struct column* column_construct(struct column*, enum expr_type, void*, const char*);
void column_free(void*);

void column_cat_description(struct column* col, String*);
int column_try_assign_source(struct column*, struct source*, int);

/* Data Access */
int column_get_int(long*, struct column*, struct vec*);
int column_get_float(double*, struct column*, struct vec*);
int column_get_stringview(struct stringview*, struct column*, struct vec*);

#endif /* COLUMN_H */
