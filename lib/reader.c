#include "reader.h"

#include <sys/types.h>
#include <csv.h>
#include "fql.h"
#include "misc.h"
#include "query.h"
#include "table.h"
#include "process.h"
#include "fqlselect.h"
#include "util/stringview.h"
#include "util/util.h"

int _subquery_get_record(reader* reader, record* rec);

reader* reader_construct(reader* self)
{
	*self = (reader) {
	        IO_UNDEFINED, /* type */
	        NULL,         /* reader_data */
	        NULL,         /* subquery_recs */
	        NULL,         /* get_record__ */
	        NULL,         /* get_record_at__ */
	        NULL,         /* free__ */
	        NULL,         /* reset__ */
	        {0},          /* file_name */
	        0,            /* max_col_idx */
	        0,            /* reclen */
	        1,            /* skip_rows */
	        false         /* eof */
	};

	string_construct(&self->file_name);

	return self;
}

void reader_destroy(reader* self)
{
	string_destroy(&self->file_name);
	if (self->free__) {
		self->free__(self->reader_data);
	}
}

int reader_assign(reader* self, table* table)
{
	switch (self->type) {
	case IO_LIBCSV:
		self->reader_data = csv_reader_new();
		self->free__ = &libcsv_free;
		self->get_record__ = &libcsv_get_record;
		self->get_record_at__ = &libcsv_get_record_at;
		self->reset__ = &libcsv_reset;
		if (csv_reader_open_mmap(self->reader_data,
		                         string_c_str(&self->file_name))) {
			csv_perror();
			return FQL_FAIL;
		}
		return FQL_GOOD;
	case IO_FIXED:
		self->reader_data = new_(fixedreader, table->schema->columns);
		self->free__ = &fixedreader_free;
		self->get_record__ = &fixedreader_get_record;
		self->get_record_at__ = &fixedreader_get_record_at;
		self->reset__ = &fixedreader_reset;
		fail_if_(fixedreader_open(self, string_c_str(&self->file_name)));
		return FQL_GOOD;
	case IO_SUBQUERY:
		//self->free__ = &query_free;
		self->get_record__ = &_subquery_get_record;
		//self->get_record_at__ = NULL; /* TODO */
		//self->reset__ = &_subquery_reset;
		//self->reader_data = table->schema;
		return FQL_GOOD;
	default:
		fprintf(stderr, "%d: unknown read_type\n", self->type);
		return FQL_FAIL;
	}
}

size_t reader_get_file_size(reader* self)
{
	switch (self->type) {
	case IO_LIBCSV:
		return csv_reader_get_file_size(self->reader_data);
	case IO_FIXED:
		return ((fixedreader*)(self->reader_data))->file_size;
	case IO_SUBQUERY:
	default:
		return 0;
	}
}

/* This becomes a big copy operation because.... 
 * NO it the fuck doesn't... TODO!!
 */
int _subquery_get_record(reader* reader, record* rec)
{
	fqlselect* select = reader->reader_data;
	vec* sub_col_vec = select->_selection_cols;

	unsigned i = 0;
	column** sub_cols = vec_begin(sub_col_vec);

	for (; i < sub_col_vec->size; ++i) {
		stringview sv;
		column_get_stringview(&sv, sub_cols[i], sub_recs);
		string* s = vec_at(rec->_field_data, i);
		string_copy_from_stringview(s, &sv);
		stringview* rec_sv = vec_at(rec->fields, i);
		rec_sv->data = s->data;
		rec_sv->len = s->size;
	}

	return FQL_GOOD;
}
