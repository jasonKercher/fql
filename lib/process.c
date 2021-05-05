#include "process.h"

#include <stdbool.h>
#include <pthread.h>
#include "fql.h"
#include "table.h"
#include "group.h"
#include "operation.h"
#include "reader.h"
#include "record.h"
#include "util/vec.h"
#include "util/util.h"
#include "util/dgraph.h"
#include "util/fifo.h"
#include "util/stringview.h"

#define FIFO_SIZE 256

Process* process_new(const char* action, Plan* plan)
{
	Process* new_proc = NULL;
	malloc_(new_proc, sizeof(*new_proc));

	return process_construct(new_proc, action, plan);
}

Process* process_construct(Process* proc, const char* action, Plan* plan)
{
	*proc = (Process) {
		 0                              /* thread */
		,NULL                           /* records */
		,&fql_no_op                     /* action__ */
		,{ NULL, NULL }                 /* fifo_in */
		,{ NULL, NULL }                 /* fifo_out */
		,NULL                           /* proc_data */
		,string_from_char_ptr(action)   /* action_msg */
		,NULL                           /* root_group */
		,plan->source_count             /* fifo_width */
		,plan->plan_id                  /* plan_id */
		,0                              /* subquery_plan_id */
		,0                              /* root_fifo */
		,false                          /* is_secondary */
		,false                          /* is_passive */
		,true                           /* is_enabled */
		,false                          /* is_const */
		,true                           /* wait_for_in0 */
		,false                          /* wait_for_in0_end */
	};

	return proc;
}

void process_node_free(Dnode* proc_node)
{
	process_free(proc_node->data, proc_node->is_root);
}

void process_free(Process* proc, _Bool is_root)
{
	if (is_root && proc->records != NULL) {
		Vec* it = vec_begin(proc->records);
		for (; it != vec_end(proc->records); ++it) {
			Record** rec = vec_back(it);
			record_free(*rec);
			vec_destroy(it);
		}
		vec_free(proc->records);
	}
	if (proc->fifo_in[0] != NULL) {
		fifo_free(proc->fifo_in[0]);
	}
	if (proc->fifo_in[1] != NULL) {
		fifo_free(proc->fifo_in[1]);
	}
	string_free(proc->action_msg);
	free_(proc);
}

void process_activate(Dnode* proc_node, Plan* plan)
{
	Process* proc = proc_node->data;
	unsigned graph_size = plan->processes->nodes->size;
	_Bool is_subquery = proc->subquery_plan_id > 0;
	if (is_subquery) {
		proc->root_group = vec_at(plan->recycle_groups, proc->subquery_plan_id);
	} else {
		proc->root_group = vec_at(plan->recycle_groups, proc->plan_id);
	}

	if (!proc_node->is_root) {
		proc->fifo_in[0] = fifo_new_(Vec*, FIFO_SIZE);
		return;
	}

	/* lol subquery hack
	 * proc->root_group is subquery root
	 * However, proc is a root of the upper query
	 */
	Vec* true_root_group = proc->root_group;
	if (is_subquery) {
		true_root_group = vec_at(plan->recycle_groups, proc->plan_id);
	}

	vec_push_back(true_root_group, &proc_node);
	if (proc->root_fifo == 1) {
		proc->fifo_in[0] = fifo_new_(Vec*, FIFO_SIZE);
		if (proc->is_const) {
			fifo_advance(proc->fifo_in[0]);
		}
	}

	proc->fifo_in[proc->root_fifo] = fifo_new_(Vec*, FIFO_SIZE * graph_size);

	int field_count = 1;

	_Bool owns_data = true;
	if (proc->action__ == &fql_read) {
		Table* table = proc->proc_data;
		Reader* reader = table->reader;
		field_count = reader->max_col_idx + 1;
		owns_data = false;
	}
	else if (proc->action__ == &fql_read_subquery) {
		Table* table = proc->proc_data;
		field_count = table->schema->columns->size;
	}
	else if (proc->action__ == &fql_groupby) {
		Group* group = proc->proc_data;
		field_count = group->columns.size;
	}

	proc->records = vec_new_(Vec);
	vec_resize(proc->records, FIFO_SIZE * graph_size);

	int i = 0;

	/* If root, it will own the vector of Records */
	Vec* buf = proc->fifo_in[proc->root_fifo]->buf;
	for (; i < proc->records->size; ++i) {
		Vec* new_recs = vec_at(proc->records, i);
		vec_construct_(new_recs, Record*);
		vec_resize(new_recs, proc->fifo_width);

		Record** new_rec = vec_back(new_recs);
		*new_rec = record_new(i, field_count, owns_data);

		Vec** recs = vec_at(buf, i);
		*recs = new_recs;
	}

	/* TODO: once we stop hard coding fifo size,
	 *       this if block can go.
	 */
	if (proc->is_const) {
		fifo_advance(proc->fifo_in[proc->root_fifo]);
		return;
	}
	fifo_set_full(proc->fifo_in[proc->root_fifo]);
}

void process_add_second_input(Process* proc)
{
	proc->fifo_in[1] = fifo_new_(Vec*, FIFO_SIZE);
}

void process_disable(Process* proc)
{
	if (proc->fifo_out[0] != NULL) {
		fifo_set_open(proc->fifo_out[0], false);
	}
	if (proc->fifo_out[1] != NULL) {
		fifo_set_open(proc->fifo_out[1], false);
	}
	fifo_set_open(proc->fifo_in[0], false);
	proc->is_enabled = false;
}

