#ifndef READER_H
#define READER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "record.h"
#include "util/vec.h"
#include "util/pmap.h"
#include "util/fqlstring.h"
#include "util/stringview.h"
#include "util/util.h"

struct reader;

typedef int (*read_fn)(struct reader*, struct record* rec, unsigned char idx);

enum read_type {
        READ_UNDEFINED,
        READ_LIBCSV,
        READ_MMAPCSV,
        READ_SUBQUERY,
};

struct reader {
        enum read_type type;
        void* reader_data;
        read_fn get_record_fn;
        generic_data_fn free_fn;
        generic_data_fn reset_fn;
        String file_name;
        size_t max_col_idx;
};
typedef struct reader Reader;

struct reader* reader_new();
struct reader* reader_construct(struct reader*);
void reader_free(struct reader*);

void reader_assign(struct reader*);

/**
 * Reader types own the data that is passed from
 * process to process. The records pass a vector
 * of fields in the form of read-only StringViews.
 * To match the raw records to the correct
 * StringView, an index is passed to the *_get_record
 * family of functions.
 */

typedef struct csv_record csv_record;
typedef struct csv_reader csv_reader;

struct libcsv_reader {
        csv_reader* csv_handle;
        struct vec* csv_recs; /* vec_(csv_record*) */
        _Bool eof;
};

struct libcsv_reader* libcsv_reader_new(size_t);
struct libcsv_reader* libcsv_reader_construct(struct libcsv_reader*, size_t);
void libcsv_reader_free(void*);
int libcsv_get_record(struct reader*, struct record* rec, unsigned char);
void libcsv_reset(void*);

struct mmapcsv_data {
        csv_reader* csv_handle;
        struct vec* csv_recs; /* vec_(csv_record*) */
        struct stringview current;
        struct vec* raw;
        Pmap* rec_map;
        char* mmap_base;
        char* mp;
        size_t file_size;
        int fd;
        _Bool eof;
};

struct mmapcsv_data* mmapcsv_new(size_t);
struct mmapcsv_data* mmapcsv_construct(struct mmapcsv_data*, size_t);
void mmapcsv_free(void*);
int mmapcsv_open(struct mmapcsv_data*, const char* file_name);
int mmapcsv_get_record(struct reader*, struct record* rec, unsigned char);
void mmapcsv_reset(void*);

#ifdef __cplusplus
}
#endif

#endif /* READER_H */
