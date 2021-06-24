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
	        READ_UNDEFINED, /* type */
	        NULL,           /* reader_data */
	        NULL,           /* subquery_recs */
	        NULL,           /* get_record__ */
	        NULL,           /* get_record_at__ */
	        NULL,           /* free__ */
	        NULL,           /* reset__ */
	        {0},            /* file_name */
	        0,              /* max_col_idx */
	        0,              /* reclen */
	        1,              /* skip_rows */
	        false           /* eof */
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
	case READ_LIBCSV:
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
	case READ_FIXED_BYTE:
		self->reader_data = new_(fixedbyte, table->schema->columns);
		self->free__ = &fixedbyte_free;
		self->get_record__ = &fixedbyte_get_record;
		self->get_record_at__ = &fixedbyte_get_record_at;
		self->reset__ = &fixedbyte_reset;
		fail_if_(fixedbyte_open(self, string_c_str(&self->file_name)));
		return FQL_GOOD;
	case READ_SUBQUERY:
		//self->free__ = &query_free;
		self->get_record__ = &fqlselect_subquery_record;
		self->get_record_at__ = NULL; /* TODO */
		self->reset__ = &fqlselect_subquery_reset;
		self->reader_data = table->schema;
		return FQL_GOOD;
	default:
		fprintf(stderr, "%d: unknown read_type\n", self->type);
		return FQL_FAIL;
	}
}
