#include "writer.h"
#include <sys/stat.h>
#include "fql.h"
#include "misc.h"
#include "fqlsig.h"
#include "record.h"
#include "expression.h"

/**
 * For the most part, I'm just mimicing libcsv writer functions.
 * basically, we always want to write to a temp file. When we
 * are done, we rename.  Anything goes wrong, and we delete the
 * temp file. The user should never lose a file if we error out.
 */
fixedwriter* fixedwriter_construct(fixedwriter* self)
{
	*self = (fixedwriter) {
	        .file = stdout,
	};

	string_construct(&self->tempname);
	string_construct(&self->filename);
	string_construct(&self->buffer);
	return self;
}

void fixedwriter_destroy(fixedwriter* self)
{
	string_destroy(&self->tempname);
	string_destroy(&self->filename);
	string_destroy(&self->buffer);
}

void _set_filename(fixedwriter* self, const char* filename)
{
	string_strcpy(&self->filename, filename);
	self->is_detached = false;
}
int _mktmp(fixedwriter* self)
{
	char* targetdir = ".";
	char* filename_cp = NULL;
	if (!string_empty(&self->filename)) {
		filename_cp = strdup(string_c_str(&self->filename));
		targetdir = dirname(filename_cp);
	}
	string_strcpy(&self->tempname, targetdir);
	string_strcat(&self->tempname, "/fixed_XXXXXX");

	if (filename_cp != NULL) {
		free_(filename_cp);
	}

	int fd = mkstemp(self->tempname.data);
	self->file = fdopen(fd, "w");
	if (!self->file) {
		perror(string_c_str(&self->tempname));
		return FQL_FAIL;
	}

	self->tempnode = fqlsig_tmp_push(string_c_str(&self->tempname));
	return FQL_GOOD;
}
bool _isopen(fixedwriter* self)
{
	return (self->file != NULL && self->file != stdout);
}
int fixedwriter_open(fixedwriter* self, const char* filename)
{
	_set_filename(self, filename);
	if (_isopen(self)) {
		fprintf(stderr, "fixedwriter already open\n");
		return FQL_FAIL;
	}
	return _mktmp(self);
}

int fixedwriter_close(fixedwriter* self)
{
	if (self->file == stdout)
		return FQL_GOOD;

	fclose(self->file);
	self->file = NULL;

	if (self->is_detached) {
		self->is_detached = false;
		return FQL_GOOD;
	}

	const char* file = string_c_str(&self->filename);
	const char* tmp = string_c_str(&self->tempname);

	if (string_empty(&self->filename)) {
		fputs("file name empty\n", stderr);
		return FQL_FAIL;
	}
	if (rename(tmp, file)) {
		perror(tmp);
		return FQL_FAIL;
	}
	if (chmod(file, 0666)) {
		perror(file);
		return FQL_FAIL;
	}
	fqlsig_tmp_remove_node(self->tempnode);
	self->tempnode = NULL;
	return FQL_GOOD;
}

FILE* fixedwriter_get_file(fixedwriter* self)
{
	return self->file;
}

char* fixedwriter_take_filename(fixedwriter* self)
{
	self->is_detached = true;
	if (string_empty(&self->filename)) {
		return NULL;
	}
	return string_export(&self->filename);
}

const char* _get_filename(fixedwriter* self)
{
	string* filename = NULL;
	if (self->is_detached) {
		filename = &self->tempname;
		self->tempnode = NULL;
	} else {
		filename = &self->filename;
	}

	if (string_empty(filename)) {
		return NULL;
	}
	return string_c_str(filename);
}

const char* fixedwriter_export_temp(fixedwriter* self)
{
	if (!_isopen(self)) {
		if (_mktmp(self) == FQL_FAIL) {
			return NULL;
		}
	}

	return _get_filename(self);
}

int fixedwriter_write_record(writer* writer, vec* expr_vec, node* rg, FILE* outstream)
{
	fixedwriter* self = writer->writer_data;

	FILE* store_stream = NULL;
	if (outstream == NULL) {
		outstream = fixedwriter_get_file(self);
	} else {
		store_stream = fixedwriter_get_file(self);
		self->file = outstream;
	}

	int len = 0;

	expression** it = vec_begin(expr_vec);
	for (; it != vec_end(expr_vec); ++it) {
		stringview sv = {NULL, 0};
		if ((*it)->expr == EXPR_ASTERISK) {
			record* rec = node_data_at(rg, (*it)->src_idx);
			sv = rec->rec_ref;
		} else {
			if (writer->strict && (*it)->field_type != FIELD_STRING) {
				try_(expression_type_check(*it, rg));
			}
			try_(expression_get_stringview(&sv, *it, rg));
		}

		unsigned width = sv.len;
		if ((*it)->width != 0) {
			width = (*it)->width;
			if (width < sv.len) {
				fprintf(stderr,
				        "Truncation error: `%.*s' to `%.*s'\n",
				        sv.len,
				        sv.data,
				        width,
				        sv.data);
				return FQL_FAIL;
			}
		}

		fwrite(sv.data, 1, sv.len, self->file);
		unsigned i = sv.len;
		for (; i < width; ++i) {
			fputc(' ', self->file);
		}
		len += width;
	}

	if (store_stream) {
		self->file = store_stream;
	}

	return len;
}
