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

process* process_construct(process* self, const char* action, plan* plan)
{
	*self = (process) {
	        0,                            /* thread */
	        NULL,                         /* records */
	        &fql_no_op,                   /* action__ */
	        {NULL, NULL},                 /* fifo_in */
	        {NULL, NULL},                 /* fifo_out */
	        NULL,                         /* proc_data */
	        string_from_char_ptr(action), /* action_msg */
	        NULL,                         /* root_group */
	        NULL,                         /* wait_list */
	        new_t_(vec, dnode*),          /* union_end_nodes */
	        NULL,                         /* queued_results */
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
	if (is_root && self->records != NULL) {
		vec* it = vec_begin(self->records);
		for (; it != vec_end(self->records); ++it) {
			record** rec = vec_back(it);
			delete_(record, *rec);
			vec_destroy(it);
		}
		delete_(vec, self->records);
	}
	delete_if_exists_(fifo, self->fifo_in[0]);
	delete_if_exists_(fifo, self->fifo_in[1]);
	delete_if_exists_(vec, self->wait_list);
	delete_(vec, self->union_end_nodes);
	delete_(string, self->action_msg);
}

void process_activate(dnode* proc_node, plan* plan)
{
	process* self = proc_node->data;
	unsigned graph_size = plan->processes->nodes->size;
	unsigned fifo_size = graph_size * 4;
	bool is_subquery = self->subquery_plan_id > 0;
	if (self->root_group == NULL && is_subquery) {
		self->root_group = vec_at(plan->recycle_groups, self->subquery_plan_id);
	} else if (self->root_group == NULL) {
		self->root_group = vec_at(plan->recycle_groups, self->plan_id);
	}

	dnode** it = vec_begin(self->union_end_nodes);
	for (; it != vec_end(self->union_end_nodes); ++it) {
		process* select_proc = (*it)->data;
		select_proc->fifo_out[0] = new_t_(fifo, vec*, fifo_size);
		queue_enqueue(&self->queued_results, select_proc->fifo_out[0]);
	}

	if (!proc_node->is_root) {
		self->fifo_in[0] = new_t_(fifo, vec*, fifo_size);
		if (self->has_second_input) {
			self->fifo_in[1] = new_t_(fifo, vec*, fifo_size);
		}
		return;
	}

	/* lol subquery hack
	 * self->root_group is subquery root
	 * however, self is a root of the upper
	 * query. Who wrote this garbage??
	 */
	vec* true_root_group = self->root_group;
	if (is_subquery) {
		true_root_group = vec_at(plan->recycle_groups, self->plan_id);
	}

	vec_push_back(true_root_group, &proc_node);
	if (self->root_fifo == 1) {
		self->fifo_in[0] = new_t_(fifo, vec*, fifo_size);
		if (self->is_const) {
			fifo_advance(self->fifo_in[0]);
		}
	}

	self->fifo_in[self->root_fifo] = new_t_(fifo, vec*, fifo_size * graph_size);

	int field_count = 1;

	bool owns_data = true;
	if (self->action__ == &fql_read) {
		table* table = self->proc_data;
		reader* reader = table->reader;
		field_count = reader->max_col_idx + 1;
		owns_data = false;
	} else if (self->action__ == &fql_read_subquery) {
		fqlselect* select = self->proc_data;
		field_count = fqlselect_get_field_count(select);
	} else if (self->action__ == &fql_groupby) {
		group* group = self->proc_data;
		field_count = group->columns.size;
	}

	self->records = new_t_(vec, vec);
	vec_resize(self->records, fifo_size * graph_size);

	unsigned i = 0;

	/* if root, it will own the vector of records */
	vec* buf = self->fifo_in[self->root_fifo]->buf;
	for (; i < self->records->size; ++i) {
		vec* new_recs = vec_at(self->records, i);
		vec_construct_(new_recs, record*);
		vec_resize(new_recs, self->out_src_count);

		record** new_rec = vec_back(new_recs);
		*new_rec = new_(record, i, field_count, owns_data);

		vec** recs = vec_at(buf, i);
		*recs = new_recs;
	}

	/* TODO: once we stop hard coding fifo size,
	 *       this if block can go.
	 */
	if (self->is_const) {
		fifo_advance(self->fifo_in[self->root_fifo]);
		return;
	}
	fifo_set_full(self->fifo_in[self->root_fifo]);
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
		    && fifo_is_empty(self->fifo_in[0])) {
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
		if ((self->wait_for_in0 && fifo_is_empty(self->fifo_in[0]))
		    || (self->fifo_out[0] && !fifo_receivable(self->fifo_out[0]))
		    || (self->fifo_out[1] && !fifo_receivable(self->fifo_out[1]))) {
			++run_count;
			continue;
		}

		int ret = try_(self->action__(proc_graph, self));

		if (self->wait_for_in0) {
			++run_count;
		}
		if (self->rows_affected >= self->top_count) {
			process_disable(self);
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
			if (node->is_root) {
				fifo_wait_for_add(in0);
			} else {
				fifo_wait_for_work(in0);
			}
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
		int ret = self->action__(tdata->proc_graph, self);

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
