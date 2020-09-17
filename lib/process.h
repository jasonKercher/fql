#ifndef PROCESS_H
#define PROCESS_H

#define ACTION_MAX 512

#include "util/vec.h"

struct process_t {
        void* input;
        void* output;
        char action[ACTION_MAX];
};
typedef struct process_t process_t;

process_t* process_new(unsigned, unsigned, const char*);
void process_free(process_t*);

#endif /* PROCESS_H */
