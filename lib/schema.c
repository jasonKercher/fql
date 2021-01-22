#include "schema.h"

#include <dirent.h>
#include <csv.h>

#include "fql.h"
#include "column.h"
#include "function.h"
#include "query.h"
#include "operation.h"
#include "util/util.h"
#include "util/vec.h"

Schema* schema_new()
{
        Schema* new_schema = NULL;
        malloc_(new_schema, sizeof(*new_schema));

        return schema_construct(new_schema);
}

Schema* schema_construct(Schema* schema)
{
        *schema = (Schema) {
                 vec_new_(Column*)      /* columns */
                ,NULL                   /* col_map */
                ,""                     /* name */
        };

        return schema;
}

void schema_free(void* generic_schema)
{
        Schema* schema = generic_schema;

        Column** it = vec_begin(schema->columns);
        for (; it != vec_end(schema->columns); ++it) {
                column_free(*it);
        }

        vec_free(schema->columns);
        if (schema->col_map != NULL) {
                hmap_free(schema->col_map);
        }
        free_(schema);
}

void schema_add_column(Schema* schema, Column* col)
{
        vec_push_back(schema->columns, &col);
}

void schema_apply_column_alias(Schema* schema, const char* alias)
{
        Column* col = vec_back(schema->columns);
        strncpy_(col->alias, alias, COLUMN_NAME_MAX);
}


int schema_resolve_file(Table* table)
{
        /* TODO: could overflow here
         * Usage of PATH_MAX here is kind of silly,
         * but I'm too lazy to allocate these correctly.
         */
        char table_name_dir[PATH_MAX] = "";
        char table_name_base[PATH_MAX] = "";

        strcpy(table_name_base, table->name);
        strcpy(table_name_dir, table->name);

        char* dir = dirname(table_name_dir);
        char* base = basename(table_name_base);

        Queue* files = dir_list_files(dir);
        Queue* node = files;

        int matches = 0;
        /* Match exact */
        for (; node; node = node->next) {
                if (string_eq(node->data, base)) {
                        ++matches;
                        strcpy(table->reader->file_name, node->data);
                }
        }

        if (matches) {
                goto success_return;
        }

        /* Match exact ignoring case */
        for (node = files; node; node = node->next) {
                if (istring_eq(node->data, base)) {
                        ++matches;
                        strcpy(table->reader->file_name, node->data);
                }
        }

        if (matches > 1) {
                fprintf(stderr, "%s: ambiguous file name\n", table->name);
                return FQL_FAIL;
        } else if (matches) {
                goto success_return;
        }

        char file_noext[PATH_MAX] = "";

        /* Match file without extension */
        for (node = files; node; node = node->next) {
                getnoext(file_noext, node->data);
                if (string_eq(file_noext, base)) {
                        ++matches;
                        strcpy(table->reader->file_name, node->data);
                }
        }

        if (matches > 1) {
                fprintf(stderr, "%s: ambiguous file name\n", table->name);
                return FQL_FAIL;
        } else if (matches) {
                goto success_return;
        }

        /* Match file without extension ignoring case */
        for (node = files; node; node = node->next) {
                getnoext(file_noext, node->data);
                if (istring_eq(file_noext, base)) {
                        ++matches;
                        strcpy(table->reader->file_name, node->data);
                }
        }

        if (matches > 1) {
                fprintf(stderr, "%s: ambiguous file name\n", table->name);
                return FQL_FAIL;
        } else if (matches) {
                goto success_return;
        }

        fprintf(stderr, "%s: unable to find matching file\n", table->name);
        return FQL_FAIL;

success_return:
        queue_free_data(&files);
        return FQL_GOOD;
}

void schema_assign_header(Table* table, Vec* rec)
{
        int i = 0;

        table->schema->col_map = hmap_new(rec->size * 2, HMAP_NOCASE);

        StringView* fields = rec->data;
        for (; i < rec->size; ++i) {
                String* column_name = string_from_stringview(&fields[i]);
                Column* new_col = column_new(EXPR_COLUMN_NAME, column_name, "");
                schema_add_column(table->schema, new_col);

                new_col->location = i;
                new_col->table = table;

                /* add to hash map for easy searching */
                hmap_set(table->schema->col_map, column_name->data, new_col);
        }
}

