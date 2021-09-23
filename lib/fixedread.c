#include "reader.h"
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "fql.h"
#include "misc.h"
#include "expression.h"

fixedread* fixedread_construct(fixedread* self, vec* expressions)
{
	*self = (fixedread) {
	        .expressions = expressions,
	        .fd = -1,
	};
	return self;
}

void fixedread_free(void* generic_data)
{
	fixedread* self = generic_data;
	if (self->fd != -1) {
		munmap(self->mmap, self->file_size);
		close(self->fd);
	}
	free_(self);
}

int fixedread_open(reader* reader, const char* file_name)
{
	fixedread* self = reader->reader_data;
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

int fixedread_reopen(reader* reader)
{
	fixedread* self = reader->reader_data;
	if (self->mmap != NULL) {
		munmap(self->mmap, self->file_size);
		close(self->fd);
	}
	return fixedread_open(reader, string_c_str(&reader->file_name));
}

int fixedread_get_record_stdin(reader* reader, node* rg)
{
	record* rec = rg->data;
	fixedread* self = reader->reader_data;

	record_resize(rec, self->expressions->size);

	if (rec->_cpy == NULL) {
		/* No NULL terminator added, but + 1 for fun... */
		rec->_cpy = malloc_(reader->reclen);
	}

	size_t bytes_read = fread(rec->_cpy, 1, reader->reclen, stdin);
	if (bytes_read != reader->reclen) {
		if (feof(stdin)) {
			reader->eof = true;
			return EOF;
		}
	}

	unsigned i = 0;
	for (; i < self->expressions->size; ++i) {
		expression** col = vec_at(self->expressions, i);
		stringview sv = {(char*)rec->_cpy + (*col)->location, (*col)->width};
		vec_set(&rec->fields, i, &sv);
	}

	rec->rec_ref.data = rec->_cpy;
	rec->rec_ref.len = reader->reclen; /* redundant */
	rec->rec_idx = reader->rec_id++;

	if (reader->random_access_file) {
		fwrite(rec->rec_ref.data,
		       1,
		       rec->rec_ref.len,
		       reader->random_access_file);
	}

	rec->offset = self->offset;
	self->offset += reader->reclen;

	return FQL_GOOD;
}

int fixedread_get_record(reader* reader, node* rg)
{
	fixedread* self = reader->reader_data;
	if (self->offset + reader->reclen > self->file_size) {
		reader->eof = true;
		return EOF;
	}

	int ret = fixedread_get_record_at(reader, rg, self->offset);
	self->offset += reader->reclen;
	return ret;
}

int fixedread_get_record_at(reader* reader, node* rg, size_t offset)
{
	record* rec = rg->data;
	fixedread* self = reader->reader_data;
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

int fixedread_reset(reader* reader)
{
	fixedread* self = reader->reader_data;
	reader->eof = false;
	reader->rec_id = 0;
	self->offset = reader->reclen * reader->skip_rows;
	return FQL_GOOD;
}

int fixedread_reset_stdin(reader* reader)
{
	reader_stop_file_backed_input(reader);
	reader->reset__ = &fixedread_reset;
	reader->get_record__ = &fixedread_get_record;

	return fixedread_open(reader, string_c_str(reader->random_access_filename));
}
