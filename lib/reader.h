#ifndef READER_H
#define READER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <limits.h>

#include "util/vec.h"
#include "util/pmap.h"
#include "util/fqlstring.h"
#include "util/stringview.h"
#include "util/util.h"

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

struct libcsv_data {
        csv_reader* csv_handle;
        struct vec* csv_records; /* vec_(csv_record*) */
        _Bool eof;
};

struct libcsv_data* libcsv_new(size_t);
struct libcsv_data* libcsv_init(struct libcsv_data*, size_t);
int libcsv_get_record(void* reader_data, Vec* rec, unsigned char);
void libcsv_free(void*);

struct mmapcsv_data {
        csv_reader* csv_handle;
        struct vec* csv_records; /* vec_(csv_record*) */
        struct stringview current;
        struct vec* raw;
        Pmap* rec_map;
        char* mmap_base;
        char* mp;
        size_t file_size;
        int fd;
};

struct mmapcsv_data* mmapcsv_new(size_t);
struct mmapcsv_data* mmapcsv_init(struct mmapcsv_data*, size_t);
int mmapcsv_open(struct mmapcsv_data*, const char* file_name);
int mmapcsv_get_record(void* reader_data, Vec* rec, unsigned char);
void mmapcsv_free(void*);

typedef int (*read_next_fn)(void*, struct vec* rec, unsigned char idx);

enum read_type {
        READ_UNDEFINED,
        READ_LIBCSV,
        READ_MMAPCSV,
        READ_SUBQUERY,
};

struct reader {
        enum read_type type;
        void* reader_data;
        read_next_fn get_record_fn;
        generic_data_fn free_fn;
        char file_name[PATH_MAX];
};
typedef struct reader Reader;

struct reader* reader_new();
struct reader* reader_init(struct reader*);
void reader_free(struct reader*);

void reader_assign(struct reader*);

#ifdef __cplusplus
}
#endif

#endif /* READER_H */
