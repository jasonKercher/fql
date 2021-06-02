#include "process.h"
#include "fql.h"
#include "misc.h"
#include "group.h"
#include "logic.h"
#include "order.h"
#include "record.h"
#include "column.h"
#include "reader.h"
#include "aggregate.h"
#include "fqlselect.h"
#include "util/fifo.h"

void _recycle_specific(process* proc, vec* recs, int index)
{
	dnode** root_node = vec_at(proc->root_group, index);
	process* root = (*root_node)->data;

	if (root->is_const) {
		return;
	}

	record** rec = vec_at(recs, index);
	if (--(*rec)->ref_count > 0) {
		return;
	}

	vec* proc_recs = vec_at(root->records, (*rec)->idx);

	fifo_add(root->fifo_in[root->root_fifo], &proc_recs);
}

/* trigger appropiate roots to read the next record */
void _recycle_recs(process* proc, vec* recs, int width)
{
	/* width == 0 means constant expression */
	if (width == 0) {
		dnode** root_node = vec_begin(proc->root_group);
		process_disable((*root_node)->data);
		return;
	}
	int i = 0;
	for (; i < width; ++i) {
		_recycle_specific(proc, recs, i);
	}
}

int fql_read(dgraph* proc_graph, process* proc)
{
	table* table = proc->proc_data;
	reader* reader = table->reader;

	fifo* in = proc->fifo_in[0];
	fifo* out = proc->fifo_out[0];

	vec** it = fifo_begin(in);
	for (; it != fifo_end(in) && fifo_receivable(out); it = fifo_iter(in)) {
		record** rec = vec_back(*it);

		switch (reader->get_record__(reader, *rec)) {
		case FQL_GOOD:
			(*rec)->ref_count = 0;
			break;
		case FQL_FAIL:
			return FQL_FAIL;
		default: /* eof */
			process_disable(proc);
			return 0;
		}

		fifo_add(out, it);
	}
	fifo_update(in);

	return 1;
}

int fql_read_subquery(dgraph* proc_graph, process* proc)
{
	table* table = proc->proc_data;
	reader* reader = table->reader;

	fifo* in_sub = proc->fifo_in[0];
	fifo* in_fresh = proc->fifo_in[1];
	fifo* out = proc->fifo_out[0];

	int ret = 0;
	vec** it_sub = fifo_begin(in_sub);
	vec** it_fresh = fifo_begin(in_fresh);
	for (; it_sub != fifo_end(in_sub) && it_fresh != fifo_end(in_fresh)
	       && fifo_receivable(out);
	     it_sub = fifo_iter(in_sub), it_fresh = fifo_iter(in_fresh)) {
		record** rec_fresh = vec_back(*it_fresh);
		reader->subquery_recs = *it_sub;

		switch (reader->get_record__(reader, *rec_fresh)) {
		case FQL_GOOD:
			ret = 1;
			break;
		case FQL_FAIL:
		default:
			return FQL_FAIL;
		}

		if (proc->is_const) {
			process_disable(proc);
		} else {
			_recycle_recs(proc, *it_sub, (*it_sub)->size);
		}
		fifo_add(out, it_fresh);
	}
	fifo_update(in_sub);
	fifo_update(in_fresh);

	return ret;
}

void _adj_left_side_ref_count(vec* leftrecs, int adj)
{
	record** it = vec_begin(leftrecs);
	for (; it != vec_end(leftrecs); ++it) {
		(*it)->ref_count += adj;
	}
}

int fql_cartesian_join(dgraph* proc_graph, process* proc)
{
	table* table = proc->proc_data;
	reader* reader = table->reader;

	fifo* in_left = proc->fifo_in[0];
	fifo* in_right = proc->fifo_in[1];
	fifo* out = proc->fifo_out[0];

	vec** it_left = fifo_begin(in_left);
	vec** it_right = fifo_begin(in_right);
	for (; it_right != fifo_end(in_right); it_right = fifo_iter(in_right)) {
		record** rec_right = vec_back(*it_right);
		int ret = reader->get_record__(reader, *rec_right);

		switch (ret) {
		case FQL_GOOD:
			break;
		case FQL_FAIL:
			return FQL_FAIL;
		default: /* eof */
		{
			reader->reset__(reader);
			record** it = vec_begin(*it_left);
			fifo_iter(in_left);
			return 1;
		}
		}

		_adj_left_side_ref_count(*it_left, 1);

		unsigned i = 0;
		for (; i < (*it_left)->size; ++i) {
			record** leftrec = vec_at(*it_left, i);
			vec_set(*it_right, i, leftrec);
		}
		fifo_add(out, it_right);
	}
	fifo_update(in_right);

	return 1;
}

