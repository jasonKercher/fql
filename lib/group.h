#ifndef GROUP_H
#define GROUP_H

#include "query.h"
#include "record.h"
#include "column.h"
#include "process.h"
#include "util/vec.h"
#include "util/hashmap.h"

struct group {
	CompositeMap* expr_map;
	CompositeMap val_map;
	struct vec columns;
	struct vec aggregates;
	struct vec _indicies;
	struct vec _raw;
	struct vec _composite;  /* temporary */
	size_t _dump_idx;
};
typedef struct group Group;

struct group* group_new();
struct group* group_construct(struct group*);
void group_free(struct group*);
void group_destroy(struct group*);

void group_add_column(struct group*, struct column*);
void group_cat_description(struct group*, struct process*);

int group_record(struct group*, struct vec* rec);
int group_dump_record(struct group* group, struct record* rec);

#endif  /* GROUP_H */
