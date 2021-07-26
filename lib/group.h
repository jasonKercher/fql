#ifndef GROUP_H
#define GROUP_H

#include "query.h"
#include "expression.h"
#include "process.h"
#include "util/vec.h"
#include "util/flex.h"
#include "util/hashmap.h"

struct record;

struct group {
	compositemap* expr_map;
	compositemap val_map;
	struct vec expressions;
	struct vec aggregates;
	struct flex group_data;
	struct vec _composite; /* temporary */
	struct vec _roots;
	size_t _dump_idx;
};
typedef struct group group;

struct group* group_construct(struct group*);
void group_destroy(struct group*);
void distinct_destroy(struct group*);

void group_add_expression(struct group*, struct expression*);
void group_cat_description(struct group*, struct process*);

int group_record(struct group*, struct node* rec);
int group_dump_record(struct group* group, struct record* rec);

#endif /* GROUP_H */
