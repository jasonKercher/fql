#ifndef PLAN_H
#define PLAN_H

#include "util/queue.h"

struct plan {
        /** TODO - this needs to be a graph **/
        struct queue* process_queue;
};

//struct plan* plan_new(struct query*)
void plan_free(void*);
int build_plans(struct queue**, struct queue*);

#endif /* PLAN_H */
