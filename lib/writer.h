#ifndef WRITER_H
#define WRITER_H

#include "util/vec.h"
#include "util/stringy.h"
#include "util/util.h"

typedef struct csv_writer csv_writer;
typedef struct csv_record csv_record;

int libcsv_write_record(void* writer_data, struct vec*, struct vec*, FILE*);

struct fixedwriter {
	FILE* file;
	string tempname;
	string filename;
	string buffer;
	bool is_detached;
};
typedef struct fixedwriter fixedwriter;

struct fixedwriter* fixedwriter_construct(struct fixedwriter*);
void fixedwriter_destroy(struct fixedwriter*);
int fixedwriter_open(struct fixedwriter*, const char* filename);
int fixedwriter_close(struct fixedwriter*);
FILE* fixedwriter_get_file(struct fixedwriter*);
char* fixedwriter_take_filename(struct fixedwriter*);
const char* fixedwriter_get_tempname(struct fixedwriter*);
int fixedwriter_write_record(void*, struct vec*, struct vec*, FILE*);

typedef int (*write_fn)(void*, struct vec*, struct vec*, FILE*);

enum write_type {
	WRITE_UNDEFINED,
	WRITE_LIBCSV,
	WRITE_FIXED,
};

struct writer {
	enum write_type type;
	void* writer_data;
	write_fn write_record__;
	struct vec* raw_rec;
	string file_name;
};
typedef struct writer writer;

struct writer* writer_construct(struct writer*);
void writer_destroy(struct writer*);

int writer_open(struct writer* self, const char* file_name);
int writer_close(struct writer* self);
FILE* writer_get_file(struct writer*);
char* writer_take_filename(struct writer*);
const char* writer_get_tempname(struct writer*);

void writer_set_delimiter(struct writer*, const char*);
void writer_assign(struct writer*);

#endif /* WRTIER_H */
