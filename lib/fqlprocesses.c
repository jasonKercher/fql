#include "process.h"
#include "fql.h"
#include "misc.h"
#include "group.h"
#include "logic.h"
#include "order.h"
#include "record.h"
#include "expression.h"
#include "reader.h"
#include "aggregate.h"
#include "fqlselect.h"
#include "util/fifo.h"

void _recycle(process* proc, recgroup** rg)
{
	/* Remove reference records here so they
	 * don't get recycled.
	 */
	//recgroup_clear_refs(rg);
	fifo_add(proc->root_ref, rg);
}

int fql_read(process* proc)
{
	table* table = proc->proc_data;
	reader* reader = table->reader;

	fifo* in = proc->fifo_in[0];
	fifo* out = proc->fifo_out[0];

	recgroup** rg_iter = fifo_begin(in);
	for (; rg_iter != fifo_end(in) && fifo_receivable(out); rg_iter = fifo_iter(in)) {
		switch (reader->get_record__(reader, *rg_iter)) {
		case FQL_GOOD:
			//rec->ref_count = 0;
			break;
		case FQL_FAIL:
			return FQL_FAIL;
		default: /* eof */
			process_disable(proc);
			return 0;
		}

		fifo_add(out, rg_iter);
	}
	fifo_update(in);

	return 1;
}

int fql_cartesian_join(process* proc)
{
	table* table = proc->proc_data;
	reader* reader = table->reader;

	fifo* in_left = proc->fifo_in[0];
	fifo* in_right = proc->fifo_in[1];
	fifo* out = proc->fifo_out[0];

	recgroup** iter_left = fifo_begin(in_left);
	recgroup** iter_right = fifo_begin(in_right);
	for (; iter_right != fifo_end(in_right); iter_right = fifo_iter(in_right)) {
		//recgroup_resize(iter_right, proc->out_src_count);
		//record* rec_right = recgroup_rec_back(iter_right);
		int ret = reader->get_record__(reader, *iter_right);

		switch (ret) {
		case FQL_GOOD:
			break;
		case FQL_FAIL:
			return FQL_FAIL;
		default: { /* eof */
			reader->reset__(reader);
			fifo_iter(in_left);
			return 1;
		}
		}

		//_adj_left_side_ref_count(iter_left, 1);

		recgroup_rec_add_front(*iter_right, proc->out_src_count - 1);
		unsigned i = 0;
		for (; i < (*iter_left)->records->size; ++i) {
			record* rec = recgroup_rec_at(*iter_left, i);
			recgroup_rec_set_ref(*iter_right, i, rec);
		}
		fifo_add(out, iter_right);
	}
	fifo_update(in_right);

	return 1;
}

void _hash_join_right_side(process* proc, table* table, fifo* in_right)
{
	struct hashjoin* hj = table->join_data;
	stringview sv;

	recgroup** rg_iter = fifo_begin(in_right);
	for (; rg_iter != fifo_end(in_right); rg_iter = fifo_iter(in_right)) {
		//recgroup_rec_add_one_front(rg_iter);
		//recgroup_resize(rg_iter, proc->out_src_count);
		recgroup_rec_add_front(*rg_iter, proc->out_src_count - 1);
		expression_get_stringview(&sv, hj->right_expr, *rg_iter);

		record* rec = recgroup_rec_back(*rg_iter);
		//record** rightrec = vec_back(rg_iter);
		multimap_nset(hj->hash_data, sv.data, &rec->rec_ref.data, sv.len);

		_recycle(proc, rg_iter);
	}
	fifo_update(in_right);
}

char* _hash_join_left_side(process* proc, table* table, recgroup* left_rg)
{
	struct hashjoin* hj = table->join_data;

	if (hj->recs == NULL) {
		stringview sv;
		expression_get_stringview(&sv, hj->left_expr, left_rg);

		hj->recs = multimap_nget(hj->hash_data, sv.data, sv.len);
		if (hj->recs == NULL) {
			return NULL;
		}
		hj->rec_idx = 0;
	}

	char** right_location_ref = vec_at(hj->recs, hj->rec_idx++);
	if (hj->rec_idx >= hj->recs->size) {
		hj->recs = NULL;
	}

	return *right_location_ref;
}

