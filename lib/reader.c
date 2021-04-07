#include "reader.h"

#include <sys/types.h>
#include <csv.h>

#include "fql.h"
#include "process.h"
#include "util/stringview.h"
#include "util/util.h"




Reader* reader_new()
{
	Reader* new_reader = NULL;
	malloc_(new_reader, sizeof(*new_reader));

	return reader_construct(new_reader);
}

Reader* reader_construct(Reader* reader)
{
	*reader = (Reader) {
		 READ_UNDEFINED /* type */
		,NULL           /* reader_data */
		,NULL           /* get_record__ */
		,NULL           /* free__ */
		,NULL           /* reset__ */
		,{ 0 }          /* file_name */
		,0              /* max_col_idx */
		,false          /* eof */
	};

	string_construct(&reader->file_name);

	return reader;
}

void reader_free(Reader* reader)
{
	if (reader == NULL) {
		return;
	}
	string_destroy(&reader->file_name);
	if (reader->free__) {
		reader->free__(reader->reader_data);
	}
	free_(reader);
}

char* reader_get_delim(Reader* reader)
{
	switch (reader->type) {
	case READ_LIBCSV:
		return libcsv_get_delim(reader->reader_data);
	case READ_MMAPCSV:
		return mmapcsv_get_delim(reader->reader_data);
	case READ_SUBQUERY:
		fputs("Cannot retrieve delimiter from subquery\n", stderr);
		return NULL;
	default:
		fprintf(stderr, "%d: unknown read_type\n", reader->type);
		return NULL;
	}
}

void reader_assign(Reader* reader)
{
	int ret = 0;
	switch (reader->type) {
	case READ_LIBCSV:
	{
		struct csv_reader* csv = csv_reader_new();
		reader->reader_data = csv;
		reader->free__ = &libcsv_reader_free;
		reader->get_record__ = &libcsv_get_record;
		reader->reset__ = &libcsv_reset;
		ret = csv_reader_open(csv, reader->file_name.data);
		break;
	}
	case READ_MMAPCSV:
	{
		struct mmapcsv* csv = mmapcsv_new(PROCESS_BUFFER_SIZE);
		reader->reader_data = csv;
		reader->free__ = &mmapcsv_free;
		reader->get_record__ = &mmapcsv_get_record;
		reader->reset__ = &mmapcsv_reset;
		ret = mmapcsv_open(csv, reader->file_name.data);
		break;
	}
	case READ_SUBQUERY:
		fputs("cannot assign reader for subquery\n", stderr);
		break;
	default:
		fprintf(stderr, "%d: unknown read_type\n", reader->type);
	}

	if (ret == CSV_FAIL) {
		csv_perror_exit();
	}
}

