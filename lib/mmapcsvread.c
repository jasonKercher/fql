#include "reader.h"
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <csv.h>
#include "fql.h"


struct mmapcsv* mmapcsv_new(size_t buflen)
{
	struct mmapcsv* new_data = NULL;
	malloc_(new_data, sizeof(*new_data));

	return mmapcsv_construct(new_data, buflen);
}

struct mmapcsv* mmapcsv_construct(struct mmapcsv* csv_data, size_t buflen)
{
	*csv_data = (struct mmapcsv) {
		 csv_reader_new()       /* csv_handle */
		,{ NULL, 0 }            /* current */
		,vec_new_(stringview)   /* raw */
		,NULL                   /* rec_map */
		,NULL                   /* mmap_base */
		,0                      /* mp */
		,0                      /* file_size */
		,0                      /* fd */
	};

	return csv_data;
}

void mmapcsv_free(void* reader_data)
{
	struct mmapcsv* csv = reader_data;
	csv_reader_free(csv->csv_handle);

	if (munmap(csv->mmap_base, csv->file_size)) {
		perror("munmap");
	}
	close(csv->fd);
	vec_free(csv->raw);
	free_(csv);
}

char* mmapcsv_get_delim(struct mmapcsv* csv)
{
	return csv->csv_handle->delimiter;
}

int mmapcsv_open(struct mmapcsv* csv, const char* file_name)
{
	csv->fd = open(file_name, O_RDONLY);
	if (csv->fd == -1) {
		perror(file_name);
		return FQL_FAIL;
	}

	struct stat sb;
	if (fstat(csv->fd, &sb) == -1) {
		perror(file_name);
		return FQL_FAIL;
	}

	csv->file_size = sb.st_size;

	csv->mmap_base = mmap(NULL, csv->file_size, PROT_READ, MAP_PRIVATE, csv->fd, 0);
	if (csv->mmap_base == MAP_FAILED) {
		perror("file_name");
		return FQL_FAIL;
	}

	madvise(csv->mmap_base, csv->file_size, MADV_SEQUENTIAL);
	csv->mp = csv->mmap_base;
	return FQL_GOOD;
}

int mmapcsv_getline(reader* reader)
{
	struct mmapcsv* csv = reader->reader_data;

	/* string view will point to the record in the raw data */
	csv->current.data = csv->mp;

	while (csv->mp - csv->mmap_base < csv->file_size) {
		switch (*csv->mp) {
		case '\n':
			csv->current.len = csv->mp - csv->current.data;
			++csv->mp;
			vec_push_back(csv->raw, &csv->current);
			return FQL_GOOD;
		case '\r':
			csv->current.len = csv->mp - csv->current.data;
			if (csv->mp - csv->mmap_base + 1 < csv->file_size &&
			    *(++csv->mp) == '\n') {
				++csv->mp;
			}
			vec_push_back(csv->raw, &csv->current);
			return FQL_GOOD;
		default:
			++csv->mp;
		}
	}

	reader->eof = true;
	return EOF;
}

int mmapcsv_get_record(reader* reader, record* rec)
{
	struct mmapcsv* csv = reader->reader_data;
	if (reader->eof) {
		mmapcsv_reset(reader);
	}

	int ret = mmapcsv_getline(reader);
	if (ret) {
		return ret;
	}

	ret = csv_nparse_to(csv->csv_handle,
			    rec->libcsv_rec,
			    csv->current.data,
			    csv->current.len,
			    reader->max_col_idx+1);
	switch (ret) {
	case CSV_GOOD:
		break;
	case CSV_FAIL:
		return FQL_FAIL;
	}

	/* this should really never change unless we
	 * want this to mean something (like NULls).
	 */
	vec_resize(rec->fields, rec->libcsv_rec->size);

	stringview* sv = vec_begin(rec->fields);
	char** fields = rec->libcsv_rec->fields;

	int i = 0;
	for (; i < rec->libcsv_rec->size; ++i) {
		sv[i].data = fields[i];
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

int mmapcsv_get_record_at(reader* reader, record* rec, char* location)
{
	struct mmapcsv* csv = reader->reader_data;
	reader->eof = false;
	csv->mp = location;
	return mmapcsv_get_record(reader, rec);
}

int mmapcsv_reset(reader* reader)
{
	struct mmapcsv* csv = reader->reader_data;
	reader->eof = false;
	csv->mp = csv->mmap_base;
	/* skip header */
	return mmapcsv_getline(reader);
}
