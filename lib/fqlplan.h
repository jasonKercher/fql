#ifndef FQLPLAN_H
#define FQLPLAN_H

#include "util/queue.h"
#include "util/vec.h"
#include "util/dgraph.h"

struct query;

struct fql_plan {
	struct dgraph* processes;
	struct dnode* op_true;
	struct dnode* op_false;
	struct dnode* current; /* temp */
	struct vec* recycle_groups;
	struct query* query;
	size_t rows_affected;
	int source_count;
	int plan_id;
	bool has_stepped;
	bool loose_groups;
};
typedef struct fql_plan plan;

struct fql_plan* plan_construct(struct fql_plan*, struct query*, bool);
void plan_destroy(void*);

int build_plans(struct queue*, bool);
void print_plans(struct queue* plans);

#endif /* FQLPLAN_H */
