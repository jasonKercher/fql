#include "process.h"

#include <stdbool.h>

#include "util/util.h"

Process* process_new(const char* action)
{
        Process* new_proc = NULL;
        malloc_(new_proc, sizeof(*new_proc));

        return process_init(new_proc, action);
}

Process* process_init(Process* proc, const char* action)
{
        *proc = (Process) {
                 NULL           /* action */
                ,""             /* action_msg */
                ,false          /* is_passive */
        };

        strncpy_(proc->action_msg, action, ACTION_MAX);

        return proc;
}

void process_free(Process* proc)
{
        free_(proc);
}
