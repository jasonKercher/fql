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
#include "fqlselect.h"
#include "util/vec.h"
#include "util/util.h"
#include "util/dgraph.h"
#include "util/fifo.h"
#include "util/stringview.h"

struct thread_data {
	struct dnode* proc_node;
	struct dgraph* proc_graph;
};

process* process_construct(process* self, const char* action, plan* plan)
{
	*self = (process) {
	        0,                            /* thread */
	        NULL,                         /* inbuf */
	        NULL,                         /* outbuf */
	        &fql_no_op,                   /* action__ */
	        NULL,                         /* root_ref */
	        {NULL, NULL},                 /* fifo_in */
	        {NULL, NULL},                 /* fifo_out */
	        NULL,                         /* org_fifo_in0 */
	        NULL,                         /* proc_data */
	        string_from_char_ptr(action), /* plan_msg */
	        NULL,                         /* wait_list */
	        new_t_(vec, dnode*),          /* union_end_nodes */
	        NULL,                         /* queued_results */
	        0,                            /* rows_affected */
	        -1,                           /* max_recs_iter */
	        0,                            /* inbuf_idx */
	        0,                            /* outbuf_idx */
	        plan->source_count,           /* in_src_count */
	        plan->source_count,           /* out_src_count */
	        PROCESS_NO_ROOT,              /* root_fifo */
	        false,                        /* is_secondary */
	        false,                        /* is_dual_link */
	        false,                        /* is_passive */
	        true,                         /* is_enabled */
	        false,                        /* is_const */
	        false,                        /* has_second_input */
	        true,                         /* wait_for_in0 */
	        false                         /* wait_for_in0_end */
	};

	return self;
}

void process_node_free(dnode* proc_node)
{
	delete_(process, proc_node->data, proc_node->is_root);
}

void process_destroy(process* self, bool is_root)
{
	if (self->org_fifo_in0 != NULL && self->org_fifo_in0 != self->root_ref) {
		delete_(fifo, self->org_fifo_in0);
	}
	if (self->fifo_in[1] != NULL && self->fifo_in[1] != self->root_ref) {
		delete_(fifo, self->fifo_in[1]);
	}
	delete_if_exists_(vec, self->wait_list);
	delete_if_exists_(vec, self->inbuf);
	delete_if_exists_(vec, self->outbuf);
	delete_(vec, self->union_end_nodes);
	delete_(string, self->plan_msg);
	node_free_func(&self->queued_results, &fifo_free);
}

void process_activate(process* self, plan* plan, unsigned fifo_size)
{
	self->root_ref = plan->root;

	self->inbuf = new_t_(vec, node*);
	self->outbuf = new_t_(vec, node*);

	if (self->root_fifo != PROCESS_NO_ROOT) {
		self->fifo_in[self->root_fifo] = self->root_ref;
	}

	if (self->action__ == &fql_select) {
		fqlselect* select = self->proc_data;
		select->is_const = self->is_const;
	}

	dnode** it = vec_begin(self->union_end_nodes);
	for (; it != vec_end(self->union_end_nodes); ++it) {
		process* select_proc = (*it)->data;
		select_proc->fifo_out[0] = new_t_(fifo, node*, fifo_size);
		select_proc->fifo_out[0]->input_count = 1;
		node_enqueue(&self->queued_results, select_proc->fifo_out[0]);
	}

	if (self->fifo_in[0] == NULL) {
		self->fifo_in[0] = new_t_(fifo, node*, fifo_size);
		self->org_fifo_in0 = self->fifo_in[0];
		/* NOTE: GROUP BY hack. a constant query expression
		 *       containing a group by essentially has 2 roots.
		 *       We just give in[0] a nudge (like a root).
		 */
		if (self->action__ == &fql_groupby && self->is_const) {
			fifo_advance(self->fifo_in[0]);
		}
	}
	if (self->has_second_input) {
		/* should never happen */
		if (self->fifo_in[1] != NULL) {
			fputs("Useless has_second_input\n", stderr);
			return;
		}
		self->fifo_in[1] = new_t_(fifo, node*, fifo_size);
	}
}

void process_add_to_wait_list(process* self, const process* wait_proc)
{
	if (self->wait_list == NULL) {
		self->wait_list = new_t_(vec, process*);
	}
	vec_push_back(self->wait_list, &wait_proc);
}

