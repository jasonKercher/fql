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

reader* reader_construct(reader* self)
{
	*self = (reader) {
	        IO_UNDEFINED, /* type */
	        NULL,         /* reader_data */
	        NULL,         /* get_record__ */
	        NULL,         /* get_record_at__ */
	        NULL,         /* free__ */
	        NULL,         /* reset__ */
	        {0},          /* file_name */
	        0,            /* max_idx */
	        0,            /* reclen */
	        0,            /* rec_id */
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

int reader_assign(reader* self, table* table, struct fql_handle* fql)
{
	switch (self->type) {
	case IO_LIBCSV: {
		csv_reader* csv = csv_reader_new();
		csv->quotes = fql->props.in_std;
		if (fql->props.in_delim[0]) {
			csv_reader_set_delim(csv, fql->props.in_delim);
		}
		self->reader_data = csv;
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
	}
	case IO_FIXED:
		self->reader_data = new_(fixedreader, table->schema->expressions);
		self->free__ = &fixedreader_free;
		self->get_record__ = &fixedreader_get_record;
		self->get_record_at__ = &fixedreader_get_record_at;
		self->reset__ = &fixedreader_reset;
		try_(fixedreader_open(self, string_c_str(&self->file_name)));
		return FQL_GOOD;
	case IO_SUBQUERY:
		self->reader_data = new_(subquery, table->subquery->op);
		self->free__ = &subquery_free;
		self->get_record__ = &subquery_get_record;
		self->get_record_at__ = &subquery_get_record_at;
		self->reset__ = &subquery_reset;
		// reader_data set in schema.c
		return FQL_GOOD;
	default:
		fprintf(stderr, "%d: unknown read_type\n", self->type);
		return FQL_FAIL;
	}
}

int reader_reopen(reader* self)
{
	switch (self->type) {
	case IO_FIXED:
		try_(fixedreader_reopen(self));
		break;
	case IO_LIBCSV:
		csv_reader_close(self->reader_data);
		if (csv_reader_open_mmap(self->reader_data,
		                         string_c_str(&self->file_name))) {
			csv_perror();
			return FQL_FAIL;
		}
		break;
	default:
		return FQL_FAIL;
	}

	self->reset__(self);

	return FQL_GOOD;
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

