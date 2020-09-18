#ifndef PROCESS_H
#define PROCESS_H

#define ACTION_MAX 512

#include "util/vec.h"

struct process {
        struct vector* input;
        struct vector* output;
        char action[ACTION_MAX];
};

struct process* process_new(unsigned, unsigned, const char*);
void process_free(struct process*);

#endif /* PROCESS_H */
