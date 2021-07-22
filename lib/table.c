#include "table.h"
#include "fql.h"
#include "query.h"
#include "expression.h"
#include "reader.h"
#include "logic.h"
#include "operation.h"
#include "antlr/antlr.h"
#include "util/util.h"

/**
 * When building a hash table for a file, we can
 * get a size estimate from the file and avoid
 * resizing the hashmap.  If it is a subquery, we
 * have no such information.
 */
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

int table_resolve_schema(table* self, struct fql_handle* fql)
{
	reader* reader = self->reader;
	const char* delim = NULL;
	switch (reader->type) {
	case IO_LIBCSV:
		delim = csv_reader_get_delim(reader->reader_data);
		self->schema->write_io_type = IO_LIBCSV;
		break;
	case IO_SUBQUERY: {
		schema* sub_schema = op_get_schema(self->subquery->op);
		if (sub_schema == self->schema) {
			return FQL_GOOD;
		}
		delim = sub_schema->delimiter;
		self->schema->write_io_type = sub_schema->write_io_type;
		break;
	}
	case IO_FIXED:
		self->schema->write_io_type = IO_FIXED;
		return FQL_GOOD;
	default:
		fprintf(stderr, "%d: unknown io_type\n", reader->type);
		return FQL_FAIL;
	}

	//if (fql->props.in_delim[0]) {
	//	schema_set_delim(self->schema, fql->props.in_delim);
	//} else {
	schema_set_delim(self->schema, delim);
	//}
	return FQL_GOOD;
}

hashjoin* hashjoin_construct(hashjoin* join)
{
	*join = (struct hashjoin) {
	        NULL,       /* hash_data */
	        NULL,       /* left_expr */
	        NULL,       /* right_expr */
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

	unsigned max_expr_store = reader->max_idx;
	reader->max_idx = 0;

	recgroup* rg = new_(recgroup, 0);

	int i = 0;
	for (; i < 10; ++i) {
		if (reader->get_record__(reader, rg) != FQL_GOOD) {
			break;
		}
		/* ignore header */
		if (i) {
			record* rec = recgroup_rec_begin(rg);
			total_length += rec->rec_ref.len;
		}
	}

	if (i == 0) {
		return HASH_JOIN_MIN_SIZE;
	}

	//record_destroy(&rec);
	delete_(recgroup, rg);

	reader->max_idx = max_expr_store;

	double avg_len = total_length / (double)i - 1;
	if (avg_len < 1)
		avg_len = 1;

	size_t file_size = reader_get_file_size(reader);
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
