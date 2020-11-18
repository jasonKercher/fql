#include "process.h"
#include "util/util.h"

Process* process_new(const char* action)
{
        Process* new_proc = NULL;
        malloc_(new_proc, sizeof(*new_proc));

        *new_proc = (Process) {
                 NULL           /* action */
                ,""             /* action_msg */
        };

        strncpy_(new_proc->action_msg, action, ACTION_MAX);

        return new_proc;
}

void process_free(Process* proc)
{
        free_(proc);
}
