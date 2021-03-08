#include "fql.h"
#include "reader.h"
#include "process.h"
#include "select.h"
#include "record.h"
#include "column.h"
#include "util/fifo.h"

void _recycle_specific(Dgraph* proc_graph, Vec* recs, int index)
{
        Record** rec = vec_at(recs, index);
        if ((*rec)->ref_count) {
                --(*rec)->ref_count;
                return;
        }

        Dnode** root_node = vec_at(proc_graph->_roots, index);
        Process* root = (*root_node)->data;

        Record** a_rec = vec_at(recs, index);
        unsigned rec_idx = (*a_rec)->idx;
        Vec* proc_recs = vec_at(root->records, rec_idx);

        if (root->action__ == fql_read && fifo_is_open_ts(root->fifo_in0)) {
                fifo_recycle_ts(root->fifo_in0, &proc_recs);
        }
}

/* Trigger appropiate roots to read the next record */
void _recycle_recs(Dgraph* proc_graph, Vec* recs, int width)
{
        /* width == 0 means constant expression */
        if (width == 0) {
                Dnode** root_node = vec_begin(proc_graph->_roots);
                process_close((*root_node)->data);
                return;
        }
        int i = 0;
        for (; i < width; ++i) {
                _recycle_specific(proc_graph, recs, i);
        }
}

void _advance_specific(Dgraph* proc_graph, int index)
{
        //if (index == 0) {
        //        Dnode** root_node = vec_begin(proc_graph->_roots);
        //        process_close((*root_node)->data);
        //        return;
        //}
        Dnode** root_node = vec_at(proc_graph->_roots, index);
        Process* root = (*root_node)->data;
        if (root->action__ == fql_read && fifo_is_open_ts(root->fifo_in0)) {
                fifo_advance_ts(root->fifo_in0);
        }
}

int fql_read(Dgraph* proc_graph, Process* proc)
{
        Reader* reader = proc->proc_data;
        Vec** recs = fifo_peek_ts(proc->fifo_in0);
        
        Record** rec = vec_back(*recs);
        int ret = reader->get_record__(reader, *rec);

        switch (ret) {
        case FQL_GOOD:
                break;
        case FQL_FAIL:
                return FQL_FAIL;
        default: /* eof */
                fifo_set_open_ts(proc->fifo_in0, false);
                fifo_set_open_ts(proc->fifo_out0, false);
                while (!fifo_is_empty_ts(proc->fifo_in0)) {
                        fifo_consume_ts(proc->fifo_in0);
                }
                return 0;
        }

        fifo_consume_ts(proc->fifo_in0);
        fifo_add_ts(proc->fifo_out0, recs);

        return 1;
}

int fql_select(Dgraph* proc_graph, Process* proc)
{
        Vec** recs = fifo_get_ts(proc->fifo_in0);
        Select* select = proc->proc_data;
        int ret = select->select__(select, *recs);

        _recycle_recs(proc_graph, *recs, proc->fifo_width);

        return ret;
}

int fql_logic(Dgraph* proc_graph, Process* proc)
{
        Vec** recs = fifo_get_ts(proc->fifo_in0);
        LogicGroup* lg = proc->proc_data;

        int ret = logicgroup_eval(lg, *recs, lg->join_logic);
        if (ret == FQL_FAIL) {
                return FQL_FAIL;
        }

        if (ret) {
                fifo_add_ts(proc->fifo_out1, recs);  /* true */
        } else if (proc->fifo_out0 != NULL) {
                fifo_add_ts(proc->fifo_out0, recs);  /* false */
        } else {
                _recycle_recs(proc_graph, *recs, proc->fifo_width);
        }

        return 1;
}

void _increase_left_side_ref_count(Vec* leftrecs)
{
        Record** it = vec_begin(leftrecs);
        for (; it != vec_end(leftrecs); ++it) {
                ++(*it)->ref_count;
        }
}

