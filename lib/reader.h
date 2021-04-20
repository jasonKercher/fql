#ifndef READER_H
#define READER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "record.h"
#include "util/vec.h"
//#include "util/pmap.h"
#include "util/stringy.h"
#include "util/stringview.h"
#include "util/util.h"

struct table;
struct reader;

typedef int (*read_fn)(struct reader*, struct record* rec);
typedef int (*reset_fn)(struct reader*);

enum read_type {
	READ_UNDEFINED,
	READ_LIBCSV,
	READ_MMAPCSV,
	READ_SUBQUERY,
};

struct reader {
	enum read_type type;
	void* reader_data;
	struct vec* subquery_recs;
	read_fn get_record__;
	generic_data_fn free__;
	reset_fn reset__;
	String file_name;
	size_t max_col_idx;
	_Bool eof;
};
typedef struct reader Reader;

struct reader* reader_new();
struct reader* reader_construct(struct reader*);
void reader_free(struct reader*);

void reader_assign(struct reader*, struct table*);

/**
 * Reader types own the data that is passed from
 * process to process. The records pass a vector
 * of fields in the form of read-only StringViews.
 */

void libcsv_reader_free(void*);

char* libcsv_get_delim(struct csv_reader*);
int libcsv_get_record(struct reader*, struct record* rec);
int libcsv_reset(struct reader*);

struct mmapcsv {
	struct csv_reader* csv_handle;
	struct stringview current;
	struct vec* raw;
	struct hashmap* rec_map;
	char* mmap_base;
	char* mp;
	size_t file_size;
	int fd;
};

struct mmapcsv* mmapcsv_new(size_t);
struct mmapcsv* mmapcsv_construct(struct mmapcsv*, size_t);
void mmapcsv_free(void*);

char* mmapcsv_get_delim(struct mmapcsv*);
int mmapcsv_open(struct mmapcsv*, const char* file_name);
int mmapcsv_get_record(struct reader*, struct record* rec);
int mmapcsv_get_record_at(struct reader*, struct record* rec, char* rec_loc);
int mmapcsv_reset(struct reader*);



#ifdef __cplusplus
}
#endif

#endif /* READER_H */
