#include "select.h"
#include "fql.h"
#include "column.h"
#include "schema.h"
#include "util/util.h"

Select* select_new()
{
        Select* new_select = NULL;
        malloc_(new_select, sizeof(*new_select));

        return select_construct(new_select);
}

Select* select_construct(Select* select)
{
        *select = (Select) {
                 OP_SELECT              /* oper_type */
                ,NULL                   /* api */
                ,schema_new()           /* schema */
                ,writer_new()           /* writer */
                ,&select_record_api     /* select_record_fn */
        };

        return select;
}

void select_free(Select* select)
{
        if (select == NULL) {
                return;
        }
        writer_free(select->writer);
        schema_free(select->schema);
        free_(select);
}

void select_add_column(Select* select, Column* col)
{
        schema_add_column(select->schema, col);
}

void select_connect_api(struct select* select, Vec* api)
{
        vec_resize(api, select->schema->columns->size);
        select->api = api;
}

void select_use_non_api(Select* select)
{
        select->select_fn = &select_record;
}

int _insert_all_columns(Vec* col_vec, Source* src, unsigned src_idx, unsigned* col_idx)
{
        Vec* src_col_vec = src->table->schema->columns;

        Column** it = vec_begin(src_col_vec);
        for (; it != vec_end(src_col_vec); ++it) {
                //String* col_name = string_from_string(&(*it)->alias);
                Column* new_col = column_new(EXPR_COLUMN_NAME, &(*it)->alias.data, "");
                new_col->data_source = *it;
                new_col->src_idx = src_idx;
                new_col->field_type = (*it)->field_type;
                vec_insert(col_vec, *col_idx, &new_col);
                ++(*col_idx);
        }

        return src->table->schema->columns->size;
}

void _expand_asterisks(Query* query)
{
        Select* select = query->op;
        Vec* col_vec = select->schema->columns;
        unsigned i = 0;

        for (; i < col_vec->size; ++i) {
                Column** col = vec_at(col_vec, i);
                if ((*col)->expr != EXPR_ASTERISK) {
                        continue;
                }

                Source* srcs = vec_begin(query->sources);
                unsigned j = 0;
                unsigned asterisk_index = i++;
                for (; j < query->sources->size; ++j) {
                        Column** asterisk_col = vec_at(col_vec, asterisk_index);
                        if (string_empty(&(*col)->table_name) ||
                            istring_eq(srcs[j].alias.data, ((*asterisk_col)->table_name.data))) {
                                _insert_all_columns(col_vec, &srcs[j], j, &i);
                        }
                }

                Column** asterisk_col = vec_at(col_vec, asterisk_index);
                column_free(*asterisk_col);
                vec_remove(col_vec, asterisk_index);
        }
}

void select_apply_process(Query* query, Plan* plan)
{
        Select* select = query->op;
        Process* proc = plan->op_true->data;
        proc->action = &fql_select;
        proc->proc_data = select;
        string_cpy(proc->action_msg, "SELECT ");

        Vec* col_vec = select->schema->columns;
        Column** col = vec_begin(col_vec);
        for (; col != vec_end(col_vec); ++col) {
                if (col != vec_begin(col_vec)) {
                        string_cat(proc->action_msg, ",");
                }
                column_cat_description(*col, proc->action_msg);
        }

        _expand_asterisks(query);

        /* Initialize the raw strings used for writing */
        Vec* raw_rec = select->writer->raw_rec;
        vec_resize(raw_rec, col_vec->size);
        String* s = vec_begin(raw_rec);
        for (; s != vec_end(raw_rec); ++s) {
                string_construct(s);
        }

        proc = plan->op_false->data;
        proc->is_passive = true;
}

void select_apply_column_alias(Select* select, const char* alias)
{
        schema_apply_column_alias(select->schema, alias);
}

int select_record_api(Select* select, struct vec* rec)
{
        Writer* writer = select->writer;

        Vec* col_vec = select->schema->columns;

        Column** cols = vec_begin(col_vec);
        int i = 0;
        for (; i < col_vec->size; ++i) {
                struct fql_field* field = vec_at(select->api, i);
                switch (cols[i]->field_type) {
                case FIELD_STRING:
                {
                        StringView sv;
                        if (column_get_stringview(&sv, cols[i], rec)) {
                                return FQL_FAIL;
                        }
                        /* Even though the we are not using the
                         * writer directly, we use the data it
                         * owns for our string fields
                         */
                        String* s = vec_at(writer->raw_rec, i);
                        string_copy_from_stringview(s, &sv);
                        field->type = FQL_STRING;
                        field->data.s = s->data;
                        break;
                }
                case FIELD_INT:
                        field->type = FQL_INT;
                        if (column_get_int(&field->data.i, cols[i], rec)) {
                                return FQL_FAIL;
                        }
                        break;
                case FIELD_FLOAT:
                        field->type = FQL_FLOAT;
                        if (column_get_float(&field->data.f, cols[i], rec)) {
                                return FQL_FAIL;
                        }
                default:
                        ;
                }
        }

        return 1;
}

int select_record(Select* select, struct vec* rec)
{
        Writer* writer = select->writer;

        Vec* col_vec = select->schema->columns;

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
        return 1;
}