void _hash_join_right_side(process* proc, table* table, fifo* in_right)
{
	struct hashjoin* hj = table->join_data;
	stringview sv;

	vec** it = fifo_begin(in_right);
	for (; it != fifo_end(in_right); it = fifo_iter(in_right)) {
		column_get_stringview(&sv, hj->right_col, *it);

		record** rightrec = vec_back(*it);
		multimap_nset(&hj->hash_data,
		              sv.data,
		              &(*rightrec)->rec_raw.data,
		              sv.len);

		_recycle_specific(proc, *it, proc->in_src_count - 1);
	}
	fifo_update(in_right);
}

vec* _hash_join_left_side(process* proc, table* table, vec* leftrecs)
{
	struct hashjoin* hj = table->join_data;

	if (hj->recs == NULL) {
		stringview sv;
		column_get_stringview(&sv, hj->left_col, leftrecs);

		hj->recs = multimap_nget(&hj->hash_data, sv.data, sv.len);
		if (hj->recs == NULL) {
			return NULL;
		}
		hj->rec_idx = 0;
	}

	char** rightrec_ptr = vec_at(hj->recs, hj->rec_idx++);
	if (hj->rec_idx >= hj->recs->size) {
		hj->recs = NULL;
	}
	_adj_left_side_ref_count(leftrecs, 1);

	vec** rightrecs = fifo_get(proc->fifo_in[1]);
	record** rec = vec_back(*rightrecs);
	reader* reader = table->reader;

	reader->get_record_at__(reader, *rec, *rightrec_ptr);

	return *rightrecs;
}

int fql_hash_join(dgraph* proc_graph, process* proc)
{
	table* table = proc->proc_data;
	struct hashjoin* hj = table->join_data;

	fifo* in_left = proc->fifo_in[0];
	fifo* in_right = proc->fifo_in[1];
	fifo* out = proc->fifo_out[1];

	if (hj->state == SIDE_RIGHT && fifo_is_empty(in_right)) {
		if (!fifo_is_open(in_right)) {
			hj->state = SIDE_LEFT;
			return 1;
		} else {
			return 0;
		}
	}

	if (hj->state == SIDE_RIGHT) {
		_hash_join_right_side(proc, table, in_right);
		return 1;
	}

	/* the right side read process killed itself
	 * when it hit EOF. run it manually as no-op
	 * here just to push records.
	 */
	process* right_side_read_proc = table->read_proc;

	vec** it = fifo_begin(in_left);
	while (it != fifo_end(in_left) && fifo_receivable(out)) {
		fql_no_op(proc_graph, right_side_read_proc);

		vec* rightrecs = _hash_join_left_side(proc, table, *it);
		if (rightrecs == NULL) {
			_recycle_recs(proc, *it, proc->in_src_count - 1);
			it = fifo_iter(in_left);
			fifo_iter(in_right);
			continue;
		}

		unsigned i = 0;
		for (; i < (*it)->size; ++i) {
			record** leftrec = vec_at(*it, i);
			vec_set(rightrecs, i, leftrec);
		}

		if (hj->recs == NULL) {
			it = fifo_iter(in_left);
		}

		fifo_add(out, &rightrecs);
	}
	fifo_update(in_left);

	return 1;
}

int fql_logic(dgraph* proc_graph, process* proc)
{
	logicgroup* lg = proc->proc_data;

	fifo* in = proc->fifo_in[0];
	fifo* out_false = proc->fifo_out[0];
	fifo* out_true = proc->fifo_out[1];

	vec** it = fifo_begin(in);
	for (; it != fifo_end(in) && fifo_receivable(out_true)
	       && (!out_false || fifo_receivable(out_false));
	     it = fifo_iter(in)) {
		int ret = try_(logicgroup_eval(lg, *it, lg->join_logic));

		if (ret) {
			fifo_add(out_true, it);
		} else if (out_false != NULL) {
			fifo_add(out_false, it);
		} else {
			_recycle_recs(proc, *it, proc->in_src_count);
		}
	}
	fifo_update(in);

	return 1;
}

