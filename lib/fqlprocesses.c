#include "fql.h"
#include "reader.h"
#include "process.h"
#include "select.h"
#include "record.h"
#include "column.h"
#include "util/fifo.h"

//#define RECYCLE_DEBUG 1

void _recycle_specific(Dgraph* proc_graph, Vec* recs, int index)
{
	Record** rec = vec_at(recs, index);
	if (--(*rec)->ref_count > 0 || !(*rec)->is_recyclable) {
		return;
	}

	Dnode** root_node = vec_at(proc_graph->_roots, index);
	Process* root = (*root_node)->data;

	Vec* proc_recs = vec_at(root->records, (*rec)->idx);

#ifdef RECYCLE_DEBUG
	fprintf(stderr, "%s: %d\n", root->action_msg->data, (*rec)->idx);

	Vec* buf = root->fifo_in[root->root_fifo]->buf;
	Vec** it = vec_begin(buf);
	int i = 0;
	for (; it != vec_end(buf); ++it, ++i) {
		Record** it_rec = vec_at(*it, index);
		if (i == root->fifo_in[root->root_fifo]->tail) {
			fputc('[', stderr);
		}
		fprintf(stderr, "%d:%d", i, (*it_rec)->idx);
		if (i == root->fifo_in[root->root_fifo]->head) {
			fputc(']', stderr);
		}
		fputc(' ', stderr);
	}
	fputc('\n', stderr);
#endif

	if (root->action__ == fql_read ||
	    root->action__ == fql_cartesian_join) {
		fifo_add(root->fifo_in[root->root_fifo], &proc_recs);
	}
#ifdef RECYCLE_DEBUG
	it = vec_begin(buf);
	i = 0;
	for (; it != vec_end(buf); ++it, ++i) {
		Record** it_rec = vec_at(*it, index);
		if (i == root->fifo_in[root->root_fifo]->tail) {
			fputc('[', stderr);
		}
		fprintf(stderr, "%d:%d", i, (*it_rec)->idx);
		if (i == root->fifo_in[root->root_fifo]->head) {
			fputc(']', stderr);
		}
		fputc(' ', stderr);
	}
	fputc('\n', stderr);
#endif

}

/* Trigger appropiate roots to read the next record */
void _recycle_recs(Dgraph* proc_graph, Vec* recs, int width)
{
	/* width == 0 means constant expression */
	if (width == 0) {
		Dnode** root_node = vec_begin(proc_graph->_roots);
		process_disable((*root_node)->data);
		return;
	}
	int i = 0;
	for (; i < width; ++i) {
		_recycle_specific(proc_graph, recs, i);
	}
}

