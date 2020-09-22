#ifndef PROCESS_H
#define PROCESS_H

#define ACTION_MAX 512
#define OBJECT_MAX 128

/* process_func(records, process_data) */
typedef int (*process_func)(void**, void*);

/* This is essentially a graph structure
 * with only 2 inputs and outputs per node
 */

struct process {
        struct process* input[2];
        struct process* output[2];
        process_func* action;
        char action_msg[ACTION_MAX];
};

struct process* process_new(const char* action);
void process_free(struct process*);

#endif /* PROCESS_H */
