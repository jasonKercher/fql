#include "writer.h"
#include <csv.h>
#include "fql.h"
#include "column.h"
#include "record.h"
#include "util/stringview.h"

void libcsv_writer_free(void* writer_data)
{
	csv_writer_free(writer_data);
}

int libcsv_write_record(void* writer_data, vec* col_vec, vec* recs)
{
	csv_writer* handle = writer_data;

	column** cols = vec_begin(col_vec);
	int i = 0;
	for (; i < col_vec->size; ++i) {
		stringview sv;
		if (i > 0) {
			fputs(handle->delimiter, handle->file);
		}

		if (cols[i]->expr == EXPR_ASTERISK) {
			int quote_store = handle->quotes;
			handle->quotes = QUOTE_NONE;
			record** rec = vec_at(recs, cols[i]->src_idx);
			csv_nwrite_field(handle, (*rec)->rec_cpy->data, (*rec)->rec_cpy->size);
			handle->quotes = quote_store;
		}
		else {
			try_ (column_get_stringview(&sv, cols[i], recs));
			csv_nwrite_field(handle, sv.data, sv.len);
		}
	}

	fputs(handle->line_terminator, handle->file);

	return FQL_GOOD;
}

writer* writer_construct(writer* self)
{
	*self = (writer) {
		 WRITE_UNDEFINED        /* type */
		,NULL                   /* writer_data */
		,NULL                   /* write_record__ */
		,NULL                   /* free__ */
		,new_t_(vec, string)    /* raw_rec */
		,{ 0 }                  /* file_name */
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
	int ret = 0;
	switch(self->type) {
	case WRITE_LIBCSV:
	{
		csv_writer* data = self->writer_data;
		strncpy_(data->delimiter, delim, DELIM_LEN_MAX);
		break;
	}
	default:
		fprintf(stderr, "%d: unknown write_type\n", self->type);
	}
}


void writer_assign(writer* self)
{
	int ret = 0;
	switch(self->type) {
	case WRITE_LIBCSV:
	{
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
