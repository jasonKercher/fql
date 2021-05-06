#include "fql.h"
#include "reader.h"
#include "process.h"
#include "group.h"
#include "aggregate.h"
#include "fqlselect.h"
#include "record.h"
#include "column.h"
#include "util/fifo.h"

void _recycle_specific(process* proc, vec* recs, int index)
{
	dnode** root_node = vec_at(proc->root_group, index);
	process* root = (*root_node)->data;

	if (root->is_const) {
		return;
	}

	record** rec = vec_at(recs, index);
	if (--(*rec)->ref_count > 0 || !(*rec)->is_recyclable) {
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
	vec** recs = fifo_peek(proc->fifo_in[0]);

	record** rec = vec_back(*recs);
	int ret = reader->get_record__(reader, *rec);

	switch (ret) {
	case FQL_GOOD:
		break;
	case FQL_FAIL:
		return FQL_FAIL;
	default: /* eof */
		process_disable(proc);
		return 0;
	}

	fifo_consume(proc->fifo_in[0]);
	fifo_add(proc->fifo_out[0], recs);

	return 1;
}

int fql_read_subquery(dgraph* proc_graph, process* proc)
{
	table* table = proc->proc_data;
	reader* reader = table->reader;
	vec** recs = fifo_peek(proc->fifo_in[1]);
	record** rec = vec_back(*recs);

	vec** sub_recs = fifo_get(proc->fifo_in[0]);
	reader->subquery_recs = *sub_recs;

	int ret = reader->get_record__(reader, *rec);

	switch (ret) {
	case FQL_GOOD:
		break;
	case FQL_FAIL:
		return FQL_FAIL;
	default: /* eof */
		process_disable(proc);
		return 0;
	}

	fifo_consume(proc->fifo_in[1]);

	if (!proc->is_const) {
		_recycle_recs(proc, *sub_recs, (*sub_recs)->size);
	}

	fifo_add(proc->fifo_out[0], recs);

	return ret;
}

int fql_select(dgraph* proc_graph, process* proc)
{
	vec** recs = fifo_get(proc->fifo_in[0]);
	fqlselect* select = proc->proc_data;
	int ret = select->select__(select, *recs);

	_recycle_recs(proc, *recs, proc->fifo_width);

	return ret;
}

int fql_logic(dgraph* proc_graph, process* proc)
{
	vec** recs = fifo_get(proc->fifo_in[0]);
	logicgroup* lg = proc->proc_data;

	int ret = logicgroup_eval(lg, *recs, lg->join_logic);
	if (ret == FQL_FAIL) {
		return FQL_FAIL;
	}

	if (ret) {
		fifo_add(proc->fifo_out[1], recs);  /* true */
	} else if (proc->fifo_out[0] != NULL) {
		fifo_add(proc->fifo_out[0], recs);  /* false */
	} else {
		_recycle_recs(proc, *recs, proc->fifo_width);
	}

	return 1;
}

void _increase_left_side_ref_count(vec* leftrecs, _Bool recyclable)
{
	record** it = vec_begin(leftrecs);
	for (; it != vec_end(leftrecs); ++it) {
		++(*it)->ref_count;
		(*it)->is_recyclable = recyclable;
	}
}

int fql_cartesian_join(dgraph* proc_graph, process* proc)
{
	table* table = proc->proc_data;
	reader* reader = table->reader;

	vec** rightrecs = fifo_peek(proc->fifo_in[1]);
	record** rec = vec_back(*rightrecs);
	int ret = reader->get_record__(reader, *rec);

	vec** leftrecs = fifo_peek(proc->fifo_in[0]);

	switch (ret) {
	case FQL_GOOD:
		break;
	case FQL_FAIL:
		return FQL_FAIL;
	default: /* eof */
	 {
		reader->reset__(reader);
		record** it = vec_begin(*leftrecs);
		for (; it != vec_end(*leftrecs); ++it) {
			(*it)->is_recyclable = true;
		}
		fifo_consume(proc->fifo_in[0]);
		return 1;
	 }
	}

	fifo_consume(proc->fifo_in[1]);
	_increase_left_side_ref_count(*leftrecs, false);

	unsigned i = 0;
	for (; i < (*leftrecs)->size; ++i) {
		record** leftrec = vec_at(*leftrecs, i);
		vec_set(*rightrecs, i, leftrec);
	}
	fifo_add(proc->fifo_out[0], rightrecs);

	return 1;
}



void _hash_join_right_side(process* proc, table* table, vec* rightrecs)
{
	struct hashjoin* hj = table->join_data;

	stringview sv;
	column_get_stringview(&sv, hj->right_col, rightrecs);

	record** rightrec = vec_back(rightrecs);
	multimap_nset(&hj->hash_data, sv.data, &(*rightrec)->rec_raw.data, sv.len);
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
		fifo_consume(proc->fifo_in[0]);
		hj->recs = NULL;
		_increase_left_side_ref_count(leftrecs, true);
	} else {
		_increase_left_side_ref_count(leftrecs, false);
	}

	vec** rightrecs = fifo_get(proc->fifo_in[1]);
	record** rec = vec_back(*rightrecs);
	reader* reader = table->reader;

	/* TODO: this should be a function pointer */
	mmapcsv_get_record_at(reader, *rec, *rightrec_ptr);

	return *rightrecs;
}

