#include "process.h"

#include <stdbool.h>
#include <pthread.h>

#include "aggregate.h"
#include "fql.h"
#include "misc.h"
#include "table.h"
#include "group.h"
#include "reader.h"
#include "record.h"
#include "operation.h"
#include "util/vec.h"
#include "util/util.h"
#include "util/dgraph.h"
#include "util/fifo.h"
#include "util/stringview.h"

#define FIFO_SIZE 256

process* process_construct(process* proc, const char* action, plan* plan)
{
	*proc = (process) {
	        0,                            /* thread */
	        NULL,                         /* records */
	        &fql_no_op,                   /* action__ */
	        {NULL, NULL},                 /* fifo_in */
	        {NULL, NULL},                 /* fifo_out */
	        NULL,                         /* proc_data */
	        string_from_char_ptr(action), /* action_msg */
	        NULL,                         /* root_group */
	        NULL,                         /* wait_list */
	        0,                            /* rows_affected */
	        plan->query->top_count,       /* top_count */
	        -1,                           /* max_recs_iter */
	        plan->plan_id,                /* plan_id */
	        0,                            /* subquery_plan_id */
	        0,                            /* root_fifo */
	        plan->source_count,           /* in_src_count */
	        plan->source_count,           /* out_src_count */
	        false,                        /* is_secondary */
	        false,                        /* is_passive */
	        true,                         /* is_enabled */
	        false,                        /* is_const */
	        true,                         /* wait_for_in0 */
	        false                         /* wait_for_in0_end */
	};

	return proc;
}

void process_node_free(dnode* proc_node)
{
	delete_(process, proc_node->data, proc_node->is_root);
}

void process_destroy(process* proc, _Bool is_root)
{
	if (is_root && proc->records != NULL) {
		vec* it = vec_begin(proc->records);
		for (; it != vec_end(proc->records); ++it) {
			record** rec = vec_back(it);
			delete_(record, *rec);
			vec_destroy(it);
		}
		delete_(vec, proc->records);
	}
	delete_if_exists_(fifo, proc->fifo_in[0]);
	delete_if_exists_(fifo, proc->fifo_in[1]);
	delete_if_exists_(vec, proc->wait_list);
	delete_(string, proc->action_msg);
}

