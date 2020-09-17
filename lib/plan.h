#ifndef PLAN_H
#define PLAN_H

#include "util/queue.h"

struct plan_t {
        queue_t* process_queue;
};
typedef struct plan_t plan_t;

//plan_t* plan_new(query_t*)
void plan_free(void*);
int build_plans(queue_t**, queue_t*);

#endif /* PLAN_H */
