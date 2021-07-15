#include "fqlplan.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "misc.h"
#include "query.h"
#include "group.h"
#include "order.h"
#include "logic.h"
#include "reader.h"
#include "process.h"
#include "function.h"
#include "fqlselect.h"
#include "operation.h"
#include "aggregate.h"
#include "switchcase.h"
#include "expression.h"
#include "util/util.h"

#define PLAN_COLUMN_SEP   " | "

/**
 * Plan is basically a decision graph
 * where each node represents a process
 */

int _build(query*, struct fql_handle*, dnode* entry, bool is_union);
void _print_plan(plan*);
void _activate_procs(plan* self);

plan* plan_construct(plan* self, query* query, struct fql_handle* fql)
{
	*self = (plan) {
	        new_(dgraph),            /* processes */
	        NULL,                    /* op_true */
	        NULL,                    /* op_false */
	        NULL,                    /* current */
	        NULL,                    /* _recgroups */
	        NULL,                    /* root */
	        query,                   /* query */
	        0,                       /* rows_affected */
	        0,                       /* iterations */
	        0,                       /* source_count */
	        0,                       /* plan_id */
	        fql->props.pipe_factor,  /* pipe_factor */
	        false,                   /* is_const */
	        false,                   /* has_stepped */
	        fql->props.loose_groups, /* loose_groups */
	};

	self->plan_id = query->query_id;
	self->source_count = query->sources->size;

	self->op_true = new_(dnode, new_(process, "OP_TRUE", self));
	self->op_false = new_(dnode, new_(process, "OP_FALSE", self));

	self->source_count = 0;

	process* start = new_(process, "start", self);
	start->is_passive = true;
	self->current = dgraph_add_data(self->processes, start);
	dgraph_add_node(self->processes, self->op_true);
	dgraph_add_node(self->processes, self->op_false);

	return self;
}

void plan_destroy(void* generic_plan)
{
	plan* self = generic_plan;
	if (self->processes != NULL) {
		dnode** it = vec_begin(self->processes->nodes);
		for (; it != vec_end(self->processes->nodes); ++it) {
			process_node_free(*it);
		}
		delete_(dgraph, self->processes);
	}

	if (self->root == NULL) {
		return;
	}

	recgroup* it = vec_begin(self->_recgroups);
	for (; it != vec_end(self->_recgroups); ++it) {
		recgroup_destroy(it);
	}
	delete_(vec, self->_recgroups);
	delete_(fifo, self->root);
}

int _logic_to_process(plan* self, process* logic_proc, logicgroup* lg);
void _check_all_for_special_expression(plan* self, process* proc, vec* expressions);
void _check_for_special_expression(plan* self, process* proc, expression* expr)
{
	if (expr == NULL) {
		return;
	}

	switch (expr->expr) {
	case EXPR_SUBQUERY:
		process_add_to_wait_list(proc, expr->subquery->plan->op_true->data);
		break;
	case EXPR_SWITCH_CASE: {
		logicgroup** it = vec_begin(&expr->field.sc->tests);
		for (; it != vec_end(&expr->field.sc->tests); ++it) {
			_logic_to_process(self, proc, *it);
		}
		_check_all_for_special_expression(self, proc, &expr->field.sc->values);

		break;
	}
	case EXPR_AGGREGATE:
		_check_all_for_special_expression(self, proc, expr->field.agg->args);
		break;
	case EXPR_FUNCTION:
		_check_all_for_special_expression(self, proc, expr->field.fn->args);
		break;
	default:;
	}
}
void _check_all_for_special_expression(plan* self, process* proc, vec* expressions)
{
	if (expressions == NULL) {
		return;
	}
	expression** it = vec_begin(expressions);
	for (; it != vec_end(expressions); ++it) {
		_check_for_special_expression(self, proc, *it);
	}
}

int _subquery_inlist(plan* self, process* logic_proc, logicgroup* lg)
{
	query* subquery = lg->condition->in_data->subquery;
	/* We can assume select because subquery */
	try_(fqlselect_set_as_inlist(subquery->op, lg->condition->in_data));
	lg->condition->comp_type = COMP_SUBIN;
	process_add_to_wait_list(logic_proc, subquery->plan->op_true->data);
	return FQL_GOOD;
}

/* build process nodes from logic graph
 * assign processes for true and false
 */
