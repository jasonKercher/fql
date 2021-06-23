#include "reader.h"

#include <sys/types.h>
#include <csv.h>
#include "fql.h"
#include "query.h"
#include "process.h"
#include "table.h"
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

void reader_assign(reader* self, table* table)
{
	int ret = 0;
	switch (self->type) {
	case READ_LIBCSV: {
		struct csv_reader* csv = csv_reader_new();
		self->reader_data = csv;
		self->free__ = &libcsv_reader_free;
		self->get_record__ = &libcsv_get_record;
		self->get_record_at__ = &libcsv_get_record_at;
		self->reset__ = &libcsv_reset;
		ret = csv_reader_open_mmap(csv, self->file_name.data);
	} break;
	case READ_FIXED_BYTE:
		fputs("not supporting FIXED BYTE tables quite yet\n", stderr);
		exit(EXIT_FAILURE);
		break;
	case READ_SUBQUERY:
		//self->free__ = &query_free;
		self->get_record__ = &fqlselect_subquery_record;
		self->get_record_at__ = NULL; /* TODO */
		self->reset__ = &fqlselect_subquery_reset;
		self->reader_data = table->schema;
		break;
	default:
		fprintf(stderr, "%d: unknown read_type\n", self->type);
	}

	if (ret == CSV_FAIL) {
		csv_perror_exit();
	}
}
