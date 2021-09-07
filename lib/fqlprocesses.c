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

void _iter_states(fifo* f, string* msg)
{
	unsigned len = (msg->size > 25) ? 25 : msg->size;
	fprintf(stderr,
	        "%.*s: [ %u %u (%u) ]\n",
	        len,
	        string_c_str(msg),
	        f->tail,
	        f->head,
	        f->_iter_head);
}

enum proc_return fql_read(process* proc)
{
	fifo* out = proc->fifo_out[0];
	if (!out->is_open) {
		process_disable(proc);
		return PROC_RETURN_COMPLETE;
	}

	fifo* in = proc->fifo_in[0];
	if (fifo_is_empty(in)) {
		if (!in->is_open) {
			process_disable(proc);
		}
		return PROC_RETURN_COMPLETE;
	}

	if (!fifo_receivable(out)) {
		return PROC_RETURN_WAIT_ON_OUT0;
	}

	table* table = proc->proc_data;
	reader* reader = table->reader;

	////

	enum proc_return ret =
	        (in == proc->root_ref) ? PROC_RETURN_RUNNING : PROC_RETURN_WAIT_ON_IN0;

	node** rg_iter = fifo_begin(in);
	while (rg_iter != fifo_end(in)) {
		switch (reader->get_record__(reader, *rg_iter)) {
		case FQL_GOOD:
			break;
		case FQL_FAIL:
			return PROC_RETURN_FAIL;
		default: /* eof */
			process_disable(proc);
			return PROC_RETURN_COMPLETE;
		}

		record* rec = (*rg_iter)->data;
		rec->src_idx = table->idx;


		fifo_add(out, rg_iter);

		//_iter_states(in, proc->plan_msg);
		rg_iter = fifo_iter(in);
		if (!fifo_receivable(out)) {
			ret = PROC_RETURN_WAIT_ON_OUT0;
			break;
		}
	}
	fifo_update(in);

	return ret;
}

enum proc_return fql_cartesian_join(process* proc)
{
	fifo* out = proc->fifo_out[0];
	if (!out->is_open) {
		process_disable(proc);
		return PROC_RETURN_COMPLETE;
	}

	fifo* in_left = proc->fifo_in[0];
	fifo* in_right = proc->fifo_in[1];

	if (fifo_is_empty(in_left)) {
		if (!in_left->is_open) {
			process_disable(proc);
		}
		return PROC_RETURN_WAIT_ON_IN0;
	}

	table* table = proc->proc_data;
	reader* reader = table->reader;

	////

	enum proc_return ret = PROC_RETURN_RUNNING;

	node** iter_left = fifo_begin(in_left);
	unsigned out_space = fifo_receivable(out);

	if (proc->inbuf_iter == vec_end(proc->inbuf)) {
		vec_clear(proc->inbuf);
		if (fifo_nget(in_right, proc->inbuf, proc->out_src_count, out_space)
		    < proc->out_src_count) {
			ret = PROC_RETURN_WAIT_ON_IN1;
		}
		proc->inbuf_iter = vec_begin(proc->inbuf);
	}

	for (; proc->inbuf_iter != vec_end(proc->inbuf); ++proc->inbuf_iter) {
		record* rec = (*proc->inbuf_iter)->data;
		rec->src_idx = table->idx;
		int ret = reader->get_record__(reader, *proc->inbuf_iter);

		switch (ret) {
		case FQL_GOOD:
			break;
		case FQL_FAIL:
			return PROC_RETURN_FAIL;
		default: { /* eof */
			reader->reset__(reader);
			//_iter_states(in_left, proc->plan_msg);
			iter_left = fifo_iter(in_left);
			if (iter_left == fifo_end(in_left)) {
				return PROC_RETURN_WAIT_ON_IN0;
			}
			return PROC_RETURN_RUNNING;
		}
		}

		node* left_rec_node = node_back(*iter_left);
		node** right_head = proc->inbuf_iter;

		for (; left_rec_node; left_rec_node = left_rec_node->prev) {
			++proc->inbuf_iter;
			record* left_rec = left_rec_node->data;
			record* fresh_rec = (*proc->inbuf_iter)->data;
			record_copy(fresh_rec, left_rec);
			node_push_import(right_head, *proc->inbuf_iter);
		}
		fifo_add(out, right_head);
	}

