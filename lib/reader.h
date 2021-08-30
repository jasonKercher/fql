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
struct fql_handle;

typedef int (*read_fn)(struct reader*, struct node*);
typedef int (*read_at_fn)(struct reader*, struct node*, const char*);
typedef int (*reset_fn)(struct reader*);

struct reader {
	enum io type;
	void* reader_data;
	read_fn get_record__;
	read_at_fn get_record_at__;
	generic_data_fn free__;
	reset_fn reset__;
	string file_name;
	size_t max_idx;
	size_t reclen;
	size_t rec_id;
	unsigned skip_rows;
	bool eof;
};
typedef struct reader reader;

struct reader* reader_construct(struct reader*);
void reader_destroy(struct reader*);

int reader_assign(struct reader*, struct table*, struct fql_handle*);
size_t reader_get_file_size(struct reader*);
int reader_reopen(struct reader*);


/* Below structures extend struct reader
 * via the "reader_data" member.
 */

/** subquery **/
struct subquery {
	struct fqlselect* select;
	struct record copy_data;
};
typedef struct subquery subquery;

struct subquery* subquery_construct(struct subquery*, struct fqlselect*);
void subquery_free(void*);
int subquery_get_record(struct reader*, struct node*);
int subquery_get_record_at(struct reader*, struct node*, const char*);
int subquery_reset(struct reader*);


/** fixedreader **/
struct fixedreader {
	char* mmap;
	char* iter;
	struct vec* expressions;
	size_t file_size;
	int fd;
};
typedef struct fixedreader fixedreader;

struct fixedreader* fixedreader_construct(struct fixedreader*, struct vec* expressions);
void fixedreader_free(void*);
int fixedreader_open(struct reader*, const char* file_name);
int fixedreader_reopen(struct reader*);
int fixedreader_get_record(struct reader*, struct node*);
int fixedreader_get_record_at(struct reader*, struct node*, const char*);
int fixedreader_reset(struct reader*);


/* These functions are using libcsv's
 * "struct csv_reader" for reader_data.
 */
typedef struct csv_reader csv_reader;

void libcsv_free(void*);
int libcsv_get_record(struct reader*, struct node*);
int libcsv_get_record_at(struct reader*, struct node*, const char*);
int libcsv_reset(struct reader*);

#endif /* READER_H */