void process_disable(process* self)
{
	if (self->fifo_out[0] != NULL) {
		fifo_set_open(self->fifo_out[0], false);
	}
	if (self->fifo_out[1] != NULL) {
		fifo_set_open(self->fifo_out[1], false);
	}
	fifo_set_open(self->fifo_in[0], false);
	self->is_enabled = false;
}

void process_enable(process* self)
{
	if (self->fifo_out[0] != NULL) {
		fifo_set_open(self->fifo_out[0], true);
	}
	if (self->fifo_out[1] != NULL) {
		fifo_set_open(self->fifo_out[1], true);
	}
	fifo_set_open(self->fifo_in[0], true);
	self->is_enabled = true;
}

/* returns number of processes that executed or FQL_FAIL
 * 0 should not happen unless we are done.
 */
int _exec_one_pass(plan* plan, dgraph* proc_graph)
{
	dnode* proc_node = NULL;
	process* self = NULL;
	int run_count = 0;
	while ((proc_node = dgraph_traverse(proc_graph))) {
		self = proc_node->data;
		if (!self->is_enabled) {
			continue;
		}

		/* Check if the process in front is closed */
		if (self->fifo_out[0] && !fifo_is_open(self->fifo_out[0])) {
			process_disable(self);
			continue;
		}
		if (self->fifo_out[1] && !fifo_is_open(self->fifo_out[1])) {
			process_disable(self);
			continue;
		}

		if (self->wait_for_in0 && !self->fifo_in[0]->is_open
		    && fifo_is_empty(self->fifo_in[0])
		    && (!self->is_dual_link
		        || (!fifo_is_open(self->fifo_in[1])
		            && fifo_is_empty(self->fifo_in[1])))) {
			if (self->wait_for_in0_end) {
				self->wait_for_in0 = false;
			} else {
				process_disable(self);
				continue;
			}
		}

		if (self->wait_list != NULL) {
			bool enabled = false;
			process** it = vec_begin(self->wait_list);
			for (; it != vec_end(self->wait_list); ++it) {
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
		if (((self->wait_for_in0 && fifo_is_empty(self->fifo_in[0]))
		     && (!self->is_dual_link || fifo_is_empty(self->fifo_in[1])))
		    || (self->fifo_out[0] && !fifo_receivable(self->fifo_out[0]))
		    || (self->fifo_out[1] && !fifo_receivable(self->fifo_out[1]))) {
			++run_count;
			continue;
		}

		int ret = try_(self->action__(self));

		if (self->wait_for_in0) {
			++run_count;
		}
		if (proc_node == plan->op_true) {
			plan->rows_affected = self->rows_affected;
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
	} while (ret && ret != FQL_FAIL && plan->rows_affected == org_rows_affected);

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
	process* self = node->data;
	fifo* in0 = self->fifo_in[0];
	fifo* in1 = self->fifo_in[1];
	fifo* out0 = self->fifo_out[0];
	fifo* out1 = self->fifo_out[1];

	while (self->is_enabled) {
		if (self->wait_for_in0 && fifo_is_empty(in0)) {
			if (!in0->is_open) {
				if (self->wait_for_in0_end) {
					self->wait_for_in0 = false;
					continue;
				} else {
					process_disable(self);
					break;
				}
			}
			//if (node->is_root) {
			fifo_wait_for_add(in0);
			//} else {
			//	//fifo_wait_for_work(in0);
			//}
			if (fifo_is_empty(in0)) {
				process_disable(self);
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
		int ret = self->action__(self);

		/* TODO: what is best practice here? */
		if (ret == FQL_FAIL) {
			//process_disable(self);
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
		process* self = (*proc_node)->data;
		fail_if_(pthread_create(&self->thread, &attr, _thread_exec, tdata));
	}

	pthread_attr_destroy(&attr);

	void* status = NULL;
	for (i = 0; i < tdata_vec.size; ++i) {
		struct thread_data* tdata = vec_at(&tdata_vec, i);
		process* self = tdata->proc_node->data;
		fail_if_(pthread_join(self->thread, &status));
	}

	pthread_exit(NULL);

	return FQL_GOOD;
}
