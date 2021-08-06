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
#include "fqldelete.h"
#include "fqlupdate.h"
#include "util/fifo.h"

void _recycle(process* proc, node** rg)
{
	while (*rg) {
		node* rg_node = node_pop_export(rg);
		fifo_add(proc->root_ref, &rg_node);
	}
}

int fql_read(process* proc)
{
	fifo* in = proc->fifo_in[0];
	if (fifo_is_empty(in)) {
		if (!in->is_open) {
			process_disable(proc);
		}
		return 0;
	}

	fifo* out = proc->fifo_out[0];

	////

	table* table = proc->proc_data;
	reader* reader = table->reader;

	node** rg_iter = fifo_begin(in);
	for (; rg_iter != fifo_end(in) && fifo_receivable(out); rg_iter = fifo_iter(in)) {
		record* rec = (*rg_iter)->data;
		rec->src_idx = table->idx;
		switch (reader->get_record__(reader, *rg_iter)) {
		case FQL_GOOD:
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
	fifo* in_left = proc->fifo_in[0];
	fifo* in_right = proc->fifo_in[1];

	if (fifo_is_empty(in_left)) {
		if (!in_left->is_open) {
			process_disable(proc);
		}
		return 1;
	}

	////

	fifo* out = proc->fifo_out[0];

	table* table = proc->proc_data;
	reader* reader = table->reader;

	node** iter_left = fifo_begin(in_left);
	unsigned out_space = fifo_receivable(out);

	if (proc->inbuf_idx == proc->inbuf->size) {
		vec_clear(proc->inbuf);
		proc->inbuf_idx = 0;
		fifo_nget(in_right, proc->inbuf, proc->out_src_count, out_space);
	}
	node** iter_right = vec_at(proc->inbuf, proc->inbuf_idx);
	for (; iter_right != vec_end(proc->inbuf); ++iter_right, ++proc->inbuf_idx) {
		record* rec = (*iter_right)->data;
		rec->src_idx = table->idx;
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

		node* left_rec_node = node_back(*iter_left);
		node** right_head = iter_right;

		for (; left_rec_node; left_rec_node = left_rec_node->prev) {
			++iter_right;
			++proc->inbuf_idx;
			record* left_rec = left_rec_node->data;
			record* fresh_rec = (*iter_right)->data;
			record_copy(fresh_rec, left_rec);
			node_push_import(right_head, *iter_right);
		}
		fifo_add(out, right_head);
	}

	return 1;
}

void _hash_join_right_side(process* proc, table* table, fifo* in_right)
{
	struct hashjoin* hj = table->join_data;
	stringview sv;

	node** rg_iter = fifo_begin(in_right);
	for (; rg_iter != fifo_end(in_right); rg_iter = fifo_iter(in_right)) {
		expression_get_stringview(&sv, hj->right_expr, *rg_iter);

		record* rec = (*rg_iter)->data;
		multimap_nset(hj->hash_data, sv.data, &rec->rec_ref.data, sv.len);

		_recycle(proc, rg_iter);
	}
	fifo_update(in_right);
}

char* _hash_join_left_side(process* proc, table* table, node* left_rg)
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

	if (fifo_is_empty(in_left) && fifo_is_empty(in_right)) {
		if (!in_right->is_open && !in_left->is_open) {
			process_disable(proc);
		}
		return 1;
	}

	////

	fifo* out_false = proc->fifo_out[0];
	fifo* out_true = proc->fifo_out[1];

	if (hj->state == SIDE_RIGHT && fifo_is_empty(in_right)) {
		if (!in_right->is_open) {
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

	node** left_rg_iter = fifo_begin(in_left);
	while (left_rg_iter != fifo_end(in_left) && fifo_receivable(out_true)
	       && (out_false == NULL || fifo_receivable(out_false))) {

		char* right_location = _hash_join_left_side(proc, table, *left_rg_iter);
		if (right_location == NULL) {
			if (table->join_type == JOIN_LEFT) {
				fifo_add(out_true, left_rg_iter);
			} else if (out_false != NULL) {
				fifo_add(out_false, left_rg_iter);
			} else {
				_recycle(proc, left_rg_iter);
			}
			left_rg_iter = fifo_iter(in_left);
			continue;
		}
		node** right_rg = fifo_get(proc->root_ref);
		record* right_rec = (*right_rg)->data;
		right_rec->src_idx = proc->out_src_count - 1;
		reader* reader = table->reader;
		reader->get_record_at__(reader, *right_rg, right_location);

		/* If this is the last record match to the left
		 * side, send the left side. Otherwise, insert
		 * references to the left side to the right and
		 * send the right side.
		 */
		if (hj->recs == NULL) {
			node_enqueue_import(left_rg_iter, *right_rg);
			fifo_add(out_true, left_rg_iter);
			left_rg_iter = fifo_iter(in_left);
		} else {
			/* same process as cartesian record copy */
			node* left_rec_node = node_back(*left_rg_iter);

			for (; left_rec_node; left_rec_node = left_rec_node->prev) {
				record* left_rec = left_rec_node->data;
				node** fresh_node = fifo_get(proc->root_ref);
				record* fresh_rec = (*fresh_node)->data;
				record_copy(fresh_rec, left_rec);
				node_push_import(right_rg, *fresh_node);
			}
			fifo_add(out_true, right_rg);
		}
	}
	fifo_update(in_left);

	return 1;
}

int fql_logic(process* proc)
{
	logicgroup* lg = proc->proc_data;

	fifo* in = proc->fifo_in[0];
	if (fifo_is_empty(in)) {
		if (!in->is_open) {
			process_disable(proc);
		}
		return 1;
	}

	////

	fifo* out_false = proc->fifo_out[0];
	fifo* out_true = proc->fifo_out[1];

	node** rg_iter = fifo_begin(in);
	for (; rg_iter != fifo_end(in) && (!out_true || fifo_receivable(out_true))
	       && (!out_false || fifo_receivable(out_false));
	     rg_iter = fifo_iter(in)) {
		int ret = try_(logicgroup_eval(lg, *rg_iter, lg->join_logic));

		if (ret && out_true != NULL) {
			fifo_add(out_true, rg_iter);
		} else if (!ret && out_false != NULL) {
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

int fql_left_join_logic(process* proc)
{
	fifo* in = proc->fifo_in[0];
	if (fifo_is_empty(in)) {
		if (!in->is_open) {
			process_disable(proc);
		}
		return 1;
	}

	////

	// No out_false for left_join
	//fifo* out_false = proc->fifo_out[0];
	fifo* out_true = proc->fifo_out[1];

	logicgroup* lg = proc->proc_data;

	node** rg_iter = fifo_begin(in);
	for (; rg_iter != fifo_end(in) && fifo_receivable(out_true);
	     rg_iter = fifo_iter(in)) {
		/* If we are short a node, right side is already NULL.
		 * These records have already failed a hash join.
		 */
		if (node_count(*rg_iter) == proc->out_src_count) {
			if (!try_(logicgroup_eval(lg, *rg_iter, lg->join_logic))) {
				/* our condition failed, but we still want to pass 
				 * it on. Chop off right-most node and recycle it.
				 */
				node* right_side = node_back(*rg_iter);
				node_export(rg_iter, right_side);
				_recycle(proc, &right_side);
			}
		}

		/* TODO: handle a complex delete here */
		fifo_add(out_true, rg_iter);
	}
	fifo_update(in);

	return 1;
}

int _group_dump(process* proc, fifo* in_fresh, fifo* out)
{
	group* group = proc->proc_data;

	node** rg_iter = fifo_begin(in_fresh);
	for (; rg_iter != fifo_end(in_fresh) && fifo_receivable(out);
	     rg_iter = fifo_iter(in_fresh)) {
		record* rec = (*rg_iter)->data;
		rec->src_idx = 0;

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

	node** rg_iter = fifo_begin(in_data);
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
			node** rg = fifo_get(in_fresh);
			record* rec = (*rg)->data;
			rec->src_idx = 0;
			try_(group_dump_record(group, rec));
			fifo_add(out, rg);
			process_disable(proc);
			break;
		}
	}
	fifo_update(in_data);
	return 1;
}

int fql_distinct(process* proc)
{
	fifo* in = proc->fifo_in[0];
	if (fifo_is_empty(in)) {
		if (proc->wait_for_in0 && !in->is_open) {
			process_disable(proc);
		}
		return 1;
	}

	////

	fifo* out = proc->fifo_out[0];

	group* group = proc->proc_data;
	node** rg_iter = fifo_begin(in);
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
	/* Check if the process in front is closed */
	fifo* out = proc->fifo_out[0];
	if (out && !out->is_open) {
		process_disable(proc);
		return FQL_GOOD;
	}

	fifo* in = proc->fifo_in[0];
	if (fifo_is_empty(in)) {
		if (proc->wait_for_in0 && in->is_open) {
			return 1;
		}
		proc->wait_for_in0 = false;
	}

	////

	fqlselect* select = proc->proc_data;

	if (!proc->wait_for_in0) {
		if (select->must_run_once && select->rows_affected == 0) {
			try_(select->select__(select, NULL));
			++select->rows_affected;
			++proc->rows_affected;
			return 1;
		}
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
	node** rg_iter = fifo_begin(in);
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

int fql_delete(process* proc)
{
	fifo* in = proc->fifo_in[0];

	if (proc->wait_for_in0 && fifo_is_empty(in)) {
		if (in->is_open) {
			return 1;
		}
		proc->wait_for_in0 = false;
	}

	////

	fqldelete* delete = proc->proc_data;
	if (!proc->wait_for_in0) {
		writer_close(delete->writer);
		process_disable(proc);
		return 0;
	}

	int ret = 0;
	node** rg_iter = fifo_begin(in);
	for (; rg_iter != fifo_end(in) && delete->rows_affected < delete->top_count;
	     rg_iter = fifo_iter(in)) {
		ret = try_(delete->undelete__(delete, *rg_iter));
		++proc->rows_affected;
		_recycle(proc, rg_iter);
	}


	fifo_update(in);

	if (delete->rows_affected >= delete->top_count) {
		proc->wait_for_in0 = false;
	}
	return ret;
}

void _del_passthrough(process* proc, fqldelete* delete, fifo* in_false, fifo* in_true)
{
	node** it = fifo_begin(in_false);
	bool false_done = (it == fifo_end(in_false));
	for (; it != fifo_end(in_false); it = fifo_iter(in_false)) {
		record* rec = record_at(*it, delete->table_idx);
		if (rec->rec_idx > delete->delete_idx) {
			false_done = true;
			break;
		}
		_recycle(proc, it);
	}
	fifo_update(in_false);

	it = fifo_begin(in_true);
	if (it == fifo_end(in_true)) {
		delete->state = FILTER_OPEN;
	}
	for (; it != fifo_end(in_true); it = fifo_iter(in_true)) {
		record* rec = record_at(*it, delete->table_idx);
		if (rec->rec_idx > delete->delete_idx) {
			if (false_done) {
				delete->state = FILTER_OPEN;
			}
			break;
		}
		_recycle(proc, it);
	}
	fifo_update(in_true);
}

int _del_filtering(process* proc, fqldelete* delete, fifo* in_false, fifo* in_true)
{
	if (fifo_available(in_true)) {
		node** true_node = fifo_peek(in_true);
		record* rec = record_at(*true_node, delete->table_idx);
		if (rec->rec_idx > delete->delete_idx) {
			node** false_node = fifo_get(in_false);
			try_(delete->undelete__(delete, *false_node));
			_recycle(proc, false_node);
		} else {
			fifo_consume(in_true);
			++proc->rows_affected;
			_recycle(proc, true_node);
		}
		delete->state = FILTER_PASSTHROUGH;
		_del_passthrough(proc, delete, in_false, in_true);
		return FQL_GOOD;
	}

	if (!in_true->is_open) {
		node** false_node = fifo_get(in_false);
		try_(delete->undelete__(delete, *false_node));
		_recycle(proc, false_node);
		delete->state = FILTER_PASSTHROUGH;
		_del_passthrough(proc, delete, in_false, in_true);
		return FQL_GOOD;
	}

	node** iter_false = fifo_begin(in_false);
	for (; iter_false != fifo_end(in_false); iter_false = fifo_iter(in_false)) {
		node** look_ahead = fifo_look_ahead(in_false);
		if (look_ahead == NULL) {
			return FQL_GOOD;
		}
		record* look_ahead_rec = record_at(*look_ahead, delete->table_idx);
		if (look_ahead_rec->rec_idx > delete->delete_idx) {
			try_(delete->undelete__(delete, *iter_false));
			delete->state = FILTER_OPEN;
		}
		_recycle(proc, iter_false);
	}
	return FQL_GOOD;
}

int _del_open(process* proc, fqldelete* delete, fifo* in_false, fifo* in_true)
{
	bool process_true = false;

	node** true_node = NULL;
	record* true_rec = NULL;
	node** false_node = NULL;
	record* false_rec = NULL;

	if (fifo_available(in_false)) {
		false_node = fifo_peek(in_false);
		false_rec = record_at(*false_node, delete->table_idx);
	}

	/* If not stable, this can be skipped */
	if (fifo_available(in_true)) {
		true_node = fifo_peek(in_true);
		true_rec = record_at(*true_node, delete->table_idx);
		if (false_node == NULL || true_rec->rec_idx <= false_rec->rec_idx) {
			process_true = true;
		}
	}
	if (process_true) {
		fifo_consume(in_true);
		_recycle(proc, true_node);
		++proc->rows_affected;
		delete->delete_idx = true_rec->rec_idx;
		delete->state = FILTER_PASSTHROUGH;
		return FQL_GOOD;
	}
	delete->delete_idx = false_rec->rec_idx;
	delete->state = FILTER_FILTERING;
	return FQL_GOOD;
}

int fql_delete_filter(process* proc)
{
	fifo* in_false = proc->fifo_in[0];
	fifo* in_true = proc->fifo_in[1];

	if (fifo_is_empty(in_false) && fifo_is_empty(in_true)) {
		if (in_true->is_open || in_false->is_open) {
			return 1;
		} else {
			proc->wait_for_in0 = false;
		}
	}

	////

	fqldelete* delete = proc->proc_data;
	if (!proc->wait_for_in0) {
		writer_close(delete->writer);
		process_disable(proc);
		return 0;
	}

	switch (delete->state) {
	case FILTER_PASSTHROUGH:
		_del_passthrough(proc, delete, in_false, in_true);
		break;
	case FILTER_FILTERING:
		try_(_del_filtering(proc, delete, in_false, in_true));
		break;
	case FILTER_OPEN:
		try_(_del_open(proc, delete, in_false, in_true));
		break;
	default:;
	}
	return FQL_GOOD;
}

int fql_update(process* proc)
{

	fifo* in_false = proc->fifo_in[0];
	fifo* in_true = proc->fifo_in[1];
	if (fifo_is_empty(in_false) && fifo_is_empty(in_true)) {
		if (in_true->is_open || in_false->is_open) {
			return 1;
		} else {
			proc->wait_for_in0 = false;
		}
	}

	////

	fqlupdate* update = proc->proc_data;
	if (!proc->wait_for_in0) {
		writer_close(update->writer);
		process_disable(proc);
		return 0;
	}

	int ret = 0;
	node** rg_iter = fifo_begin(in_false);
	for (; rg_iter != fifo_end(in_false) && update->rows_affected < update->top_count;
	     rg_iter = fifo_iter(in_false)) {
		record* rec = record_at(*rg_iter, update->table_idx);
		if (rec->rec_idx < update->update_idx) {
			break;
		}
		ret = try_(update->noupdate__(update, *rg_iter));
		++update->update_idx;
		_recycle(proc, rg_iter);
	}
	fifo_update(in_false);

	rg_iter = fifo_begin(in_true);
	for (; rg_iter != fifo_end(in_true) && update->rows_affected < update->top_count;
	     rg_iter = fifo_iter(in_true)) {
		record* rec = record_at(*rg_iter, update->table_idx);
		if (rec->rec_idx < update->update_idx) {
			break;
		}
		ret = try_(update->update__(update, *rg_iter));
		++update->update_idx;
		++proc->rows_affected;
		_recycle(proc, rg_iter);
	}
	fifo_update(in_true);

	if (update->rows_affected >= update->top_count) {
		proc->wait_for_in0 = false;
	}
	return ret;
}

void _update_passthrough(process* proc, fqlupdate* update, fifo* in_false, fifo* in_true)
{
	node** it = fifo_begin(in_false);
	bool false_done = (it == fifo_end(in_false));
	for (; it != fifo_end(in_false); it = fifo_iter(in_false)) {
		record* rec = record_at(*it, update->table_idx);
		if (rec->rec_idx > update->update_idx) {
			false_done = true;
			break;
		}
		_recycle(proc, it);
	}
	fifo_update(in_false);

	it = fifo_begin(in_true);
	if (it == fifo_end(in_true)) {
		update->state = FILTER_OPEN;
	}
	for (; it != fifo_end(in_true); it = fifo_iter(in_true)) {
		record* rec = record_at(*it, update->table_idx);
		if (rec->rec_idx > update->update_idx) {
			if (false_done) {
				update->state = FILTER_OPEN;
			}
			break;
		}
		_recycle(proc, it);
	}
	fifo_update(in_false);
}

int _update_filtering(process* proc, fqlupdate* update, fifo* in_false, fifo* in_true)
{
	if (fifo_available(in_true)) {
		node** true_node = fifo_peek(in_true);
		record* rec = record_at(*true_node, update->table_idx);
		if (rec->rec_idx > update->update_idx) {
			node** false_node = fifo_get(in_false);
			try_(update->noupdate__(update, *false_node));
			_recycle(proc, false_node);
		} else {
			fifo_consume(in_true);
			try_(update->update__(update, *true_node));
			++proc->rows_affected;
			_recycle(proc, true_node);
		}
		update->state = FILTER_PASSTHROUGH;
		_update_passthrough(proc, update, in_false, in_true);
		return FQL_GOOD;
	}

	if (!in_true->is_open) {
		node** false_node = fifo_get(in_false);
		try_(update->noupdate__(update, *false_node));
		_recycle(proc, false_node);
		update->state = FILTER_PASSTHROUGH;
		_update_passthrough(proc, update, in_false, in_true);
		return FQL_GOOD;
	}

	node** iter_false = fifo_begin(in_false);
	for (; iter_false != fifo_end(in_false); iter_false = fifo_iter(in_false)) {
		node** look_ahead = fifo_look_ahead(in_false);
		if (look_ahead == NULL) {
			return FQL_GOOD;
		}
		record* look_ahead_rec = record_at(*look_ahead, update->table_idx);
		if (look_ahead_rec->rec_idx > update->update_idx) {
			try_(update->noupdate__(update, *iter_false));
			update->state = FILTER_OPEN;
		}
		_recycle(proc, iter_false);
	}
	return FQL_GOOD;
}

int _update_open(process* proc, fqlupdate* update, fifo* in_false, fifo* in_true)
{
	bool process_true = false;

	node** true_node = NULL;
	record* true_rec = NULL;
	node** false_node = NULL;
	record* false_rec = NULL;

	if (fifo_available(in_false)) {
		false_node = fifo_peek(in_false);
		false_rec = record_at(*false_node, update->table_idx);
	}

	/* If not stable, this can be skipped */
	if (fifo_available(in_true)) {
		true_node = fifo_peek(in_true);
		true_rec = record_at(*true_node, update->table_idx);
		if (false_node == NULL || true_rec->rec_idx <= false_rec->rec_idx) {
			process_true = true;
		}
	}
	if (process_true) {
		try_(update->update__(update, *true_node));
		fifo_consume(in_true);
		_recycle(proc, true_node);
		++proc->rows_affected;
		update->update_idx = true_rec->rec_idx;
		update->state = FILTER_PASSTHROUGH;
		return FQL_GOOD;
	}
	update->update_idx = false_rec->rec_idx;
	update->state = FILTER_FILTERING;
	return FQL_GOOD;
}

int fql_update_filter(process* proc)
{
	fifo* in_false = proc->fifo_in[0];
	fifo* in_true = proc->fifo_in[1];
	if (fifo_is_empty(in_false) && fifo_is_empty(in_true)) {
		if (in_true->is_open || in_false->is_open) {
			return 1;
		} else {
			proc->wait_for_in0 = false;
		}
	}

	////

	fqlupdate* update = proc->proc_data;
	if (!proc->wait_for_in0) {
		writer_close(update->writer);
		process_disable(proc);
		return 0;
	}

	switch (update->state) {
	case FILTER_PASSTHROUGH:
		_update_passthrough(proc, update, in_false, in_true);
		break;
	case FILTER_FILTERING:
		try_(_update_filtering(proc, update, in_false, in_true));
		break;
	case FILTER_OPEN:
		try_(_update_open(proc, update, in_false, in_true));
		break;
	default:;
	}
	return FQL_GOOD;
}

int fql_orderby(process* proc)
{
	fifo* in = proc->fifo_in[0];
	////
	order* order = proc->proc_data;
	if (!in->is_open && fifo_is_empty(in)) {
		if (!order->sorted) {
			try_(order_sort(order));
		}
		int ret = try_(order->select__(order, proc));
		if (ret == 0 || proc->rows_affected >= order->top_count) {
			process_disable(proc);
		}
		return ret;
	}

	unsigned iters = 0;
	node** rg_iter = fifo_begin(in);
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
	node** rg = fifo_get(proc->fifo_in[0]);
	fifo_add(proc->fifo_out[0], rg);
	return 0;
}