int schema_resolve_source(Source* source)
{
        Table* table = source->table;
        if (!vec_empty(table->schema->columns)) {
                return FQL_GOOD;  /* Schema already set */
        }

        if (source->source_type == SOURCE_SUBQUERY) {
                fputs("Not supporting subquery schema yet\n", stderr);
                return FQL_FAIL;  /* TODO: retrieve schema from subquery */
        }

        if (table->schema->name[0]) {
                fputs("Not loading schema by name yet\n", stderr);
                return FQL_FAIL;  /* TODO: load schema by name */
        }

        /* If we've made it this far, we want to try and determine
         * schema by reading the top row of the file and assume a
         * delimited list of field names.
         */
        if (schema_resolve_file(source->table)) {
                return FQL_FAIL;
        }

        /* Retrieve schema using libcsv */
        if (source->join_type == JOIN_FROM) {
                table->reader->type = READ_LIBCSV;
        } else {
                table->reader->type = READ_MMAPCSV;
        }
        reader_assign(table->reader);

        Vec rec;
        vec_construct_(&rec, StringView);
        table->reader->get_record_fn(table->reader->reader_data, &rec, 0);

        schema_assign_header(table, &rec);
        vec_destroy(&rec);

        return FQL_GOOD;
}

void _evaluate_if_const(Column* col)
{
        Function* func = col->field.fn;
        Column** it = vec_begin(func->args);
        for (; it != vec_end(func->args); ++it) {
                if ((*it)->expr != EXPR_CONST) {
                        return;
                }
        }

        union field new_field;
        if (col->field_type == FIELD_STRING) {
                new_field.s = string_new();
        }
        func->caller(func, &new_field, NULL);

        function_free(func);
        col->expr = EXPR_CONST;
        col->field = new_field;
}

int schema_assign_columns_limited(Vec* columns, Vec* sources, int limit)
{
        Column** it = vec_begin(columns);
        for (; it != vec_end(columns); ++it) {
                if ((*it)->expr == EXPR_FUNCTION) {
                        Function* func = (*it)->field.fn;
                        function_validate(func);
                        schema_assign_columns_limited(func->args, sources, limit);
                        _evaluate_if_const(*it);
                        continue;
                }
                if ((*it)->expr != EXPR_COLUMN_NAME) {
                        continue;
                }

                int matches = 0;
                int j = 0;

                for (; j <= limit; ++j) {
                        Source* search_src = vec_at(sources, j);
                        if ((*it)->table_name[0] == '\0' ||
                            istring_eq((*it)->table_name, search_src->alias)) {
                                matches += column_try_assign_source(*it, search_src, j);
                        }
                }

                if (matches > 1) {
                        fprintf(stderr, "%s: ambiguous column\n", (*it)->alias);
                        return FQL_FAIL;
                }

                if (matches == 0) {
                        fprintf(stderr, "%s: cannot find column\n", (*it)->alias);
                        return FQL_FAIL;
                }
        }

        return FQL_GOOD;
}

int schema_assign_columns(Vec* columns, Vec* sources)
{
        return schema_assign_columns_limited(columns,
                                             sources,
                                             sources->size - 1);
}

int schema_resolve_query(Query* query)
{
       Vec* sources = query->sources;

       int i = 0;
       for (; i < sources->size; ++i) {
               Source* src = vec_at(query->sources, i);
               if (schema_resolve_source(src)) {
                       return FQL_FAIL;
               }
               if (schema_assign_columns_limited(src->validation_list,
                                         sources, i)) {
                       return FQL_FAIL;
               }
       }

       Vec* op_cols = op_get_validation_list(query->op);
       if (schema_assign_columns(op_cols, sources)) {
               return FQL_FAIL;
       }

       return FQL_GOOD;
}

int schema_resolve(Queue* query_node)
{
        for (; query_node; query_node = query_node->next) {
                Query* query = query_node->data;
                if (schema_resolve_query(query)) {
                        return FQL_FAIL;
                }
        }

        return FQL_GOOD;
}