int fql_hash_join(process* proc)
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
	 * when left_rg_iter hit EOF. run left_rg_iter manually as no-op
	 * here just to push records.
	 */
	process* right_side_read_proc = table->read_proc;

	recgroup** left_rg_iter = fifo_begin(in_left);
	while (left_rg_iter != fifo_end(in_left) && fifo_receivable(out)) {

		char* right_location = _hash_join_left_side(proc, table, *left_rg_iter);
		if (right_location == NULL) {
			_recycle(proc, left_rg_iter);
			left_rg_iter = fifo_iter(in_left);
			continue;
		}
		fql_no_op(right_side_read_proc);
		recgroup** right_rg = fifo_get(proc->fifo_in[1]);

		reader* reader = table->reader;

		reader->get_record_at__(reader, *right_rg, right_location);
		recgroup_rec_add_front(*right_rg, proc->out_src_count - 1);

		/* If this is the last record match to the left
		 * side, send the left side. Otherwise, insert
		 * references to the left side to the right and
		 * send the right side.
		 */
		if (hj->recs == NULL) {
			recgroup_rec_push_back(*left_rg_iter,
			                       recgroup_rec_pop(*right_rg));
			fifo_add(out, left_rg_iter);
			left_rg_iter = fifo_iter(in_left);
			//recgroup_resize(*right_rg, 0);
			_recycle(proc, right_rg);
		} else {
			unsigned i = 0;
			for (; i < (*left_rg_iter)->records->size; ++i) {
				record* leftrec = recgroup_rec_at(*left_rg_iter, i);
				recgroup_rec_set_ref(*right_rg, i, leftrec);
			}
			fifo_add(out, right_rg);
		}
	}
	fifo_update(in_left);

	return 1;
}

int fql_logic(process* proc)
{
	logicgroup* lg = proc->proc_data;

	fifo* in = proc->fifo_in[0];
	fifo* out_false = proc->fifo_out[0];
	fifo* out_true = proc->fifo_out[1];

	recgroup** rg_iter = fifo_begin(in);
	for (; rg_iter != fifo_end(in) && fifo_receivable(out_true)
	       && (!out_false || fifo_receivable(out_false));
	     rg_iter = fifo_iter(in)) {
		int ret = try_(logicgroup_eval(lg, *rg_iter, lg->join_logic));

		if (ret) {
			fifo_add(out_true, rg_iter);
		} else if (out_false != NULL) {
			fifo_add(out_false, rg_iter);
		} else {
			_recycle(proc, rg_iter);
		}

		if (proc->is_const) {
			process_disable(proc);
			break;
		}
	}
	fifo_update(in);

	return 1;
}

int _group_dump(process* proc, fifo* in_fresh, fifo* out)
{
	group* group = proc->proc_data;

	recgroup** rg_iter = fifo_begin(in_fresh);
	for (; rg_iter != fifo_end(in_fresh) && fifo_receivable(out);
	     rg_iter = fifo_iter(in_fresh)) {
		recgroup_resize(*rg_iter, 1);
		record* rec = recgroup_rec_begin(*rg_iter);

		switch (group_dump_record(group, rec)) {
		case FQL_FAIL:
			return FQL_FAIL;
		case FQL_GOOD:
			fifo_add(out, rg_iter);
			break;
		default:
			process_disable(proc);
			return 0;
		}
	}
	fifo_update(in_fresh);

	return 1;
}