int _logic_to_process(plan* self, process* logic_proc, logicgroup* lg)
{
	switch (lg->type) {
	case LG_ROOT:
		string_strcat(logic_proc->plan_msg, "(");
		break;
	case LG_AND:
		string_strcat(logic_proc->plan_msg, "AND(");
		break;
	case LG_NOT:
		if (lg->negation) {
			string_strcat(logic_proc->plan_msg, "NOT(");
		} else {
			string_strcat(logic_proc->plan_msg, "(");
		}
		if (lg->condition != NULL) {
			if (lg->condition->in_data && lg->condition->in_data->subquery) {
				try_(_subquery_inlist(self, logic_proc, lg));
			}
			_check_for_special_expression(self,
			                              logic_proc,
			                              lg->condition->expr[0]);
			_check_for_special_expression(self,
			                              logic_proc,
			                              lg->condition->expr[1]);
			try_(logic_assign_process(lg->condition, logic_proc));
		}
		break;
	default:
		fprintf(stderr, "unexpected logic group %d\n", lg->type);
		return FQL_FAIL;
	}

	logicgroup** it = vec_begin(&lg->items);
	for (; it != vec_end(&lg->items); ++it) {
		try_(_logic_to_process(self, logic_proc, *it));
	}

	string_strcat(logic_proc->plan_msg, ")");
	return FQL_GOOD;
}

