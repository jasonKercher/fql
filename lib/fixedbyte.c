#include "reader.h"
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "fql.h"
#include "column.h"

fixedbyte* fixedbyte_construct(fixedbyte* self, vec* columns)
{
	*self = (fixedbyte) {
	        NULL,    /* mmap */
	        NULL,    /* iter */
	        columns, /* columns */
	        0,       /* file_size */
	        0,       /* fd */
	};
	return self;
}

void fixedbyte_free(void* generic_data)
{
	fixedbyte* self = generic_data;
	free_(self);
}

int fixedbyte_open(reader* reader, const char* file_name)
{
	fixedbyte* self = reader->reader_data;
	self->fd = open(file_name, O_RDONLY);
	if (self->fd == -1) {
		perror(file_name);
		return FQL_FAIL;
	}

	struct stat sb;
	if (fstat(self->fd, &sb) == -1) {
		perror(file_name);
		return FQL_FAIL;
	}

	/* TODO test this */
	if (sb.st_size == 0) {
		return FQL_GOOD;
	}

	self->file_size = sb.st_size;
	self->mmap = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, self->fd, 0);

	if (self->mmap == MAP_FAILED) {
		perror(file_name);
		return FQL_FAIL;
	}
	madvise(self->mmap, sb.st_size, MADV_SEQUENTIAL);
	self->iter = self->mmap;
	return FQL_GOOD;
}

int fixedbyte_get_record(reader* reader, record* rec)
{
	if (reader->eof) {
		return FQL_FAIL;
	}

	fixedbyte* self = reader->reader_data;
	const char* end = self->mmap + self->file_size;
	if (self->iter + reader->reclen >= end) {
		reader->eof = true;
		return EOF;
	}

	int ret = fixedbyte_get_record_at(reader, rec, self->iter);
	self->iter += reader->reclen;
	return ret;
}

int fixedbyte_get_record_at(reader* reader, record* rec, const char* begin)
{
	fixedbyte* self = reader->reader_data;

	vec_clear(rec->fields);

	column** it = vec_begin(self->columns);
	for (; it != vec_end(self->columns); ++it) {
		stringview sv = {begin + (*it)->location, (*it)->width};
		vec_push_back(rec->fields, &sv);
	}

	rec->rec_raw.data = begin;
	rec->rec_raw.len = reader->reclen; /* redundant */

	return FQL_GOOD;
}

int fixedbyte_reset(reader* reader)
{
	fixedbyte* self = reader->reader_data;
	self->iter = self->mmap + reader->reclen * reader->skip_rows;
	return FQL_GOOD;
}