int fql_hash_join(dgraph* proc_graph, process* proc)
{
	table* table = proc->proc_data;
	struct hashjoin* hj = table->join_data;

	if (fifo_is_empty(proc->fifo_in[1]) && hj->state == SIDE_RIGHT) {
		if (!fifo_is_open(proc->fifo_in[1])) {
			hj->state = SIDE_LEFT;
			return 1;
		} else {
			return 0;
		}
	}

	if (hj->state == SIDE_RIGHT) {
		vec** rightrecs = fifo_get(proc->fifo_in[1]);
		_hash_join_right_side(proc, table, *rightrecs);
		_recycle_specific(proc, *rightrecs, proc->fifo_width-1);
		return 1;
	}

	/* the right side read process killed itself
	 * when it hit EOF. run it manually as no-op
	 * here just to push any available records.
	 */
	process* right_side_read_proc = table->read_proc;
	fql_no_op(proc_graph, right_side_read_proc);

	vec** leftrecs = fifo_peek(proc->fifo_in[0]);
	vec* rightrecs = _hash_join_left_side(proc, table, *leftrecs);
	if (rightrecs == NULL) {
		fifo_consume(proc->fifo_in[0]);
		_recycle_recs(proc, *leftrecs, proc->fifo_width-1);
		return 1;
	}

	unsigned i = 0;
	for (; i < (*leftrecs)->size; ++i) {
		record** leftrec = vec_at(*leftrecs, i);
		vec_set(rightrecs, i, leftrec);
	}

	fifo_add(proc->fifo_out[1], &rightrecs);

	return 1;
}

int fql_distinct(dgraph* proc_graph, process* proc)
{
	vec** recs = fifo_get(proc->fifo_in[0]);

	group* group = proc->proc_data;
	int ret = group_record(group, *recs);
	if (ret == 1) {
		fifo_add(proc->fifo_out[0], recs);
	} else {
		_recycle_recs(proc, *recs, (*recs)->size);
	}
	return 1;
}

int fql_groupby(dgraph* proc_graph, process* proc)
{
	group* group = proc->proc_data;
	if (!proc->fifo_in[0]->is_open
	 && fifo_is_empty(proc->fifo_in[0])) {
		vec** recs = fifo_get(proc->fifo_in[1]);
		record** rec = vec_at(*recs, 0);

		int ret = group_dump_record(group, *rec);
		switch (ret) {
		case FQL_FAIL:
			return FQL_FAIL;
		case FQL_GOOD:
			fifo_add(proc->fifo_out[0], recs);
			return 1;
		default:
			process_disable(proc);
			return 0;
		}
	}
	vec** recs = fifo_get(proc->fifo_in[0]);

	if (!vec_empty(&group->columns)) {
		int ret = group_record(group, *recs);
		if (ret == FQL_FAIL) {
			return FQL_FAIL;
		}
	}

	/* this entire if block needs to exist in case
	 * you _really_ need to know the results of:
	 * select count(*)  -- blasphemy
	 */
	if (*recs != NULL) {
		_recycle_recs(proc, *recs, (*recs)->size);
	} else {
		vec** recs = fifo_get(proc->fifo_in[1]);
		record** rec = vec_at(*recs, 0);
		int ret = group_dump_record(group, *rec);
		if (ret == FQL_FAIL) {
			return FQL_FAIL;
		}
		fifo_add(proc->fifo_out[0], recs);
		process_disable(proc);
	}
	return 1;
}

int fql_no_op(dgraph* proc_graph, process* proc)
{
	//fprintf(stderr, "no-op: %s\n", (char*) proc->action_msg->data);
	vec** recs = fifo_get(proc->fifo_in[0]);
	fifo_add(proc->fifo_out[0], recs);
	return 0;
}

