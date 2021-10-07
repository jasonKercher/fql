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
struct fqlhandle;

typedef int (*read_fn)(struct reader*, struct node*);
typedef int (*read_at_fn)(struct reader*, struct node*, size_t off);
typedef int (*reset_fn)(struct reader*);

struct reader {
	enum io type;
	void* reader_data;
	FILE* random_access_file;
	string* random_access_filename;
	struct node* random_access_tempnode;
	struct csv_writer* subquery_writer;
	record* first_rec;
	read_fn get_record__;
	read_at_fn get_record_at__;
	generic_data_fn free__;
	reset_fn reset__;
	string file_name;
	size_t max_idx;
	size_t reclen;
	_Atomic size_t rec_id;
	unsigned skip_rows;
	_Atomic bool eof;
};
typedef struct reader reader;

struct reader* reader_construct(struct reader*);
void reader_destroy(struct reader*);

int reader_assign(struct reader*, struct table*, struct fqlhandle*);
int reader_reopen(struct reader*);
size_t reader_get_file_size(struct reader*);
int reader_start_file_backed_input(struct reader*);
int reader_stop_file_backed_input(struct reader*);


/* Below structures extend struct reader
 * via the "reader_data" member.
 */

/** subquery **/
struct subquery {
	struct fqlselect* select;
	struct query* query;
	struct record copy_data;
};
typedef struct subquery subquery;

struct subquery* subquery_construct(struct subquery*, struct query*);
void subquery_free(void*);
int subquery_get_record(struct reader*, struct node*);
int subquery_get_record_at(struct reader*, struct node*, size_t offset);
int subquery_reset(struct reader*);
int subquery_reset_from_join(struct reader*);
int subquery_start_file_backed_input(struct reader*);


/** fixedread **/
struct fixedread {
	char* mmap;
	struct vec* expressions;
	size_t offset;
	size_t file_size;
	int fd;
};
typedef struct fixedread fixedread;

struct fixedread* fixedread_construct(struct fixedread*, struct vec* expressions);
void fixedread_free(void*);
int fixedread_open(struct reader*, const char* file_name);
int fixedread_reopen(struct reader*);
int fixedread_get_record(struct reader*, struct node*);
int fixedread_get_record_stdin(struct reader*, struct node*);
int fixedread_get_record_at(struct reader*, struct node*, size_t offset);
int fixedread_reset(struct reader*);
int fixedread_reset_stdin(struct reader*);


/* These functions are using libcsv's
 * "struct csv_reader" for reader_data.
 */
typedef struct csv_reader csv_reader;

void libcsv_free(void*);
int libcsv_get_record(struct reader*, struct node*);
int libcsv_get_record_at(struct reader*, struct node*, size_t offset);
int libcsv_reset_stdin(struct reader*);
int libcsv_reset(struct reader*);

#endif /* READER_H */
