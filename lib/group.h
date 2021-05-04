#ifndef GROUP_H
#define GROUP_H

#include "query.h"
#include "column.h"
#include "process.h"
#include "util/vec.h"
#include "util/hashmap.h"

struct group;

struct aggresult {
	union {
		long i;
		double f;
		String s;
	} data;
	unsigned qty;
};

struct aggregate;
typedef int(*aggregate_fn)(struct aggregate*, struct group*, struct vec* rec);

struct aggregate {
	struct vec results;
	aggregate_fn call__;
};
typedef struct aggregate Aggregate;

int aggregate_resolve(struct aggregate*, enum aggregate_function);

int fql_count(struct aggregate*, struct group*, struct vec* rec, unsigned idx);

struct group {
	CompositeMap* expr_map;
	CompositeMap val_map;
	struct vec columns;
	struct vec aggregates;
	struct vec _indicies;
	struct vec _raw;
	struct vec _composite;  /* temporary */
};
typedef struct group Group;

struct group* group_new();
struct group* group_construct(struct group*);
void group_free(struct group*);
void group_destroy(struct group*);

void group_add_column(struct group*, struct column*);
void group_cat_description(struct group*, struct process*);

int group_record(struct group*, struct vec* rec);

#endif  /* GROUP_H */
