#include "table.h"
#include "fql.h"
#include "misc.h"
#include "query.h"
#include "logic.h"
#include "reader.h"
#include "operation.h"
#include "expression.h"
#include "antlr/antlr.h"
#include "util/util.h"

table* table_construct(table* self,
                       char* name,
                       const char* alias,
                       size_t idx,
                       enum join_type join_type)
{
	*self = (table) {
	        .reader = new_(reader),
	        .schema = new_(schema),
	        .idx = idx,
	        .source_type = SOURCE_TABLE,
	        .join_type = join_type,
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
	        .subquery = subquery,
	        .reader = new_(reader),
	        .idx = idx,
	        .source_type = SOURCE_SUBQUERY,
	        .join_type = join_type,
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

int table_reset(table* self, bool has_executed)
{
	if (self->must_reopen) {
		try_(reader_reopen(self->reader));
		try_(self->reader->reset__(self->reader));
		hashjoin_reset(self->join_data);
	} else if (has_executed) {
		try_(self->reader->reset__(self->reader));
		hashjoin_reset(self->join_data);
	}

	return FQL_GOOD;
}

int table_resolve_schema(table* self, struct fqlhandle* fql)
{
	if (self->schema->is_preresolved) {
		return FQL_GOOD;
	}
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
	        .state = SIDE_RIGHT,
	        .comp_type = FIELD_UNDEFINED,
	};

	return join;
}

void hashjoin_destroy(struct hashjoin* join)
{
	delete_if_exists_(multimap, join->hash_data);
}

void hashjoin_reset(struct hashjoin* join)
{
	if (join != NULL) {
		multimap_clear(join->hash_data);
		join->state = SIDE_RIGHT;
	}
}

/* The sole purpose of this function is to derive a good
 * starting size for the hashmap that will avoid resizing.
 */
size_t _guess_row_count(table* self)
{
	size_t guess = 0;
	size_t total_length = 0;

	reader* reader = self->reader;

	unsigned max_expr_store = reader->max_idx;
	reader->max_idx = 0;

	node rg = {0};
	rg.data = new_(record, 0);

	int i = 0;
	for (; i < 10; ++i) {
		if (reader->get_record__(reader, &rg) != FQL_GOOD) {
			break;
		}
		/* ignore header */
		if (i) {
			record* rec = rg.data;
			total_length += rec->rec_ref.len;
		}
	}

	if (i == 0) {
		return HASH_JOIN_MIN_SIZE;
	}

	delete_(record, rg.data);

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
	size_t guessed_row_count = HASH_JOIN_MIN_SIZE;
	if (!self->is_stdin && self->subquery == NULL) {
		guessed_row_count = _guess_row_count(self);
	}

	struct hashjoin* join = self->join_data;

	if (join->comp_type == FIELD_STRING) {
		join->hash_data = new_t_(multimap,
		                         size_t,
		                         guessed_row_count * 2,
		                         HASHMAP_PROP_NOCASE | HASHMAP_PROP_RTRIM);
	} else {
		join->hash_data = new_t_(multimap,
		                         size_t,
		                         guessed_row_count * 2,
		                         HASHMAP_PROP_DEFAULT);
	}
}
