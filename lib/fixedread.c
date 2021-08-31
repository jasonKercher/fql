#include "reader.h"
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "fql.h"
#include "misc.h"
#include "expression.h"

fixedreader* fixedreader_construct(fixedreader* self, vec* expressions)
{
	*self = (fixedreader) {
	        NULL,        /* mmap */
	        expressions, /* expressions */
	        0,           /* offset */
	        0,           /* file_size */
	        -1,          /* fd */
	};
	return self;
}

void fixedreader_free(void* generic_data)
{
	fixedreader* self = generic_data;
	if (self->fd != -1) {
		munmap(self->mmap, self->file_size);
		close(self->fd);
	}
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
	self->offset = 0;
	return FQL_GOOD;
}

int fixedreader_reopen(reader* reader)
{
	fixedreader* self = reader->reader_data;
	if (self->mmap != NULL) {
		munmap(self->mmap, self->file_size);
		close(self->fd);
	}
	return fixedreader_open(reader, string_c_str(&reader->file_name));
}

int fixedreader_get_record(reader* reader, node* rg)
{
	if (reader->eof) {
		return EOF;
	}

	fixedreader* self = reader->reader_data;
	if (self->offset + reader->reclen > self->file_size) {
		reader->eof = true;
		return EOF;
	}

	int ret = fixedreader_get_record_at(reader, rg, self->offset);
	self->offset += reader->reclen;
	return ret;
}

int fixedreader_get_record_at(reader* reader, node* rg, size_t offset)
{
	record* rec = rg->data;
	fixedreader* self = reader->reader_data;
	const char* begin = self->mmap + offset;

	record_resize(rec, self->expressions->size);

	unsigned i = 0;
	for (; i < self->expressions->size; ++i) {
		expression** col = vec_at(self->expressions, i);
		stringview sv = {begin + (*col)->location, (*col)->width};
		vec_set(&rec->fields, i, &sv);
	}

	rec->rec_ref.data = begin;
	rec->rec_ref.len = reader->reclen; /* redundant */
	rec->rec_idx = reader->rec_id++;
	rec->offset = self->offset;

	return FQL_GOOD;
}

int fixedreader_reset(reader* reader)
{
	fixedreader* self = reader->reader_data;
	reader->eof = false;
	reader->rec_id = 0;
	self->offset = reader->reclen * reader->skip_rows;
	return FQL_GOOD;
}
