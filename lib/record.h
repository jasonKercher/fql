#ifndef RECORD_H
#define RECORD_H

#include <inttypes.h>
#include <csv.h>
#include <stdint.h>
#include <sys/types.h>
#include "util/vec.h"
#include "util/stringview.h"
#include "util/stringy.h"

/* NOTE:
 *
 * This is the structure that we run through
 * the process fifos. We pass the entire struct
 * through the fifo. It's probably faster to pass
 * pointers, but instead I just packed it all sorts
 * of stupid becasue I want...
 *
 * // this
 * recgroup *rg = fifo_get(in);
 * int i = rg->fifo_idx;
 *
 * // not this
 * recgroup** rg = fifo_get(in);
 * int i = (*rg)->fifo_idx;
 *
 * recgroup is just that. A group of records.
 * consider:
 *
 * SELECT *
 * FROM t1 JOIN t2
 *   ON t1.foo = t2.foo
 *
 * t1.foo and t2.foo come from 2 different records.
 * Now, put that in your pipe and smoke it.
 */
struct recgroup {
	struct vec* records;
	uint32_t select_len;
	uint8_t _fifo_idx[3];
	uint8_t _max_sources;
};
typedef struct recgroup recgroup;

struct recgroup* recgroup_construct(struct recgroup*, unsigned fifo_idx);
void recgroup_destroy(struct recgroup*);

unsigned recgroup_get_idx(const struct recgroup*);
void recgroup_resize(struct recgroup*, unsigned);
struct record* recgroup_rec_add_one(struct recgroup*);
struct record* recgroup_rec_add_one_front(struct recgroup*);
struct record* recgroup_rec_at(const struct recgroup*, unsigned);
struct record* recgroup_rec_begin(const struct recgroup*);
struct record* recgroup_rec_back(const struct recgroup*);
void recgroup_rec_set(struct recgroup*, unsigned, const struct record*);
void recgroup_rec_set_ref(struct recgroup*, unsigned, const struct record*);
void recgroup_rec_push_back(struct recgroup*, const struct record*);
void recgroup_rec_add_front(struct recgroup*, size_t);
struct record* recgroup_rec_pop(struct recgroup*);
void recgroup_clear_refs(struct recgroup*);


struct record {
	struct vec fields;
	struct vec* subquery_strings;
	struct vec* group_strings;
	struct csv_record* libcsv_rec;
	stringview rec_ref;
	size_t offset;
	unsigned max_subquery_count;
	unsigned max_group_count;
	bool is_ref;
};
typedef struct record record;

struct record* record_construct(struct record*);
void record_destroy(struct record*);
void record_resize(struct record*, unsigned size);
void record_swap(struct record*, struct record* src);
void record_clear_strings(struct record*, struct vec*);
string* record_generate_groupby_string(struct record*);
string* record_generate_subquery_string(struct record*);

#endif /* RECORD_H */
