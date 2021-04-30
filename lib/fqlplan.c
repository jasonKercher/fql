#include "fqlplan.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "column.h"
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

void _print_plan(Plan* plan);

Plan* plan_new(Query* query)
{
	Plan* new_plan = NULL;
	malloc_(new_plan, sizeof(*new_plan));

	return plan_construct(new_plan, query);
}

Plan* plan_construct(Plan* plan, Query* query)
{
	*plan = (Plan) {
		 dgraph_new()      /* processes */
		,NULL              /* op_true */
		,NULL              /* op_false */
		,NULL              /* current */
		,NULL              /* recycle_groups */
		,0                 /* rows_affected */
		,0                 /* source_count */
		,false             /* has_stepped */
	};

	plan->plan_id = query->query_id;
	plan->source_count = query->sources->size;

	plan->op_true = dnode_new(process_new("OP_TRUE", plan));
	plan->op_false = dnode_new(process_new("OP_FALSE", plan));

	plan->source_count = 0;

	if (query->query_id == 0) {
		plan->recycle_groups = vec_new_(Vec);
		vec_resize(plan->recycle_groups, query->query_total);
		Vec* it = vec_begin(plan->recycle_groups);
		for (; it != vec_end(plan->recycle_groups); ++it) {
			vec_construct_(it, Dnode*);
		}
	}

	Process* start = process_new("start", plan);
	start->is_passive = true;
	plan->current = dgraph_add_data(plan->processes, start);
	dgraph_add_node(plan->processes, plan->op_true);
	dgraph_add_node(plan->processes, plan->op_false);

	return plan;
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
	Plan* plan = generic_plan;
	Dnode** it = vec_begin(plan->processes->nodes);
	for (; it != vec_end(plan->processes->nodes); ++it) {
		process_node_free(*it);
	}
	if (plan->recycle_groups != NULL) {
		Vec* it = vec_begin(plan->recycle_groups);
		for (; it != vec_end(plan->recycle_groups); ++it) {
			vec_destroy(it);
		}
		vec_free(plan->recycle_groups);
	}
	dgraph_free(plan->processes);
}

/* build process nodes from logic graph
 * assign processes for true and false
 */
void _logic_to_process(Process* logic_proc, LogicGroup* lg)
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
		fprintf(stderr, "Unexpected logic group %d\n", lg->type);
		return;
	}

	LogicGroup** it = vec_begin(&lg->items);
	for (; it != vec_end(&lg->items); ++it) {
		_logic_to_process(logic_proc, *it);
	}

	string_strcat(logic_proc->action_msg, ")");
}

void _logicgroup_process(Plan* plan, LogicGroup* lg, _Bool is_from_hash_join)
{
	Process* logic_proc = process_new("", plan);
	logic_proc->action__ = &fql_logic;
	logic_proc->proc_data = lg;
	_logic_to_process(logic_proc, lg);
	Dnode* logic_node = dgraph_add_data(plan->processes, logic_proc);

	if (is_from_hash_join) {
		plan->current->out[1] = logic_node;
		if (logicgroup_get_condition_count(lg) == 1) {
			logic_proc->is_passive = true;
		}
	} else {
		plan->current->out[0] = logic_node;
	}

	logic_node->out[0] = plan->op_false;

	Process* logic_true = process_new("Logic True", plan);
	logic_true->is_passive = true;
	Dnode* logic_true_node = dgraph_add_data(plan->processes, logic_true);

	logic_node->out[1] = logic_true_node;

	plan->current = logic_true_node;
}

Process* _new_join_proc(enum join_type type, const char* algorithm, Plan* plan)
{
	++plan->source_count;
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
	return process_new(buffer, plan);
}

