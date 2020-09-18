#include "process.h"
#include "util/util.h"

struct process* process_new(unsigned in, unsigned out, const char* action)
{
        struct process* new_proc = NULL;
        malloc_(new_proc, sizeof(*new_proc));

        *new_proc = (struct process) {
                 {NULL, NULL}         /* Input */
                ,{NULL, NULL}         /* Output */
                ,""                   /* action */
        };

        strncpy_(new_proc->action, action, ACTION_MAX);

        return new_proc;
}

void process_free(struct process* proc)
{
        free_(proc);
}
