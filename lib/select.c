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
                ,vec_new_(Column*)
        };

        return select;
}

void select_free(Select* select)
{
        free_(select->schema);
        free_(select);
}

void select_add_column(Select* select, Column* col)
{
        schema_add_column(select->schema, col);
        vec_push_back(select->validation_list, &col); /* TODO: remove */
}

void select_apply_process(Select* select, Plan* plan)
{
        Process* proc = plan->op_true->data;
        strcpy(proc->action_msg, "SELECT ");

        Vec* col_vec = select->schema->columns;
        Column** it = vec_begin(col_vec);
        for (; it != vec_end(col_vec); ++it) {
                column_cat_description(*it, proc->action_msg);
        }

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
