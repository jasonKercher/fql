#include "fqlplan.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "column.h"
#include "group.h"
#include "logic.h"
#include "process.h"
#include "query.h"
#include "reader.h"
#include "util/util.h"

#define PLAN_COLUMN_SEP " | "

/**
 * Implementation for Plan
 *
 * Plan is basically a decision graph
 * where each node represents a process
 */

void _print_plan(plan*);

plan* plan_construct(plan* self, query* query)
{
	*self = (plan) {
		 new_(dgraph)      /* processes */
		,NULL              /* op_true */
		,NULL              /* op_false */
		,NULL              /* current */
		,NULL              /* recycle_groups */
		,0                 /* rows_affected */
		,0                 /* source_count */
		,false             /* has_stepped */
	};

	self->plan_id = query->query_id;
	self->source_count = query->sources->size;

	self->op_true = new_(dnode, new_(process, "OP_TRUE", self));
	self->op_false = new_(dnode, new_(process, "OP_FALSE", self));

	self->source_count = 0;

	if (query->query_id == 0) {
		self->recycle_groups = new_t_(vec, vec);
		vec_resize(self->recycle_groups, query->query_total);
		vec* it = vec_begin(self->recycle_groups);
		for (; it != vec_end(self->recycle_groups); ++it) {
			vec_construct(it, sizeof(dnode*));
		}
	}

	process* start = new_(process, "start", self);
	start->is_passive = true;
	self->current = dgraph_add_data(self->processes, start);
	dgraph_add_node(self->processes, self->op_true);
	dgraph_add_node(self->processes, self->op_false);

	return self;
}

void plan_free(void* generic_plan)
{
        if (generic_plan == NULL) {
                return;
        }
	plan_destroy(generic_plan);
	free_(generic_plan);
}

void plan_destroy(void* generic_plan)
{
	plan* self = generic_plan;
	dnode** it = vec_begin(self->processes->nodes);
	for (; it != vec_end(self->processes->nodes); ++it) {
		process_node_free(*it);
	}
	if (self->recycle_groups != NULL) {
		vec* it = vec_begin(self->recycle_groups);
		for (; it != vec_end(self->recycle_groups); ++it) {
			vec_destroy(it);
		}
		delete_(vec, self->recycle_groups);
	}
	delete_(dgraph, self->processes);
}

/* build process nodes from logic graph
 * assign processes for true and false
 */
void _logic_to_process(process* logic_proc, logicgroup* lg)
{
	switch (lg->type) {
	case LG_ROOT:
		string_strcat(logic_proc->action_msg, "(");
		break;
	case LG_AND:
		string_strcat(logic_proc->action_msg, "AND(");
		break;
	case LG_NOT:
		string_strcat(logic_proc->action_msg, "NOT(");
		if (lg->condition != NULL) {
			logic_assign_process(lg->condition, logic_proc);
		}
		break;
	default:
		fprintf(stderr, "unexpected logic group %d\n", lg->type);
		return;
	}

	logicgroup** it = vec_begin(&lg->items);
	for (; it != vec_end(&lg->items); ++it) {
		_logic_to_process(logic_proc, *it);
	}

	string_strcat(logic_proc->action_msg, ")");
}

void _logicgroup_process(plan* self, logicgroup* lg, _Bool is_from_hash_join)
{
	process* logic_proc = new_(process, "", self);
	logic_proc->action__ = &fql_logic;
	logic_proc->proc_data = lg;
	_logic_to_process(logic_proc, lg);
	dnode* logic_node = dgraph_add_data(self->processes, logic_proc);

	if (is_from_hash_join) {
		self->current->out[1] = logic_node;
		if (logicgroup_get_condition_count(lg) == 1) {
			logic_proc->is_passive = true;
		}
	} else {
		self->current->out[0] = logic_node;
	}

	logic_node->out[0] = self->op_false;

	process* logic_true = new_(process, "logic true", self);
	logic_true->is_passive = true;
	dnode* logic_true_node = dgraph_add_data(self->processes, logic_true);

	logic_node->out[1] = logic_true_node;

	self->current = logic_true_node;
}

