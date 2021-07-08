#ifndef READER_H
#define READER_H

#include "io.h"
#include "record.h"
#include "util/vec.h"
//#include "util/pmap.h"
#include "util/stringy.h"
#include "util/stringview.h"
#include "util/util.h"

struct table;
struct reader;
struct fqlselect;

typedef int (*read_fn)(struct reader*, struct record*);
typedef int (*read_at_fn)(struct reader*, struct record*, const char*);
typedef int (*reset_fn)(struct reader*);

struct reader {
	enum io type;
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
	bool eof;
};
typedef struct reader reader;

struct reader* reader_construct(struct reader*);
void reader_destroy(struct reader*);

int reader_assign(struct reader*, struct table*);
size_t reader_get_file_size(struct reader*);

int reader_subquery_get_record(struct fqlselect*, struct record*, struct vec*);

/**
 * reader types own the data that is passed from
 * process to process. the records pass a vector
 * of fields in the form of read-only stringviews.
 */
void libcsv_free(void*);
int libcsv_get_record(struct reader*, struct record*);
int libcsv_get_record_at(struct reader*, struct record*, const char*);
int libcsv_reset(struct reader*);

struct fixedreader {
	char* mmap;
	char* iter;
	struct vec* columns;
	size_t file_size;
	int fd;
};
typedef struct fixedreader fixedreader;

struct fixedreader* fixedreader_construct(struct fixedreader*, struct vec* columns);
void fixedreader_free(void*);
int fixedreader_open(struct reader*, const char* file_name);
int fixedreader_get_record(struct reader*, struct record*);
int fixedreader_get_record_at(struct reader*, struct record*, const char*);
int fixedreader_reset(struct reader*);

#endif /* READER_H */
