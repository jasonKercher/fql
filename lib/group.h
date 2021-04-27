#ifndef GROUP_H
#define GROUP_H

#include "column.h"
#include "process.h"
#include "util/vec.h"
#include "util/hashmap.h"

struct group;

union aggdata {
	long i;
	double f;
	String* s;
};

typedef int(*aggregate_fn)(struct group*, struct vec* rec);

struct aggregate {
	union aggdata _agg;
	aggregate_fn call__;
	unsigned count;
};
typedef struct aggregate Aggregate;

struct group {
	struct vec* columns;
	struct vec* aggregates;
	struct hashmap* groups;
	struct vec _composite;  /* temporary */
};
typedef struct group Group;

struct group* group_new();
struct group* group_construct(struct group*);
void group_free(struct group*);
void group_destroy(struct group*);

void group_add_column(struct group*, struct column*);
void group_cat_description(struct group*, struct process*);

#endif  /* GROUP_H */
