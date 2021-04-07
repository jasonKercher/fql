#ifndef PROCESS_H
#define PROCESS_H

#define PROCESS_BUFFER_SIZE 256

#include "fqlplan.h"
#include "util/stringy.h"
#include "util/fifo.h"
#include "util/queue.h"
#include "util/dgraph.h"

struct process;
typedef int (process_fn)(struct dgraph*, struct process*);

struct process {
	pthread_t thread;               /* pthread handle */
	struct vec* records;            /* This is the owned record data for roots */
	process_fn* action__;           /* function pointer for process */
	Fifo* fifo_in[2];               /* ring buffer of records */
	Fifo* fifo_out[2];              /* default next process fifo */
	void* proc_data;                /* process specific data */
	String* action_msg;             /* Message that prints with plan */
	int fifo_width;                 /* Number of sources at this step */
	int root_fifo;                  /* Signify which fifo_inx is the root */
	_Bool is_secondary;             /* fifo_out should link to a fifo_in1 */
	_Bool is_passive;               /* denotes process that does nothing */
	_Bool is_enabled;               /* enabled means it still has data to process */
};
typedef struct process Process;

struct thread_data {
	struct dnode* proc_node;
	struct dgraph* proc_graph;
};

struct process* process_new(const char* action, int width);
struct process* process_construct(struct process*, const char*, int width);
void process_node_free(struct dnode* proc_node);
void process_free(struct process*, _Bool);

void process_activate(Dnode* proc_node, unsigned graph_size);
void process_add_second_input(struct process*);
int process_step(Plan* plan);
int process_exec_plan(struct fql_plan*);
int process_exec_plan_thread(struct fql_plan* plan);
void process_enable(struct process*);
void process_disable(struct process*);

int fql_read(struct dgraph*, struct process*);
int fql_select(struct dgraph*, struct process*);
int fql_logic(struct dgraph*, struct process*);
int fql_cartesian_join(struct dgraph*, struct process*);
int fql_hash_join(struct dgraph*, struct process*);
int fql_no_op(struct dgraph*, struct process*);

#endif /* PROCESS_H */