int _logicgroup_process(plan* self,
                        logicgroup* lg,
                        bool is_from_hash_join,
                        bool is_from_having)
{
	process* logic_proc = new_(process, "", self);
	//if (is_from_having) {
	//	logic_proc->root_group = &self->query->groupby->_roots;
	//}
	logic_proc->action__ = &fql_logic;
	logic_proc->proc_data = lg;
	try_(_logic_to_process(self, logic_proc, lg));
	dnode* logic_node = dgraph_add_data(self->processes, logic_proc);

	/* If we are dealing with join logic that
	 * has been optimized to a hash join, the
	 * hash join process will take control of
	 * the logic condition that it was created
	 * from. If there is only one condition in
	 * the relevant logic, it must be the join
	 * condition. If this is all true, the
	 * logic process can be marked passive.
	 */
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

	return FQL_GOOD;
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
int _from(plan* self, query* query, struct fql_handle* fql)
{
	if (query->sources->size == 0) {
		return FQL_GOOD;
	}

	string plan_msg;
	string_construct(&plan_msg);
	table* table_iter = vec_begin(query->sources);

	process* from_proc = NULL;
	dnode* from_node = NULL;
	++self->source_count;
	if (table_iter->source_type == SOURCE_TABLE) {
		string_sprintf(&plan_msg,
		               "%s: %s",
		               table_iter->reader->file_name.data,
		               "stream read");

		from_proc = new_(process, plan_msg.data, self);
		from_proc->root_fifo = 0;
		from_node = dgraph_add_data(self->processes, from_proc);
		from_node->is_root = true;
	} else {
		from_proc = new_(process, "subquery select", self);
		from_proc->action__ = &fql_read;
		//from_proc->root_fifo = 1;
		from_node = dgraph_add_data(self->processes, from_proc);
		//from_node->is_root = true;
		try_(_build(table_iter->subquery, fql, from_node, false));
		plan* subquery_plan = table_iter->subquery->plan;
		//process* sub_true_proc = subquery_plan->op_true->data;
		//fqlselect* sub_select = sub_true_proc->proc_data;
		//from_proc->proc_data = sub_select;
		dgraph_consume(self->processes, subquery_plan->processes);
		subquery_plan->processes = NULL;
	}
	from_proc->proc_data = table_iter;
	from_proc->action__ = &fql_read;

	self->current->out[0] = from_node;
	self->current = from_node;
	dnode* join_node = NULL;

	for (++table_iter; table_iter != vec_end(query->sources); ++table_iter) {

		/* TODO */
		if (table_iter->source_type == SOURCE_SUBQUERY) {
			fputs("currently cannot join with a"
			      "sub-query on the right side\n",
			      stderr);
			return FQL_FAIL;
		}

		process* join_proc = NULL;
		bool is_hash_join = (table_iter->condition->join_logic != NULL);
		if (is_hash_join) {
			join_proc = _new_join_proc(table_iter->join_type, "hash", self);
			join_proc->action__ = &fql_hash_join;
			table_hash_join_init(table_iter);
			join_proc->has_second_input = true;

			process* read_proc = NULL;
			dnode* read_node = NULL;

			/* TODO: subquery on right side of join */
			//if (table_iter->source_type == SOURCE_TABLE) {
			string_sprintf(&plan_msg,
			               "%s: %s",
			               table_iter->reader->file_name.data,
			               "mmap read");

			read_proc = new_(process, plan_msg.data, self);
			read_proc->action__ = &fql_read;
			read_node = dgraph_add_data(self->processes, read_proc);
			//} else {
			//	read_proc = new_(process, "subquery select", self);
			//	read_proc->action__ = &fql_read_subquery;
			//	read_proc->root_fifo = 1;
			//	read_node = dgraph_add_data(self->processes, read_proc);
			//	read_node->is_root = true;
			//	Plan* subquery_plan = _build(table_iter->subquery, read_node);
			//	fail_if_ (subquery_plan == NULL);
			//	read_proc->subquery_plan_id = subquery_plan->plan_id;
			//	dgraph_consume(self->processes, subquery_plan->processes);
			//	subquery_plan->processes = NULL;
			//}
			table_iter->read_proc = read_proc;
			read_proc->proc_data = table_iter;
			read_proc->is_secondary = true;
			read_proc->root_fifo = 0;
			read_node->is_root = true;

			join_node = dgraph_add_data(self->processes, join_proc);
			read_node->out[0] = join_node;
		} else {
			join_proc =
			        _new_join_proc(table_iter->join_type, "cartesian", self);
			join_proc->action__ = &fql_cartesian_join;
			join_proc->root_fifo = 1;
			join_node = dgraph_add_data(self->processes, join_proc);
			join_node->is_root = true;
		}
		join_proc->proc_data = table_iter;
		self->current->out[0] = join_node;
		self->current = join_node;

		if (table_iter->condition != NULL) {
			try_(_logicgroup_process(self,
			                         table_iter->condition,
			                         is_hash_join,
			                         false));
		}
	}

	string_destroy(&plan_msg);

	return FQL_GOOD;
}

int _where(plan* self, query* query)
{
	if (query->where == NULL) {
		return FQL_GOOD;
	}

	return _logicgroup_process(self, query->where, false, false);
}

/* NOTE: If there is a grouping, the grouping becomes the
 *       one and only source. hence source_count = 1...
 */
void _group(plan* self, query* query)
{
	if (query->groupby != NULL && self->loose_groups) {
		delete_if_exists_(distinct, query->distinct);
		query->distinct = query->groupby;
		query->groupby = NULL;
	}
	if (query->groupby != NULL) {
		process* group_proc = new_(process, "GROUP BY ", self);
		_check_all_for_special_expression(self,
		                                  group_proc,
		                                  &query->groupby->expressions);
		self->source_count = 1;
		group_proc->out_src_count = 1;
		process* true_proc = self->op_true->data;
		true_proc->in_src_count = 1;
		true_proc->out_src_count = 1;

		group_proc->action__ = &fql_groupby;
		group_proc->proc_data = query->groupby;
		group_proc->wait_for_in0_end = true;
		group_proc->root_fifo = 1;
		group_cat_description(query->groupby, group_proc);
		dnode* group_node = dgraph_add_data(self->processes, group_proc);
		vec_push_back(&query->groupby->_roots, &group_node);
		group_node->is_root = true;
		self->current->out[0] = group_node;
		self->current = group_node;
	}

	if (query->distinct) {
		process* group_proc = new_(process, "DISTINCT ", self);
		//if (query->groupby) {
		//	group_proc->root_group = &query->groupby->_roots;
		//}
		group_proc->action__ = &fql_distinct;
		group_proc->proc_data = query->distinct;
		group_cat_description(query->distinct, group_proc);
		dnode* group_node = dgraph_add_data(self->processes, group_proc);
		self->current->out[0] = group_node;
		self->current = group_node;
	}
}

int _having(plan* self, query* query)
{
	if (query->having == NULL) {
		return FQL_GOOD;
	}

	return _logicgroup_process(self, query->having, false, true);
}

void _operation(plan* self, query* query, dnode* entry, bool is_union)
{
	dnode* prev = self->current;
	prev->out[0] = self->op_true;
	self->current = self->op_true;
	//if (query->groupby) {
	//	process* true_proc = self->op_true->data;
	//	true_proc->root_group = &query->groupby->_roots;
	//}
	_check_all_for_special_expression(self,
	                                  self->op_true->data,
	                                  op_get_expressions(query->op));

	/* Current no longer matters. After operation, we
	 * do order where current DOES matter... BUT
	 * if we are in a union we should not encounter
	 * ORDER BY...
	 */
	if (is_union) {
		self->current = prev;
		return;
	}
	op_apply_process(query, self, (entry != NULL));
	if (entry == NULL) {
		return;
	}

	self->op_true->out[0] = entry;
}

int _union(plan* self, query* aquery, struct fql_handle* fql)
{
	if (vec_empty(aquery->unions)) {
		return FQL_GOOD;
	}
	/* we have a union, we can assume, our operation is select */
	process* select_proc = self->op_true->data;

	query** it = vec_begin(aquery->unions);
	for (; it != vec_end(aquery->unions); ++it) {
		try_(_build(*it, fql, NULL, true));
		plan* union_plan = (*it)->plan;
		vec_push_back(select_proc->union_end_nodes, &union_plan->current);
		process* union_proc = union_plan->op_true->data;
		union_proc->is_passive = true;
		union_proc = union_plan->op_false->data;
		union_proc->is_passive = true;
		dgraph_consume(self->processes, union_plan->processes);
		union_plan->processes = NULL;
	}
	return FQL_GOOD;
}

void _order(plan* self, query* query)
{
	if (query->orderby == NULL) {
		return;
	}
	process* order_proc = new_(process, "ORDER BY ", self);
	//if (query->groupby) {
	//	order_proc->root_group = &query->groupby->_roots;
	//}
	_check_all_for_special_expression(self, order_proc, &query->orderby->expressions);
	order_proc->action__ = &fql_orderby;
	order_proc->proc_data = query->orderby;
	order_proc->wait_for_in0_end = true;
	order_cat_description(query->orderby, order_proc);
	dnode* order_node = dgraph_add_data(self->processes, order_proc);
	self->current->out[0] = order_node;
	self->current = order_node;
	self->op_true = order_node;
}

/* In an effort to make building of the process graph easier
 * passive nodes are used as a sort of link between the steps.
 * TODO: Fix my shit design
 */
void _clear_passive(plan* self)
{
	vec* node_vec = self->processes->nodes;
	dnode** nodes = vec_begin(node_vec);

	/* re-link nodes so passive ones get skipped during traversal */
	unsigned i = 0;
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
 */
void _update_pipes(dgraph* proc_graph)
{
	while (dgraph_traverse(proc_graph))
		;

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
			proc->fifo_out[0] = (proc->is_secondary) ? proc0->fifo_in[1]
			                                         : proc0->fifo_in[0];
		}

		if ((*nodes)->out[1] != NULL) {
			process* proc1 = (*nodes)->out[1]->data;
			proc->fifo_out[1] = (proc->is_secondary) ? proc1->fifo_in[1]
			                                         : proc1->fifo_in[0];
		}
	}
}