process* _new_join_proc(enum join_type type, const char* algorithm, plan* self)
{
	++self->source_count;
	char buffer[100];
	switch (type) {
	case JOIN_INNER:
		sprintf(buffer, "INNER JOIN (%s)", algorithm);
		break;
	case JOIN_LEFT:
		sprintf(buffer, "LEFT JOIN (%s)", algorithm);
		break;
	case JOIN_RIGHT:
		sprintf(buffer, "RIGHT JOIN (%s)", algorithm);
		break;
	case JOIN_FULL:
		sprintf(buffer, "FULL JOIN (%s)", algorithm);
		break;
	case JOIN_CROSS:
		sprintf(buffer, "CROSS JOIN (%s)", algorithm);
		break;
	case JOIN_FROM:
		sprintf(buffer, "unexpected: JOIN_FROM (%s)", algorithm);
		break;
	}
	return new_(process, buffer, self);
}

/* lol */
int _from(plan* self, query* query)
{
	if (query->sources->size == 0) {
		return FQL_GOOD;
	}

	string action_msg;
	string_construct(&action_msg);
	table* table_iter = vec_begin(query->sources);

	process* from_proc = NULL;
	dnode* from_node = NULL;
	++self->source_count;
	if (table_iter->source_type == SOURCE_TABLE) {
		string_sprintf(&action_msg,
			       "%s: %s",
			       table_iter->reader->file_name.data,
			       "stream read");

		from_proc = new_(process, action_msg.data, self);
		from_proc->action__ = &fql_read;
		from_node = dgraph_add_data(self->processes, from_proc);
	} else {
		from_proc = new_(process, "subquery select", self);
		from_proc->action__ = &fql_read_subquery;
		from_proc->root_fifo = 1;
		from_node = dgraph_add_data(self->processes, from_proc);
		from_node->is_root = true;
		plan* subquery_plan = plan_build(table_iter->subquery, from_node);
		if (subquery_plan == NULL) {
			return FQL_FAIL;
		}
		from_proc->subquery_plan_id = subquery_plan->plan_id;
		dgraph_consume(self->processes, subquery_plan->processes);
	}
	table_iter->read_proc = from_proc;
	from_proc->proc_data = table_iter;

	from_node->is_root = true;

	self->current->out[0] = from_node;
	self->current = from_node;
	dnode* join_node = NULL;

	for (++table_iter; table_iter != vec_end(query->sources); ++table_iter) {

		/* TODO */
		if (table_iter->source_type == SOURCE_SUBQUERY) {
			fputs("currently cannot join with a"
			      "sub-query on the right side\n", stderr);
			return FQL_FAIL;
		}

		process* join_proc = NULL;
		_Bool is_hash_join = (table_iter->condition->join_logic != NULL);
		if (is_hash_join) {
			join_proc = _new_join_proc(table_iter->join_type, "hash", self);
			join_proc->action__ = &fql_hash_join;
			table_hash_join_init(table_iter);
			process_add_second_input(join_proc);

			process* read_proc = NULL;
			dnode* read_node = NULL;

			/* TODO: subquery on right side of join */
			//if (table_iter->source_type == SOURCE_TABLE) {
				string_sprintf(&action_msg,
					       "%s: %s",
					       table_iter->reader->file_name.data,
					       "mmap read");

				read_proc = new_(process, action_msg.data, self);
				read_proc->action__ = &fql_read;
				read_node = dgraph_add_data(self->processes, read_proc);
			//} else {
			//	read_proc = new_(process, "subquery select", self);
			//	read_proc->action__ = &fql_read_subquery;
			//	read_proc->root_fifo = 1;
			//	read_node = dgraph_add_data(self->processes, read_proc);
			//	read_node->is_root = true;
			//	Plan* subquery_plan = plan_build(table_iter->subquery, read_node);
			//	if (subquery_plan == NULL) {
			//		return FQL_FAIL;
			//	}
			//	read_proc->subquery_plan_id = subquery_plan->plan_id;
			//	dgraph_consume(self->processes, subquery_plan->processes);
			//}
			table_iter->read_proc = read_proc;
			read_proc->proc_data = table_iter;
			read_proc->is_secondary = true;
			read_node->is_root = true;

			join_node = dgraph_add_data(self->processes, join_proc);
			read_node->out[0] = join_node;
		} else {
			join_proc = _new_join_proc(table_iter->join_type, "cartesian", self);
			join_proc->action__ = &fql_cartesian_join;
			join_proc->root_fifo = 1;
			join_node = dgraph_add_data(self->processes, join_proc);
			join_node->is_root = true;
		}
		join_proc->proc_data = table_iter;
		self->current->out[0] = join_node;
		self->current = join_node;

		if (table_iter->condition != NULL) {
			_logicgroup_process(self, table_iter->condition, is_hash_join);
		}
	}

	string_destroy(&action_msg);

	return FQL_GOOD;
}

void _where(plan* self, query* query)
{
	if (query->where == NULL) {
		return;
	}

	_logicgroup_process(self, query->where, false);
}

