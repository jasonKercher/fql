#include "process.h"

#include <stdbool.h>

#include "fql.h"
#include "plan.h"
#include "util/vec.h"
#include "util/dgraph.h"
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
                ,fifo_new_(Vec*, UCHAR_MAX)     /* fifo_in0 */
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

int _exec_plan(Plan* plan)
{
        Dgraph* proc_graph = plan->processes;
        Dnode* proc_node = NULL;

        dgraph_traverse_reset(proc_graph);

        while ((proc_node = dgraph_traverse(proc_graph))) {
                Process* proc = proc_node->data;
                fprintf(stderr, "%s\n", (char*)proc->action_msg->data);
        }
        
        return FQL_GOOD;
}

int process_exec_plans(Queue* plans)
{
        int i = 0;
        for (; plans; plans = plans->next) {
                Plan* plan = plans->data;
                if (_exec_plan(plan)) {
                        return FQL_FAIL;
                }
        }
        return FQL_GOOD;
}