void _mark_roots_const(vec* roots)
{
	dnode** it = vec_begin(roots);
	for (; it != vec_end(roots); ++it) {
		process* proc = (*it)->data;
		if (proc->action__ != fql_read) {
			if (proc->root_fifo == PROCESS_NO_ROOT) {
				proc->root_fifo = 0;
			}
			proc->is_const = true;
		}
	}
}

int _build(query* aquery, struct fql_handle* fql, dnode* entry, bool is_union)
{
	/* Loop through constant value subqueries and
	 * build their plans
	 */
	query** it = vec_begin(aquery->subquery_const_vec);
	for (; it != vec_end(aquery->subquery_const_vec); ++it) {
		try_(_build(*it, fql, NULL, false));
	}

	aquery->plan = new_(plan, aquery, fql);
	plan* self = aquery->plan;

	/* aquery */
	try_(_from(self, aquery, fql));
	try_(_where(self, aquery));
	_group(self, aquery);
	try_(_having(self, aquery));
	_operation(self, aquery, entry, is_union);
	try_(_union(self, aquery, fql));
	_order(self, aquery);

	/* Uncomment this to view the plan *with* passive nodes */
	//_print_plan(self);

	_clear_passive(self);

	if (vec_empty(self->processes->nodes)) {
		process* entry_proc = entry->data;
		entry_proc->is_const = true;
		return FQL_GOOD;
	}

	dgraph_get_roots(self->processes);
	/* Loop through constant value subqueries again,
	 * but this time, consume all process nodes into
	 * current plan.
	 */
	it = vec_begin(aquery->subquery_const_vec);
	for (; it != vec_end(aquery->subquery_const_vec); ++it) {
		dgraph_consume(aquery->plan->processes, (*it)->plan->processes);
		(*it)->plan->processes = NULL;
	}
	dgraph_get_roots(self->processes);

	if (vec_empty(aquery->sources)) {
		_mark_roots_const(self->processes->_roots);
	}

	if (aquery->query_id != 0) { /* is subquery */
		return FQL_GOOD;
	}
	_activate_procs(self);
	_make_pipes(self);
	_update_pipes(self->processes);
	//_union2(self, aquery);

	return FQL_GOOD;
}

