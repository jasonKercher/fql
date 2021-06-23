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
	unsigned skip_rows;
	_Bool eof;
};
typedef struct reader reader;

struct reader* reader_construct(struct reader*);
void reader_destroy(struct reader*);

void reader_assign(struct reader*, struct table*);

/**
 * reader types own the data that is passed from
 * process to process. the records pass a vector
 * of fields in the form of read-only stringviews.
 */

void libcsv_reader_free(void*);
int libcsv_get_record(struct reader*, struct record*);
int libcsv_get_record_at(struct reader*, struct record*, const char*);
int libcsv_reset(struct reader*);

#ifdef __cplusplus
}
#endif

#endif /* READER_H */
