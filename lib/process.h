#ifndef PROCESS_H
#define PROCESS_H

#define ACTION_MAX 512
#define OBJECT_MAX 128

/* This is essentially a graph structure
 * with only 2 inputs and outputs per node
 */

struct process {
        void* input[2];
        void* output[2];
        char action[ACTION_MAX];
};

struct process* process_new(const char* action);
void process_free(struct process*);

#endif /* PROCESS_H */