int build_plans(struct fql_handle* fql)
{
	queue* node = fql->query_list;

	for (; node; node = node->next) {
		query* query = node->data;
		try_(_build(query, fql, NULL, false));
	}

	return FQL_GOOD;
}

void _expr_sep(int n)
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
		int len = proc->plan_msg->size;
		if (len > max_len) {
			max_len = len;
		}
	}

	/* Print Header */
	fputs("NODE", stderr);
	_expr_sep(max_len - strlen("NODE"));
	fputs("BRANCH 0", stderr);
	_expr_sep(max_len - strlen("BRANCH 0"));
	fputs("BRANCH 1\n", stderr);

	int i = 0;
	for (i = 0; i < max_len; ++i) {
		fputc('=', stderr);
	}
	_expr_sep(0);
	for (i = 0; i < max_len; ++i) {
		fputc('=', stderr);
	}
	_expr_sep(0);
	for (i = 0; i < max_len; ++i) {
		fputc('=', stderr);
	}

	/* print adjacency list */
	for (it = vec_begin(nodes); it != vec_end(nodes); ++it) {
		fputc('\n', stderr);

		process* proc = (*it)->data;
		int len = proc->plan_msg->size;
		fputs(proc->plan_msg->data, stderr);
		_expr_sep(max_len - len);
		len = 0;

		if ((*it)->out[0] != NULL) {
			proc = (*it)->out[0]->data;
			len = proc->plan_msg->size;
			fputs(proc->plan_msg->data, stderr);
		}
		_expr_sep(max_len - len);

		if ((*it)->out[1] != NULL) {
			proc = (*it)->out[1]->data;
			fputs(proc->plan_msg->data, stderr);
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

unsigned _get_union_pipe_count(vec* nodes)
{
	unsigned total = 0;
	dnode** it = vec_begin(nodes);
	for (; it != vec_end(nodes); ++it) {
		process* proc = (*it)->data;
		total += proc->union_end_nodes->size;
	}
	return total;
}

void _activate_procs(plan* self)
{
	/* First, build root record pipe */
	unsigned graph_size = self->processes->nodes->size;
	unsigned union_pipes = _get_union_pipe_count(self->processes->nodes);
	unsigned pipe_count = graph_size + union_pipes;

	unsigned fifo_base_size = pipe_count * self->pipe_factor;
	unsigned root_size = fifo_base_size * pipe_count;

	self->_recgroups = new_t_(vec, recgroup);
	vec_resize(self->_recgroups, root_size);
	self->root = new_t_(fifo, recgroup*, root_size);

	unsigned i = 0;
	for (; i < self->_recgroups->size; ++i) {
		recgroup* rg = vec_at(self->_recgroups, i);
		recgroup_construct(rg, i);
		vec_set(self->root->buf, i, &rg);
	}

	vec* node_vec = self->processes->nodes;
	dnode** nodes = vec_begin(node_vec);

	dnode** root_node = vec_begin(self->processes->_roots);
	process* first_root = (*root_node)->data;

	if (first_root->is_const) {
		self->is_const = true;
		fifo_advance(self->root);
	} else {
		self->is_const = false;
		fifo_set_full(self->root);
	}

	/* (*nodes)->data is passing the process...
	 * in the ugliest possible way.
	 *
	 * This could all be done using the bottom loop,
	 * but I unrolled a few for easier tracking.
	 */
	process_activate((*nodes)->data, self, fifo_base_size);
	if (++nodes == vec_end(node_vec))
		return;
	process_activate((*nodes)->data, self, fifo_base_size);
	if (++nodes == vec_end(node_vec))
		return;
	process_activate((*nodes)->data, self, fifo_base_size);
	if (++nodes == vec_end(node_vec))
		return;
	process_activate((*nodes)->data, self, fifo_base_size);
	if (++nodes == vec_end(node_vec))
		return;
	process_activate((*nodes)->data, self, fifo_base_size);
	if (++nodes == vec_end(node_vec))
		return;
	process_activate((*nodes)->data, self, fifo_base_size);

	for (++nodes; nodes != vec_end(node_vec); ++nodes) {
		process_activate((*nodes)->data, self, fifo_base_size);
	}
}
