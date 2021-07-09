#ifndef RECORD_H
#define RECORD_H

#include <stdatomic.h>
#include <csv.h>
#include "util/vec.h"
#include "util/stringview.h"
#include "util/stringy.h"

struct record {
	struct vec fields;
	struct vec* field_strings;
	struct csv_record* libcsv_rec;
	stringview rec_ref;
	size_t offset;
	//_Atomic int ref_count;
};
typedef struct record record;

struct record* record_construct(struct record*);
void record_destroy(struct record*);
void record_resize(struct record*, unsigned size);
void record_init_strings(struct record*);

struct recgroup {
	struct vec records;
	unsigned max_sources;
	unsigned select_len;
	unsigned idx;
};
typedef struct recgroup recgroup;

struct recgroup* recgroup_construct(struct recgroup*, unsigned idx);
void recgroup_destroy(struct recgroup*);
struct record* recgroup_rec_at(const struct recgroup*, unsigned);
void recgroup_resize(struct recgroup*, unsigned);

#endif /* RECORD_H */