/* Lol */
int _from(Plan* plan, Query* query)
{
	if (query->sources->size == 0) {
		return FQL_GOOD;
	}

	String action_msg;
	string_construct(&action_msg);
	Table* table_iter = vec_begin(query->sources);

	Process* from_proc = NULL;
	Dnode* from_node = NULL;
	++plan->source_count;
	if (table_iter->source_type == SOURCE_TABLE) {
		string_sprintf(&action_msg,
			       "%s: %s",
			       table_iter->reader->file_name.data,
			       "stream read");

		from_proc = process_new(action_msg.data, plan);
		from_proc->action__ = &fql_read;
		from_node = dgraph_add_data(plan->processes, from_proc);
	} else {
		from_proc = process_new("subquery select", plan);
		from_proc->action__ = &fql_read_subquery;
		from_proc->root_fifo = 1;
		from_node = dgraph_add_data(plan->processes, from_proc);
		from_node->is_root = true;
		Plan* subquery_plan = plan_build(table_iter->subquery, from_node);
		if (subquery_plan == NULL) {
			return FQL_FAIL;
		}
		from_proc->subquery_plan_id = subquery_plan->plan_id;
		dgraph_consume(plan->processes, subquery_plan->processes);
	}
	table_iter->read_proc = from_proc;
	from_proc->proc_data = table_iter;

	from_node->is_root = true;

	plan->current->out[0] = from_node;
	plan->current = from_node;
	Dnode* join_node = NULL;

	for (++table_iter; table_iter != vec_end(query->sources); ++table_iter) {

		/* TODO */
		if (table_iter->source_type == SOURCE_SUBQUERY) {
			fputs("currently cannot join with a"
			      "sub-query on the right side\n", stderr);
			return FQL_FAIL;
		}

		Process* join_proc = NULL;
		_Bool is_hash_join = (table_iter->condition->join_logic != NULL);
		if (is_hash_join) {
			join_proc = _new_join_proc(table_iter->join_type, "hash", plan);
			join_proc->action__ = &fql_hash_join;
			table_hash_join_init(table_iter);
			process_add_second_input(join_proc);

			Process* read_proc = NULL;
			Dnode* read_node = NULL;

			/* TODO: subquery on right side of join */
			//if (table_iter->source_type == SOURCE_TABLE) {
				string_sprintf(&action_msg,
					       "%s: %s",
					       table_iter->reader->file_name.data,
					       "mmap read");

				read_proc = process_new(action_msg.data, plan);
				read_proc->action__ = &fql_read;
				read_node = dgraph_add_data(plan->processes, read_proc);
			//} else {
			//	read_proc = process_new("subquery select", plan);
			//	read_proc->action__ = &fql_read_subquery;
			//	read_proc->root_fifo = 1;
			//	read_node = dgraph_add_data(plan->processes, read_proc);
			//	read_node->is_root = true;
			//	Plan* subquery_plan = plan_build(table_iter->subquery, read_node);
			//	if (subquery_plan == NULL) {
			//		return FQL_FAIL;
			//	}
			//	read_proc->subquery_plan_id = subquery_plan->plan_id;
			//	dgraph_consume(plan->processes, subquery_plan->processes);
			//}
			table_iter->read_proc = read_proc;
			read_proc->proc_data = table_iter;
			read_proc->is_secondary = true;
			read_node->is_root = true;

			join_node = dgraph_add_data(plan->processes, join_proc);
			read_node->out[0] = join_node;
		} else {
			join_proc = _new_join_proc(table_iter->join_type, "cartesian", plan);
			join_proc->action__ = &fql_cartesian_join;
			join_proc->root_fifo = 1;
			join_node = dgraph_add_data(plan->processes, join_proc);
			join_node->is_root = true;
		}
		join_proc->proc_data = table_iter;
		plan->current->out[0] = join_node;
		plan->current = join_node;

		if (table_iter->condition != NULL) {
			_logicgroup_process(plan, table_iter->condition, is_hash_join);
		}
	}

	string_destroy(&action_msg);

	return FQL_GOOD;
}

void _where(Plan* plan, Query* query)
{
	if (query->where == NULL) {
		return;
	}

	_logicgroup_process(plan, query->where, false);
}

void _group(Plan* plan, Query* query)
{
	if (!vec_empty(&query->groupby->columns)) {
		Process* group_proc = process_new("GROUP BY ", plan);
		group_proc->action__ = &fql_groupby;
		group_proc->proc_data = query->groupby;
		group_cat_description(query->groupby, group_proc);
		Dnode* group_node = dgraph_add_data(plan->processes, group_proc);
		plan->current->out[0] = group_node;
		plan->current = group_node;
	}

	if (query->distinct) {
		Process* group_proc = process_new("DISTINCT ", plan);
		group_proc->action__ = &fql_groupby;
		group_proc->proc_data = query->distinct;
		group_cat_description(query->distinct, group_proc);
		Dnode* group_node = dgraph_add_data(plan->processes, group_proc);
		plan->current->out[0] = group_node;
		plan->current = group_node;
	}
}

void _having(Plan* plan, Query* query) { }

void _operation(Plan* plan, Query* query, Dnode* entry)
{
	plan->current->out[0] = plan->op_true;
	if (entry == NULL) {
		op_apply_process(query, plan);
		return;
	}
	plan->op_true->out[0] = entry;

	/* We are selecting in a subquery. The process
	 * that reads from the subquery will read what
	 * is needed and recycle the record back into
	 * the subquery.
	 *
	 * This reading process will take the place of
	 * op_true so both op_true and op_false can be
	 * marked as passive.
	 */
	Process* true_proc = plan->op_true->data;
	true_proc->is_passive = true;
	Process* false_proc = plan->op_false->data;
	false_proc->is_passive = true;
}

void _limit(Plan* plan, Query* query) { }

/* In an effort to make building of the process graph easier
 * passive nodes are used as a sort of link between the steps.
 * TODO: Fix my shit design
 */
