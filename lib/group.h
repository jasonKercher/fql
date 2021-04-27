#ifndef GROUP_H
#define GROUP_H

#include "process.h"
#include "util/vec.h"

struct group {
	struct vec* columns;
};
typedef struct group Group;

struct group* group_new();
struct group* group_construct(struct group*);
void group_free(struct group*);
void group_destroy(struct group*);

void group_cat_description(struct group*, struct process*);

#endif  /* GROUP_H */
