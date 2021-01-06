#include "process.h"

#include <stdbool.h>

#include "fql.h"
#include "plan.h"
#include "reader.h"
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
                 &fql_no_op                     /* action */
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
        Vec** recs = vec_begin(buf);
        for (; recs != vec_end(buf); ++recs) {
                *recs = vec_new_(Vec*);
                vec_resize(*recs, width);
                Vec** rec = vec_begin(*recs);
                for (; rec != vec_end(*recs); ++rec) {
                        *rec = vec_new_(StringView);
                }
        }

        return proc;
}

void process_free(Process* proc)
{
        string_free(proc->action_msg);
        free_(proc);
}

void process_set_root(Dnode* proc_node)
{
        proc_node->is_root = true;

        /* root processes own most data. Here, we enter
         * fifo data into this root process
         */
        Process* proc = proc_node->data;
        
        /* There should only be one! */
        Vec* rec = vec_begin(proc->fifo_in0->buf);
        fifo_add(proc->fifo_in0, rec);
}

/* Trigger appropiate roots to read the next record */
void _recycle_recs(Dgraph* proc_graph, Process* proc)
{
        int i = 0;
        for (; i < proc->fifo_width; ++i) {
                Dnode** root_node = vec_at(proc_graph->_roots, i);
                Process* root = (*root_node)->data;
                fifo_advance(root->fifo_in0);
        }
}

/* returns number of processes that executed or FQL_FAIL
 * 0 should not happen unless we EOF
 */
int _exec_one_pass(Plan* plan, Dgraph* proc_graph)
{
        Dnode* proc_node = NULL;
        Process* proc = NULL;
        int run_count = 0;
        while ((proc_node = dgraph_traverse(proc_graph))) {
                proc = proc_node->data;
                if (fifo_is_empty(proc->fifo_in0)) {
                        continue;
                }
                int ret = proc->action(proc);
                if (ret == FQL_FAIL) {
                        return FQL_FAIL;
                }

                run_count += ret;

                /* is leaf? */
                if (proc_node->out[0] == NULL && proc_node->out[1] == NULL) {
                        _recycle_recs(proc_graph, proc);
                }
        }

        return run_count;
}

int _exec_plan(Plan* plan)
{
        Dgraph* proc_graph = plan->processes;
        int ret = 0;

        do {
                dgraph_traverse_reset(proc_graph);
                ret = _exec_one_pass(plan, proc_graph);
        } while (ret && ret != FQL_FAIL);

        return ret;
}

int process_exec_plans(Queue* plans)
{
        int i = 0;
        for (; plans; plans = plans->next) {
                Plan* plan = plans->data;
                if (_exec_plan(plan) == FQL_FAIL) {
                        return FQL_FAIL;
                }
        }
        return FQL_GOOD;
}
