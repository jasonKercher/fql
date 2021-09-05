#include "process.h"

#include <stdbool.h>
#include <pthread.h>

#include "fql.h"
#include "misc.h"
#include "table.h"
#include "group.h"
#include "reader.h"
#include "record.h"
#include "aggregate.h"
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
	        plan->fql_ref,                /* fql_ref */
	        NULL,                         /* inbuf */
	        NULL,                         /* inbuf_iter */
	        &fql_no_op,                   /* action__ */
	        NULL,                         /* root_ref */
	        {NULL, NULL},                 /* fifo_in */
	        {NULL, NULL},                 /* fifo_out */
	        NULL,                         /* org_fifo_in0 */
	        NULL,                         /* proc_data */
	        string_from_char_ptr(action), /* plan_msg */
	        new_t_(vec, dnode*),          /* union_end_nodes */
	        NULL,                         /* queued_results */
	        NULL,                         /* wait_list */
	        NULL,                         /* waitee_proc */
	        {0},                          /* wait_mutex */
	        {0},                          /* wait_cond */
	        0,                            /* rows_affected */
	        -1,                           /* max_recs_iter */
	        plan->source_count,           /* in_src_count */
	        plan->source_count,           /* out_src_count */
	        PROCESS_NO_PIPE_INDEX,        /* root_fifo */
	        PROCESS_NO_PIPE_INDEX,        /* killed_pipe */
	        false,                        /* is_secondary */
	        false,                        /* is_dual_link */
	        false,                        /* is_passive */
	        true,                         /* is_enabled */
	        false,                        /* is_const */
	        false,                        /* is_op_true */
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
	delete_(vec, self->union_end_nodes);
	delete_(string, self->plan_msg);
	node_free_func(&self->queued_results, &fifo_free);
}

