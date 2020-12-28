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
                 NULL                           /* action */
                ,fifo_new_(Vec*)                /* fifo_in0 */
                ,NULL                           /* fifo_in1 */
                ,NULL                           /* fifo_out0 */
                ,NULL                           /* fifo_out1 */
                ,NULL                           /* proc_data */
                ,string_from_char_ptr(action)   /* action_msg */
                ,false                          /* is_secondary */
                ,false                          /* is_passive */
        };

        return proc;
}

void process_free(Process* proc)
{
        string_free(proc->action_msg);
        free_(proc);
}
