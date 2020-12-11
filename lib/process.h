#ifndef PROCESS_H
#define PROCESS_H

#include "util/fqlstring.h"

/* process_func(records, process_data) */
typedef int (*process_func)(void**, void*);

struct process {
        process_func* action;
        String* action_msg;
        _Bool is_passive;
};
typedef struct process Process;

struct process* process_new(const char* action);
struct process* process_init(struct process*, const char*);
void process_free(struct process*);

#endif /* PROCESS_H */