void _clear_passive(Plan* plan)
{
	Vec* node_vec = plan->processes->nodes;
	Dnode** nodes = vec_begin(node_vec);
	int i = 0;

	/* Re-link nodes so passive ones get skipped during traversal */
	for (; i < node_vec->size; ++i) {
		/* Check branch 0 */
		while (nodes[i]->out[0] != NULL) {
			Process* proc = nodes[i]->out[0]->data;
			if (proc->is_passive) {
			        nodes[i]->out[0] = nodes[i]->out[0]->out[0];
			} else {
			        break;
			}
		}

		/* Check branch 1 */
		_Bool first_pass = true;
		while (nodes[i]->out[1] != NULL) {
			Process* proc = nodes[i]->out[1]->data;
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
		Process* proc = nodes[i]->data;
		if (proc->is_passive) {
			process_free(proc, false);
			dgraph_remove(plan->processes, &nodes[i]);
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
void _update_pipes(Dgraph* proc_graph)
{
	while (dgraph_traverse(proc_graph));

	Dnode** it = vec_begin(proc_graph->nodes);
	for (; it != vec_end(proc_graph->nodes); ++it) {
		/* This should be impossible */
		if ((*it)->visit_count == 0) {
			return;
		}
		Process* proc = (*it)->data;
		proc->fifo_in[0]->input_count = (*it)->visit_count;

		if (proc->fifo_in[1]) {
			proc->fifo_in[1]->input_count = (*it)->visit_count;
		}
	}

	dgraph_traverse_reset(proc_graph);
}

void _activate_procs(Plan* plan)
{
	Vec* node_vec = plan->processes->nodes;
	Dnode** nodes = vec_begin(node_vec);

	for (; nodes != vec_end(node_vec); ++nodes) {
	        process_activate(*nodes, plan);
	}
}

/* Run through processes and link up fifos. Input fifos are owned
 * by the process. Output fifos are just links to other processes'
 * fifos. This would read better if I called them pipes...
 */
void _make_pipes(Plan* plan)
{
	Vec* node_vec = plan->processes->nodes;
	Dnode** nodes = vec_begin(node_vec);

	for (; nodes != vec_end(node_vec); ++nodes) {
		Process* proc = (*nodes)->data;
		if ((*nodes)->out[0] != NULL) {
			Process* proc0 = (*nodes)->out[0]->data;
			proc->fifo_out[0] = (proc->is_secondary) ?
					    proc0->fifo_in[1] :
					    proc0->fifo_in[0];
		}

		if ((*nodes)->out[1] != NULL) {
			Process* proc1 = (*nodes)->out[1]->data;
			proc->fifo_out[1] = (proc->is_secondary) ?
					    proc1->fifo_in[1] :
					    proc1->fifo_in[0];
		}
	}

}

void _mark_roots_const(Vec* roots)
{
	Dnode** it = vec_begin(roots);
	for (; it != vec_end(roots); ++it) {
		Process* proc = (*it)->data;
		proc->is_const = true;
	}
}

Plan* plan_build(Query* query, Dnode* entry)
{
	query->plan = plan_new(query);
	Plan* plan = query->plan;

	/* Query */
	if (_from(plan, query)) {
		plan_free(plan);
		return NULL;
	}
	_where(plan, query);
	_group(plan, query);
	_having(plan, query);
	_operation(plan, query, entry);
	_limit(plan, query);

	/* Uncomment this to view the plan *with* passive nodes */
	//_print_plan(plan);

	_clear_passive(plan);

	if (vec_empty(plan->processes->nodes)) {
		Process* entry_proc = entry->data;
		entry_proc->is_const = true;
		return plan;
	}

	dgraph_get_roots(plan->processes);

	if (vec_empty(query->sources)) {
		_mark_roots_const(plan->processes->_roots);
	}

	if (query->query_id != 0) { /* is subquery */
		return plan;
	}
	_activate_procs(plan);
	_make_pipes(plan);
	_update_pipes(plan->processes);

	return plan;
}

int build_plans(Queue* query_list)
{
	Queue* node = query_list;

	for (; node; node = node->next) {
		Query* query = node->data;
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

void _print_plan(Plan* plan)
{
	Dgraph* proc_graph = plan->processes;
	Vec* nodes = proc_graph->nodes;

	/* retrieve longest message */
	int max_len = strlen("BRANCH 0");

	Dnode** it = vec_begin(nodes);
	for (; it != vec_end(nodes); ++it) {
		Process* proc = (*it)->data;
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


	/* Print adjacency list */
	for (it = vec_begin(nodes); it != vec_end(nodes); ++it) {
		fputc('\n', stderr);

		Process* proc = (*it)->data;
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

void print_plans(Queue* query_list)
{
	int i = 0;
	Queue* node = query_list;
	for (; node; node = node->next, ++i) {
		Query* query = query_list->data;
		fprintf(stderr, "\nQUERY %d\n", ++i);
		_print_plan(query->plan);
	}
}
