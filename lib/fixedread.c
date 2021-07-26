#include "reader.h"
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "fql.h"
#include "expression.h"

fixedreader* fixedreader_construct(fixedreader* self, vec* expressions)
{
	*self = (fixedreader) {
	        NULL,        /* mmap */
	        NULL,        /* iter */
	        expressions, /* expressions */
	        0,           /* file_size */
	        0,           /* fd */
	};
	return self;
}

void fixedreader_free(void* generic_data)
{
	fixedreader* self = generic_data;
	free_(self);
}

int fixedreader_open(reader* reader, const char* file_name)
{
	fixedreader* self = reader->reader_data;
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

int fixedreader_get_record(reader* reader, node* rg)
{
	if (reader->eof) {
		return FQL_FAIL;
	}

	fixedreader* self = reader->reader_data;
	const char* end = self->mmap + self->file_size;
	if (self->iter + reader->reclen > end) {
		reader->eof = true;
		return EOF;
	}

	int ret = fixedreader_get_record_at(reader, rg, self->iter);
	self->iter += reader->reclen;
	return ret;
}

int fixedreader_get_record_at(reader* reader, node* rg, const char* begin)
{
	record* rec = rg->data;
	//node_resize(rg, 1);
	fixedreader* self = reader->reader_data;

	record_resize(rec, self->expressions->size);

	unsigned i = 0;
	for (; i < self->expressions->size; ++i) {
		expression** col = vec_at(self->expressions, i);
		stringview sv = {begin + (*col)->location, (*col)->width};
		vec_set(&rec->fields, i, &sv);
	}

	rec->rec_ref.data = begin;
	rec->rec_ref.len = reader->reclen; /* redundant */

	return FQL_GOOD;
}

int fixedreader_reset(reader* reader)
{
	fixedreader* self = reader->reader_data;
	reader->eof = false;
	self->iter = self->mmap + reader->reclen * reader->skip_rows;
	return FQL_GOOD;
}