int fql_read(Dgraph* proc_graph, Process* proc)
{
	Reader* reader = proc->proc_data;
	Vec** recs = fifo_peek(proc->fifo_in[0]);

	Record** rec = vec_back(*recs);
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

/* Same thing as fql_read, but we recycle */
int fql_read_subquery(Dgraph* proc_graph, Process* proc)
{
	Reader* reader = proc->proc_data;
	Vec** recs = fifo_peek(proc->fifo_in[0]);

	Record** rec = vec_back(*recs);
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

	/* TODO: query ID's for targeted recycling */

	fifo_add(proc->fifo_out[0], recs);

	return ret;
}

int fql_select(Dgraph* proc_graph, Process* proc)
{
	Vec** recs = fifo_get(proc->fifo_in[0]);
	Select* select = proc->proc_data;
	int ret = select->select__(select, *recs);

	_recycle_recs(proc_graph, *recs, proc->fifo_width);

	return ret;
}

int fql_logic(Dgraph* proc_graph, Process* proc)
{
	Vec** recs = fifo_get(proc->fifo_in[0]);
	LogicGroup* lg = proc->proc_data;

	int ret = logicgroup_eval(lg, *recs, lg->join_logic);
	if (ret == FQL_FAIL) {
		return FQL_FAIL;
	}

	if (ret) {
		fifo_add(proc->fifo_out[1], recs);  /* true */
	} else if (proc->fifo_out[0] != NULL) {
		fifo_add(proc->fifo_out[0], recs);  /* false */
	} else {
		_recycle_recs(proc_graph, *recs, proc->fifo_width);
	}

	return 1;
}

void _increase_left_side_ref_count(Vec* leftrecs, _Bool recyclable)
{
	Record** it = vec_begin(leftrecs);
	for (; it != vec_end(leftrecs); ++it) {
		++(*it)->ref_count;
		(*it)->is_recyclable = recyclable;
	}
}

int fql_cartesian_join(Dgraph* proc_graph, Process* proc)
{
	Table* table = proc->proc_data;
	Reader* reader = table->reader;

	Vec** rightrecs = fifo_peek(proc->fifo_in[1]);
	Record** rec = vec_back(*rightrecs);
	int ret = reader->get_record__(reader, *rec);

	Vec** leftrecs = fifo_peek(proc->fifo_in[0]);

	switch (ret) {
	case FQL_GOOD:
		break;
	case FQL_FAIL:
		return FQL_FAIL;
	default: /* eof */
	 {
		reader->reset__(reader);
		Record** it = vec_begin(*leftrecs);
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
		Record** leftrec = vec_at(*leftrecs, i);
		vec_set(*rightrecs, i, leftrec);
	}
	fifo_add(proc->fifo_out[0], rightrecs);

	return 1;
}



void _hash_join_right_side(Process* proc, Table* table, Vec* rightrecs)
{
	struct hashjoin* hj = table->join_data;

	StringView sv;
	column_get_stringview(&sv, hj->right_col, rightrecs);

	Record** rightrec = vec_back(rightrecs);
	multimap_nset(&hj->hash_data, sv.data, &(*rightrec)->rec_raw.data, sv.len);
}

Vec* _hash_join_left_side(Process* proc, Table* table, Vec* leftrecs)
{
	struct hashjoin* hj = table->join_data;

	if (hj->recs == NULL) {
		StringView sv;
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

	Vec** rightrecs = fifo_get(proc->fifo_in[1]);
	Record** rec = vec_back(*rightrecs);
	Reader* reader = table->reader;

	/* TODO: this should be a function pointer */
	mmapcsv_get_record_at(reader, *rec, *rightrec_ptr);

	return *rightrecs;
}

int fql_hash_join(Dgraph* proc_graph, Process* proc)
{
	Table* table = proc->proc_data;
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
		Vec** rightrecs = fifo_get(proc->fifo_in[1]);
		_hash_join_right_side(proc, table, *rightrecs);
		_recycle_specific(proc_graph, *rightrecs, proc->fifo_width-1);
		return 1;
	}

	/* The right side read process killed itself
	 * when it hit EOF. Run it manually as no-op
	 * here just to push any available records.
	 */
	Process* right_side_read_proc = table->read_proc;
	fql_no_op(proc_graph, right_side_read_proc);

	//Vec** leftrecs = fifo_peek(proc->fifo_in[0]);
	Vec** leftrecs = fifo_peek(proc->fifo_in[0]);
	Vec* rightrecs = _hash_join_left_side(proc, table, *leftrecs);
	if (rightrecs == NULL) {
		fifo_consume(proc->fifo_in[0]);
		_recycle_recs(proc_graph, *leftrecs, proc->fifo_width-1);
		return 1;
	}

	unsigned i = 0;
	for (; i < (*leftrecs)->size; ++i) {
		Record** leftrec = vec_at(*leftrecs, i);
		vec_set(rightrecs, i, leftrec);
	}

	fifo_add(proc->fifo_out[1], &rightrecs);

	return 1;
}

int fql_no_op(Dgraph* proc_graph, Process* proc)
{
	//fprintf(stderr, "No-op: %s\n", (char*) proc->action_msg->data);
	Vec** recs = fifo_get(proc->fifo_in[0]);
	fifo_add(proc->fifo_out[0], recs);
	return 0;
}

