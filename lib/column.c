#include "column.h"
#include "fql.h"
#include "query.h"
#include "process.h"
#include "util/util.h"

Column* column_new(enum expr_type expr, void* data, const char* table_name)
{
        Column* new_column = NULL;
        malloc_(new_column, sizeof(*new_column));

        return column_init(new_column, expr, data, table_name);
}

Column* column_init(Column* col, enum expr_type expr, void* data, const char* table_name)
{
        *col = (Column) {
                 expr                   /* expr */
                ,NULL                   /* table */
                ,NULL                   /* data_source */
                ,""                     /* alias */
                ,""                     /* table_name */
                ,FIELD_UNDEFINED        /* field_type */
                ,NULL                   /* field */
                ,0                      /* location */
                ,0                      /* width */
        };

        strncpy_(col->table_name, table_name, TABLE_NAME_MAX);

        switch (expr) {
        case EXPR_COLUMN_NAME:
                strncpy_(col->alias, ((String*) data)->data, COLUMN_NAME_MAX);
                col->field.s = data;
                break;
        case EXPR_FUNCTION:
                col->field.fn = data;
                break;
        default:
                ;
        }

        return col;
}

void column_free(void* generic_col)
{
        Column* col = generic_col;
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
        col->data_source = hmap_get(src->table->schema->col_map, col->alias);
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
                string_init(&s);
                Vec* rec = vec_at(recs, col->src_idx);
                StringView* sv = vec_at(rec, col->data_source->location);
                string_copy_from_stringview(&s, sv);
                if (str2long(ret, s.data)) {
                        string_destroy(&s);
                        return FQL_FAIL;
                }
                string_destroy(&s);
                return FQL_GOOD;
        }
        case EXPR_CONST:
                if (field_to_int(ret, &col->field, &col->field_type)) {
                        return FQL_FAIL;
                }
                break;
        default:
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
                string_init(&s);
                Vec* rec = vec_at(recs, col->src_idx);
                StringView* sv = vec_at(rec, col->data_source->location);
                string_copy_from_stringview(&s, sv);
                if (str2double(ret, s.data)) {
                        string_destroy(&s);
                        return FQL_FAIL;
                }
                string_destroy(&s);
                return FQL_GOOD;
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
                Vec* rec = vec_at(recs, col->src_idx);
                ret = vec_at(rec, col->data_source->location);
                return FQL_GOOD;
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

