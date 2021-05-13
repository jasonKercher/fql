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
	pthread_t thread;       /* pthread handle */
	struct vec* records;    /* this is the owned record data for roots */
	process_fn* action__;   /* function pointer for process */
	fifo* fifo_in[2];       /* ring buffer of records */
	fifo* fifo_out[2];      /* default next process fifo */
	void* proc_data;        /* process specific data */
	string* action_msg;     /* message that prints with plan */
	vec* root_group;        /* group of recyclable roots for this process */
	short plan_id;          /* plan ID for root grouping */
	short subquery_plan_id; /* plan ID for subquery */
	short root_fifo;        /* signify which fifo_inx is the root */
	short in_src_count;     /* number of input sources at this step */
	short out_src_count;    /* number of output sources at this step */
	_Bool is_secondary;     /* fifo_out should link to a fifo_in1 */
	_Bool is_passive;       /* denotes process that does nothing */
	_Bool is_enabled;       /* enabled means it still has data to process */
	_Bool is_const;         /* should only run 1 time */
	_Bool wait_for_in0;     /* allow process to start before in0 populated */
	_Bool wait_for_in0_end; /* do not kill process if in0 proc terminated */
};
typedef struct process process;

struct thread_data {
	struct dnode* proc_node;
	struct dgraph* proc_graph;
};

struct process* process_construct(struct process*, const char*, struct fql_plan*);
void process_destroy(struct process*, _Bool);
void process_node_free(struct dnode* proc_node);

void process_activate(dnode* proc_node, struct fql_plan*);
void process_add_second_input(struct process*);
int process_step(plan* plan);
int process_exec_plan(struct fql_plan*);
int process_exec_plan_thread(struct fql_plan* plan);
void process_enable(struct process*);
void process_disable(struct process*);

int fql_read(struct dgraph*, struct process*);
int fql_read_subquery(struct dgraph*, struct process*);
int fql_select(struct dgraph*, struct process*);
int fql_logic(struct dgraph*, struct process*);
int fql_cartesian_join(struct dgraph*, struct process*);
int fql_hash_join(struct dgraph*, struct process*);
int fql_distinct(struct dgraph*, struct process*);
int fql_groupby(struct dgraph*, struct process*);
int fql_no_op(struct dgraph*, struct process*);

#endif /* PROCESS_H */