	return ret;
}

enum proc_return _hash_join_right_side(process* proc, table* table, fifo* in_right)
{
	struct hashjoin* hj = table->join_data;

	long int_hold = 0;
	double float_hold = 0;
	stringview sv;

	node** rg_iter = fifo_begin(in_right);
	for (; rg_iter != fifo_end(in_right); rg_iter = fifo_iter(in_right)) {
		switch (hj->comp_type) {
		case FIELD_INT:
			try_(expression_get_int(&int_hold, hj->right_expr, *rg_iter));
			stringview_nset(&sv, &int_hold, sizeof(int_hold));
			break;
		case FIELD_FLOAT:
			try_(expression_get_float(&float_hold, hj->right_expr, *rg_iter));
			stringview_nset(&sv, &float_hold, sizeof(float_hold));
			break;
		case FIELD_STRING:
			try_(expression_get_stringview(&sv, hj->right_expr, *rg_iter));
			break;
		default:
			return PROC_RETURN_FAIL;
		}

		record* rec = (*rg_iter)->data;
		multimap_nset(hj->hash_data, sv.data, &rec->offset, sv.len);

		_recycle(proc, rg_iter);
		//_iter_states(in_right, proc->plan_msg);
	}
	fifo_update(in_right);

	return PROC_RETURN_WAIT_ON_IN1;
}

long _hash_join_left_side(process* proc, table* table, node* left_rg)
{
	struct hashjoin* hj = table->join_data;

	if (hj->recs == NULL) {
		long int_hold = 0;
		double float_hold = 0;
		stringview sv;
		switch (hj->comp_type) {
		case FIELD_INT:
			try_(expression_get_int(&int_hold, hj->left_expr, left_rg));
			stringview_nset(&sv, &int_hold, sizeof(int_hold));
			break;
		case FIELD_FLOAT:
			try_(expression_get_float(&float_hold, hj->left_expr, left_rg));
			stringview_nset(&sv, &float_hold, sizeof(float_hold));
			break;
		case FIELD_STRING:
			try_(expression_get_stringview(&sv, hj->left_expr, left_rg));
			break;
		default:
			return FQL_FAIL;
		}

		hj->recs = multimap_nget(hj->hash_data, sv.data, sv.len);
		if (hj->recs == NULL) {
			return -1;
		}
		hj->rec_idx = 0;
	}

	size_t* offset = vec_at(hj->recs, hj->rec_idx++);
	if (hj->rec_idx >= hj->recs->size) {
		hj->recs = NULL;
	}

	return *offset;
}

enum proc_return fql_hash_join(process* proc)
{
	fifo* out_false = proc->fifo_out[0];
	fifo* out_true = proc->fifo_out[1];
	if (!out_true->is_open || (out_false && !out_false->is_open)) {
		process_disable(proc);
		return PROC_RETURN_COMPLETE;
	}

	fifo* in_left = proc->fifo_in[0];
	fifo* in_right = proc->fifo_in[1];

	table* table = proc->proc_data;
	struct hashjoin* hj = table->join_data;

	////

	if (hj->state == SIDE_RIGHT) {
		if (fifo_is_empty(in_right)) {
			if (!in_right->is_open) {
				hj->state = SIDE_LEFT;
				table->reader->reset__(table->reader);
				return PROC_RETURN_RUNNING;
			}
			return PROC_RETURN_WAIT_ON_IN1;
		}
		return _hash_join_right_side(proc, table, in_right);
	}

	//// We can assume SIDE_LEFT if we made it this far
	if (!fifo_receivable(out_true)) {
		return PROC_RETURN_WAIT_ON_OUT1;
	}
	if (out_false && !fifo_receivable(out_false)) {
		return PROC_RETURN_WAIT_ON_OUT0;
	}

