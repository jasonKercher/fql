#include "process.h"
#include "util/util.h"

struct process* process_new(unsigned in, unsigned out, const char* action)
{
        struct process* new_proc = NULL;
        malloc_(new_proc, sizeof(*new_proc));

        *new_proc = (struct process) {
                 vector_new(in)         /* Input */
                ,vector_new(out)        /* Output */
                ,""                     /* action */
        };

        strncpy_(new_proc->action, action, ACTION_MAX);

        return new_proc;
}

void process_free(struct process* proc)
{
        /* TODO - Free elements */
        vector_free(proc->input);
        vector_free(proc->output);
        free_(proc);
}
