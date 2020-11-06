#ifndef PROCESS_H
#define PROCESS_H

#define ACTION_MAX 512
#define OBJECT_MAX 128

/* process_func(records, process_data) */
typedef int (*process_func)(void**, void*);

struct process {
        process_func* action;
        char action_msg[ACTION_MAX];
};

struct process* process_new(const char* action);
void process_free(struct process*);

#endif /* PROCESS_H */
