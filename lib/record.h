#ifndef RECORD_H
#define RECORD_H

#include <inttypes.h>
#include <csv.h>
#include <stdint.h>
#include <sys/types.h>
#include "util/vec.h"
#include "util/node.h"
#include "util/stringy.h"
#include "util/stringview.h"

struct record {
	struct vec fields;
	struct vec* subquery_strings;
	struct vec* group_strings;
	struct csv_record* libcsv_rec;
	stringview rec_ref;
	size_t offset;
	unsigned rec_idx;
	unsigned select_len;
	unsigned max_subquery_count;
	unsigned max_group_count;
};
typedef struct record record;

struct record* record_construct(struct record*, unsigned idx);
void record_destroy(struct record*);
void record_copy(struct record* dest, const struct record* src);
void record_resize(struct record*, unsigned size);
void record_swap(struct record*, struct record* src);
void record_clear_strings(struct record*, struct vec*);
string* record_generate_groupby_string(struct record*);
string* record_generate_subquery_string(struct record*);

#endif /* RECORD_H */
