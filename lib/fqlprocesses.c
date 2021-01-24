#include "fql.h"
#include "reader.h"
#include "process.h"
#include "select.h"

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

int fql_read(Dgraph* proc_graph, Process* proc)
{
        Reader* reader = proc->proc_data;
        struct libcsv_data* data = reader->reader_data;
        Vec** recs = fifo_get(proc->fifo_in0);
        /* We can assume recs is of size 1 */
        Vec** rec = vec_begin(*recs);
        size_t tail = proc->fifo_in0->tail;
        tail = (tail) ? tail-1 : proc->fifo_in0->buf->size;
        int ret = reader->get_record_fn(data, *rec, tail);

        switch (ret) {
        case FQL_GOOD:
                break;
        case FQL_FAIL:
                return FQL_FAIL;
        default: /* eof */
                return 0;
        }

        /* to next process */
        fifo_add(proc->fifo_out0, recs);

        return 1;
}

int fql_select(Dgraph* proc_graph, Process* proc)
{
        if (fifo_is_empty(proc->fifo_in0)) {
                return 0;
        }

        Vec** rec = fifo_get(proc->fifo_in0);
        Select* select = proc->proc_data;
        int ret = select->select_fn(select, *rec);

        /* I suppose select should always be a leaf? */
        _recycle_rec(proc_graph, proc->fifo_width);

        return ret;
}

int fql_logic(Dgraph* proc_graph, Process* proc)
{
        if (fifo_is_empty(proc->fifo_in0)) {
                return 0;
        }

        Vec** recs = fifo_get(proc->fifo_in0);
        Logic* logic = proc->proc_data;
        if (logic->logic_fn(logic, *recs)) {
                fifo_add(proc->fifo_out1, recs);
        } else if (proc->fifo_out0 != NULL) {
                fifo_add(proc->fifo_out0, recs);
        } else {
                _recycle_rec(proc_graph, proc->fifo_width);
        }
        
        return 1;
}

int fql_no_op(Dgraph* proc_graph, Process* proc)
{
        fprintf(stderr, "No-op: %s\n", (char*) proc->action_msg->data);
        if (proc->fifo_out0 == NULL) {
                return 0;
        }
        Vec** rec = fifo_get(proc->fifo_in0);
        fifo_add(proc->fifo_out0, rec);
        return 0;
}

