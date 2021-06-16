#include "table.h"
#include "query.h"
#include "column.h"
#include "reader.h"
#include "logic.h"
#include "antlr/antlr.h"
#include "util/util.h"

#define SUBQUERY_HASH_TABLE_INITIAL_SIZE 512

table* table_construct(table* self,
                       char* name,
                       const char* alias,
                       size_t idx,
                       enum join_type join_type)
{
	*self = (table) {
	        {0},                  /* name */
	        {0},                  /* alias */
	        NULL,                 /* subquery */
	        new_(reader),         /* reader */
	        new_(schema),         /* schema */
	        NULL,                 /* condition */
	        NULL,                 /* read_proc */
	        NULL,                 /* join_data */
	        idx,                  /* idx */
	        SOURCE_TABLE,         /* source_type */
	        join_type             /* join_type */
	};

	string_construct_take(&self->name, name);

	if (alias[0] == '\0') {
		string_construct_from_string(&self->alias, &self->name);
	} else {
		string_construct_from_char_ptr(&self->alias, alias);
	}

	return self;
}

table* table_construct_subquery(table* self,
                                query* subquery,
                                const char* alias,
                                size_t idx,
                                enum join_type join_type)
{
	*self = (table) {
	        {0},             /* name */
	        {0},             /* alias */
	        subquery,        /* subquery */
	        new_(reader),    /* reader */
	        NULL,            /* schema */
	        NULL,            /* condition */
	        NULL,            /* read_proc */
	        NULL,            /* join_data */
	        idx,             /* idx */
	        SOURCE_SUBQUERY, /* source_type */
	        join_type        /* join_type */
	};

	string_construct(&self->name);

	if (alias[0] == '\0') {
		string_construct(&self->alias);
	} else {
		string_construct_from_char_ptr(&self->alias, alias);
	}

	return self;
}

void table_destroy(table* self)
{
	string_destroy(&self->name);
	delete_(reader, self->reader);
	delete_if_exists_(logicgroup, self->condition);
	string_destroy(&self->alias);
	delete_if_exists_(hashjoin, self->join_data);
	if (self->subquery == NULL) {
		delete_(schema, self->schema);
	}
	delete_if_exists_(query, self->subquery);
}

const char* table_get_delim(table* self)
{
	reader* reader = self->reader;
	switch (reader->type) {
	case READ_LIBCSV:
		return csv_reader_get_delim(reader->reader_data);
	case READ_SUBQUERY:
		return table_get_delim(vec_begin(self->subquery->sources));
	default:
		fprintf(stderr, "%d: unknown read_type\n", reader->type);
		return NULL;
	}
}

hashjoin* hashjoin_construct(hashjoin* join)
{
	*join = (struct hashjoin) {
	        NULL,       /* hash_data */
	        NULL,       /* left_col */
	        NULL,       /* right_col */
	        NULL,       /* recs */
	        SIDE_RIGHT, /* state */
	        0           /* rec_idx */
	};

	return join;
}

void hashjoin_destroy(struct hashjoin* join)
{
	delete_if_exists_(multimap, join->hash_data);
}

size_t _guess_row_count(table* self)
{
	/* just allow the hash map to grow with it */
	if (self->subquery != NULL) {
		return SUBQUERY_HASH_TABLE_INITIAL_SIZE / 2;
	}

	size_t guess = 0;
	size_t total_length = 0;

	reader* reader = self->reader;

	unsigned max_col_store = reader->max_col_idx;
	reader->max_col_idx = 0;

	record rec;
	record_construct(&rec, 0, 0, false);

	int i = 0;
	for (; i < 10; ++i) {
		if (reader->get_record__(reader, &rec) != FQL_GOOD) {
			break;
		}
		/* ignore header */
		if (i) {
			total_length += rec.rec_raw.len;
		}
	}

	if (i == 0) {
		return HASH_JOIN_MIN_SIZE;
	}

	record_destroy(&rec);

	reader->max_col_idx = max_col_store;

	double avg_len = total_length / (double)i - 1;
	if (avg_len < 1)
		avg_len = 1;

	/* TODO: schema assumed here */
	struct csv_reader* csv = reader->reader_data;
	size_t file_size = csv_reader_get_file_size(csv);
	reader->reset__(reader);

	guess = file_size / avg_len;

	return (guess < HASH_JOIN_MIN_SIZE) ? HASH_JOIN_MIN_SIZE : guess + 1;
}

void table_hash_join_init(table* self)
{
	size_t guessed_row_count = _guess_row_count(self);

	struct hashjoin* join = self->join_data;

	join->hash_data = new_t_(multimap,
	                         char*, /* T_ */
	                         guessed_row_count * 2,
	                         HASHMAP_PROP_NOCASE | HASHMAP_PROP_RTRIM);
}
