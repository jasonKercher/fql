#include "select.h"
#include "fql.h"
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
                ,writer_new()
        };

        return select;
}

void select_free(Select* select)
{
        if (select == NULL) {
                return;
        }
        if (select->writer->free_fn != NULL) {
                select->writer->free_fn(select->writer->writer_data);
        }
        writer_free(select->writer);
        schema_free(select->schema);
        free_(select);
}

void select_add_column(Select* select, Column* col)
{
        schema_add_column(select->schema, col);
}

void select_apply_process(Select* select, Plan* plan)
{
        Process* proc = plan->op_true->data;
        proc->action = &fql_select;
        proc->proc_data = select;
        string_cpy(proc->action_msg, "SELECT ");

        Vec* col_vec = select->schema->columns;
        Column** it = vec_begin(col_vec);
        for (; it != vec_end(col_vec); ++it) {
                if (it != vec_begin(col_vec)) {
                        string_cat(proc->action_msg, ",");
                }
                column_cat_description(*it, proc->action_msg);
        }

        proc = plan->op_false->data;
        proc->is_passive = true;
}

void select_apply_column_alias(Select* select, const char* alias)
{
        schema_apply_column_alias(select->schema, alias);
}

int select_record(Select* select, struct vec* rec)
{
        Writer* writer = select->writer;

        Vec* col_vec = select->schema->columns;

        /* TODO: in reality this should never change */
        vec_resize(writer->raw_rec, col_vec->size);

        Column** cols = vec_begin(col_vec);
        int i = 0;
        for (; i < col_vec->size; ++i) {
                StringView sv;
                if (column_get_stringview(&sv, cols[i], rec)) {
                        return FQL_FAIL;
                }
                String* s = vec_at(writer->raw_rec, i);
                string_copy_from_stringview(s, &sv);
        }

        writer->write_record_fn(writer->writer_data, writer->raw_rec);
        return FQL_GOOD;
}
