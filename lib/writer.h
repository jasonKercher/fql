#ifndef WRITER_H
#define WRITER_H

#include "io.h"
#include "record.h"
#include "util/util.h"
#include "util/stringy.h"

typedef struct csv_writer csv_writer;
typedef struct csv_record csv_record;

struct writer;

int libcsv_write_record(struct writer*, struct vec*, struct recgroup*, FILE*);

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
int fixedwriter_write_record(struct writer*, struct vec*, struct recgroup*, FILE*);

typedef int (*write_fn)(struct writer*, struct vec*, struct recgroup*, FILE*);

struct writer {
	enum io type;
	void* writer_data;
	write_fn write_record__;
	struct vec* raw_rec;
	string file_name;
	int strict;
};
typedef struct writer writer;

struct writer* writer_construct(struct writer*, enum io, int strict);
void writer_destroy(struct writer*);

int writer_open(struct writer* self, const char* file_name);
int writer_close(struct writer* self);
FILE* writer_get_file(struct writer*);
char* writer_take_filename(struct writer*);
const char* writer_get_tempname(struct writer*);

void writer_set_delimiter(struct writer*, const char*);
void writer_set_rec_terminator(struct writer*, const char*);
void writer_assign(struct writer*);

#endif /* WRTIER_H */
