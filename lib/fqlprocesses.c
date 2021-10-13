#include "process.h"
#include "fql.h"
#include "misc.h"
#include "group.h"
#include "logic.h"
#include "order.h"
#include "fqlbranch.h"
#include "record.h"
#include "reader.h"
#include "aggregate.h"
#include "fqlselect.h"
#include "fqldelete.h"
#include "fqlupdate.h"
#include "fqlset.h"
#include "expression.h"
#include "util/fifo.h"

/* comment this line to print fifo states as it runs */
#define _fqlprocess_print_iter_states(X_)

void _fqlprocess_print_fifo_info(const char* fname, fifo* f, string* msg, bool is_root)
{
	unsigned len = (msg->size > 25) ? 25 : msg->size;
	if (is_root) {
		fprintf(stderr,
		        "%.*s: [%s] [ %u %u (%u) ]\n",
		        len,
		        string_c_str(msg),
		        fname,
		        f->tail,
		        f->head,
		        f->_iter_head);
		return;
	}

	fprintf(stderr,
	        "%.*s: %s [ %u %u (%u) ]\n",
	        len,
	        string_c_str(msg),
	        fname,
	        f->tail,
	        f->head,
	        f->_iter_head);
}

#ifndef _fqlprocess_print_iter_states
void _fqlprocess_print_iter_states(process* proc)
{
	if (proc->fifo_in[0] != NULL) {
		bool is_root = (proc->root_fifo == 0);
		_fqlprocess_print_fifo_info("I0",
		                            proc->fifo_in[0],
		                            proc->plan_msg,
		                            is_root);
	}
	if (proc->fifo_in[1] != NULL) {
		bool is_root = (proc->root_fifo == 1);
		_fqlprocess_print_fifo_info("I1",
		                            proc->fifo_in[1],
		                            proc->plan_msg,
		                            is_root);
	}
	if (proc->fifo_out[0] != NULL) {
		_fqlprocess_print_fifo_info("O0",
		                            proc->fifo_out[0],
		                            proc->plan_msg,
		                            false);
	}
	if (proc->fifo_out[1] != NULL) {
		_fqlprocess_print_fifo_info("O1",
		                            proc->fifo_out[1],
		                            proc->plan_msg,
		                            false);
	}
}
#endif

int _fill_inbuf(process* proc, fifo* input)
{
	if (proc->inbuf_iter != vec_end(proc->inbuf)) {
		return 1;
	}
	vec_clear(proc->inbuf);
	int ret =
	        fifo_nget(input, proc->inbuf, proc->out_src_count, proc->fifo_base_size);
	proc->inbuf_iter = vec_begin(proc->inbuf);
	return ret;
}

int _fqlprocess_add_or_buffer(fifo* out, node* rg, vec* buf, unsigned limit)
{
	if (vec_empty(buf)) {                 /* not yet buffering */
		if (fifo_add_try(out, &rg)) { /* no lock achieved  */
			vec_push_back(buf, &rg);
			return -1;
		}
		return 0;
	}
	vec_push_back(buf, &rg);
	if (buf->size >= limit) {
		fifo_nadd(out, buf);
		return 0;
	}
	return fifo_nadd_try(out, buf);
}

void fqlprocess_recycle(process* proc, node** rg)
{
	while (*rg) {
		node* rg_node = node_pop_export(rg);
		record* rec = rg_node->data;
		fifo** root_fifo = vec_at(proc->rootvec_ref, rec->root_fifo_idx);

		// 3 Optional implementations for this...

		/////////
		//_fqlprocess_add_or_buffer(*root_fifo,
		//                          rg_node,
		//                          proc->rebuf,
		//                          proc->fifo_base_size);
		/////////

		/////////
		//vec_push_back(proc->rebuf, &rg_node);
		//fifo_nadd(*root_fifo, proc->rebuf);
		/////////

		/////////
		fifo_add(*root_fifo, &rg_node);
		/////////
	}
}

