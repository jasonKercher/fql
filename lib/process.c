#include "process.h"
#include "util/util.h"

struct process* process_new(const char* action)
{
        struct process* new_proc = NULL;
        malloc_(new_proc, sizeof(*new_proc));

        *new_proc = (struct process) {
                 NULL           /* action */
                ,""             /* action_msg */
        };

        strncpy_(new_proc->action_msg, action, ACTION_MAX);

        return new_proc;
}

void process_free(struct process* proc)
{
        free_(proc);
}
