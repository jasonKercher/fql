#include "writer.h"
#include <csv.h>
#include "io.h"
#include "fql.h"
#include "misc.h"

writer* writer_construct(writer* self)
{
	*self = (writer) {
	        IO_UNDEFINED,        /* type */
	        NULL,                /* writer_data */
	        NULL,                /* write_record__ */
	        new_t_(vec, string), /* raw_rec */
	        {0}                  /* file_name */
	};

	string_construct(&self->file_name);

	/* TODO: this should not be here. this should
	 *       be dependant on output schema
	 */
	self->type = IO_LIBCSV;
	writer_assign(self);

	return self;
}

void writer_destroy(writer* self)
{
	switch (self->type) {
	case IO_LIBCSV:
		csv_writer_free(self->writer_data);
		break;
	case IO_FIXED:
		delete_(fixedwriter, self->writer_data);
		break;
	default:;
	}
	string_destroy(&self->file_name);
	string* s = vec_begin(self->raw_rec);
	for (; s != vec_end(self->raw_rec); ++s) {
		string_destroy(s);
	}
	delete_(vec, self->raw_rec);
}

int writer_open(writer* self, const char* file_name)
{
	switch (self->type) {
	case IO_LIBCSV:
		if (csv_writer_open(self->writer_data, file_name) == CSV_FAIL) {
			return FQL_FAIL;
		}
		return FQL_GOOD;
	case IO_FIXED:
		try_(fixedwriter_open(self->writer_data, file_name));
		return FQL_GOOD;
	default:
		return FQL_GOOD;
	}
}

int writer_close(writer* self)
{
	switch (self->type) {
	case IO_LIBCSV:
		if (csv_writer_close(self->writer_data) == CSV_FAIL) {
			return FQL_FAIL;
		}
		return FQL_GOOD;
	case IO_FIXED:
		try_(fixedwriter_close(self->writer_data));
		return FQL_GOOD;
	default:
		return FQL_FAIL;
	}
}

FILE* writer_get_file(writer* self)
{
	switch (self->type) {
	case IO_LIBCSV:
		return csv_writer_get_file(self->writer_data);
	case IO_FIXED:
		return fixedwriter_get_file(self->writer_data);
	default:
		return NULL;
	}
}

char* writer_take_filename(writer* self)
{
	switch (self->type) {
	case IO_LIBCSV:
		return csv_writer_detach_filename(self->writer_data);
	case IO_FIXED:
		return fixedwriter_take_filename(self->writer_data);
	default:
		return NULL;
	}
}

const char* writer_get_tempname(writer* self)
{
	switch (self->type) {
	case IO_LIBCSV:
		break;
	case IO_FIXED:
		return fixedwriter_get_tempname(self->writer_data);
	default:
		return NULL;
	}

	csv_writer* csv = self->writer_data;
	if (!csv_writer_isopen(csv)) {
		if (csv_writer_mktmp(csv) == CSV_FAIL) {
			return NULL;
		}
	}

	return csv_writer_get_filename(csv);
}

void writer_set_delimiter(writer* self, const char* delim)
{
	switch (self->type) {
	case IO_LIBCSV: {
		csv_writer* csv = self->writer_data;
		csv_writer_set_delim(csv, delim);
		break;
	}
	case IO_FIXED: /* no delimiter for this */
		break;
	default:
		fprintf(stderr, "%d: unknown write_type\n", self->type);
	}
}

void writer_assign(writer* self)
{
	switch (self->type) {
	case IO_LIBCSV: {
		csv_writer* data = csv_writer_new();
		self->writer_data = data;
		self->write_record__ = &libcsv_write_record;
		break;
	}
	case IO_FIXED: {
		fixedwriter* data = new_(fixedwriter);
		self->writer_data = data;
		self->write_record__ = &fixedwriter_write_record;
		break;
	}
	default:
		fprintf(stderr, "%d: unknown write_type\n", self->type);
	}
}
