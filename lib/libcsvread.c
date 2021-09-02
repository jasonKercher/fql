#include "reader.h"
#include <csv.h>
#include "fql.h"
#include "misc.h"

void libcsv_free(void* csv)
{
	csv_reader_close(csv);
	csv_reader_free(csv);
}

int libcsv_get_record(reader* self, node* rg)
{
	record* rec = rg->data;
	struct csv_reader* csv = self->reader_data;

	if (self->eof) {
		return EOF;
	}

	if (rec->libcsv_rec == NULL) {
		rec->libcsv_rec = csv_record_new();
	}

	if (!self->random_access_file) {
		rec->offset = csv->offset;
	}

	int ret = 0;
	/* This comes from delimiter discovery in schema.c:_resolve_source */
	if (self->first_rec != NULL) {
		delete_(record, rec);
		rec = self->first_rec;
		self->first_rec = NULL;
		rg->data = rec;
		ret = CSV_GOOD;
	} else {
		ret = csv_get_record_to(csv, rec->libcsv_rec, self->max_idx + 1);
	}
	switch (ret) {
	case CSV_GOOD:
		break;
	case CSV_FAIL:
		return FQL_FAIL;
	default:
		self->eof = true;
		return EOF;
	}

	record_resize(rec, rec->libcsv_rec->size);

	stringview* sv = vec_begin(&rec->fields);
	struct csv_field* fields = rec->libcsv_rec->fields;

	int i = 0;
	for (; i < rec->libcsv_rec->size; ++i) {
		sv[i].data = fields[i].data;
		sv[i].len = fields[i].len;
	}

	rec->rec_ref.data = rec->libcsv_rec->rec;
	rec->rec_ref.len = rec->libcsv_rec->reclen;

	if (self->random_access_file) {
		rec->offset = ftello(self->random_access_file);
		fwrite(rec->rec_ref.data, 1, rec->rec_ref.len, self->random_access_file);
		fputc('\n', self->random_access_file);
	}

	rec->rec_idx = self->rec_id++;

	return FQL_GOOD;
}

int libcsv_get_record_at(reader* self, node* rg, size_t offset)
{
	if (csv_reader_seek(self->reader_data, offset) == CSV_FAIL) {
		return FQL_FAIL;
	}

	self->eof = false;
	return libcsv_get_record(self, rg);
}

int libcsv_reset(reader* self)
{
	struct csv_reader* csv = self->reader_data;
	self->rec_id = 0;
	self->eof = false;
	csv_reader_reset(self->reader_data);

	/* skip header */
	struct csv_record* libcsv_rec = csv_record_new();
	int ret = 0;
	unsigned i = 0;
	for (; i < self->skip_rows; ++i) {
		ret = csv_get_record(csv, libcsv_rec);
		if (ret == CSV_FAIL) {
			return FQL_FAIL;
		} else if (ret != CSV_GOOD) {
			break;
		}
	}
	csv_record_free(libcsv_rec);

	return ret;
}

int libcsv_reset_stdin(reader* self)
{
	reader_stop_file_backed_input(self);
	self->reset__ = libcsv_reset;
	if (csv_reader_open_mmap(self->reader_data,
	                         string_c_str(self->random_access_filename))) {
		csv_perror();
		return FQL_FAIL;
	}

	return FQL_GOOD;
}