int fql_groupby(process* proc)
{
	fifo* in_data = proc->fifo_in[0];
	fifo* in_fresh = proc->fifo_in[1];
	fifo* out = proc->fifo_out[0];

	if (!in_data->is_open && fifo_is_empty(in_data)) {
		return _group_dump(proc, in_fresh, out);
	}

	group* group = proc->proc_data;

	recgroup** rg_iter = fifo_begin(in_data);
	for (; rg_iter != fifo_end(in_data); rg_iter = fifo_iter(in_data)) {
		/* TODO: something with returns? */
		if (!vec_empty(&group->expressions)) {
			try_(group_record(group, *rg_iter));
		}

		/* this entire if block needs to exist in case
		 * you _really_ need to know the results of:
		 * select count(*)  -- blasphemy
		 */
		if (!proc->is_const) {
			_recycle(proc, rg_iter);
		} else {
			recgroup** rg = fifo_get(in_fresh);
			recgroup_resize(*rg, 1);
			record* rec = recgroup_rec_at(*rg, 0);
			try_(group_dump_record(group, rec));
			fifo_add(out, rg);
			process_disable(proc);
		}
	}
	fifo_update(in_data);
	return 1;
}

int fql_distinct(process* proc)
{
	group* group = proc->proc_data;

	fifo* in = proc->fifo_in[0];
	fifo* out = proc->fifo_out[0];

	recgroup** rg_iter = fifo_begin(in);
	for (; rg_iter != fifo_end(in) && fifo_receivable(out); rg_iter = fifo_iter(in)) {
		switch (group_record(group, *rg_iter)) {
		case FQL_FAIL:
			return FQL_FAIL;
		case 1:
			fifo_add(proc->fifo_out[0], rg_iter);
			break;
		case 0:
		default:
			_recycle(proc, rg_iter);
		}
	}
	fifo_update(in);
	return 1;
}

int fql_select(process* proc)
{
	fqlselect* select = proc->proc_data;

	fifo* in = proc->fifo_in[0];
	fifo* out = proc->fifo_out[0];

	if (!proc->wait_for_in0) {
		/* subquery reads expect union schema to
		 * be "in sync" with the subquery select's
		 * current schema
		 */
		if (out && !fifo_is_empty(out)) {
			return 1;
		}

		if (fqlselect_next_union(select)) {
			proc->wait_for_in0 = true;
			proc->fifo_in[0] = proc->queued_results->data;
			if (proc->queued_results->next != NULL) {
				proc->queued_results = proc->queued_results->next;
			}
			return 1;
		} else {
			writer_close(select->writer);
			process_disable(proc);
			return 0;
		}
	}

	int ret = 0;
	unsigned iters = 0;
	recgroup** rg_iter = fifo_begin(in);
	for (; iters++ < proc->max_recs_iter && rg_iter != fifo_end(in)
	       && (!out || fifo_receivable(out))
	       && select->rows_affected < select->top_count;
	     rg_iter = fifo_iter(in)) {
		ret = try_(select->select__(select, *rg_iter));

		++proc->rows_affected;
		++select->rows_affected;

		if (out) {
			fifo_add(out, rg_iter);
		} else if (!select->is_const) {
			_recycle(proc, rg_iter);
		}

		if (select->is_const) {
			proc->wait_for_in0 = false;
			break;
		}
	}


	fifo_update(in);

	if (select->rows_affected >= select->top_count) {
		proc->wait_for_in0 = false;
	}
	return ret;
}

int fql_orderby(process* proc)
{
	order* order = proc->proc_data;
	fifo* in = proc->fifo_in[0];
	if (!in->is_open && fifo_is_empty(in)) {
		if (!order->sorted) {
			try_(order_sort(order));
		}
		int ret = try_(order->select__(order, proc));
		if (proc->rows_affected >= proc->top_count) {
			process_disable(proc);
		}
		return ret;
	}

	unsigned iters = 0;
	recgroup** rg_iter = fifo_begin(in);
	for (; iters++ < proc->max_recs_iter && rg_iter != fifo_end(in);
	     rg_iter = fifo_iter(in)) {
		try_(order_add_record(order, *rg_iter));
		_recycle(proc, rg_iter);
	}
	fifo_update(in);
	return 0;
}

int fql_no_op(process* proc)
{
	recgroup** rg = fifo_get(proc->fifo_in[0]);
	fifo_add(proc->fifo_out[0], rg);
	return 0;
}
