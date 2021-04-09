#ifndef FQLPLAN_H
#define FQLPLAN_H

#include "util/queue.h"
#include "util/vec.h"
#include "util/dgraph.h"

struct query;
struct process;

struct fql_plan {
	struct dgraph* processes;
	struct dnode* op_true;
	struct dnode* op_false;
	struct dnode* current;  /* temp */
	size_t rows_affected;
	int source_count;
	_Bool has_stepped;
};
typedef struct fql_plan Plan;

struct fql_plan* plan_new(int);
struct fql_plan* plan_construct(struct fql_plan*, int);
void plan_free(void*);
void plan_destroy(void*);

struct fql_plan* plan_build(struct query*, struct process*);
int build_plans(struct queue*);
void print_plans(struct queue* plans);

#endif /* FQLPLAN_H */