	if (fifo_is_empty(in_left)) {    // && fifo_is_empty(in_right)) {
		if (!in_left->is_open) { // && !in_right->is_open) {
			process_disable(proc);
		}
		return PROC_RETURN_WAIT_ON_IN0;
	}

	enum proc_return ret = PROC_RETURN_WAIT_ON_IN0;

	node** left_rg_iter = fifo_begin(in_left);
	while (left_rg_iter != fifo_end(in_left)) {
		long offset = try_(_hash_join_left_side(proc, table, *left_rg_iter));
		if (offset == -1) {
			if (table->join_type == JOIN_LEFT) {
				fifo_add(out_true, left_rg_iter);
			} else if (out_false != NULL) {
				fifo_add(out_false, left_rg_iter);
			} else {
				_recycle(proc, left_rg_iter);
			}
			//_iter_states(in_left, proc->plan_msg);
			left_rg_iter = fifo_iter(in_left);
			if (!fifo_receivable(out_true)) {
				ret = PROC_RETURN_WAIT_ON_OUT1;
				break;
			}
			if (out_false && !fifo_receivable(out_false)) {
				ret = PROC_RETURN_WAIT_ON_OUT0;
				break;
			}
			continue;
		}
		node** right_rg = fifo_get(proc->root_ref);
		record* right_rec = (*right_rg)->data;
		right_rec->src_idx = proc->out_src_count - 1;
		reader* reader = table->reader;
		try_(reader->get_record_at__(reader, *right_rg, offset));

		/* If this is the last record match to the left
		 * side, send the left side. Otherwise, insert
		 * references to the left side to the right and
		 * send the right side.
		 */
		if (hj->recs == NULL) {
			node_enqueue_import(left_rg_iter, *right_rg);
			fifo_add(out_true, left_rg_iter);
			//_iter_states(in_left, proc->plan_msg);
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

		if (!fifo_receivable(out_true)) {
			ret = PROC_RETURN_WAIT_ON_OUT1;
			break;
		}
		if (out_false && !fifo_receivable(out_false)) {
			ret = PROC_RETURN_WAIT_ON_OUT0;
			break;
		}
	}
	fifo_update(in_left);

	return ret;
}

enum proc_return fql_logic(process* proc)
{
	fifo* out_false = proc->fifo_out[0];
	fifo* out_true = proc->fifo_out[1];
	if ((out_true && !out_true->is_open) || (out_false && !out_false->is_open)) {
		process_disable(proc);
		return PROC_RETURN_COMPLETE;
	}

	fifo* in = proc->fifo_in[0];
	if (fifo_is_empty(in)) {
		if (!in->is_open) {
			process_disable(proc);
		}
		return PROC_RETURN_WAIT_ON_IN0;
	}

	if (out_true && !fifo_receivable(out_true)) {
		return PROC_RETURN_WAIT_ON_OUT1;
	}
	if (out_false && !fifo_receivable(out_false)) {
		return PROC_RETURN_WAIT_ON_OUT0;
	}

	logicgroup* lg = proc->proc_data;

	////

	enum proc_return ret = PROC_RETURN_WAIT_ON_IN0;

	node** rg_iter = fifo_begin(in);
	while (rg_iter != fifo_end(in)) {
		int result = try_(logicgroup_eval(lg, *rg_iter, lg->join_logic));

		if (result && out_true != NULL) {
			fifo_add(out_true, rg_iter);
		} else if (!result && out_false != NULL) {
			fifo_add(out_false, rg_iter);
		} else {
			_recycle(proc, rg_iter);
		}

		if (proc->is_const) {
			process_disable(proc);
			break;
		}

		//_iter_states(in, proc->plan_msg);
		rg_iter = fifo_iter(in);

		if (out_true && !fifo_receivable(out_true)) {
			ret = PROC_RETURN_WAIT_ON_OUT1;
			break;
		}
		if (out_false && !fifo_receivable(out_false)) {
			ret = PROC_RETURN_WAIT_ON_OUT0;
			break;
		}
	}
	fifo_update(in);