void process_activate(dnode* proc_node, plan* plan)
{
	process* proc = proc_node->data;
	unsigned graph_size = plan->processes->nodes->size;
	_Bool is_subquery = proc->subquery_plan_id > 0;
	if (proc->root_group == NULL && is_subquery) {
		proc->root_group =
		        vec_at(plan->recycle_groups, proc->subquery_plan_id);
	} else if (proc->root_group == NULL) {
		proc->root_group = vec_at(plan->recycle_groups, proc->plan_id);
	}

	if (!proc_node->is_root) {
		proc->fifo_in[0] = new_t_(fifo, vec*, FIFO_SIZE);
		return;
	}

	/* lol subquery hack
	 * proc->root_group is subquery root
	 * however, proc is a root of the upper
	 * query. Who wrote this garbage??
	 */
	vec* true_root_group = proc->root_group;
	if (is_subquery) {
		true_root_group = vec_at(plan->recycle_groups, proc->plan_id);
	}

	vec_push_back(true_root_group, &proc_node);
	if (proc->root_fifo == 1) {
		proc->fifo_in[0] = new_t_(fifo, vec*, FIFO_SIZE);
		if (proc->is_const) {
			fifo_advance(proc->fifo_in[0]);
		}
	}

	proc->fifo_in[proc->root_fifo] =
	        new_t_(fifo, vec*, FIFO_SIZE * graph_size);

	int field_count = 1;

	_Bool owns_data = true;
	if (proc->action__ == &fql_read) {
		table* table = proc->proc_data;
		reader* reader = table->reader;
		field_count = reader->max_col_idx + 1;
		owns_data = false;
	} else if (proc->action__ == &fql_read_subquery) {
		table* table = proc->proc_data;
		field_count = table->schema->columns->size;
	} else if (proc->action__ == &fql_groupby) {
		group* group = proc->proc_data;
		field_count = group->columns.size;
	}

	proc->records = new_t_(vec, vec);
	vec_resize(proc->records, FIFO_SIZE * graph_size);

	unsigned i = 0;

	/* if root, it will own the vector of records */
	vec* buf = proc->fifo_in[proc->root_fifo]->buf;
	for (; i < proc->records->size; ++i) {
		vec* new_recs = vec_at(proc->records, i);
		vec_construct_(new_recs, record*);
		vec_resize(new_recs, proc->out_src_count);

		record** new_rec = vec_back(new_recs);
		*new_rec = new_(record, i, field_count, owns_data);

		vec** recs = vec_at(buf, i);
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

void process_add_second_input(process* proc)
{
	proc->fifo_in[1] = new_t_(fifo, vec*, FIFO_SIZE);
}

void process_add_to_wait_list(process* proc, const process* wait_proc)
{
	if (proc->wait_list == NULL) {
		proc->wait_list = new_t_(vec, process*);
	}
	vec_push_back(proc->wait_list, &wait_proc);
}

void process_disable(process* proc)
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

void process_enable(process* proc)
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
 * 0 should not happen unless we are done.
 */
int _exec_one_pass(plan* plan, dgraph* proc_graph)
{
	dnode* proc_node = NULL;
	process* proc = NULL;
	int run_count = 0;
	while ((proc_node = dgraph_traverse(proc_graph))) {
		proc = proc_node->data;
		if (!proc->is_enabled) {
			continue;
		}


		/* Check if the process in front is closed */
		if (proc->fifo_out[0] && !fifo_is_open(proc->fifo_out[0])) {
			process_disable(proc);
			continue;
		}
		if (proc->fifo_out[1] && !fifo_is_open(proc->fifo_out[1])) {
			process_disable(proc);
			continue;
		}

		if (proc->wait_for_in0 && !proc->fifo_in[0]->is_open
		    && fifo_is_empty(proc->fifo_in[0])) {
			if (proc->wait_for_in0_end) {
				proc->wait_for_in0 = false;
			} else {
				process_disable(proc);
				continue;
			}
		}

		if (proc->wait_list != NULL) {
			_Bool enabled = false;
			process** it = vec_begin(proc->wait_list);
			for (; it != vec_end(proc->wait_list); ++it) {
				if ((*it)->is_enabled) {
					enabled = true;
					break;
				}
			}
			if (enabled) {
				++run_count;
				continue;
			}
		}

		/* check to see that there is something to process
		 * as well as a place for it to go.
		 */
		if ((proc->wait_for_in0 && fifo_is_empty(proc->fifo_in[0]))
		    || (proc->fifo_out[0]
		        && !fifo_receivable(proc->fifo_out[0]))
		    || (proc->fifo_out[1]
		        && !fifo_receivable(proc->fifo_out[1]))) {
			++run_count;
			continue;
		}

		int ret = try_(proc->action__(proc_graph, proc));

		if (proc->wait_for_in0) {
			++run_count;
		}
		if (proc->rows_affected >= proc->top_count) {
			process_disable(proc);
		}
		if (proc_node == plan->op_true) {
			plan->rows_affected = proc->rows_affected;
		}
		run_count += ret;
	}

	return run_count;
}

int process_step(plan* plan)
{
	dgraph* proc_graph = plan->processes;
	int ret = 0;

	size_t org_rows_affected = plan->rows_affected;

	do {
		dgraph_traverse_reset(proc_graph);
		ret = _exec_one_pass(plan, proc_graph);
	} while (ret && ret != FQL_FAIL
	         && plan->rows_affected == org_rows_affected);

	return (ret > 0) ? 1 : ret;
}

int process_exec_plan(plan* plan)
{
	dgraph* proc_graph = plan->processes;
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
	dnode* node = tdata->proc_node;
	process* proc = node->data;
	fifo* in0 = proc->fifo_in[0];
	fifo* in1 = proc->fifo_in[1];
	fifo* out0 = proc->fifo_out[0];
	fifo* out1 = proc->fifo_out[1];

	while (proc->is_enabled) {
		if (proc->wait_for_in0 && fifo_is_empty(in0)) {
			if (!in0->is_open) {
				if (proc->wait_for_in0_end) {
					proc->wait_for_in0 = false;
					continue;
				} else {
					process_disable(proc);
					break;
				}
			}
			if (node->is_root) {
				fifo_wait_for_add(in0);
			} else {
				fifo_wait_for_work(in0);
			}
			if (fifo_is_empty(in0)) {
				process_disable(proc);
				break;
			}
		}
		if (in1) {
			if (fifo_is_open(in1) && fifo_is_open(in0)
			    && fifo_is_empty(in1)) {
				//fifo_wait_for_work(in1);
				fifo_wait_for_add(in1);
			}
		}
		if (out0) {
			while (!fifo_receivable(out0)) {
				fifo_wait_for_get(out0);
			}
		}
		if (out1) {
			while (!fifo_receivable(out1)) {
				fifo_wait_for_get(out1);
			}
		}
		int ret = proc->action__(tdata->proc_graph, proc);

		/* TODO: what is best practice here? */
		if (ret == FQL_FAIL) {
			//process_disable(proc);
			exit(EXIT_FAILURE);
			break;
		}
	}

	pthread_exit(NULL);
	return NULL;
}

int process_exec_plan_thread(plan* plan)
{
	dgraph* proc_graph = plan->processes;

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	vec tdata_vec = {0};
	vec_construct_(&tdata_vec, struct thread_data);
	vec_resize(&tdata_vec, proc_graph->nodes->size);

	unsigned i = 0;

	for (; i < tdata_vec.size; ++i) {
		struct thread_data* tdata = vec_at(&tdata_vec, i);
		dnode** proc_node = vec_at(proc_graph->nodes, i);
		tdata->proc_node = *proc_node;
		tdata->proc_graph = proc_graph;
		process* proc = (*proc_node)->data;
		fail_if_(pthread_create(&proc->thread,
		                        &attr,
		                        _thread_exec,
		                        tdata));
	}

	pthread_attr_destroy(&attr);

	void* status = NULL;
	for (i = 0; i < tdata_vec.size; ++i) {
		struct thread_data* tdata = vec_at(&tdata_vec, i);
		process* proc = tdata->proc_node->data;
		fail_if_(pthread_join(proc->thread, &status));
	}

	pthread_exit(NULL);

	return FQL_GOOD;
}
