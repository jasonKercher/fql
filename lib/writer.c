#include "writer.h"
#include <csv.h>
#include "fql.h"
#include "column.h"
#include "record.h"
#include "misc.h"
#include "util/stringview.h"

void libcsv_writer_free(void* writer_data)
{
	csv_writer_free(writer_data);
}

int libcsv_write_record(void* writer_data,
                        vec* col_vec,
                        vec* recs,
                        FILE* outstream)
{
	csv_writer* handle = writer_data;

	FILE* store_stream = NULL;
	if (outstream == NULL) {
		outstream = csv_writer_get_file(handle);
	} else {
		store_stream = csv_writer_get_file(handle);
		csv_writer_set_file(handle, outstream);
	}

	const struct csv_field delim = csv_writer_get_delim_field(handle);
	const struct csv_field terminator =
	        csv_writer_get_terminator_field(handle);
	int len = 0;

	column** cols = vec_begin(col_vec);
	unsigned i = 0;
	for (; i < col_vec->size; ++i) {
		stringview sv;
		if (i > 0) {
			fputs(delim.data, outstream);
			len += delim.len;
		}

		if (cols[i]->expr == EXPR_ASTERISK) {
			int quote_store = handle->quotes;
			handle->quotes = QUOTE_NONE;
			record** rec = vec_at(recs, cols[i]->src_idx);
			struct csv_field field = {(*rec)->rec_raw.data,
			                          (*rec)->rec_raw.len};
			len += csv_write_field(handle, &field);
			handle->quotes = quote_store;
		} else {
			try_(column_get_stringview(&sv, cols[i], recs));
			struct csv_field field = {sv.data, sv.len};
			len += csv_write_field(handle, &field);
		}
	}

	fputs(terminator.data, outstream);
	len += terminator.len;

	if (store_stream) {
		csv_writer_set_file(handle, store_stream);
	}

	return len;
}

writer* writer_construct(writer* self)
{
	*self = (writer) {
	        WRITE_UNDEFINED,     /* type */
	        NULL,                /* writer_data */
	        NULL,                /* write_record__ */
	        NULL,                /* free__ */
	        new_t_(vec, string), /* raw_rec */
	        {0}                  /* file_name */
	};

	string_construct(&self->file_name);

	/* TODO: this should not be here. this should
	 *       be dependant on output schema
	 */
	self->type = WRITE_LIBCSV;
	writer_assign(self);

	return self;
}

void writer_destroy(writer* self)
{
	if (self->free__) {
		self->free__(self->writer_data);
	}
	string_destroy(&self->file_name);
	string* s = vec_begin(self->raw_rec);
	for (; s != vec_end(self->raw_rec); ++s) {
		string_destroy(s);
	}
	delete_(vec, self->raw_rec);
}

void writer_set_delimiter(writer* self, const char* delim)
{
	switch (self->type) {
	case WRITE_LIBCSV: {
		csv_writer* csv = self->writer_data;
		csv_writer_set_delim(csv, delim);
		break;
	}
	default:
		fprintf(stderr, "%d: unknown write_type\n", self->type);
	}
}

void writer_assign(writer* self)
{
	switch (self->type) {
	case WRITE_LIBCSV: {
		csv_writer* data = csv_writer_new();
		self->writer_data = data;
		self->write_record__ = &libcsv_write_record;
		self->free__ = &libcsv_writer_free;
		break;
	}
	default:
		fprintf(stderr, "%d: unknown write_type\n", self->type);
	}
}