	return ret;
}

enum proc_return fql_left_join_logic(process* proc)
{
	// No out_false for left_join
	//fifo* out_false = proc->fifo_out[0];
	fifo* out_true = proc->fifo_out[1];
	if (out_true && !out_true->is_open) {
		process_disable(proc);
		return PROC_RETURN_COMPLETE;
	}

	fifo* in = proc->fifo_in[0];
	if (fifo_is_empty(in)) {
		if (!in->is_open) {
			process_disable(proc);
		}
		return PROC_RETURN_WAIT_ON_IN0;
	}

	if (!out_true && !fifo_receivable(out_true)) {
		return PROC_RETURN_WAIT_ON_OUT1;
	}

	logicgroup* lg = proc->proc_data;

	////

	enum proc_return ret = PROC_RETURN_WAIT_ON_IN0;

	node** rg_iter = fifo_begin(in);
	while (rg_iter != fifo_end(in)) {
		/* If the right-most expected node is missing,
		 * these records have already failed a hash join.
		 */
		if (record_at(*rg_iter, proc->out_src_count - 1) != NULL) {
			if (!try_(logicgroup_eval(lg, *rg_iter, lg->join_logic))) {
				/* our condition failed, but we still want to pass 
				 * it on. Chop off right-most node and recycle it.
				 */
				node* right_side = node_back(*rg_iter);
				node_export(rg_iter, right_side);
				_recycle(proc, &right_side);
			}
		}

		fifo_add(out_true, rg_iter);


		//_iter_states(in, proc->plan_msg);
		rg_iter = fifo_iter(in);
		if (!out_true && !fifo_receivable(out_true)) {
			ret = PROC_RETURN_WAIT_ON_OUT1;
			break;
		}
	}
	fifo_update(in);

	return ret;
}

enum proc_return _group_dump(process* proc, fifo* in_fresh, fifo* out)
{
	if (!fifo_receivable(out)) {
		return PROC_RETURN_WAIT_ON_OUT0;
	}

	group* group = proc->proc_data;

	enum proc_return ret = PROC_RETURN_WAIT_ON_IN0;

	node** rg_iter = fifo_begin(in_fresh);
	while (rg_iter != fifo_end(in_fresh)) {
		record* rec = (*rg_iter)->data;
		rec->src_idx = 0;

		switch (group_dump_record(group, rec)) {
		case FQL_FAIL:
			return PROC_RETURN_FAIL;
		case FQL_GOOD:
			fifo_add(out, rg_iter);
			break;
		default:
			process_disable(proc);
			return PROC_RETURN_COMPLETE;
		}

		//_iter_states(in_fresh, proc->plan_msg);
		rg_iter = fifo_iter(in_fresh);
		if (!fifo_receivable(out)) {
			ret = PROC_RETURN_WAIT_ON_OUT0;
			break;
		}
	}
	fifo_update(in_fresh);

	return ret;
}

enum proc_return fql_groupby(process* proc)
{
	fifo* out = proc->fifo_out[0];

	if (!out->is_open) {
		process_disable(proc);
		return PROC_RETURN_COMPLETE;
	}

	fifo* in_data = proc->fifo_in[0];
	fifo* in_fresh = proc->fifo_in[1];

	group* group = proc->proc_data;

	////

	if (!in_data->is_open && fifo_is_empty(in_data)) {
		return _group_dump(proc, in_fresh, out);
	}

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
		//_iter_states(in_data, proc->plan_msg);
	}
	fifo_update(in_data);
	return PROC_RETURN_WAIT_ON_IN0;
}

enum proc_return fql_distinct(process* proc)
{
	fifo* out = proc->fifo_out[0];
	if (!out->is_open) {
		process_disable(proc);
		return PROC_RETURN_COMPLETE;
	}

