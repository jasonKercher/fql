#include "process.h"

#include <stdbool.h>

#include "fql.h"
#include "plan.h"
#include "util/vec.h"
#include "util/util.h"
#include "util/dgraph.h"
#include "util/fifo.h"
#include "util/stringview.h"

Process* process_new(const char* action, int width)
{
        Process* new_proc = NULL;
        malloc_(new_proc, sizeof(*new_proc));

        return process_init(new_proc, action, width);
}

Process* process_init(Process* proc, const char* action, int width)
{
        *proc = (Process) {
                 NULL                           /* action */
                ,fifo_new_(Vec*, UCHAR_MAX)     /* fifo_in0 */
                ,NULL                           /* fifo_in1 */
                ,NULL                           /* fifo_out0 */
                ,NULL                           /* fifo_out1 */
                ,NULL                           /* proc_data */
                ,string_from_char_ptr(action)   /* action_msg */
                ,width                          /* fifo_width */
                ,false                          /* is_secondary */
                ,false                          /* is_passive */
        };

        Vec* buf = proc->fifo_in0->buf;
        Vec** it = vec_begin(buf);
        for (; it != vec_end(buf); ++it) {
                *it = vec_new_(StringView);
        }

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
                if ((!proc_node->is_root && fifo_is_empty(proc->fifo_in0)) ||
                    (proc_node->is_root && fifo_is_full(proc->fifo_in0))) {
                        continue;
                }
                int ret = proc->action(proc);
                if (ret == FQL_FAIL) {
                        return FQL_FAIL;
                }
                //fprintf(stderr, "%s\n", (char*)proc->action_msg->data);
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
