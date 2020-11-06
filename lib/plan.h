#ifndef PLAN_H
#define PLAN_H

#include "util/queue.h"
#include "process.h"

struct plan {
        struct dgraph* processes;
        struct dnode* op_true;
        struct dnode* op_false;
        /* temp */
        struct dnode* current;
};

//struct plan* plan_new(struct query*)
void plan_free(void*);
int build_plans(struct queue**, struct queue*);

void print_plans(struct queue* plans);

#endif /* PLAN_H */