void _group(plan* self, query* query)
{
	if (!vec_empty(&query->groupby->columns)
	 || !vec_empty(&query->groupby->aggregates)) {
		process* group_proc = new_(process, "GROUP BY ", self);
		group_proc->action__ = &fql_groupby;
		group_proc->proc_data = query->groupby;
		group_proc->wait_for_in0_end = true;
		group_proc->root_fifo = 1;
		group_cat_description(query->groupby, group_proc);
		dnode* group_node = dgraph_add_data(self->processes, group_proc);
		group_node->is_root = true;
		self->current->out[0] = group_node;
		self->current = group_node;
	}

	if (query->distinct) {
		process* group_proc = new_(process, "DISTINCT ", self);
		group_proc->action__ = &fql_distinct;
		group_proc->proc_data = query->distinct;
		group_cat_description(query->distinct, group_proc);
		dnode* group_node = dgraph_add_data(self->processes, group_proc);
		self->current->out[0] = group_node;
		self->current = group_node;
	}
}

void _having(plan* self, query* query) { }

void _operation(plan* self, query* query, dnode* entry)
{
	self->current->out[0] = self->op_true;
	if (entry == NULL) {
		op_apply_process(query, self);
		return;
	}
	self->op_true->out[0] = entry;

	/* We are selecting in a subquery. The process
	 * that reads from the subquery will read what
	 * is needed and recycle the record back into
	 * the subquery.
	 *
	 * This reading process will take the place of
	 * op_true so both op_true and op_false can be
	 * marked as passive.
	 */
	process* true_proc = self->op_true->data;
	true_proc->is_passive = true;
	process* false_proc = self->op_false->data;
	false_proc->is_passive = true;
}

void _limit(plan* self, query* query) { }

/* In an effort to make building of the process graph easier
 * passive nodes are used as a sort of link between the steps.
 * TODO: Fix my shit design
 */
void _clear_passive(plan* self)
{
	vec* node_vec = self->processes->nodes;
	dnode** nodes = vec_begin(node_vec);
	int i = 0;

	/* re-link nodes so passive ones get skipped during traversal */
	for (; i < node_vec->size; ++i) {
		/* Check branch 0 */
		while (nodes[i]->out[0] != NULL) {
			process* proc = nodes[i]->out[0]->data;
			if (proc->is_passive) {
			        nodes[i]->out[0] = nodes[i]->out[0]->out[0];
			} else {
			        break;
			}
		}

		/* Check branch 1 */
		_Bool first_pass = true;
		while (nodes[i]->out[1] != NULL) {
			process* proc = nodes[i]->out[1]->data;
			if (proc->is_passive) {
			        if (nodes[i]->out[1]->out[1] != NULL) {
			                nodes[i]->out[1] = nodes[i]->out[1]->out[1];
			        } else {
			                nodes[i]->out[1] = nodes[i]->out[1]->out[0];
			        }
			} else {
			        break;
			}
		}
	}

	/* At this point, all passive nodes should be unreachable */
	for (i = 0; i < node_vec->size;) {
		process* proc = nodes[i]->data;
		if (proc->is_passive) {
			delete_(process, proc, false);
			dgraph_remove(self->processes, &nodes[i]);
		} else {
			++i;
		}
	}
}

/* Assign input counts to each process by traversing
 * and copying the visit_count. This important when
 * threading because "is not full" is not a valid
 * test for whether we are allowed to send data
 * into the pipe IF there is more than one input.
 *
 * Also, update the pipeline size. If we just waited
 * for each pipe to be half full, we could dead lock.
 */
void _update_pipes(dgraph* proc_graph)
{
	while (dgraph_traverse(proc_graph));

	dnode** it = vec_begin(proc_graph->nodes);
	for (; it != vec_end(proc_graph->nodes); ++it) {
		/* This should be impossible */
		if ((*it)->visit_count == 0) {
			return;
		}
		process* proc = (*it)->data;
		proc->fifo_in[0]->input_count = (*it)->visit_count;

		if (proc->fifo_in[1]) {
			proc->fifo_in[1]->input_count = (*it)->visit_count;
		}
	}

	dgraph_traverse_reset(proc_graph);
}

void _activate_procs(plan* self)
{
	vec* node_vec = self->processes->nodes;
	dnode** nodes = vec_begin(node_vec);

	for (; nodes != vec_end(node_vec); ++nodes) {
	        process_activate(*nodes, self);
	}
}

