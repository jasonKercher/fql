#ifndef PROCESS_H
#define PROCESS_H

#define PROCESS_BUFFER_SIZE 256

#include "fqlplan.h"
#include "util/fqlstring.h"
#include "util/fifo.h"
#include "util/queue.h"
#include "util/dgraph.h"

struct process;
typedef int (process_fn)(struct process*);

struct process {
        process_fn* action;             /* function pointer for process */
        Fifo* fifo_in0;                 /* ring buffer of records */
        Fifo* fifo_in1;                 /* optional second input */
        Fifo* fifo_out0;                /* default next process fifo */
        Fifo* fifo_out1;                /* optional second output */
        void* proc_data;                /* process specific data */
        String* action_msg;             /* Message that prints with plan */
        int fifo_width;                 /* Number of sources at this step */
        _Bool is_secondary;             /* fifo_out should link to a fifo_in1 */
        _Bool is_passive;               /* denotes process that does nothing */
};
typedef struct process Process;

struct process* process_new(const char* action, int width);
struct process* process_init(struct process*, const char*, int width);
void process_free(struct process*);

void process_activate(Dnode* proc_node);
int process_exec_plans(struct fql_plan*, int);
int process_exec_plan(struct fql_plan*);

int fql_read(struct process*);
int fql_select(struct process*);
int fql_no_op(struct process*);

#endif /* PROCESS_H */
