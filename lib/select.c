#include "select.h"

#include "column.h"
#include "schema.h"
#include "util/util.h"

struct select* select_new()
{
        struct select* new_select = NULL;
        malloc_(new_select, sizeof(*new_select));

        *new_select = (struct select) {
                 OP_SELECT
                ,schema_new()
        };

        return new_select;
}

void select_free(struct select* select)
{
        free_(select->schema);
        free_(select);
}

void select_add_column(struct select* select
                      ,struct expression* expr
                      ,const char* table_name)
{
        struct column* new_col = column_new(expr, table_name);
        schema_add_column(select->schema, new_col);
}

void select_apply_column_alias(struct select* select, const char* alias)
{
        schema_apply_column_alias(select->schema, alias);
}