void process_activate(process* self,
                      plan* plan,
                      unsigned fifo_size)
{
	self->root_ref = plan->root;

	self->inbuf = new_t_(vec, node*);
	vec_reserve(self->inbuf, fifo_size);
	self->inbuf_iter = vec_begin(self->inbuf);

	if (self->root_fifo != PROCESS_NO_PIPE_INDEX) {
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

	if (self->killed_pipe != PROCESS_NO_PIPE_INDEX) {
		self->fifo_in[self->killed_pipe]->is_open = false;
	}

	if (self->fql_ref->props.verbose) {
		fprintf(stderr, "Activate process: %s\n", string_c_str(self->plan_msg));
	}
}

void process_add_to_wait_list(process* self, process* wait_proc)
{
	if (self->wait_list == NULL) {
		self->wait_list = new_t_(vec, process*);
	}
	pthread_mutex_init(&self->wait_mutex, NULL);
	pthread_cond_init(&self->wait_cond, NULL);
	wait_proc->waitee_proc = self;
	vec_push_back(self->wait_list, &wait_proc);
}

void process_disable(process* self)
{
	if (!self->is_enabled) {
		fprintf(stderr,
		        "process `%s' already disabled\n",
		        string_c_str(self->plan_msg));
		return;
	}

	if (self->fifo_out[0] != NULL) {
		fifo_set_open(self->fifo_out[0], false);
	}
	if (self->fifo_out[1] != NULL) {
		fifo_set_open(self->fifo_out[1], false);
	}
	fifo_set_open(self->fifo_in[0], false);
	self->is_enabled = false;

	/* Now we want to recycle any records sitting
	 * in the input fifos
	 */

	if (self->fifo_in[0] != NULL && self->fifo_in[0] != self->root_ref) {
		fifo* in = self->fifo_in[0];
		node** it = fifo_begin(in);
		for (; it != fifo_end(in); it = fifo_iter(in)) {
			fifo_add(self->root_ref, it);
		}
		fifo_update(in);
	}
	if (self->fifo_in[1] != NULL && self->fifo_in[1] != self->root_ref) {
		fifo* in = self->fifo_in[1];
		node** it = fifo_begin(in);
		for (; it != fifo_end(in); it = fifo_iter(in)) {
			fifo_add(self->root_ref, it);
		}
		fifo_update(in);
	}

	/* This process must terminate before waitee_proc can
	 * Begin. Time to let it know we have finished.
	 */
	if (self->waitee_proc != NULL) {
		pthread_mutex_lock(&self->waitee_proc->wait_mutex);
		pthread_cond_signal(&self->waitee_proc->wait_cond);
		pthread_mutex_unlock(&self->waitee_proc->wait_mutex);
	}

	if (self->fql_ref->props.verbose) {
		unsigned len = (self->plan_msg->size > 25) ? 25 : self->plan_msg->size;
		fprintf(stderr,
		        "Process `%.*s' stopped\n",
		        len,
		        string_c_str(self->plan_msg));
	}
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

bool _check_wait_list(struct vec* wait_list)
{
	bool enabled = false;
	process** it = vec_begin(wait_list);
	for (; it != vec_end(wait_list); ++it) {
		if ((*it)->is_enabled) {
			enabled = true;
			break;
		}
	}
	return enabled;
}

/* Pass through each process in the graph one time.
 * Returns:
 *   number > 0: still running
 *   0         : finished
 *   FQL_FAIL  : runtime error
 */
int _exec_one_pass(plan* plan, dgraph* proc_graph)
{
	int run_count = 0;

	process** it = vec_begin(plan->execution_vector);
	for (; it != vec_end(plan->execution_vector); ++it) {
		if (!(*it)->is_enabled) {
			continue;
		}

		if ((*it)->wait_list != NULL) {
			if (_check_wait_list((*it)->wait_list)) {
				++run_count;
				continue;
			}
		}

		int ret = try_((*it)->action__(*it));

		if ((*it)->wait_for_in0) {
			++run_count;
		}
		if ((*it)->is_op_true) {
			plan->rows_affected = (*it)->rows_affected;
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
		ret = _exec_one_pass(plan, proc_graph);
	} while (ret && ret != FQL_FAIL && plan->rows_affected == org_rows_affected);

	return (ret > 0) ? 1 : ret;
}

int process_exec_plan(plan* plan)
{
	dgraph* proc_graph = plan->processes;
	int ret = 0;

	do {
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

	/* If there is any reason this process must wait in
	 * order to execute, wait on that first. Then, we can
	 * enter the regular process loop. An example of such a
	 * case would be a subquery as an expression:
	 *
	 * SELECT 'hello', (select max(name) from list)
	 *
	 * This select process cannot execute until that subquery
	 * has completed. In this example, the select process for
	 * the subquery will own a reference to this process to so
	 * it can wake it up when it is complete.
	 */

	if (self->wait_list != NULL) {
		pthread_mutex_lock(&self->wait_mutex);
		while (_check_wait_list(self->wait_list)) {
			pthread_cond_wait(&self->wait_cond, &self->wait_mutex);
		}
		pthread_mutex_unlock(&self->wait_mutex);
	}

	while (self->is_enabled) {
		switch (self->action__(self)) {
		case PROC_RETURN_COMPLETE:
			//process_disable(self);
			break;
		case PROC_RETURN_FAIL:
			/* TODO */
			exit(EXIT_FAILURE);
		case PROC_RETURN_WAIT_ON_IN0:
			if (in0->is_open && fifo_is_empty(in0)) {
				fifo_wait_for_add(in0);
			}
			break;
		case PROC_RETURN_WAIT_ON_IN1:
			if (in1->is_open && fifo_is_empty(in1)) {
				fifo_wait_for_add(in1);
			}
			break;
		case PROC_RETURN_WAIT_ON_OUT0:
			if (out0->is_open && fifo_is_full(out0)) {
				fifo_wait_for_get(out0);
			}
			break;
		case PROC_RETURN_WAIT_ON_OUT1:
			if (out1->is_open && fifo_is_full(out1)) {
				fifo_wait_for_get(out1);
			}
			break;
		case PROC_RETURN_WAIT_ON_IN_EITHER:
			if (in0->is_open && in1->is_open && fifo_is_empty(in0)
			    && fifo_is_empty(in1)) {
				fifo_wait_for_add_either(in0, in1);
			}
			break;
		case PROC_RETURN_WAIT_ON_IN_BOTH:
			if (in0->is_open && in1->is_open
			    && (fifo_is_empty(in0) || fifo_is_empty(in1))) {
				fifo_wait_for_add_both(in0, in1);
			}
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
