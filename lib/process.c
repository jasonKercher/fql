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

#define THREAD_MONITOR_INTERVAL_U 2000

struct thread_data {
	struct dnode* proc_node;
	struct fqlplan* plan;
};

process* process_construct(process* self, const char* action, plan* plan)
{
	*self = (process) {
	        .fql_ref = plan->fql_ref,
	        .action__ = &fql_no_op,
	        .plan_msg = string_from_char_ptr(action),
	        .union_end_nodes = new_t_(vec, dnode*),
	        .max_recs_iter = -1,
	        .plan_id = plan->plan_id,
	        .in_src_count = plan->source_count,
	        .out_src_count = plan->source_count,
	        .root_fifo = PROCESS_NO_PIPE_INDEX,
	        .root_fifo_idx = -1,
	        .killed_pipe = PROCESS_NO_PIPE_INDEX,
	        .is_enabled = true,
	        .wait_for_in0 = true,
	};

	return self;
}

void process_node_free(dnode* proc_node)
{
	delete_(process, proc_node->data, proc_node->is_root);
}

void process_destroy(process* self, bool is_root)
{
	if (self->org_fifo_in0 != NULL && self->root_fifo != 0) {
		delete_(fifo, self->org_fifo_in0);
	}
	if (self->fifo_in[1] != NULL && self->root_fifo != 1) {
		delete_(fifo, self->fifo_in[1]);
	}
	delete_if_exists_(vec, self->wait_list);
	delete_if_exists_(vec, self->inbuf);
	delete_if_exists_(vec, self->rebuf);
	delete_if_exists_(vec, self->outbuf);
	delete_(vec, self->union_end_nodes);
	delete_(string, self->plan_msg);
	node_free_func(&self->queued_results, &fifo_free);
}

fifo* _proc_new_fifo_root(vec* roots, unsigned* pipe_count)
{
	fifo* new_root = new_t_(fifo, node*, 2);
	vec_push_back(roots, &new_root);
	++(*pipe_count);
	return new_root;
}

