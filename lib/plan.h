#ifndef PLAN_H
#define PLAN_H

#include "util/queue.h"

struct process {
        char name[128];
        char item[128];
        char description[512];
};

typedef struct process process_t;


struct plan {
        queue_t* process_queue;
};

typedef struct plan plan_t;


#endif
