#ifndef READER_H
#define READER_H

#include "record.h"
#include "util/vec.h"
//#include "util/pmap.h"
#include "util/stringy.h"
#include "util/stringview.h"
#include "util/util.h"

struct table;
struct reader;

typedef int (*read_fn)(struct reader*, struct record*);
typedef int (*read_at_fn)(struct reader*, struct record*, const char*);
typedef int (*reset_fn)(struct reader*);

enum read_type {
	READ_UNDEFINED,
	READ_LIBCSV,
	READ_FIXED_BYTE,
	READ_SUBQUERY,
};

struct reader {
	enum read_type type;
	void* reader_data;
	struct vec* subquery_recs;
	read_fn get_record__;
	read_at_fn get_record_at__;
	generic_data_fn free__;
	reset_fn reset__;
	string file_name;
	size_t max_col_idx;
	size_t reclen;
	unsigned skip_rows;
	_Bool eof;
};
typedef struct reader reader;

struct reader* reader_construct(struct reader*);
void reader_destroy(struct reader*);

int reader_assign(struct reader*, struct table*);

/**
 * reader types own the data that is passed from
 * process to process. the records pass a vector
 * of fields in the form of read-only stringviews.
 */
void libcsv_free(void*);
int libcsv_get_record(struct reader*, struct record*);
int libcsv_get_record_at(struct reader*, struct record*, const char*);
int libcsv_reset(struct reader*);

struct fixedbyte {
	char* mmap;
	char* iter;
	struct vec* columns;
	size_t file_size;
	int fd;
};
typedef struct fixedbyte fixedbyte;

struct fixedbyte* fixedbyte_construct(struct fixedbyte*, struct vec* columns);
void fixedbyte_free(void*);
int fixedbyte_open(struct reader*, const char* file_name);
int fixedbyte_get_record(struct reader*, struct record*);
int fixedbyte_get_record_at(struct reader*, struct record*, const char*);
int fixedbyte_reset(struct reader*);

#endif /* READER_H */
