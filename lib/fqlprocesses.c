#include "fql.h"
#include "reader.h"
#include "process.h"
#include "select.h"
#include "record.h"
#include "column.h"

/* Trigger appropiate roots to read the next record */
void _recycle_rec(Dgraph* proc_graph, int width)
{
        int i = 0;
        for (; i < width; ++i) {
                Dnode** root_node = vec_at(proc_graph->_roots, i);
                Process* root = (*root_node)->data;
                fifo_advance(root->fifo_in0);
        }
}

void _recycle_specific(Dgraph* proc_graph, int width)
{
        Dnode** root_node = vec_at(proc_graph->_roots, width - 1);
        Process* root = (*root_node)->data;
        fifo_advance(root->fifo_in0);
}

int fql_read(Dgraph* proc_graph, Process* proc)
{
        if (!proc->fifo_out0->is_open) {
                return 0;
        }

        Reader* reader = proc->proc_data;
        Vec** recs = fifo_peek(proc->fifo_in0);
        /* We can assume recs is of size 1 */
        Record** rec = vec_begin(*recs);
        size_t tail = proc->fifo_in0->tail;
        tail = (tail) ? tail-1 : proc->fifo_in0->buf->size;
        int ret = reader->get_record_fn(reader, *rec, tail);

        switch (ret) {
        case FQL_GOOD:
                break;
        case FQL_FAIL:
                return FQL_FAIL;
        default: /* eof */
                proc->fifo_out0->is_open = false;
                return 0;
        }

        fifo_consume(proc->fifo_in0);

        /* to next process */
        fifo_add(proc->fifo_out0, recs);

        return 1;
}

int fql_select(Dgraph* proc_graph, Process* proc)
{
        Vec** recs = fifo_get(proc->fifo_in0);
        Select* select = proc->proc_data;
        int ret = select->select_fn(select, *recs);

        /* TODO: should not assume select is a leaf */
        _recycle_rec(proc_graph, proc->fifo_width);

        return ret;
}

int fql_logic(Dgraph* proc_graph, Process* proc)
{
        Vec** recs = fifo_get(proc->fifo_in0);
        LogicGroup* lg = proc->proc_data;

        int ret = logicgroup_eval(lg, *recs);
        if (ret == FQL_FAIL) {
                return FQL_FAIL;
        }

        if (ret) {
                fifo_add(proc->fifo_out1, recs);  /* true */
        } else if (proc->fifo_out0 != NULL) {
                fifo_add(proc->fifo_out0, recs);  /* false */
        } else {
                _recycle_rec(proc_graph, proc->fifo_width);
        }

        return 1;
}

int fql_cartesian_join(Dgraph* proc_graph, Process* proc)
{
        Vec** leftrecs = fifo_peek(proc->fifo_in0);

        /* Re-open fifo if eof reached and consume */
        if (fifo_is_empty(proc->fifo_in1)) {
                if (proc->fifo_in1->is_open) {
                        return 0;
                }
                _recycle_rec(proc_graph, proc->fifo_width - 1);
                fifo_consume(proc->fifo_in0);
                proc->fifo_in1->is_open = true;
                return 1;
        }

        /* We can assume recs is of size 1 */
        Vec** rightrecs = fifo_get(proc->fifo_in1);
        Record** rightrec = vec_begin(*rightrecs);

        /* Copy rightrec into appropriate index */
        Source* src = proc->proc_data;
        vec_set(*leftrecs, src->idx, rightrec);

        fifo_add(proc->fifo_out0, leftrecs);

        return 1;
}



void _hash_join_right_side(Process* proc, Source* src)
{
        Vec** leftrecs = fifo_peek(proc->fifo_in0);

        /* We can assume recs is of size 1 */
        Vec** rightrecs = fifo_get(proc->fifo_in1);
        Record** rightrec = vec_begin(*rightrecs);

        /* We don't need anything on the left side,
         * BUT we need a vector with the right side
         * at the right index.
         */
        vec_set(*leftrecs, src->idx, rightrec);

        struct hashjoin* hj = src->join_data;

        StringView sv;
        column_get_stringview(&sv, hj->right_col, *leftrecs);

        pmap_nset(&hj->hash_data, sv.data, (void*) (*rightrec)->rec_raw.data, sv.len);
}

Record* _hash_join_left_side(Process* proc, Source* src, Vec* leftrecs)
{
        struct hashjoin* hj = src->join_data;

        if (hj->recs == NULL) {
                StringView sv;
                column_get_stringview(&sv, hj->left_col, leftrecs);

                hj->recs = pmap_nget(&hj->hash_data, sv.data, sv.len);
                if (hj->recs == NULL) {
                        return NULL;
                }
                hj->rec_idx = 0;
        }

        if (hj->rec_idx >= hj->recs->size) {
                hj->recs = NULL;
                return NULL;
        }
        char** rightrec_ptr = vec_at(hj->recs, hj->rec_idx++);

        /* Let's assume that this read never fails
         * because it has already parsed...
         */
        Vec** rightrecs = fifo_get(proc->fifo_in1);
        /* We can assume recs is of size 1 */
        Record** rec = vec_begin(*rightrecs);
        size_t tail = proc->fifo_in1->tail;
        tail = (tail) ? tail-1 : proc->fifo_in1->buf->size;

        Reader* reader = src->table->reader;
        mmapcsv_get_record_at(reader, *rec, tail, *rightrec_ptr);

        return *rec;
}

int fql_hash_join(Dgraph* proc_graph, Process* proc)
{
        Source* src = proc->proc_data;

        if (proc->fifo_in1->is_open) {
                if (fifo_is_empty(proc->fifo_in1)) {
                        return 0;
                }
                _hash_join_right_side(proc, src);
                _recycle_specific(proc_graph, proc->fifo_width);
                return 1;
        }
        Vec** leftrecs = fifo_peek(proc->fifo_in0);

        Record* rightrec = _hash_join_left_side(proc, src, *leftrecs);
        if (rightrec == NULL) {
                fifo_consume(proc->fifo_in0);
                _recycle_rec(proc_graph, proc->fifo_width);
                return 1;
        }

        /* Copy rightrec into appropriate index */
        vec_set(*leftrecs, src->idx, &rightrec);

        fifo_add(proc->fifo_out0, leftrecs);

        return 1;
}

int fql_no_op(Dgraph* proc_graph, Process* proc)
{
        fprintf(stderr, "No-op: %s\n", (char*) proc->action_msg->data);
        Vec** recs = fifo_get(proc->fifo_in0);
        fifo_add(proc->fifo_out0, recs);
        return 0;
}

