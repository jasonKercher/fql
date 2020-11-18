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

struct reader {
        void* handle;
        read_next_func get_record_f;
        generic_data_func free_f;
        char file_name[PATH_MAX];
};
typedef struct reader Reader;

struct reader* reader_new();
void reader_free(struct reader*);

void reader_assign(struct reader*, enum read_type);

#ifdef __cplusplus
}
#endif

#endif /* READER_H */
