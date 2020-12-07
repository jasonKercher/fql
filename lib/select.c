#include "select.h"

#include "column.h"
#include "schema.h"
#include "util/util.h"

Select* select_new()
{
        Select* new_select = NULL;
        malloc_(new_select, sizeof(*new_select));

        return select_init(new_select);
}

Select* select_init(Select* select)
{
        *select = (Select) {
                 OP_SELECT
                ,schema_new()
        };

        return select;
}

void select_free(Select* select)
{
        free_(select->schema);
        free_(select);
}

void select_add_column(Select* select
                      ,Expression* expr
                      ,const char* table_name)
{
        Column* new_col = column_new(expr, table_name);
        schema_add_column(select->schema, new_col);
}

void select_apply_process(Select* select, Plan* plan)
{
        Process* proc = plan->op_true->data;
        strcpy(proc->action_msg, "SELECT");

        proc = plan->op_false->data;
        proc->is_passive = true;
}

void select_apply_column_alias(Select* select, const char* alias)
{
        schema_apply_column_alias(select->schema, alias);
}

_Bool select_validate(Select* select)
{
        return true;
}
