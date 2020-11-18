#include "schema.h"

#include <dirent.h>
#include <csv.h>

#include "column.h"
#include "query.h"
#include "operation.h"
#include "util/util.h"
#include "util/vec.h"

Schema* schema_new()
{
        Schema* new_schema = NULL;
        malloc_(new_schema, sizeof(*new_schema));

        *new_schema = (Schema) {
                 NULL   /* columns */
                ,NULL   /* col_map */
                ,""     /* name */
        };

        return new_schema;
}

void schema_free(void* generic_schema)
{
        Schema* schema = generic_schema;
        stack_free_func(&schema->columns, &column_free);
        if (schema->col_map != NULL) {
                hmap_free(schema->col_map);
        }
        free_(schema);
}

void schema_add_column(Schema* schema, Column* col)
{
        stack_push(&schema->columns, col);
}

void schema_apply_column_alias(Schema* schema, const char* alias)
{
        Column* col = schema->columns->data;
        strncpy_(col->alias, alias, COLUMN_NAME_MAX);
}


void schema_resolve_file(Table* table)
{
        /* Usage of PATH_MAX here is kind of silly,
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
                exit(EXIT_FAILURE);
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
                exit(EXIT_FAILURE);
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
                exit(EXIT_FAILURE);
        } else if (matches) {
                goto success_return;
        }

        fprintf(stderr, "%s: unable to find matching file\n", table->name);
        exit(EXIT_FAILURE);

success_return:
        queue_free_data(&files);
}

void schema_assign_header(Table* table, csv_record* rec)
{
        int i = 0;

        table->schema->col_map = hmap_new(rec->size * 2, HMAP_NOCASE);

        for (; i < rec->size; ++i) {
                char* column_name = strdup(rec->fields[i]);
                Expression* new_expr = expression_new(EXPR_COLUMN_NAME
                                                            ,column_name);
                Column* new_col = column_new(new_expr, "");
                schema_add_column(table->schema, new_col);

                new_col->location = i;
                new_col->table = table;

                /* add to hash map for easy searching */
                hmap_set(table->schema->col_map, column_name, new_col);
        }
}

void schema_resolve_source(Source* source)
{
        if (source->source_type == SOURCE_SUBQUERY) {
                fputs("Not supporting subquery schema yet\n", stderr);
                exit(EXIT_FAILURE);  /* TODO: retrieve schema from subquery */
        }

        schema_resolve_file(source->table);

        Table* table = source->table;
        if (table->schema->columns) {
                return;  /* Schema already set */
        }

        if (table->schema->name[0]) {
                fputs("Not loading schema by name yet\n", stderr);
                exit(EXIT_FAILURE);  /* TODO: load schema by name */
        }

        /* Retrieve schema using libcsv */
        reader_assign(table->reader, READ_LIBCSV);

        csv_record* rec = csv_record_new();
        table->reader->get_record_f(table->reader->handle, rec);

        schema_assign_header(table, rec);

        csv_record_free(rec);
}

void schema_validate_logic_columns(Vec* sources, int limit)
{
        Source* src = sources->vector[limit];
        Vec* columns = src->logic_columns;

        int i = 0;
        for (; i < columns->size; ++i) {
                int matches = 0;
                Column* col = columns->vector[i];

                int j = 0;

                for (; j <= limit; ++j) {
                        Source* search_src = sources->vector[j];
                        if (col->table_name[0] == '\0' ||
                            istring_eq(col->table_name, search_src->alias)) {
                                matches += column_try_assign_source(col, search_src);
                        }
                }

                if (matches > 1) {
                        fprintf(stderr, "%s: ambiguous column\n", col->alias);
                        exit(EXIT_FAILURE);
                }

                if (matches == 0) {
                        fprintf(stderr, "%s: cannot find column\n", col->alias);
                        exit(EXIT_FAILURE);
                }
        }
}

void schema_resolve(Queue* query_node)
{
        for (; query_node; query_node = query_node->next) {
                Query* query = query_node->data;

                int i = 0;
                for (; i < query->sources->size; ++i) {
                        schema_resolve_source(query->sources->vector[i]);
                        schema_validate_logic_columns(query->sources, i);
                }

                Schema* op_schema = op_get_schema(query);
        }


}


