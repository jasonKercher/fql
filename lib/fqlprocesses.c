#include "fql.h"
#include "reader.h"
#include "process.h"

int fql_read(Process* proc)
{
        struct libcsv_data* data = proc->proc_data;
        Vec** rec = fifo_get(proc->fifo_in0);
        size_t tail = proc->fifo_in0->tail;
        tail = (tail) ? tail-1 : proc->fifo_in0->buf->size;
        int ret = libcsv_get_record(data, *rec, tail);

        switch (ret) {
        case FQL_GOOD:
                break;
        case FQL_FAIL:
                return FQL_FAIL;
        default: /* eof */
                return 0;
        }
        
        /* to next process */
        fifo_add(proc->fifo_out0, rec);

        return 1;
}

int fql_no_op(Process* proc)
{
        fprintf(stderr, "No-op: %s\n", proc->action_msg->data);
        if (proc->fifo_out0 == NULL) {
                return 0;
        }
        Vec** rec = fifo_get(proc->fifo_in0);
        fifo_add(proc->fifo_out0, rec);
        return 0;
}
