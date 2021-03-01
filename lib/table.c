#include "table.h"
#include "antlr/antlr.h"
#include "query.h"
#include "column.h"
#include "util/util.h"

Table* table_new()
{
        Table* new_table = NULL;
        malloc_(new_table, sizeof(*new_table));

        return table_construct(new_table);
}

Table* table_construct(Table* table)
{
        *table = (Table) {
                 reader_new() /* reader */
                ,schema_new() /* schema */
                ,{ 0 }        /* name */
        };

        string_construct(&table->name);

        return table;
}

void table_free(Table* table)
{
        if (table == NULL)
                return;

        string_destroy(&table->name);
        reader_free(table->reader);
        schema_free(table->schema);
        free_(table);
}



Source* source_new(Table* table,
                   const char* alias,
                   size_t idx,
                   enum source_type source_type,
                   enum join_type join_type)
{
        Source* new_source = NULL;
        malloc_(new_source, sizeof(*new_source));

        return source_construct(new_source, table, alias, idx, source_type, join_type);
}

Source* source_construct(Source* src,
                         Table* table,
                         const char* alias,
                         size_t idx,
                         enum source_type source_type,
                         enum join_type join_type)
{
        *src = (Source) {
                 table                  /* table */
                ,NULL                   /* condition */
                ,vec_new_(Column*)      /* validation_list */
                ,NULL                   /* read_proc */
                ,NULL                   /* join_data */
                ,{ 0 }                  /* alias */
                ,idx                    /* idx */
                ,source_type            /* source_type */
                ,join_type              /* join_type */

        };

        if (alias[0] == '\0') {
                string_construct_from_string(&src->alias, &table->name);
        } else {
                string_construct_from_char_ptr(&src->alias, alias);
        }

        return src;
}

void source_free(Source* source)
{
        source_destroy(source);
        free_(source);
}

void source_destroy(Source* source)
{
        Column** it = vec_begin(source->validation_list);
        for (; it != vec_end(source->validation_list); ++it) {
                column_free(*it);
        }
        string_destroy(&source->alias);
        vec_free(source->validation_list);
        table_free(source->table);
}




struct hashjoin* hashjoin_new()
{
        struct hashjoin* new_join;
        malloc_(new_join, sizeof(*new_join));

        *new_join = (struct hashjoin) {
                 { 0 }          /* hash_data */
                ,NULL           /* left_col */
                ,NULL           /* right_col */
                ,NULL           /* recs */
                ,SIDE_RIGHT     /* state */
                ,0              /* rec_idx */
        };

        return new_join;
}


void hashjoin_free(struct hashjoin* join)
{
        pmap_destroy(&join->hash_data);
        free_(join);
}


size_t _guess_row_count(Source* src)
{
        size_t guess = 0;
        size_t total_length = 0;

        Reader* reader = src->table->reader;

        unsigned max_col_store = reader->max_col_idx;
        reader->max_col_idx = 0;

        Record rec;
        record_construct(&rec);

        int i = 0;
        for (; i < 10; ++i) {
                if (reader->get_record__(reader, &rec) != FQL_GOOD) {
                        break;
                }
                total_length += rec.rec_raw.len;
        }

        if (i == 0) {
                return HASH_JOIN_MIN_SIZE;
        }

        record_destroy(&rec);

        reader->max_col_idx = max_col_store;

        double avg_len = total_length / (double) i;
        if (avg_len < 1)
                avg_len = 1;

        struct mmapcsv_data* data = reader->reader_data;
        reader->reset__(reader);

        guess = data->file_size / avg_len;

        return (guess < HASH_JOIN_MIN_SIZE) ? HASH_JOIN_MIN_SIZE : guess + 1;
}

void source_hash_join_init(Source* src)
{
        size_t guessed_row_count = _guess_row_count(src);

        struct hashjoin* join = src->join_data;

        pmap_construct(&join->hash_data, guessed_row_count * 2, HMAP_NOCASE | HMAP_RTRIM);
}
