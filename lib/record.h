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

//struct recgroup {
//	struct node* rec_list;
//	uint32_t select_len;
//	uint32_t _fifo_idx;
//};
//typedef struct recgroup recgroup;
//
//struct recgroup* recgroup_construct(struct recgroup*, unsigned fifo_idx);
//void recgroup_destroy(struct recgroup*);
//
//void recgroup_resize(struct recgroup*, unsigned);
//struct record* recgroup_rec_at(const struct recgroup*, unsigned);
//struct record* recgroup_rec_begin(const struct recgroup*);
//struct record* recgroup_rec_back(const struct recgroup*);
////void recgroup_rec_set_ref(struct recgroup*, unsigned, struct node*);
//void recgroup_rec_push_back(struct recgroup*, struct node*);
//void recgroup_rec_push_front(struct recgroup*, struct node*);
//struct node* recgroup_rec_pop_back(struct recgroup*);


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
	bool is_ref;
};
typedef struct record record;

struct record* record_construct(struct record*, unsigned idx);
void record_destroy(struct record*);
void record_resize(struct record*, unsigned size);
void record_swap(struct record*, struct record* src);
void record_clear_strings(struct record*, struct vec*);
string* record_generate_groupby_string(struct record*);
string* record_generate_subquery_string(struct record*);

#endif /* RECORD_H */