void fqlprocess_recycle_buffer(process* proc, vec* buffer, node*** iter)
{
	for (; *iter != vec_end(buffer); ++(*iter)) {
		fqlprocess_recycle(proc, *iter);
	}
	vec_clear(buffer);
	*iter = vec_begin(buffer);
}

enum proc_return fql_set(process* proc)
{
	fqlset* declstmt = proc->proc_data;

	////

	variable* var = vec_at(proc->fql_ref->variables, declstmt->variable_idx);

	if (declstmt->init_expr == NULL) {
		variable_reset(var);
		return PROC_RETURN_COMPLETE;
	}

	var->is_null = false;
	int expr_ret = 0;

	switch (var->type) {
	case SQL_INT:
		expr_ret = try_(
		        expression_get_int(&var->value.i, declstmt->init_expr, NULL));
		break;
	case SQL_BIT:
		expr_ret = try_(
		        expression_get_int(&var->value.i, declstmt->init_expr, NULL));
		if (var->value.i) {
			var->value.i = 1;
		}
		break;
	case SQL_FLOAT:
		expr_ret = try_(
		        expression_get_float(&var->value.f, declstmt->init_expr, NULL));
		break;
	case SQL_TEXT:
	case SQL_CHAR:
	case SQL_VARCHAR: {
		stringview sv;
		expr_ret =
		        try_(expression_get_stringview(&sv, declstmt->init_expr, NULL));
		if (expr_ret != FQL_NULL) {
			variable_set_stringview(var, &sv);
		}
		break;
	}
	case SQL_UNDEFINED:
		return PROC_RETURN_FAIL;
	}

	if (expr_ret == FQL_NULL) {
		var->is_null = true;
	}

	return PROC_RETURN_COMPLETE;
}

enum proc_return fql_if(process* proc)
{
	fqlbranch* ifstmt = proc->proc_data;

	////

	logicgroup* condition = ifstmt->condition;
	int result = try_(logicgroup_eval(condition, NULL, NULL));
	*ifstmt->next_query_idx_ref = (result) ? ifstmt->true_idx : ifstmt->false_idx;

	return PROC_RETURN_COMPLETE;
}

enum proc_return fql_read(process* proc)
{
	fifo* out = proc->fifo_out[0];
	if (!out->is_open) {
		return PROC_RETURN_COMPLETE;
	}

	fifo* in = proc->fifo_in[0];
	if (fifo_is_empty(in) && (proc->root_fifo == 0 || vec_empty(proc->inbuf))) {
		if (!in->is_open) {
			return PROC_RETURN_COMPLETE;
		}
		return PROC_RETURN_WAIT_ON_IN0;
	}

	if (!fifo_receivable(out)) {
		return PROC_RETURN_WAIT_ON_OUT0;
	}

	table* table = proc->proc_data;
	reader* reader = table->reader;

	////

	enum proc_return ret =
	        (proc->root_fifo == 0) ? PROC_RETURN_RUNNING : PROC_RETURN_WAIT_ON_IN0;

	node** rg_iter = fifo_begin(in);
	while (rg_iter != fifo_end(in)) {
		switch (reader->get_record__(reader, *rg_iter)) {
		case FQL_GOOD:
			break;
		case FQL_FAIL:
			return PROC_RETURN_FAIL;
		default: /* eof */
			return PROC_RETURN_COMPLETE;
		}

		record* rec = (*rg_iter)->data;
		rec->src_idx = table->idx;


		fifo_add(out, rg_iter);

		_fqlprocess_print_iter_states(proc);
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
		return PROC_RETURN_COMPLETE;
	}

	fifo* in_left = proc->fifo_in[0];
	fifo* in_right = proc->fifo_in[1];

	if (fifo_is_empty(in_left)) {
		if (!in_left->is_open) {
			return PROC_RETURN_COMPLETE;
		}
		return PROC_RETURN_WAIT_ON_IN0;
	}

