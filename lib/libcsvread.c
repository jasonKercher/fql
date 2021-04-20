#include "reader.h"
#include <csv.h>
#include "fql.h"

void libcsv_reader_free(void* csv)
{
	csv_reader_free(csv);
}

char* libcsv_get_delim(struct csv_reader* csv)
{
	return csv->delimiter;
}

int libcsv_get_record(Reader* reader, Record* rec)
{
	struct csv_reader* csv = reader->reader_data;

	if (reader->eof) {
		return csv_reader_reset(csv);
	}

	int ret = csv_get_record_to(csv, rec->libcsv_rec, reader->max_col_idx+1);
	switch (ret) {
	case CSV_GOOD:
		break;
	case CSV_FAIL:
		return FQL_FAIL;
	default:
		reader->eof = true;
		return EOF;
	}

	/* This should really never change...
	 * _guess_row_count relies on this
	 */
	vec_resize(rec->fields, rec->libcsv_rec->size);

	StringView* sv = vec_begin(rec->fields);
	char** fields = rec->libcsv_rec->fields;

	int i = 0;
	for (; i < rec->libcsv_rec->size; ++i) {
		sv[i].data = fields[i];
		/* TODO: (libcsv) would be nice if it created a trailing
		 *       pointer that pointed at the ending NULL
		 *       terminator. Then, this if block can go.
		 */
		if (i != rec->libcsv_rec->size - 1) {
			sv[i].len = fields[i+1] - fields[i] - 1;
		} else {
			sv[i].len = strlen(fields[i]);
		}
	}

	string_strncpy(rec->rec_cpy, rec->libcsv_rec->raw, rec->libcsv_rec->raw_len);
	rec->rec_raw.data = rec->libcsv_rec->raw;
	rec->rec_raw.len = rec->libcsv_rec->raw_len;

	return FQL_GOOD;
}

int libcsv_reset(Reader* reader)
{
	struct csv_reader* csv = reader->reader_data;
	reader->eof = false;
	csv_reader_reset(reader->reader_data);

	/* Skip header */
	struct csv_record* libcsv_rec = csv_record_new();
	int ret = csv_get_record_to(csv, libcsv_rec, reader->max_col_idx+1);
	csv_record_free(libcsv_rec);

	return ret;
}
