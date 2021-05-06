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
	struct dnode* current;  /* temp */
	struct vec* recycle_groups;
	size_t rows_affected;
	int source_count;
	int plan_id;
	_Bool has_stepped;
};
typedef struct fql_plan plan;

struct fql_plan* plan_new(struct query*);
struct fql_plan* plan_construct(struct fql_plan*, struct query*);
void plan_free(void*);
void plan_destroy(void*);

struct fql_plan* plan_build(struct query*, struct dnode* entry);
int build_plans(struct queue*);
void print_plans(struct queue* plans);

#endif /* FQLPLAN_H */