	table* table = proc->proc_data;
	reader* reader = table->reader;

	////

	_fill_inbuf(proc, in_right);

	//node** iter_left = fifo_begin(in_left);
	node** iter_left = fifo_peek(in_left);
	while (proc->inbuf_iter != vec_end(proc->inbuf)) {
		record* rec = (*proc->inbuf_iter)->data;
		rec->src_idx = table->idx;

		int ret = reader->get_record__(reader, *proc->inbuf_iter);

		switch (ret) {
		case FQL_FAIL:
			return PROC_RETURN_FAIL;
		case FQL_GOOD: {
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
			++proc->inbuf_iter;
			_fqlprocess_print_iter_states(proc);
			break;
		}
		default: { /* eof */
			reader->reset__(reader);
			_fqlprocess_print_iter_states(proc);
			fqlprocess_recycle(proc, iter_left);
			fifo_consume(in_left);
			iter_left = fifo_peek(in_left);
			//iter_left = fifo_iter(in_left);
			if (iter_left == fifo_end(in_left)) {
				return PROC_RETURN_WAIT_ON_IN0;
			}
			return PROC_RETURN_RUNNING;
		}
		}

		if (!fifo_receivable(out)) {
			return PROC_RETURN_WAIT_ON_OUT0;
		}
	}

	return PROC_RETURN_RUNNING;
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

		fqlprocess_recycle(proc, rg_iter);
		_fqlprocess_print_iter_states(proc);
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

	if (fifo_is_empty(in_left)) {
		if (!in_left->is_open) {
			return PROC_RETURN_COMPLETE;
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
				_fqlprocess_print_iter_states(proc);
			} else if (out_false != NULL) {
				fifo_add(out_false, left_rg_iter);
				_fqlprocess_print_iter_states(proc);
			} else {
				fqlprocess_recycle(proc, left_rg_iter);
			}
			_fqlprocess_print_iter_states(proc);
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
		node** right_rg = fifo_get(proc->global_root_ref);
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
			_fqlprocess_print_iter_states(proc);
			left_rg_iter = fifo_iter(in_left);
		} else {
			/* same process as cartesian record copy */
			node* left_rec_node = node_back(*left_rg_iter);

			for (; left_rec_node; left_rec_node = left_rec_node->prev) {
				record* left_rec = left_rec_node->data;
				node** fresh_node = fifo_get(proc->global_root_ref);
				record* fresh_rec = (*fresh_node)->data;
				record_copy(fresh_rec, left_rec);
				node_push_import(right_rg, *fresh_node);
			}
			fifo_add(out_true, right_rg);
			_fqlprocess_print_iter_states(proc);
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
		return PROC_RETURN_COMPLETE;
	}

	fifo* in = proc->fifo_in[0];
	if (fifo_is_empty(in)) {
		if (!in->is_open) {
			return PROC_RETURN_COMPLETE;
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
			fqlprocess_recycle(proc, rg_iter);
		}

		if (proc->is_const) {
			ret = PROC_RETURN_COMPLETE;
			break;
		}

		_fqlprocess_print_iter_states(proc);
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
		return PROC_RETURN_COMPLETE;
	}

	fifo* in = proc->fifo_in[0];
	if (fifo_is_empty(in)) {
		if (!in->is_open) {
			return PROC_RETURN_COMPLETE;
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
				//node_export(right_side);
				fqlprocess_recycle(proc, &right_side);
			}
		}

		fifo_add(out_true, rg_iter);
		_fqlprocess_print_iter_states(proc);


