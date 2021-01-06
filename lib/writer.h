#ifndef WRITER_H
#define WRITER_H

#include "util/vec.h"
#include "util/util.h"

typedef struct csv_writer csv_writer;
typedef struct csv_record csv_record;

/* writer's do not own data, and only write one
 * record at a time so only need one csv_record.
 */
struct libcsv_writer {
        csv_writer* csv_handle;
        csv_record* csv_rec;
};

struct libcsv_writer* libcsv_writer_new();
struct libcsv_writer* libcsv_writer_init(struct libcsv_writer*);
void libcsv_writer_free(void*);
int libcsv_write_record(void* writer_data, struct vec*);

typedef int (*write_fn)(void*, struct vec*);

enum write_type {
        WRITE_UNDEFINED,
        WRITE_LIBCSV,
};

struct writer {
        enum write_type type;
        void* writer_data;
        write_fn write_record_fn;
        generic_data_fn free_fn;
        struct vec* raw_rec;
        char file_name[PATH_MAX];
};
typedef struct writer Writer;

struct writer* writer_new();
struct writer* writer_init(struct writer*);
void writer_free(struct writer*);

void writer_assign(struct writer*);

#endif  /* WRTIER_H */
