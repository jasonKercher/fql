#include "table.h"
#include "antlr/antlr.h"
#include "query.h"
#include "column.h"
#include "reader.h"
#include "util/util.h"

//Table* table_new()
//{
//        Table* new_table = NULL;
//        malloc_(new_table, sizeof(*new_table));
//
//        return table_construct(new_table);
//}
//
//Table* table_construct(Table* table)
//{
//        *table = (Table) {
//        };
//
//        string_construct(&table->name);
//
//        return table;
//}
//
//void table_free(Table* table)
//{
//        if (table == NULL)
//                return;
//
//        string_destroy(&table->name);
//        reader_free(table->reader);
//        schema_free(table->schema);
//        free_(table);
//}



Table* table_new(char* name,
                 const char* alias,
                 size_t idx,
                 enum source_type source_type,
                 enum join_type join_type)
{
        Table* new_table = NULL;
        malloc_(new_table, sizeof(*new_table));

        return table_construct(new_table, name, alias, idx, source_type, join_type);
}

Table* table_construct(Table* table,
                       char* name,
                       const char* alias,
                       size_t idx,
                       enum source_type source_type,
                       enum join_type join_type)
{
        *table = (Table) {
                 { 0 }                  /* name */
                ,{ 0 }                  /* alias */
                ,reader_new()           /* reader */
                ,schema_new()           /* schema */
                ,NULL                   /* condition */
                ,vec_new_(Column*)      /* validation_list */
                ,NULL                   /* read_proc */
                ,NULL                   /* join_data */
                ,idx                    /* idx */
                ,source_type            /* source_type */
                ,join_type              /* join_type */
        };
        
        string_construct_take(&table->name, name);

        if (alias[0] == '\0') {
                string_construct_from_string(&table->alias, &table->name);
        } else {
                string_construct_from_char_ptr(&table->alias, alias);
        }

        return table; 
}

void table_free(Table* table)
{
        table_destroy(table);
        free_(table);
}

void table_destroy(Table* table)
{
        string_destroy(&table->name);
        reader_free(table->reader);
        schema_free(table->schema);
        logicgroup_free(table->condition);
        vec_free(table->validation_list);
        string_destroy(&table->alias);
        hashjoin_free(table->join_data);
        //Column** it = vec_begin(source->validation_list);
        //for (; it != vec_end(source->validation_list); ++it) {
        //        column_free(*it);
        //}
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
        if (join == NULL) {
                return;
        }
        multimap_destroy(&join->hash_data);
        free_(join);
}


size_t _guess_row_count(Table* table)
{
        size_t guess = 0;
        size_t total_length = 0;

        Reader* reader = table->reader;

        unsigned max_col_store = reader->max_col_idx;
        reader->max_col_idx = 0;

        Record rec;
        record_construct(&rec, 0);

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

        struct mmapcsv* csv = reader->reader_data;
        reader->reset__(reader);

        guess = csv->file_size / avg_len;

        return (guess < HASH_JOIN_MIN_SIZE) ? HASH_JOIN_MIN_SIZE : guess + 1;
}

void table_hash_join_init(Table* table)
{
        size_t guessed_row_count = _guess_row_count(table);

        struct hashjoin* join = table->join_data;

        multimap_construct_(
                 &join->hash_data
                ,char* /* T_ */
                ,guessed_row_count * 2
                ,HASHMAP_PROP_NOCASE | HASHMAP_PROP_RTRIM
        );
}
