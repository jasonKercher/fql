#ifndef RECORD_H
#define RECORD_H

#include <stdatomic.h>
#include <csv.h>
#include "util/vec.h"
#include "util/stringview.h"
#include "util/stringy.h"

struct record {
	struct vec* fields;
	struct vec* _field_data;
	struct csv_record* libcsv_rec;
	stringview rec_raw;
	size_t offset;
	unsigned max_size;
	unsigned select_len;
	unsigned idx;
	_Atomic int ref_count;
};
typedef struct record record;

struct record* record_construct(struct record*, unsigned idx);
void record_destroy(struct record*);
void rec_resize(struct record*, unsigned size);

#endif /* RECORD_H */