void process_enable(Process* proc)
{
	if (proc->fifo_out[0] != NULL) {
		fifo_set_open(proc->fifo_out[0], true);
	}
	if (proc->fifo_out[1] != NULL) {
		fifo_set_open(proc->fifo_out[1], true);
	}
	fifo_set_open(proc->fifo_in[0], true);
	proc->is_enabled = true;
}

/* returns number of processes that executed or FQL_FAIL
 * 0 should not happen unless we EOF
 */
int _exec_one_pass(Plan* plan, Dgraph* proc_graph)
{
	Dnode* proc_node = NULL;
	Process* proc = NULL;
	int run_count = 0;
	while ((proc_node = dgraph_traverse(proc_graph))) {
		proc = proc_node->data;
		if (!proc->is_enabled) {
			continue;
		}
		if (proc->wait_for_in0
		 && !proc->fifo_in[0]->is_open 
		 && fifo_is_empty(proc->fifo_in[0])) {
			if (proc->wait_for_in0_end) {
				proc->wait_for_in0 = false;
			} else {
				process_disable(proc);
				continue;
			}
		}

		/* Check to see that there is something to process
		 * as well as a place for it to go.
		 */
		if (proc->wait_for_in0 && fifo_is_empty(proc->fifo_in[0])
		 || (proc->fifo_out[0] && !fifo_is_receivable(proc->fifo_out[0]))
		 || (proc->fifo_out[1] && !fifo_is_receivable(proc->fifo_out[1]))) {
			continue;
		}
		int ret = proc->action__(proc_graph, proc);
		if (ret == FQL_FAIL) {
			return FQL_FAIL;
		}

		if (proc_node == plan->op_true) {
			++plan->rows_affected;
		}
		run_count += ret;
	}

	return run_count;
}

int process_step(Plan* plan)
{
	Dgraph* proc_graph = plan->processes;
	int ret = 0;

	plan->rows_affected = 0;

	do {
		dgraph_traverse_reset(proc_graph);
		ret = _exec_one_pass(plan, proc_graph);
	} while (!plan->rows_affected && ret && ret != FQL_FAIL);

	return (ret > 0) ? 1 : ret;
}

int process_exec_plan(Plan* plan)
{
	Dgraph* proc_graph = plan->processes;
	int ret = 0;

	do {
		dgraph_traverse_reset(proc_graph);
		ret = _exec_one_pass(plan, proc_graph);
	} while (ret && ret != FQL_FAIL);

	return ret;
}

void* _thread_exec(void* data)
{
	struct thread_data* tdata = data;
	Process* proc = tdata->proc_node->data;

	while (proc->is_enabled) {
		if (proc->wait_for_in0 && fifo_is_empty(proc->fifo_in[0])) {
			if (!proc->fifo_in[0]->is_open) {
				if (proc->wait_for_in0_end) {
					proc->wait_for_in0 = false;
					continue;
				} else {
					process_disable(proc);
					break;
				}
			}
			if (tdata->proc_node->is_root) {
				fifo_wait_for_add(proc->fifo_in[0]);
			} else {
				fifo_wait_for_work(proc->fifo_in[0]);
			}
			if (fifo_is_empty(proc->fifo_in[0])) {
				process_disable(proc);
				break;
			}
		}
		if (proc->fifo_in[1]) {
			if (fifo_is_open(proc->fifo_in[1]) &&
			    fifo_is_open(proc->fifo_in[0]) &&
			    fifo_is_empty(proc->fifo_in[1])) {
				//fifo_wait_for_work(proc->fifo_in[1]);
				fifo_wait_for_add(proc->fifo_in[1]);
			}
		}
		if (proc->fifo_out[0]) {
			while (!fifo_is_receivable(proc->fifo_out[0])) {
				fifo_wait_for_get(proc->fifo_out[0]);
			}
		}
		if (proc->fifo_out[1]) {
			while (!fifo_is_receivable(proc->fifo_out[1])) {
				fifo_wait_for_get(proc->fifo_out[1]);
			}
		}
		int ret = proc->action__(tdata->proc_graph, proc);

		/* TODO: What is best practice here? */
		if (ret == FQL_FAIL) {
			//process_disable(proc);
			exit(EXIT_FAILURE);
			break;
		}
	}

	pthread_exit(NULL);
	return NULL;
}

int process_exec_plan_thread(Plan* plan)
{
	Dgraph* proc_graph = plan->processes;

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	Vec tdata_vec = { 0 };
	vec_construct_(&tdata_vec, struct thread_data);
	vec_resize(&tdata_vec, proc_graph->nodes->size);

	int i = 0;

	for (; i < tdata_vec.size; ++i) {
		struct thread_data* tdata = vec_at(&tdata_vec, i);
		Dnode** proc_node = vec_at(proc_graph->nodes, i);
		tdata->proc_node = *proc_node;
		tdata->proc_graph = proc_graph;
		Process* proc = (*proc_node)->data;
		if (pthread_create(&proc->thread, &attr, _thread_exec, tdata)) {
			return FQL_FAIL;
		}
	}

	pthread_attr_destroy(&attr);

	void* status = NULL;
	for (i = 0; i < tdata_vec.size; ++i) {
		struct thread_data* tdata = vec_at(&tdata_vec, i);
		Process* proc = tdata->proc_node->data;
		if (pthread_join(proc->thread, &status)) {
			return FQL_FAIL;
		}
	}

	pthread_exit(NULL);

	return FQL_GOOD;
}
