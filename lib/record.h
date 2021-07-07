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
	unsigned select_len;
	unsigned idx;
	_Atomic int ref_count;
};
typedef struct record record;

struct record* record_construct(struct record*, unsigned idx, unsigned n, bool owns_recs);
void record_destroy(struct record*);

#endif /* RECORD_H */
