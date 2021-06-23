#include "reader.h"
#include <csv.h>
#include "fql.h"
#include "misc.h"

void libcsv_reader_free(void* csv)
{
	csv_reader_close(csv);
	csv_reader_free(csv);
}

int libcsv_get_record(reader* self, record* rec)
{
	struct csv_reader* csv = self->reader_data;

	if (self->eof) {
		return csv_reader_reset(csv);
	}

	int ret =
	        csv_get_record_to(csv, rec->libcsv_rec, self->max_col_idx + 1);
	switch (ret) {
	case CSV_GOOD:
		break;
	case CSV_FAIL:
		return FQL_FAIL;
	default:
		self->eof = true;
		return EOF;
	}

	/* this should really never change...
	 * _guess_row_count relies on this
	 */
	vec_resize(rec->fields, rec->libcsv_rec->size);

	stringview* sv = vec_begin(rec->fields);
	struct csv_field* fields = rec->libcsv_rec->fields;

	int i = 0;
	for (; i < rec->libcsv_rec->size; ++i) {
		sv[i].data = fields[i].data;
		sv[i].len = fields[i].len;
	}

	/* TODO: if reading from stdin, we may need to make a copy */
	//string_strncpy(rec->rec_cpy,
	//	       rec->libcsv_rec->rec,
	//	       rec->libcsv_rec->reclen);
	//rec->rec_raw.data = rec->rec_cpy->data;
	//rec->rec_raw.len = rec->rec_cpy->size;

	rec->rec_raw.data = rec->libcsv_rec->rec;
	rec->rec_raw.len = rec->libcsv_rec->reclen;

	return FQL_GOOD;
}

int libcsv_get_record_at(reader* self, record* rec, const char* location)
{
	try_(csv_reader_goto(self->reader_data, location));
	self->eof = false;
	return libcsv_get_record(self, rec);
}

int libcsv_reset(reader* self)
{
	struct csv_reader* csv = self->reader_data;
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