int _group_dump(process* proc, fifo* in_fresh, fifo* out)
{
	group* group = proc->proc_data;

	vec** it = fifo_begin(in_fresh);
	for (; it != fifo_end(in_fresh) && fifo_receivable(out);
	     it = fifo_iter(in_fresh)) {
		record** rec = vec_at(*it, 0);

		switch (group_dump_record(group, *rec)) {
		case FQL_FAIL:
			return FQL_FAIL;
		case FQL_GOOD:
			fifo_add(out, it);
			break;
		default:
			process_disable(proc);
			return 0;
		}
	}
	fifo_update(in_fresh);

	return 1;
}

int fql_groupby(dgraph* proc_graph, process* proc)
{
	fifo* in_data = proc->fifo_in[0];
	fifo* in_fresh = proc->fifo_in[1];
	fifo* out = proc->fifo_out[0];

	if (!in_data->is_open && fifo_is_empty(in_data)) {
		return _group_dump(proc, in_fresh, out);
	}

	group* group = proc->proc_data;

	vec** it = fifo_begin(in_data);
	for (; it != fifo_end(in_data); it = fifo_iter(in_data)) {
		/* TODO: something with returns? */
		if (!vec_empty(&group->columns)) {
			int ret = try_(group_record(group, *it));
		}

		/* this entire if block needs to exist in case
		 * you _really_ need to know the results of:
		 * select count(*)  -- blasphemy
		 */
		if (!proc->is_const) {
			_recycle_recs(proc, *it, (*it)->size);
		} else {
			vec** recs = fifo_get(in_fresh);
			record** rec = vec_at(*recs, 0);
			int ret = try_(group_dump_record(group, *rec));
			fifo_add(out, recs);
			process_disable(proc);
		}
	}
	fifo_update(in_data);
	return 1;
}

int fql_distinct(dgraph* proc_graph, process* proc)
{
	group* group = proc->proc_data;

	fifo* in = proc->fifo_in[0];
	fifo* out = proc->fifo_out[0];

	vec** it = fifo_begin(in);
	for (; it != fifo_end(in) && fifo_receivable(out); it = fifo_iter(in)) {
		switch (group_record(group, *it)) {
		case FQL_FAIL:
			return FQL_FAIL;
		case 1:
			fifo_add(proc->fifo_out[0], it);
			break;
		case 0:
		default:
			_recycle_recs(proc, *it, (*it)->size);
		}
	}
	fifo_update(in);
	return 1;
}

int fql_select(dgraph* proc_graph, process* proc)
{
	if (!proc->wait_for_in0) {
		fqlselect_close(proc->proc_data);
		process_disable(proc);
		return 0;
	}

	fifo* in = proc->fifo_in[0];
	fifo* out = proc->fifo_out[0];

	int ret = 0;
	unsigned iters = 0;
	vec** it = fifo_begin(in);
	for (; iters++ < proc->max_recs_iter && it != fifo_end(in)
	       && (!out || fifo_receivable(out));
	     it = fifo_iter(in)) {
		fqlselect* select = proc->proc_data;
		ret = try_(select->select__(select, *it));

		if (out) {
			fifo_add(out, it);
		} else {
			_recycle_recs(proc, *it, proc->in_src_count);
		}
	}
	fifo_update(in);
	return ret;
}

int fql_orderby(dgraph* proc_graph, process* proc)
{
	order* order = proc->proc_data;
	fifo* in = proc->fifo_in[0];
	if (!in->is_open && fifo_is_empty(in)) {
		if (!order->sorted) {
			order_sort(order);
		}
		return order->select__(order);
	}

	unsigned iters = 0;
	vec** it = fifo_begin(in);
	for (; iters++ < proc->max_recs_iter && it != fifo_end(in);
	     it = fifo_iter(in)) {
		int ret = try_(order_add_record(order, *it));
		_recycle_recs(proc, *it, (*it)->size);
	}
	fifo_update(in);
	return 0;
}

int fql_no_op(dgraph* proc_graph, process* proc)
{
	vec** recs = fifo_get(proc->fifo_in[0]);
	fifo_add(proc->fifo_out[0], recs);
	return 0;
}
