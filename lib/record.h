#ifndef RECORD_H
#define RECORD_H

#include <stdatomic.h>
#include <csv.h>
#include "util/vec.h"
#include "util/stringview.h"
#include "util/stringy.h"

struct record {
        struct vec* fields;
        struct csv_record* libcsv_rec;
        String* rec_cpy;
        StringView rec_raw;
        unsigned idx;
        _Atomic int ref_count;
        _Atomic _Bool is_recyclable;
};
typedef struct record Record;

struct record* record_new(unsigned idx);
struct record* record_construct(struct record*, unsigned idx);
void record_free(struct record*);
void record_destroy(struct record*);


#endif  /* RECORD_H */