	fifo* in = proc->fifo_in[0];
	if (fifo_is_empty(in)) {
		// hmmm... this may be impossible...
		if (proc->wait_for_in0 && !in->is_open) {
			process_disable(proc);
		}
		return PROC_RETURN_WAIT_ON_IN0;
	}

	if (!fifo_receivable(out)) {
		return PROC_RETURN_WAIT_ON_OUT0;
	}

	group* group = proc->proc_data;

	////

	enum proc_return ret = PROC_RETURN_WAIT_ON_IN0;

	node** rg_iter = fifo_begin(in);
	while (rg_iter != fifo_end(in)) {
		switch (group_record(group, *rg_iter)) {
		case FQL_FAIL:
			return PROC_RETURN_FAIL;
		case 1:
			fifo_add(proc->fifo_out[0], rg_iter);
			break;
		case 0:
		default:
			_recycle(proc, rg_iter);
		}

		//_iter_states(in, proc->plan_msg);
		rg_iter = fifo_iter(in);
		if (!fifo_receivable(out)) {
			ret = PROC_RETURN_WAIT_ON_OUT0;
			break;
		}
	}
	fifo_update(in);
	return ret;
}

enum proc_return fql_select(process* proc)
{
	fqlselect* select = proc->proc_data;

	/* Check if the process in front is closed */
	fifo* out = proc->fifo_out[0];
	if (out && !out->is_open) {
		dnode** it = vec_begin(proc->union_end_nodes);
		for (; it != vec_end(proc->union_end_nodes); ++it) {
			process* union_end_proc = (*it)->data;
			union_end_proc->fifo_out[0]->is_open = false;
		}
		process_disable(proc);
		return PROC_RETURN_COMPLETE;
	}

	fifo* in = proc->fifo_in[0];
	if (fifo_is_empty(in)) {
		if (proc->wait_for_in0 && in->is_open) {
			return PROC_RETURN_WAIT_ON_IN0;
		}
		proc->wait_for_in0 = false;
	}

	if (out && !fifo_receivable(out)) {
		return PROC_RETURN_WAIT_ON_OUT0;
	}

	////

	if (!proc->wait_for_in0) {
		if (select->must_run_once && select->rows_affected == 0) {
			try_(select->select__(select, NULL));
			++select->rows_affected;
			++proc->rows_affected;
			return PROC_RETURN_RUNNING;
		}
		/* subquery reads expect union schema to
		 * be "in sync" with the subquery select's
		 * current schema
		 */
		if (out && !fifo_is_empty(out)) {
			return PROC_RETURN_RUNNING;
		}

		if (fqlselect_next_union(select)) {
			proc->wait_for_in0 = true;
			proc->fifo_in[0] = proc->queued_results->data;
			if (proc->queued_results->next != NULL) {
				proc->queued_results = proc->queued_results->next;
			}
			return PROC_RETURN_RUNNING;
		}
		try_(writer_close(select->writer));
		process_disable(proc);
		return PROC_RETURN_COMPLETE;
	}

	enum proc_return ret = PROC_RETURN_WAIT_ON_IN0;

	unsigned iters = 0;
	node** rg_iter = fifo_begin(in);
	while (rg_iter != fifo_end(in)) {
		if (iters++ >= proc->max_recs_iter
		    || select->rows_affected >= select->top_count) {
			ret = PROC_RETURN_RUNNING;
			break;
		}

		//ret = try_(select->select__(select, *rg_iter));
		try_(select->select__(select, *rg_iter));

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

		//_iter_states(in, proc->plan_msg);
		rg_iter = fifo_iter(in);

		if (out && !fifo_receivable(out)) {
			ret = PROC_RETURN_WAIT_ON_OUT0;
			break;
		}
	}
	fifo_update(in);

	if (select->rows_affected >= select->top_count) {
		proc->wait_for_in0 = false;
	}
	return ret;
}

enum proc_return fql_delete(process* proc)
{
	fifo* in = proc->fifo_in[0];

