#include "writer.h"
#include "fqlhandle.h"
#include <csv.h>
#include "fql.h"
#include "misc.h"

void _assign(writer*, struct fqlhandle*);

writer* writer_construct(writer* self, enum io io, struct fqlhandle* fql)
{
	*self = (writer) {
	        .type = io,
	        .raw_rec = new_t_(vec, string),
	        .strict = fql->props.strictness,
	};

	string_construct(&self->file_name);

	/* TODO: this should not be here. this should
	 *       be dependant on output schema
	 */
	_assign(self, fql);

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

void _assign(writer* self, struct fqlhandle* fql)
{
	switch (self->type) {
	case IO_LIBCSV: {
		csv_writer* csv = csv_writer_new();
		csv->quotes = fql->props.out_std;
		self->writer_data = csv;
		self->write_record__ = &libcsv_write_record;
		break;
	}
	case IO_FIXED: {
		self->writer_data = new_(fixedwriter);
		self->write_record__ = &fixedwriter_write_record;
		break;
	}
	default:
		fprintf(stderr, "%d: unknown write_type\n", self->type);
	}
}

int writer_open(writer* self, const char* file_name)
{
	switch (self->type) {
	case IO_LIBCSV:
		if (csv_writer_open(self->writer_data, file_name) == CSV_FAIL) {
			csv_perror();
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
	if (self == NULL) {
		return FQL_GOOD;
	}
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

void writer_set_rec_terminator(writer* self, const char* term)
{
	switch (self->type) {
	case IO_LIBCSV: {
		csv_writer* csv = self->writer_data;
		csv_writer_set_line_ending(csv, term);
		break;
	}
	case IO_FIXED: /* no record terminator for this */
		break;
	default:
		fprintf(stderr, "%d: unknown write_type\n", self->type);
	}
}
