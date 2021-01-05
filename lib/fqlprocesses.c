#include "fql.h"
#include "reader.h"
#include "process.h"

int fql_read(Process* proc)
{
        struct libcsv_data* data = proc->proc_data;
        Vec* rec = fifo_get(proc->fifo_in0);
        size_t tail = proc->fifo_in0->tail;
        tail = (tail) ? proc->fifo_in0->buf->size : tail-1;
        libcsv_get_record(data, rec, tail);

        return FQL_GOOD;
}