		_fqlprocess_print_iter_states(proc);
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
			_fqlprocess_print_iter_states(proc);
			break;
		default:
			return PROC_RETURN_COMPLETE;
		}

		_fqlprocess_print_iter_states(proc);
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
		return PROC_RETURN_COMPLETE;
	}

	fifo* in_data = proc->fifo_in[0];
	fifo* in_fresh = proc->fifo_in[1];

	group* group = proc->proc_data;

	////

	if (!in_data->is_open && fifo_is_empty(in_data)) {
		return _group_dump(proc, in_fresh, out);
	}

	enum proc_return ret = PROC_RETURN_WAIT_ON_IN0;

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
			fqlprocess_recycle(proc, rg_iter);
		} else {
			node** rg = fifo_get(in_fresh);
			record* rec = (*rg)->data;
			rec->src_idx = 0;
			try_(group_dump_record(group, rec));
			fifo_add(out, rg);
			ret = PROC_RETURN_COMPLETE;
			break;
		}
		_fqlprocess_print_iter_states(proc);
	}
	fifo_update(in_data);
	return ret;
}

enum proc_return fql_distinct(process* proc)
{
	fifo* out = proc->fifo_out[0];
	if (!out->is_open) {
		return PROC_RETURN_COMPLETE;
	}

	fifo* in = proc->fifo_in[0];
	if (fifo_is_empty(in)) {
		// hmmm... this may be impossible...
		if (proc->wait_for_in0 && !in->is_open) {
			return PROC_RETURN_COMPLETE;
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
			fqlprocess_recycle(proc, rg_iter);
		}

		_fqlprocess_print_iter_states(proc);
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
	fqlselect* current_select = select->current_select;

	/* Check if the process in front is closed */
	fifo* out = proc->fifo_out[0];
	if (out && !out->is_open) {
		dnode** it = vec_begin(proc->union_end_nodes);
		for (; it != vec_end(proc->union_end_nodes); ++it) {
			process* union_end_proc = (*it)->data;
			fifo_set_open(union_end_proc->fifo_out[0], false);
		}
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
			fifo_set_open(proc->fifo_in[0], false);
			proc->fifo_in[0] = proc->queued_results->data;
			if (proc->queued_results->next != NULL) {
				proc->queued_results = proc->queued_results->next;
			}
			return PROC_RETURN_RUNNING;
		}
		try_(writer_close(select->writer));
		return PROC_RETURN_COMPLETE;
	}

	enum proc_return ret = PROC_RETURN_WAIT_ON_IN0;

	unsigned iters = 0;
	node** rg_iter = fifo_begin(in);
	while (rg_iter != fifo_end(in)) {
		if (iters++ >= proc->max_recs_iter
		    || select->rows_affected >= current_select->top_count) {
			ret = PROC_RETURN_RUNNING;
			break;
		}

		try_(select->select__(select, *rg_iter));

		++proc->rows_affected;
		++select->rows_affected;

		if (out) {
			fifo_add(out, rg_iter);
		} else if (!current_select->is_const) {
			fqlprocess_recycle(proc, rg_iter);
		}

		if (current_select->is_const) {
			proc->wait_for_in0 = false;
			break;
		}

		_fqlprocess_print_iter_states(proc);
		rg_iter = fifo_iter(in);

		if (out && !fifo_receivable(out)) {
			ret = PROC_RETURN_WAIT_ON_OUT0;
			break;
		}
	}
	fifo_update(in);

	if (select->rows_affected >= current_select->top_count) {
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
		return PROC_RETURN_COMPLETE;
	}

	enum proc_return ret = PROC_RETURN_WAIT_ON_IN0;