void process_activate(process* self, plan* plan, unsigned base_size, unsigned* pipe_count)
{
	self->rootvec_ref = plan->root_fifo_vec;
	self->fifo_base_size = base_size;

	/* inbuf is not a pipe, but we should count it because it
	 * can pull records out of the pipeline and hold them. If
	 * we do not account for the input buffers, we could run
	 * out of records in the pipeline and deadlock.
	 */
	++(*pipe_count);
	self->inbuf = new_t_(vec, node*);
	vec_reserve(self->inbuf, base_size);
	self->inbuf_iter = vec_begin(self->inbuf);

	//++(*pipe_count);
	//self->outbuf = new_t_(vec, node*);
	//vec_reserve(self->outbuf, base_size);
	//self->outbuf_iter = vec_begin(self->outbuf);

	++(*pipe_count);
	self->rebuf = new_t_(vec, node*);
	vec_reserve(self->rebuf, base_size);
	self->rebuf_iter = vec_begin(self->rebuf);

	if (self->root_fifo != PROCESS_NO_PIPE_INDEX) {
		self->root_fifo_idx = plan->root_fifo_vec->size;
		self->fifo_in[self->root_fifo] =
		        _proc_new_fifo_root(plan->root_fifo_vec, pipe_count);
	}

	if (self->action__ == &fql_select) {
		fqlselect* select = self->proc_data;
		select->is_const = self->is_const;
	}

	dnode** it = vec_begin(self->union_end_nodes);
	for (; it != vec_end(self->union_end_nodes); ++it) {
		process* select_proc = (*it)->data;
		++(*pipe_count);
		select_proc->fifo_out[0] = new_t_(fifo, node*, base_size);
		select_proc->fifo_out[0]->input_count = 1;
		node_enqueue(&self->queued_results, select_proc->fifo_out[0]);
	}

	if (self->fifo_in[0] == NULL) {
		++(*pipe_count);
		self->fifo_in[0] = new_t_(fifo, node*, base_size);
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
		++(*pipe_count);
		self->fifo_in[1] = new_t_(fifo, node*, base_size);
	}

	self->org_fifo_in0 = self->fifo_in[0];

	if (self->killed_pipe != PROCESS_NO_PIPE_INDEX) {
		self->fifo_in[self->killed_pipe]->is_open = false;
	}

	if (self->fifo_aux_root != NULL) {
		self->fifo_aux_root =
		        _proc_new_fifo_root(plan->root_fifo_vec, pipe_count);
	}

	if (self->fql_ref->props.verbosity == FQL_DEBUG) {
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

void process_enable(process* self)
{
	if (self->rebuf != NULL) {
		fqlprocess_recycle_buffer(self, self->rebuf, &self->rebuf_iter);
	}
	if (self->inbuf != NULL) {
		fqlprocess_recycle_buffer(self, self->inbuf, &self->inbuf_iter);
	}
	if (self->outbuf != NULL) {
		fqlprocess_recycle_buffer(self, self->outbuf, &self->outbuf_iter);
	}

	if (self->queued_results != NULL) {
		for (;;) {
			fifo_reset(self->queued_results->data);
			if (self->queued_results->prev == NULL) {
				break;
			}
			self->queued_results = self->queued_results->prev;
		}
		self->fifo_in[0] = self->org_fifo_in0;
	}

	self->rows_affected = 0;
	fifo_reset(self->fifo_in[0]);
	fifo_reset(self->fifo_in[1]);
	fifo_reset(self->fifo_out[0]);
	fifo_reset(self->fifo_out[1]);

	if (self->killed_pipe != PROCESS_NO_PIPE_INDEX) {
		self->fifo_in[self->killed_pipe]->is_open = false;
	}

	if (!self->in0_always_dead) {
		self->wait_for_in0 = true;
	}
	self->is_enabled = true;

	/* Same GROUP BY hack from process_activate */
	if (self->action__ == &fql_groupby && self->is_const) {
		fifo_advance(self->fifo_in[0]);
	}
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
	self->is_enabled = false;
	fifo_set_open(self->fifo_in[0], false);

	/* Now we want to recycle any records sitting
	 * in the input fifos
	 */

	if (self->fifo_in[0] != NULL && self->root_fifo != 0) {
		fifo* in = self->fifo_in[0];
		node** it = fifo_begin(in);
		for (; it != fifo_end(in); it = fifo_iter(in)) {
			fqlprocess_recycle(self, it);
		}
		fifo_update(in);
	}
	if (self->fifo_in[1] != NULL && self->root_fifo != 1) {
		//fifo_set_open(self->fifo_in[1], false);
		fifo* in = self->fifo_in[1];
		node** it = fifo_begin(in);
		for (; it != fifo_end(in); it = fifo_iter(in)) {
			fqlprocess_recycle(self, it);
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

	if (self->fql_ref->props.verbosity == FQL_DEBUG) {
		unsigned len = (self->plan_msg->size > 25) ? 25 : self->plan_msg->size;
		fprintf(stderr,
		        "Process `%.*s' stopped\n",
		        len,
		        string_c_str(self->plan_msg));
	}
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

		enum proc_return ret = try_((*it)->action__(*it));
		if (ret == PROC_RETURN_COMPLETE) {
			process_disable(*it);
		}

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

	if (ret == 0) {
		plan->is_complete = true;
	}

	return (ret > 0) ? 1 : ret;
}

int process_exec_plan(plan* plan)
{
	dgraph* proc_graph = plan->processes;
	int ret = 0;

	do {
		ret = _exec_one_pass(plan, proc_graph);
	} while (ret && ret != FQL_FAIL);

	plan->is_complete = true;

	return ret;
}

void* _thread_exec(void* data)
{
	struct thread_data* tdata = data;
	dnode* node = tdata->proc_node;
	process* self = node->data;
	//fifo* in0 = self->fifo_in[0];
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
	 * the subquery will own a reference to this process so
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
			process_disable(self);
			break;
		case PROC_RETURN_FAIL:
			/* TODO */
			exit(EXIT_FAILURE);
		case PROC_RETURN_WAIT_ON_IN0:
			if (self->fifo_in[0]->is_open
			    && fifo_is_empty(self->fifo_in[0])) {
				fifo_wait_for_add(self->fifo_in[0]);
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
			if (self->fifo_in[0]->is_open && in1->is_open
			    && fifo_is_empty(self->fifo_in[0]) && fifo_is_empty(in1)) {
				fifo_wait_for_add_either(self->fifo_in[0], in1);
			}
			break;
		case PROC_RETURN_WAIT_ON_IN_BOTH:
			if (self->fifo_in[0]->is_open && in1->is_open
			    && (fifo_is_empty(self->fifo_in[0]) || fifo_is_empty(in1))) {
				fifo_wait_for_add_both(self->fifo_in[0], in1);
			}
			break;
		}
	}

	if (node == tdata->plan->op_true) {
		tdata->plan->rows_affected = self->rows_affected;
	}

	pthread_exit(NULL);
	return NULL;
}

int _join_threads(vec* tdata_vec)
{
	int ret = 0;
	unsigned i = 0;
	void* status = NULL;
	for (; i < tdata_vec->size; ++i) {
		struct thread_data* tdata = vec_at(tdata_vec, i);
		process* proc = tdata->proc_node->data;
		ret += pthread_join(proc->thread, &status);
	}
	return ret;
}

void _print_fifo_info(process* self, fifo** root_fifo, fifo* pipe, FILE* debug_file)
{
	int root_idx = -1;
	if (root_fifo != NULL && *root_fifo == pipe) {
		root_idx = self->root_fifo_idx;
	}
	fprintf(debug_file,
	        "%d\t%u\t%u\t%u\t%u\n",
	        root_idx,
	        pipe->head,
	        pipe->tail,
	        fifo_available(pipe),
	        fifo_receivable(pipe));
}

void _print_proc_info(process* self, FILE* debug_file, size_t poll_id, unsigned idx)
{
	fifo** root_fifo = NULL;
	if (self->root_fifo != PROCESS_NO_PIPE_INDEX) {
		root_fifo = vec_at(self->rootvec_ref, self->root_fifo_idx);
	}
	if (self->fifo_in[0] != NULL) {
		fprintf(debug_file, "%lu\t%u\t%d\tin0\t", poll_id, idx, self->is_enabled);
		_print_fifo_info(self, root_fifo, self->fifo_in[0], debug_file);
	}
	if (self->fifo_in[1] != NULL) {
		fprintf(debug_file, "%lu\t%u\t%d\tin1\t", poll_id, idx, self->is_enabled);
		_print_fifo_info(self, root_fifo, self->fifo_in[1], debug_file);
	}
	if (self->fifo_out[0] != NULL) {
		fprintf(debug_file,
		        "%lu\t%u\t%d\tout0\t",
		        poll_id,
		        idx,
		        self->is_enabled);
		_print_fifo_info(self, root_fifo, self->fifo_out[0], debug_file);
	}
	if (self->fifo_out[1] != NULL) {
		fprintf(debug_file,
		        "%lu\t%u\t%d\tout1\t",
		        poll_id,
		        idx,
		        self->is_enabled);
		_print_fifo_info(self, root_fifo, self->fifo_out[1], debug_file);
	}
}

/* this is only really for debugging purposes */
int _monitor_threads(vec* tdata_vec)
{
	void* status = NULL;
	int threading_procs = 0;

	FILE* debug_file = fopen("_debug.txt", "w");
	if (debug_file == NULL) {
		perror("debug.txt");
		return _join_threads(tdata_vec);
	}

	/* print info to stderr */
	unsigned i = 0;
	struct thread_data* it = vec_begin(tdata_vec);
	for (; it != vec_end(tdata_vec); ++it) {
		dnode* proc_node = it->proc_node;
		process* proc = proc_node->data;
		fprintf(stderr, "%u: %s\n", i++, string_c_str(proc->plan_msg));
	}

	/* print header to debug_file */
	fputs("pollid\tidx\trunning\tfifo\trootidx\thead\ttail\tavail\treceiv\n",
	      debug_file);
	size_t poll_id = 0;

	/* begin monitoring */
	do {
		++poll_id;
		usleep(THREAD_MONITOR_INTERVAL_U);

		i = 0;
		threading_procs = 0;

		for (it = vec_begin(tdata_vec); it != vec_end(tdata_vec); ++it) {
			dnode* proc_node = it->proc_node;
			process* proc = proc_node->data;
			if (proc->is_threading
			    && pthread_tryjoin_np(proc->thread, &status) == 0) {
				proc->is_threading = false;
			}
			threading_procs += proc->is_threading;

			_print_proc_info(proc, debug_file, poll_id, i++);
		}
	} while (threading_procs > 0);

	fclose(debug_file);

	return FQL_GOOD;
}

int process_exec_plan_thread(plan* plan, fqlhandle* fql)
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
		tdata->plan = plan;
		process* proc = (*proc_node)->data;
		proc->is_threading = true;
		if (pthread_create(&proc->thread, &attr, _thread_exec, tdata)) {
			/* TODO */
			fputs("pthread_create fail\n", stderr);
			exit(EXIT_FAILURE);
		}
	}

	pthread_attr_destroy(&attr);


	int ret = 0;

	if (fql->props.verbosity == FQL_DEBUG) {
		ret = _monitor_threads(&tdata_vec);
	} else {
		ret = _join_threads(&tdata_vec);
	}

	vec_destroy(&tdata_vec);
	plan->is_complete = true;

	return ret;
}