int fql_cartesian_join(Dgraph* proc_graph, Process* proc)
{
        Vec** leftrecs = fifo_peek_ts(proc->fifo_in0);

        /* Re-open fifo if eof reached and consume */
        if (fifo_is_empty_ts(proc->fifo_in1)) {
                if (fifo_is_open_ts(proc->fifo_in1)) {
                        return 0;
                }
                fifo_consume_ts(proc->fifo_in0);

                if (!fifo_is_open_ts(proc->fifo_in0) &&
                    fifo_is_empty_ts(proc->fifo_in0)) {
                        return 0;
                }

                _recycle_recs(proc_graph, *leftrecs, (*leftrecs)->size);

                Source* src = proc->proc_data;
                Process* right_side_read_proc = src->read_proc;
                fifo_advance_ts(right_side_read_proc->fifo_in0);
                fifo_set_open_ts(right_side_read_proc->fifo_in0, true);
                fifo_set_open_ts(proc->fifo_in1, true);
                return 1;
        }

        _increase_left_side_ref_count(*leftrecs);
        Vec** rightrecs = fifo_get_ts(proc->fifo_in1);

        unsigned i = 0;
        for (; i < (*leftrecs)->size; ++i) {
                Record** leftrec = vec_at(*leftrecs, i);
                vec_set(*rightrecs, i, leftrec);
        }

        fifo_add_ts(proc->fifo_out0, rightrecs);

        return 1;
}



void _hash_join_right_side(Process* proc, Source* src, Vec* rightrecs)
{
        struct hashjoin* hj = src->join_data;

        StringView sv;
        column_get_stringview(&sv, hj->right_col, rightrecs);

        Record** rightrec = vec_back(rightrecs);
        pmap_nset(&hj->hash_data, sv.data, (void*) (*rightrec)->rec_raw.data, sv.len);
}

Vec* _hash_join_left_side(Process* proc, Source* src, Vec* leftrecs)
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

        char** rightrec_ptr = vec_at(hj->recs, hj->rec_idx++);
        if (hj->rec_idx >= hj->recs->size) {
                fifo_consume_ts(proc->fifo_in0);
                hj->recs = NULL;
        } else {
                _increase_left_side_ref_count(leftrecs);
        }

        Vec** rightrecs = fifo_get_ts(proc->fifo_in1);
        Record** rec = vec_back(*rightrecs);
        Reader* reader = src->table->reader;

        /* TODO: this should be a function pointer */
        mmapcsv_get_record_at(reader, *rec, *rightrec_ptr);

        return *rightrecs;
}

int fql_hash_join(Dgraph* proc_graph, Process* proc)
{
        Source* src = proc->proc_data;
        struct hashjoin* hj = src->join_data;

        if (fifo_is_empty_ts(proc->fifo_in1) && hj->state == SIDE_RIGHT) {
                if (!fifo_is_open_ts(proc->fifo_in1)) {
                        hj->state = SIDE_LEFT;
                        _advance_specific(proc_graph, proc->fifo_width-1);
                        return 1;
                } else {
                        return 0;
                }
        }

        if (hj->state == SIDE_RIGHT) {
                Vec** rightrecs = fifo_get_ts(proc->fifo_in1);
                _hash_join_right_side(proc, src, *rightrecs);
                _recycle_specific(proc_graph, *rightrecs, proc->fifo_width-1);
                return 1;
        }

        /* The right side read process killed itself
         * when it hit EOF. Run it manually as no-op
         * here just to push any available records.
         */
        Process* right_side_read_proc = src->read_proc;
        fifo_advance_ts(right_side_read_proc->fifo_in0);
        fql_no_op(proc_graph, right_side_read_proc);

        Vec** leftrecs = fifo_peek_ts(proc->fifo_in0);
        Vec* rightrecs = _hash_join_left_side(proc, src, *leftrecs);
        if (rightrecs == NULL) {
                fifo_consume_ts(proc->fifo_in0);
                _recycle_recs(proc_graph, *leftrecs, proc->fifo_width-1);
                return 1;
        }

        unsigned i = 0;
        for (; i < (*leftrecs)->size; ++i) {
                Record** leftrec = vec_at(*leftrecs, i);
                vec_set(rightrecs, i, leftrec);
        }

        fifo_add_ts(proc->fifo_out0, &rightrecs);

        return 1;
}

int fql_no_op(Dgraph* proc_graph, Process* proc)
{
        //fprintf(stderr, "No-op: %s\n", (char*) proc->action_msg->data);
        Vec** recs = fifo_get_ts(proc->fifo_in0);
        fifo_add_ts(proc->fifo_out0, recs);
        return 0;
}

