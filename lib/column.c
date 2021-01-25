#include "column.h"
#include "fql.h"
#include "query.h"
#include "process.h"
#include "util/util.h"

Column* column_new(enum expr_type expr, void* data, const char* table_name)
{
        Column* new_column = NULL;
        malloc_(new_column, sizeof(*new_column));

        return column_construct(new_column, expr, data, table_name);
}

Column* column_construct(Column* col, enum expr_type expr, void* data, const char* table_name)
{
        *col = (Column) {
                 expr                   /* expr */
                ,NULL                   /* table */
                ,NULL                   /* data_source */
                ,{ 0 }                  /* alias */
                ,{ 0 }                  /* table_name */
                ,FIELD_UNDEFINED        /* field_type */
                ,NULL                   /* field */
                ,0                      /* location */
                ,0                      /* width */
        };

        string_construct_from_char_ptr(&col->table_name, table_name);

        switch (expr) {
        case EXPR_COLUMN_NAME:
                string_construct_from_string(&col->alias, data);
                col->field.s = data;
                break;
        case EXPR_FUNCTION:
                col->field.fn = data;
        default:
                string_construct(&col->alias);
        }

        return col;
}

void column_free(void* generic_col)
{
        Column* col = generic_col;

        if (col->expr == EXPR_COLUMN_NAME || (
                        col->expr == EXPR_CONST &&
                        col->field_type == FIELD_STRING)
           ) {
                string_free(col->field.s);
        } else if (col->expr == EXPR_FUNCTION) {
                function_free(col->field.fn);
        }

        string_destroy(&col->alias);
        string_destroy(&col->table_name);
        free_(col);
}

void column_cat_description(Column* col, String* msg)
{
        switch (col->expr) {
        case EXPR_COLUMN_NAME:
                string_append(msg, col->field.s);
                break;
        case EXPR_FUNCTION:
        {
                Function* func = col->field.fn;
                string_cat(msg, func->name);
                string_push_back(msg, '(');

                Column** it = vec_begin(func->args);
                for (; it != vec_end(func->args); ++it) {
                        if (it != vec_begin(func->args)) {
                                string_push_back(msg, ',');
                        }
                        column_cat_description(*it, msg);
                }
                string_push_back(msg, ')');
                break;
        }
        case EXPR_CONST:
                switch (col->field_type) {
                case FIELD_STRING:
                        string_push_back(msg, '\'');
                        string_append(msg, col->field.s);
                        string_push_back(msg, '\'');
                        break;
                case FIELD_INT:
                {
                        char buf[20];
                        sprintf(buf, "%ld", col->field.i);
                        string_cat(msg, buf);
                        break;
                }
                case FIELD_FLOAT:
                {
                        char buf[30];
                        sprintf(buf, "%lf", col->field.f);
                        string_cat(msg, buf);
                        break;
                }
                default:
                        string_cat(msg, "<<const>>");

                }
                break;
        case EXPR_ASTERISK:
                string_cat(msg, "*");
                break;
        case EXPR_SOURCE:
                string_cat(msg, "TABLE SOURCE");
                break;
        case EXPR_SUBQUERY:
                string_cat(msg, "SUBQUERY");
                break;
        case EXPR_SUBQUERY_CONST:
                string_cat(msg, "SUBQUERY CONST");
                break;
        case EXPR_NONE:
                string_cat(msg, "No expression");
                break;
        }
}

int column_try_assign_source(Column* col, Source* src, int idx)
{
        col->data_source = hmap_get(src->table->schema->col_map, col->alias.data);
        if (col->data_source) {
                col->src_idx = idx;
                return 1;
        }

        return 0;
}

/* TODO: field_to_xx functions should only be called once.
 *       After which we should be able to assume the correct
 *       type. The best way to handle this would be to set
 *       the type during parsing.
 */
int column_get_int(long* ret, Column* col, Vec* recs)
{
        switch (col->expr) {
        case EXPR_COLUMN_NAME:
        {
                /* this is a rather unfortunate necessity */
                String s;
                string_construct(&s);
                Vec** rec = vec_at(recs, col->src_idx);
                StringView* sv = vec_at(*rec, col->data_source->location);
                string_copy_from_stringview(&s, sv);
                if (str2long(ret, s.data)) {
                        string_destroy(&s);
                        return FQL_FAIL;
                }
                string_destroy(&s);
                return FQL_GOOD;
        }
        case EXPR_FUNCTION:
        {
                Function* func = col->field.fn;
                union field new_field;
                if (col->field_type == FIELD_STRING) {
                        new_field.s = &func->ret_buf;
                        string_clear(new_field.s);
                }
                func->caller(func, &new_field, recs);
                if (field_to_int(ret, &new_field, &col->field_type)) {
                        return FQL_FAIL;
                }
                break;
        }
        case EXPR_CONST:
                if (field_to_int(ret, &col->field, &col->field_type)) {
                        return FQL_FAIL;
                }
                break;
        default:
                fprintf(stderr, "col_get_int: Unexpected expression\n");
                return FQL_FAIL;
        }

        return FQL_GOOD;
}

int column_get_float(double* ret, Column* col, Vec* recs)
{
        switch (col->expr) {
        case EXPR_COLUMN_NAME:
        {
                /* this is a rather unfortunate necessity */
                String s;
                string_construct(&s);
                Vec** rec = vec_at(recs, col->src_idx);
                StringView* sv = vec_at(*rec, col->data_source->location);
                string_copy_from_stringview(&s, sv);
                if (str2double(ret, s.data)) {
                        string_destroy(&s);
                        return FQL_FAIL;
                }
                string_destroy(&s);
                return FQL_GOOD;
        }
        case EXPR_FUNCTION:
        {
                Function* func = col->field.fn;
                union field new_field;
                if (col->field_type == FIELD_STRING) {
                        new_field.s = &func->ret_buf;
                        string_clear(new_field.s);
                }
                func->caller(func, &new_field, recs);
                if (field_to_float(ret, &new_field, &col->field_type)) {
                        return FQL_FAIL;
                }
                break;
        }
        case EXPR_CONST:
                if (field_to_float(ret, &col->field, &col->field_type)) {
                        return FQL_FAIL;
                }
                break;
        default:
                return FQL_FAIL;
        }

        return FQL_GOOD;
}

int column_get_stringview(StringView* ret, Column* col, Vec* recs)
{
        switch (col->expr) {
        case EXPR_COLUMN_NAME:
        {
                Vec** rec = vec_at(recs, col->src_idx);
                StringView* sv = vec_at(*rec, col->data_source->location);
                ret->data = sv->data;
                ret->len = sv->len;
                return FQL_GOOD;
        }
        case EXPR_FUNCTION:
        {
                Function* func = col->field.fn;
                union field new_field;
                if (col->field_type == FIELD_STRING) {
                        new_field.s = &func->ret_buf;
                        string_clear(new_field.s);
                }
                func->caller(func, &new_field, recs);
                if (field_to_stringview(ret, &new_field, &col->field_type)) {
                        return FQL_FAIL;
                }
                break;
        }
        case EXPR_CONST:
                if (field_to_stringview(ret, &col->field, &col->field_type)) {
                        return FQL_FAIL;
                }
                break;
        default:
                return FQL_FAIL;
        }

        return FQL_GOOD;
}

