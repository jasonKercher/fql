#include "process.h"

#include <stdbool.h>

#include "fql.h"
#include "operation.h"
#include "reader.h"
#include "record.h"
#include "util/vec.h"
#include "util/util.h"
#include "util/dgraph.h"
#include "util/fifo.h"
#include "util/stringview.h"

Process* process_new(const char* action, int width)
{
        Process* new_proc = NULL;
        malloc_(new_proc, sizeof(*new_proc));

        return process_construct(new_proc, action, width);
}

Process* process_construct(Process* proc, const char* action, int width)
{
        *proc = (Process) {
                 &fql_no_op                     /* action */
                ,NULL                           /* fifo_in0 */
                ,NULL                           /* fifo_in1 */
                ,NULL                           /* fifo_out0 */
                ,NULL                           /* fifo_out1 */
                ,NULL                           /* proc_data */
                ,string_from_char_ptr(action)   /* action_msg */
                ,width                          /* fifo_width */
                ,false                          /* is_secondary */
                ,false                          /* is_passive */
        };

        return proc;
}

void process_node_free(Dnode* proc_node)
{
        if (!proc_node->is_root) {
                process_free(proc_node->data);
                return;
        }
        Process* proc = proc_node->data;
        if (proc->fifo_in0 != NULL) {
                Vec** it = vec_begin(proc->fifo_in0->buf);
                for (; it != vec_end(proc->fifo_in0->buf); ++it) {
                        Vec** it2 = vec_begin(*it);
                        for (; it2 != vec_end(*it); ++it2) {
                                vec_free(*it2);
                        }
                        vec_free(*it);
                }
        }
        if (proc->fifo_in1 != NULL) {
                Vec** it = vec_begin(proc->fifo_in1->buf);
                for (; it != vec_end(proc->fifo_in1->buf); ++it) {
                        Vec** it2 = vec_begin(*it);
                        for (; it2 != vec_end(*it); ++it2) {
                                vec_free(*it2);
                        }
                        vec_free(*it);
                }
        }
        process_free(proc_node->data);
}

void process_free(Process* proc)
{
        if (proc->fifo_in0 != NULL) {
                fifo_free(proc->fifo_in0);
        }
        if (proc->fifo_in1 != NULL) {
                fifo_free(proc->fifo_in1);
        }
        string_free(proc->action_msg);
        free_(proc);
}

void process_activate(Dnode* proc_node)
{
        Process* proc = proc_node->data;
        proc->fifo_in0 = fifo_new_(Vec*, UCHAR_MAX);
        if (!proc_node->is_root) {
                return;
        }

        Reader* reader = proc->proc_data;
        int field_count = reader->max_col_idx + 1;

        /* If root, it will own the vector of StringViews */
        Vec* buf = proc->fifo_in0->buf;
        Vec** recs = vec_begin(buf);
        for (; recs != vec_end(buf); ++recs) {
                *recs = vec_new_(Vec*);
                vec_resize(*recs, proc->fifo_width);

                Record** rec = vec_begin(*recs);
                for (; rec != vec_end(*recs); ++rec) {
                        *rec = record_new();
                        vec_resize(&(*rec)->fields, field_count);
                }

                if (proc->fifo_out0 != NULL) {
                        fifo_add(proc->fifo_out0, *recs);
                        fifo_consume(proc->fifo_out0);
                }
        }

        fifo_advance(proc->fifo_in0);
}

void process_add_second_input(Process* proc)
{
        proc->fifo_in1 = fifo_new_(Vec*, UCHAR_MAX);
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
                /* Check to see that there is something to process
                 * as well as a place for it to go.
                 */
                if (fifo_is_empty(proc->fifo_in0) ||
                    (proc->fifo_out0 && !fifo_is_receivable(proc->fifo_out0)) ||
                    (proc->fifo_out1 && !fifo_is_receivable(proc->fifo_out1))) {
                        continue;
                }
                int ret = proc->action(proc_graph, proc);
                if (ret == FQL_FAIL) {
                        return FQL_FAIL;
                }

                if (proc_node == plan->op_true) {
                        ++plan->rows_affected;
                }
                run_count += ret;
        }

        return run_count;
}

int process_step(Plan* plan)
{
        Dgraph* proc_graph = plan->processes;
        int ret = 0;

        plan->rows_affected = 0;

        do {
                dgraph_traverse_reset(proc_graph);
                ret = _exec_one_pass(plan, proc_graph);
        } while (!plan->rows_affected && ret && ret != FQL_FAIL);

        return (ret > 0) ? 1 : ret;
}

int process_exec_plan(Plan* plan)
{
        Dgraph* proc_graph = plan->processes;
        int ret = 0;

        do {
                dgraph_traverse_reset(proc_graph);
                ret = _exec_one_pass(plan, proc_graph);
        } while (ret && ret != FQL_FAIL);

        return ret;
}