/* Run through processes and link up fifos. Input fifos are owned
 * by the process. Output fifos are just links to other processes'
 * fifos. This would read better if I called them pipes...
 */
void _make_pipes(plan* self)
{
	vec* node_vec = self->processes->nodes;
	dnode** nodes = vec_begin(node_vec);

	for (; nodes != vec_end(node_vec); ++nodes) {
		process* proc = (*nodes)->data;
		if ((*nodes)->out[0] != NULL) {
			process* proc0 = (*nodes)->out[0]->data;
			proc->fifo_out[0] = (proc->is_secondary) ?
					    proc0->fifo_in[1] :
					    proc0->fifo_in[0];
		}

		if ((*nodes)->out[1] != NULL) {
			process* proc1 = (*nodes)->out[1]->data;
			proc->fifo_out[1] = (proc->is_secondary) ?
					    proc1->fifo_in[1] :
					    proc1->fifo_in[0];
		}
	}

}

void _mark_roots_const(vec* roots)
{
	dnode** it = vec_begin(roots);
	for (; it != vec_end(roots); ++it) {
		process* proc = (*it)->data;
		proc->is_const = true;
	}
}

plan* plan_build(query* query, dnode* entry)
{
	query->plan = new_(plan, query);
	plan* self = query->plan;

	/* query */
	if (_from(self, query)) {
		plan_free(self);
		return NULL;
	}
	_where(self, query);
	_group(self, query);
	_having(self, query);
	_operation(self, query, entry);
	_limit(self, query);

	/* Uncomment this to view the plan *with* passive nodes */
	//_print_plan(self);

	_clear_passive(self);

	if (vec_empty(self->processes->nodes)) {
		process* entry_proc = entry->data;
		entry_proc->is_const = true;
		return self;
	}

	dgraph_get_roots(self->processes);

	if (vec_empty(query->sources)) {
		_mark_roots_const(self->processes->_roots);
	}

	if (query->query_id != 0) { /* is subquery */
		return self;
	}
	_activate_procs(self);
	_make_pipes(self);
	_update_pipes(self->processes);

	return self;
}

int build_plans(queue* query_list)
{
	queue* node = query_list;

	for (; node; node = node->next) {
		query* query = node->data;
		if (plan_build(query, NULL) == NULL) {
			return FQL_FAIL;
		}
	}

	return FQL_GOOD;
}

void _col_sep(int n)
{
	for (; n > 0; --n) {
		fputc(' ', stderr);
	}
	fputs(PLAN_COLUMN_SEP, stderr);
}

void _print_plan(plan* self)
{
	dgraph* proc_graph = self->processes;
	vec* nodes = proc_graph->nodes;

	/* retrieve longest message */
	int max_len = strlen("BRANCH 0");

	dnode** it = vec_begin(nodes);
	for (; it != vec_end(nodes); ++it) {
		process* proc = (*it)->data;
		int len = proc->action_msg->size;
		if (len > max_len) {
			max_len = len;
		}
	}


	/* Print Header */
	fputs("NODE", stderr);
	_col_sep(max_len - strlen("NODE"));
	fputs("BRANCH 0", stderr);
	_col_sep(max_len - strlen("BRANCH 0"));
	fputs("BRANCH 1\n", stderr);

	int i = 0;
	for (i = 0; i < max_len; ++i) {
		fputc('=', stderr);
	}
	_col_sep(0);
	for (i = 0; i < max_len; ++i) {
		fputc('=', stderr);
	}
	_col_sep(0);
	for (i = 0; i < max_len; ++i) {
		fputc('=', stderr);
	}


	/* print adjacency list */
	for (it = vec_begin(nodes); it != vec_end(nodes); ++it) {
		fputc('\n', stderr);

		process* proc = (*it)->data;
		int len = proc->action_msg->size;
		fputs(proc->action_msg->data, stderr);
		_col_sep(max_len - len);
		len = 0;

		if ((*it)->out[0] != NULL) {
			proc = (*it)->out[0]->data;
			len = proc->action_msg->size;
			fputs(proc->action_msg->data, stderr);
		}
		_col_sep(max_len - len);

		if ((*it)->out[1] != NULL) {
			proc = (*it)->out[1]->data;
			fputs(proc->action_msg->data, stderr);
		}
	}
	fputs("\n", stderr);
}

void print_plans(queue* query_list)
{
	int i = 0;
	queue* node = query_list;
	for (; node; node = node->next, ++i) {
		query* query = query_list->data;
		fprintf(stderr, "\nQUERY %d\n", ++i);
		_print_plan(query->plan);
	}
}