	if (proc->wait_for_in0 && fifo_is_empty(in)) {
		if (in->is_open) {
			return PROC_RETURN_WAIT_ON_IN0;
		}
		proc->wait_for_in0 = false;
	}

	fqldelete* delete = proc->proc_data;

	////

	if (!proc->wait_for_in0) {
		try_(writer_close(delete->writer));
		process_disable(proc);
		return PROC_RETURN_COMPLETE;
	}

	enum proc_return ret = PROC_RETURN_WAIT_ON_IN0;

	node** rg_iter = fifo_begin(in);
	while (rg_iter != fifo_end(in)) {
		//ret = try_(delete->undelete__(delete, *rg_iter));
		try_(delete->undelete__(delete, *rg_iter));
		++proc->rows_affected;
		_recycle(proc, rg_iter);

		//_iter_states(in, proc->plan_msg);
		rg_iter = fifo_iter(in);

		if (delete->rows_affected >= delete->top_count) {
			proc->wait_for_in0 = false;
			ret = PROC_RETURN_RUNNING;
			break;
		}
	}
	fifo_update(in);

	return ret;
}

enum proc_return
_del_passthrough(process* proc, fqldelete* delete, fifo* in_false, fifo* in_true)
{
	enum proc_return ret = PROC_RETURN_WAIT_ON_IN_EITHER;

	node** it = fifo_begin(in_false);
	bool false_done = (it == fifo_end(in_false));
	for (; it != fifo_end(in_false); it = fifo_iter(in_false)) {
		record* rec = record_at(*it, delete->table_idx);
		if (rec->rec_idx > delete->delete_idx) {
			false_done = true;
			break;
		}
		_recycle(proc, it);
		//_iter_states(in_false, proc->plan_msg);
	}
	fifo_update(in_false);

	it = fifo_begin(in_true);
	if (it == fifo_end(in_true)) {
		delete->state = FILTER_OPEN;
		return PROC_RETURN_RUNNING;
	}

	for (; it != fifo_end(in_true); it = fifo_iter(in_true)) {
		record* rec = record_at(*it, delete->table_idx);
		if (rec->rec_idx > delete->delete_idx) {
			if (false_done) {
				delete->state = FILTER_OPEN;
				ret = PROC_RETURN_RUNNING;
			}
			break;
		}
		_recycle(proc, it);
		//_iter_states(in_true, proc->plan_msg);
	}
	fifo_update(in_true);

	return ret;
}

enum proc_return
_del_filtering(process* proc, fqldelete* delete, fifo* in_false, fifo* in_true)
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
		return PROC_RETURN_RUNNING;
	}

	if (!in_true->is_open) {
		node** false_node = fifo_get(in_false);
		try_(delete->undelete__(delete, *false_node));
		_recycle(proc, false_node);
		delete->state = FILTER_PASSTHROUGH;
		_del_passthrough(proc, delete, in_false, in_true);
		return PROC_RETURN_RUNNING;
	}

	node** iter_false = fifo_begin(in_false);
	for (; iter_false != fifo_end(in_false); iter_false = fifo_iter(in_false)) {
		node** look_ahead = fifo_look_ahead(in_false);
		if (look_ahead == NULL) {
			return PROC_RETURN_RUNNING;
		}
		record* look_ahead_rec = record_at(*look_ahead, delete->table_idx);
		if (look_ahead_rec->rec_idx > delete->delete_idx) {
			try_(delete->undelete__(delete, *iter_false));
			delete->state = FILTER_OPEN;
		}
		_recycle(proc, iter_false);
		//_iter_states(in_false, proc->plan_msg);
	}
	return PROC_RETURN_RUNNING;
}

enum proc_return
_del_open(process* proc, fqldelete* delete, fifo* in_false, fifo* in_true)
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
		return PROC_RETURN_RUNNING;
	}
	delete->delete_idx = false_rec->rec_idx;
	delete->state = FILTER_FILTERING;
	return PROC_RETURN_RUNNING;
}

