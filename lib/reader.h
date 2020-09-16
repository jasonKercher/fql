#ifndef READER_H
#define READER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <limits.h>

#include "util/util.h"

typedef struct csv_record csv_record;
typedef struct csv_reader csv_reader;

typedef int (*read_next_func)(void*, csv_record*);

enum read_type {
        READ_LIBCSV,
        READ_SUBQUERY,
};

struct reader_t {
        void* handle;
        read_next_func get_f;
        generic_data_func free_f;
        char file_name[PATH_MAX];
};
typedef struct reader_t reader_t;

reader_t* reader_new();
void reader_free(reader_t*);

void reader_assign(reader_t*, enum read_type);

#ifdef __cplusplus
}
#endif

#endif /* READER_H */
