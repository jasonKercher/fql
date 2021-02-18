#ifndef RECORD_H
#define RECORD_H

#include "util/vec.h"
#include "util/stringview.h"
#include "util/stringy.h"

struct record {
        struct vec fields;
        String rec_cpy;
        StringView rec_raw;
};
typedef struct record Record;

struct record* record_new();
struct record* record_construct(struct record*);
void record_free(struct record*);
void record_destroy(struct record*);


#endif  /* RECORD_H */
