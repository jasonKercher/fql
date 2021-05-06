#ifndef WRITER_H
#define WRITER_H

#include "util/vec.h"
#include "util/stringy.h"
#include "util/util.h"

typedef struct csv_writer csv_writer;
typedef struct csv_record csv_record;

/* writer's do not own data */
//struct libcsv_writer {
//        csv_writer* csv_handle;
//};
//
//struct libcsv_writer* libcsv_writer_new();
//struct libcsv_writer* libcsv_writer_construct(struct libcsv_writer*);
//void libcsv_writer_free(void*);
int libcsv_write_record(void* writer_data, struct vec*, struct vec*);

typedef int (*write_fn)(void*, struct vec*, struct vec*);

enum write_type {
	WRITE_UNDEFINED,
	WRITE_LIBCSV,
};

struct writer {
	enum write_type type;
	void* writer_data;
	write_fn write_record__;
	generic_data_fn free__;
	struct vec* raw_rec;
	string file_name;
};
typedef struct writer writer;

struct writer* writer_construct(struct writer*);
void writer_destroy(struct writer*);

void writer_set_delimiter(struct writer*, const char*);
void writer_assign(struct writer*);

#endif  /* WRTIER_H */
