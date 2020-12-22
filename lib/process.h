#ifndef PROCESS_H
#define PROCESS_H

#define PROCESS_BUFFER_SIZE 256

#include "util/fqlstring.h"
#include "util/fifo.h"

struct process;

/* process_fn(records, process_data) */
typedef int (process_fn)(struct process*);

struct process {
        process_fn* action;             /* function pointer for process */
        Fifo* records;                  /* ring buffer of records */
        void* proc_data;                /* process specific data */
        String* action_msg;             /* Message that prints with plan */
        _Bool is_passive;               /* denotes process that does nothing */
};
typedef struct process Process;

struct process* process_new(const char* action);
struct process* process_init(struct process*, const char*);
void process_free(struct process*);

int libcsv_read(struct process*);
int libcsv_read_mmap(struct process*);

#endif /* PROCESS_H */