enum proc_return fql_delete_filter(process* proc)
{
	fifo* in_false = proc->fifo_in[0];
	fifo* in_true = proc->fifo_in[1];

	if (fifo_is_empty(in_false) && fifo_is_empty(in_true)) {
		if (in_true->is_open || in_false->is_open) {
			return PROC_RETURN_WAIT_ON_IN_EITHER;
		} else {
			proc->wait_for_in0 = false;
		}
	}

	fqldelete* delete = proc->proc_data;

	////

	if (!proc->wait_for_in0) {
		try_(writer_close(delete->writer));
		process_disable(proc);
		return PROC_RETURN_COMPLETE;
	}

	switch (delete->state) {
	case FILTER_PASSTHROUGH:
		return _del_passthrough(proc, delete, in_false, in_true);
	case FILTER_FILTERING:
		return _del_filtering(proc, delete, in_false, in_true);
	case FILTER_OPEN:
	default:
		return _del_open(proc, delete, in_false, in_true);
	}
}

enum proc_return fql_update(process* proc)
{

	fifo* in_false = proc->fifo_in[0];
	fifo* in_true = proc->fifo_in[1];
	if (fifo_is_empty(in_false) && fifo_is_empty(in_true)) {
		if (in_true->is_open || in_false->is_open) {
			return PROC_RETURN_WAIT_ON_IN_EITHER;
		} else {
			proc->wait_for_in0 = false;
		}
	}

	fqlupdate* update = proc->proc_data;

	////

	if (!proc->wait_for_in0) {
		try_(writer_close(update->writer));
		process_disable(proc);
		return PROC_RETURN_COMPLETE;
	}

	enum proc_return ret = PROC_RETURN_WAIT_ON_IN_EITHER;

	node** rg_iter = fifo_begin(in_false);
	for (; rg_iter != fifo_end(in_false); rg_iter = fifo_iter(in_false)) {
		record* rec = record_at(*rg_iter, update->table_idx);
		if (rec->rec_idx > update->update_idx) {
			break;
		}
		try_(update->noupdate__(update, *rg_iter));
		++update->update_idx;
		_recycle(proc, rg_iter);
		//_iter_states(in_false, proc->plan_msg);
	}
	fifo_update(in_false);

	rg_iter = fifo_begin(in_true);
	for (; rg_iter != fifo_end(in_true); rg_iter = fifo_iter(in_true)) {
		record* rec = record_at(*rg_iter, update->table_idx);
		if (rec->rec_idx > update->update_idx) {
			break;
		}
		try_(update->update__(update, *rg_iter));
		++update->update_idx;
		++proc->rows_affected;
		_recycle(proc, rg_iter);

		if (update->rows_affected >= update->top_count) {
			proc->wait_for_in0 = false;
			ret = PROC_RETURN_RUNNING;
			break;
		}
		//_iter_states(in_true, proc->plan_msg);
	}
	fifo_update(in_true);

	return ret;
}

enum proc_return
_update_passthrough(process* proc, fqlupdate* update, fifo* in_false, fifo* in_true)
{
	enum proc_return ret = PROC_RETURN_WAIT_ON_IN_EITHER;

	node** it = fifo_begin(in_false);
	bool false_done = (it == fifo_end(in_false));
	for (; it != fifo_end(in_false); it = fifo_iter(in_false)) {
		record* rec = record_at(*it, update->table_idx);
		if (rec->rec_idx > update->update_idx) {
			false_done = true;
			break;
		}
		_recycle(proc, it);
		//_iter_states(in_false, proc->plan_msg);
	}
	fifo_update(in_false);

	it = fifo_begin(in_true);
	if (it == fifo_end(in_true)) {
		update->state = FILTER_OPEN;
		return PROC_RETURN_RUNNING;
	}

	for (; it != fifo_end(in_true); it = fifo_iter(in_true)) {
		record* rec = record_at(*it, update->table_idx);
		if (rec->rec_idx > update->update_idx) {
			if (false_done) {
				update->state = FILTER_OPEN;
				ret = PROC_RETURN_RUNNING;
			}
			break;
		}
		_recycle(proc, it);
		//_iter_states(in_true, proc->plan_msg);
	}
	fifo_update(in_true);

	return ret;
}

