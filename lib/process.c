#include "process.h"
#include "util/util.h"

process_t* process_new(unsigned in, unsigned out, const char* action)
{
        process_t* new_proc = NULL;
        malloc_(new_proc, sizeof(*new_proc));

        *new_proc = (process_t) {
                 vec_new(in)    /* Input */
                ,vec_new(out)   /* Output */
                ,""             /* action */
        };

        strncpy_(new_proc->action, action, ACTION_MAX);

        return new_proc;
}

void process_free(process_t* proc)
{
        free_(proc);
}
