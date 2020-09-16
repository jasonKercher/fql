#ifndef PLAN_H
#define PLAN_H

#include "util/queue.h"

struct process_t {
        char name[128];
        char item[128];
        char description[512];
};
typedef struct process_t process_t;


struct plan_t {
        queue_t* process_queue;
};
typedef struct plan_t plan_t;

int build_plans(queue_t**, queue_t*);

#endif /* PLAN_H */