	node** rg_iter = fifo_begin(in);
	while (rg_iter != fifo_end(in)) {
		//ret = try_(delete->undelete__(delete, *rg_iter));
		try_(delete->undelete__(delete, *rg_iter));
		++proc->rows_affected;
		fqlprocess_recycle(proc, rg_iter);

		_fqlprocess_print_iter_states(proc);
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
		fqlprocess_recycle(proc, it);
		_fqlprocess_print_iter_states(proc);
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
		fqlprocess_recycle(proc, it);
		_fqlprocess_print_iter_states(proc);
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
			fqlprocess_recycle(proc, false_node);
		} else {
			fifo_consume(in_true);
			++proc->rows_affected;
			fqlprocess_recycle(proc, true_node);
		}
		delete->state = FILTER_PASSTHROUGH;
		_del_passthrough(proc, delete, in_false, in_true);
		return PROC_RETURN_RUNNING;
	}

	if (!in_true->is_open) {
		node** false_node = fifo_get(in_false);
		try_(delete->undelete__(delete, *false_node));
		fqlprocess_recycle(proc, false_node);
		delete->state = FILTER_PASSTHROUGH;
		_del_passthrough(proc, delete, in_false, in_true);
		return PROC_RETURN_RUNNING;
	}

	node** iter_false = fifo_begin(in_false);
	for (; iter_false != fifo_end(in_false); iter_false = fifo_iter(in_false)) {
		node** look_ahead = fifo_look_ahead(in_false);
		if (look_ahead == NULL) {
			break;
		}
		record* look_ahead_rec = record_at(*look_ahead, delete->table_idx);
		if (look_ahead_rec->rec_idx > delete->delete_idx) {
			try_(delete->undelete__(delete, *iter_false));
			delete->state = FILTER_OPEN;
		}
		fqlprocess_recycle(proc, iter_false);
		_fqlprocess_print_iter_states(proc);
	}
	fifo_update(in_false);
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
		fqlprocess_recycle(proc, true_node);
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
		fqlprocess_recycle(proc, rg_iter);
		_fqlprocess_print_iter_states(proc);
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
		fqlprocess_recycle(proc, rg_iter);

		if (update->rows_affected >= update->top_count) {
			proc->wait_for_in0 = false;
			ret = PROC_RETURN_RUNNING;
			break;
		}
		_fqlprocess_print_iter_states(proc);
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
		fqlprocess_recycle(proc, it);
		_fqlprocess_print_iter_states(proc);
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
		fqlprocess_recycle(proc, it);
		_fqlprocess_print_iter_states(proc);
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
			fqlprocess_recycle(proc, false_node);
		} else {
			fifo_consume(in_true);
			try_(update->update__(update, *true_node));
			++proc->rows_affected;
			fqlprocess_recycle(proc, true_node);
		}
		update->state = FILTER_PASSTHROUGH;
		_update_passthrough(proc, update, in_false, in_true);
		return PROC_RETURN_RUNNING;
	}

	if (!in_true->is_open) {
		node** false_node = fifo_get(in_false);
		try_(update->noupdate__(update, *false_node));
		fqlprocess_recycle(proc, false_node);
		update->state = FILTER_PASSTHROUGH;
		_update_passthrough(proc, update, in_false, in_true);
		return PROC_RETURN_RUNNING;
	}

	node** iter_false = fifo_begin(in_false);
	for (; iter_false != fifo_end(in_false); iter_false = fifo_iter(in_false)) {
		node** look_ahead = fifo_look_ahead(in_false);
		if (look_ahead == NULL) {
			break;
		}
		record* look_ahead_rec = record_at(*look_ahead, update->table_idx);
		if (look_ahead_rec->rec_idx > update->update_idx) {
			try_(update->noupdate__(update, *iter_false));
			update->state = FILTER_OPEN;
		}
		fqlprocess_recycle(proc, iter_false);
		_fqlprocess_print_iter_states(proc);
	}
	fifo_update(in_false);
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
		fqlprocess_recycle(proc, true_node);
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
			return PROC_RETURN_COMPLETE;
		}
		return PROC_RETURN_RUNNING;
	}

	unsigned iters = 0;
	node** rg_iter = fifo_begin(in);
	for (; iters++ < proc->max_recs_iter && rg_iter != fifo_end(in);
	     rg_iter = fifo_iter(in)) {
		try_(order_add_record(order, *rg_iter));
		fqlprocess_recycle(proc, rg_iter);
		_fqlprocess_print_iter_states(proc);
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
