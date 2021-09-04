#ifndef PROCESS_H
#define PROCESS_H

#define PROCESS_NO_PIPE_INDEX -1

#include "fql.h"
#include "fqlplan.h"
#include "util/stringy.h"
#include "util/fifo.h"
#include "util/node.h"
#include "util/dgraph.h"

struct process;
typedef int(process_fn)(struct process*);

struct process {
	pthread_t thread;             /* pthread handle */
	vec* inbuf;                   /* just a buffer to hold spare records */
	vec* outbuf;                  /* Holds records that don't fit in pipe */
	process_fn* action__;         /* function pointer for process */
	struct fifo* root_ref;        /* Reference to plan-wide root-records */
	struct fifo* fifo_in[2];      /* input record fifos */
	struct fifo* fifo_out[2];     /* output record fifos */
	struct fifo* org_fifo_in0;    /* Used to avoid memory leak with UNION */
	void* proc_data;              /* process specific data */
	string* plan_msg;             /* message that prints with plan */
	struct vec* wait_list;        /* list of fifos that we wait for */
	struct vec* union_end_nodes;  /* nodes that will re-link for union */
	struct node* queued_results;  /* list of additional input fifos */
	size_t rows_affected;         /* if process is true proc, track this */
	size_t max_recs_iter;         /* Max recs allowed per iteration */
	unsigned inbuf_idx;           /* idx for inbuf */
	unsigned outbuf_idx;          /* idx for outbuf */
	short in_src_count;           /* number of input sources at this step */
	short out_src_count;          /* number of output sources at this step */
	short root_fifo;              /* index of root for fifo_in[x] */
	short killed_pipe;            /* start a process with a closed pipe */
	bool is_secondary;            /* fifo_out0 should link to a fifo_in1 */
	bool is_dual_link;            /* both fifo_out link both fifo_in */
	bool is_passive;              /* denotes process that does nothing */
	bool is_enabled;              /* enabled means it still has work to do*/
	bool is_const;                /* should only run 1 time */
	bool is_op_true;              /* is proc_graph->op_true->data */
	bool has_second_input;        /* will activate fifo_in[1] */
	bool wait_for_in0;            /* allow start before in0 populated */
	bool wait_for_in0_end;        /* allow more processing after in0 done */
};
typedef struct process process;

struct process*
process_construct(struct process*, const char*, struct fql_plan*);
void process_destroy(struct process*, bool);
void process_node_free(struct dnode* proc_node);

void process_activate(struct process*, struct fql_plan*, unsigned fifo_size);
int process_step(struct fql_plan*);
int process_exec_plan(struct fql_plan*);
int process_exec_plan_thread(struct fql_plan*);
void process_add_to_wait_list(struct process*, const struct process*);
void process_enable(struct process*);
void process_disable(struct process*);

enum proc_return {
	PROC_RETURN_FAIL = FQL_FAIL,
	PROC_RETURN_COMPLETE = 0,
	PROC_RETURN_RUNNING,
	PROC_RETURN_WAIT_ON_IN0,
	PROC_RETURN_WAIT_ON_IN1,
	PROC_RETURN_WAIT_ON_IN_ANY,
	PROC_RETURN_WAIT_ON_IN_BOTH,
	PROC_RETURN_WAIT_ON_OUT0,
	PROC_RETURN_WAIT_ON_OUT1,
};

enum proc_return fql_read(struct process*);
enum proc_return fql_select(struct process*);
enum proc_return fql_delete(struct process*);
enum proc_return fql_delete_filter(struct process*);
enum proc_return fql_update(struct process*);
enum proc_return fql_update_filter(struct process*);
enum proc_return fql_logic(struct process*);
enum proc_return fql_left_join_logic(struct process*);
enum proc_return fql_cartesian_join(struct process*);
enum proc_return fql_hash_join(struct process*);
enum proc_return fql_distinct(struct process*);
enum proc_return fql_groupby(struct process*);
enum proc_return fql_orderby(struct process*);
enum proc_return fql_no_op(struct process*);

#endif /* PROCESS_H */
