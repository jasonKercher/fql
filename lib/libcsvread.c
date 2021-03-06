#include "reader.h"
#include <csv.h>
#include "fql.h"
#include "misc.h"

void libcsv_free(void* csv)
{
	csv_reader_close(csv);
	csv_reader_free(csv);
}

int libcsv_get_record(reader* self, recgroup* rg)
{
	recgroup_resize(rg, 1);
	record* rec = recgroup_rec_begin(rg);
	struct csv_reader* csv = self->reader_data;

	if (self->eof) {
		return FQL_FAIL;
	}

	if (rec->libcsv_rec == NULL) {
		rec->libcsv_rec = csv_record_new();
	}

	int ret = csv_get_record_to(csv, rec->libcsv_rec, self->max_idx + 1);
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

	return FQL_GOOD;
}

int libcsv_get_record_at(reader* self, recgroup* rg, const char* location)
{
	try_(csv_reader_goto(self->reader_data, location));
	self->eof = false;
	return libcsv_get_record(self, rg);
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
