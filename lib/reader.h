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

typedef struct csv_record csv_record;
typedef struct csv_reader csv_reader;

struct libcsv_data {
        csv_reader* handle;
        struct vec* records;
};

struct libcsv_data* libcsv_new(size_t);
struct libcsv_data* libcsv_init(struct libcsv_data*, size_t);
int libcsv_get_record(void* reader_data, Vec* rec, unsigned char);
void libcsv_free(void*);

struct mmapcsv_data {
        csv_reader* handle;
        struct vec* records;
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
        READ_LIBCSV,
        READ_MMAPCSV,
        READ_SUBQUERY,
};

struct reader {
        void* reader_data;
        read_next_fn get_record_fn;
        generic_data_fn free_fn;
        char file_name[PATH_MAX];
};
typedef struct reader Reader;

struct reader* reader_new();
struct reader* reader_init(struct reader*);
void reader_free(struct reader*);

void reader_assign(struct reader*, enum read_type);

#ifdef __cplusplus
}
#endif

#endif /* READER_H */
