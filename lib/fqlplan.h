#ifndef FQLPLAN_H
#define FQLPLAN_H

#include "util/queue.h"
#include "util/vec.h"

struct query;

struct fql_plan {
        struct dgraph* processes;
        struct dnode* op_true;
        struct dnode* op_false;
        /* temp */
        struct dnode* current;
        size_t rows_affected;
        int source_count;
};
typedef struct fql_plan Plan;

struct fql_plan* plan_new(int);
struct fql_plan* plan_init(struct fql_plan*, int);
void plan_free(void*);

struct fql_plan* plan_build(struct query*);
int build_plans(struct vec*, struct queue*);
void print_plans(struct vec* plans);

#endif /* FQLPLAN_H */
