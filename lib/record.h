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
	string* rec_cpy;
	stringview rec_raw;
	unsigned idx;
	_Atomic int ref_count;
	_Atomic _Bool is_recyclable;
};
typedef struct record record;

struct record* record_construct(struct record*, unsigned idx, unsigned n, _Bool owns_recs);
void record_destroy(struct record*);


#endif  /* RECORD_H */