enum proc_return
_update_filtering(process* proc, fqlupdate* update, fifo* in_false, fifo* in_true)
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
		return PROC_RETURN_RUNNING;
	}

	if (!in_true->is_open) {
		node** false_node = fifo_get(in_false);
		try_(update->noupdate__(update, *false_node));
		_recycle(proc, false_node);
		update->state = FILTER_PASSTHROUGH;
		_update_passthrough(proc, update, in_false, in_true);
		return PROC_RETURN_RUNNING;
	}

	node** iter_false = fifo_begin(in_false);
	for (; iter_false != fifo_end(in_false); iter_false = fifo_iter(in_false)) {
		node** look_ahead = fifo_look_ahead(in_false);
		if (look_ahead == NULL) {
			return PROC_RETURN_RUNNING;
		}
		record* look_ahead_rec = record_at(*look_ahead, update->table_idx);
		if (look_ahead_rec->rec_idx > update->update_idx) {
			try_(update->noupdate__(update, *iter_false));
			update->state = FILTER_OPEN;
		}
		_recycle(proc, iter_false);
		//_iter_states(in_false, proc->plan_msg);
	}
	return PROC_RETURN_RUNNING;
}

enum proc_return
_update_open(process* proc, fqlupdate* update, fifo* in_false, fifo* in_true)
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
		return PROC_RETURN_RUNNING;
	}
	update->update_idx = false_rec->rec_idx;
	update->state = FILTER_FILTERING;
	return PROC_RETURN_RUNNING;
}

enum proc_return fql_update_filter(process* proc)
{
	fifo* in_false = proc->fifo_in[0];
	fifo* in_true = proc->fifo_in[1];
	if (fifo_is_empty(in_false) && fifo_is_empty(in_true)) {
		if (in_true->is_open || in_false->is_open) {
			return PROC_RETURN_WAIT_ON_IN_EITHER;
		} else {
			proc->wait_for_in0 = false;
		}
	}

	fqlupdate* update = proc->proc_data;

	////

	if (!proc->wait_for_in0) {
		try_(writer_close(update->writer));
		process_disable(proc);
		return PROC_RETURN_COMPLETE;
	}

	switch (update->state) {
	case FILTER_PASSTHROUGH:
		return _update_passthrough(proc, update, in_false, in_true);
	case FILTER_FILTERING:
		return _update_filtering(proc, update, in_false, in_true);
	case FILTER_OPEN:
	default:
		return _update_open(proc, update, in_false, in_true);
	}
}

enum proc_return fql_orderby(process* proc)
{
	fifo* in = proc->fifo_in[0];
	order* order = proc->proc_data;

	////

	enum proc_return ret = PROC_RETURN_WAIT_ON_IN0;

	if (!in->is_open && fifo_is_empty(in)) {
		if (!order->sorted) {
			try_(order_sort(order));
		}
		int ret = try_(order->select__(order, proc));
		if (ret == 0 || proc->rows_affected >= order->top_count) {
			process_disable(proc);
			return PROC_RETURN_COMPLETE;
		}
		return PROC_RETURN_RUNNING;
	}

	unsigned iters = 0;
	node** rg_iter = fifo_begin(in);
	for (; iters++ < proc->max_recs_iter && rg_iter != fifo_end(in);
	     rg_iter = fifo_iter(in)) {
		try_(order_add_record(order, *rg_iter));
		_recycle(proc, rg_iter);
		//_iter_states(in, proc->plan_msg);
	}
	fifo_update(in);
	return ret;
}

enum proc_return fql_no_op(process* proc)
{
	return PROC_RETURN_FAIL;
	//node** rg = fifo_get(proc->fifo_in[0]);
	//fifo_add(proc->fifo_out[0], rg);
	//return PROC_RETURN_COMPLETE;
}
