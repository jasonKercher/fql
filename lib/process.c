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

#define FIFO_SIZE 512

Process* process_new(const char* action, int width)
{
        Process* new_proc = NULL;
        malloc_(new_proc, sizeof(*new_proc));

        return process_construct(new_proc, action, width);
}

Process* process_construct(Process* proc, const char* action, int width)
{
        *proc = (Process) {
                 0                              /* thread */
                ,NULL                           /* records */
                ,&fql_no_op                     /* action__ */
                ,NULL                           /* fifo_in0 */
                ,NULL                           /* fifo_in1 */
                ,NULL                           /* fifo_out0 */
                ,NULL                           /* fifo_out1 */
                ,NULL                           /* proc_data */
                ,string_from_char_ptr(action)   /* action_msg */
                ,width                          /* fifo_width */
                ,false                          /* is_secondary */
                ,false                          /* is_passive */
                ,true                           /* is_enabled */
        };

        return proc;
}

void process_node_free(Dnode* proc_node)
{
        //if (!proc_node->is_root) {
        //        process_free(proc_node->data);
        //        return;
        //}
        //Process* proc = proc_node->data;
        //if (proc->fifo_in0 != NULL) {
        //        Vec** it = vec_begin(proc->fifo_in0->buf); /* vec_(Vec*) */
        //        for (; it != vec_end(proc->fifo_in0->buf); ++it) {
        //                Record** it2 = vec_begin(*it); /* vec_(Record*) */
        //                for (; it2 != vec_end(*it); ++it2) {
        //                        record_free(*it2);
        //                }
        //                //vec_free(*it);
        //        }
        //}
        //if (proc->fifo_in1 != NULL) {
        //        Vec** it = vec_begin(proc->fifo_in1->buf); /* vec_(Vec*) */
        //        for (; it != vec_end(proc->fifo_in1->buf); ++it) {
        //                Record** it2 = vec_begin(*it); /* vec_(Record*) */
        //                for (; it2 != vec_end(*it); ++it2) {
        //                        record_free(*it2);
        //                }
        //                //vec_free(*it);
        //        }
        //}
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

        fprintf(stderr, "Activating %s\n", proc->action_msg->data);

        proc->fifo_in0 = fifo_new_(Vec*, FIFO_SIZE);
        if (!proc_node->is_root) {
                return;
        }

        int field_count = 1;
        if (proc->action__ == &fql_read) {
                Reader* reader = proc->proc_data;
                field_count = reader->max_col_idx + 1;
        }

        proc->records = vec_new_(Vec);
        vec_resize(proc->records, FIFO_SIZE);

        int i = 0;

        /* If root, it will own the vector of Records */
        Vec* buf = proc->fifo_in0->buf;
        for (; i < proc->records->size; ++i) {
                Vec* new_recs = vec_at(proc->records, i);
                vec_construct_(new_recs, Record*);
                vec_resize(new_recs, proc->fifo_width);

                Record** new_rec = vec_back(new_recs);
                *new_rec = record_new(i);
                vec_resize((*new_rec)->fields, field_count);

                Vec** recs = vec_at(buf, i);
                *recs = new_recs;
        }

        /* TODO: once we stop hard coding fifo size,
         *       this if block can go.
         */
        if (proc->action__ != &fql_read) {
                fifo_advance(proc->fifo_in0);
                return;
        }
        fifo_set_full(proc->fifo_in0);
}

void process_add_second_input(Process* proc)
{
        proc->fifo_in1 = fifo_new_(Vec*, FIFO_SIZE);
}

void process_disable(Process* proc)
{
        if (proc->fifo_out0 != NULL) {
                fifo_set_open_ts(proc->fifo_out0, false);
        }
        if (proc->fifo_out1 != NULL) {
                fifo_set_open_ts(proc->fifo_out1, false);
        }
        fifo_set_open_ts(proc->fifo_in0, false);
        proc->is_enabled = false;
}

void process_enable(Process* proc)
{
        if (proc->fifo_out0 != NULL) {
                fifo_set_open_ts(proc->fifo_out0, true);
        }
        if (proc->fifo_out1 != NULL) {
                fifo_set_open_ts(proc->fifo_out1, true);
        }
        fifo_set_open_ts(proc->fifo_in0, true);
        proc->is_enabled = true;
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
                if (!proc->is_enabled) {
                        continue;
                }
                if (!proc->fifo_in0->is_open && fifo_is_empty(proc->fifo_in0)) {
                        process_disable(proc);
                        continue;
                }
                //++run_count;

                /* Check to see that there is something to process
                 * as well as a place for it to go.
                 */
                if (fifo_is_empty(proc->fifo_in0) ||
                    (proc->fifo_out0 && !fifo_is_receivable(proc->fifo_out0)) ||
                    (proc->fifo_out1 && !fifo_is_receivable(proc->fifo_out1))) {
                        continue;
                }
                int ret = proc->action__(proc_graph, proc);
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

void* _thread_exec(void* data)
{
        struct thread_data* tdata = data;
        Process* proc = tdata->proc_node->data;

        while (proc->is_enabled) {
                if (fifo_is_empty_ts(proc->fifo_in0)) {
                        if (!proc->fifo_in0->is_open) {
                                process_disable(proc);
                                break;
                        }
                        if (tdata->proc_node->is_root) {
                                fifo_wait_for_add(proc->fifo_in0);
                        } else {
                                fifo_wait_for_half(proc->fifo_in0);
                        }
                        if (fifo_is_empty_ts(proc->fifo_in0)) {
                                process_disable(proc);
                                break;
                        }
                }
                if (proc->fifo_in1) {
                        if (fifo_is_empty_ts(proc->fifo_in1)) {
                                fifo_wait_for_add(proc->fifo_in1);
                        }
                }
                if (proc->fifo_out0) {
                        while (!fifo_is_receivable_ts(proc->fifo_out0)) {
                                fifo_wait_for_get(proc->fifo_out0);
                        }
                }
                if (proc->fifo_out1) {
                        while (!fifo_is_receivable_ts(proc->fifo_out1)) {
                                fifo_wait_for_get(proc->fifo_out1);
                        }
                }
                int ret = proc->action__(tdata->proc_graph, proc);
                if (ret == FQL_FAIL) {
                        break;
                }
        }

        pthread_exit(NULL);
        return NULL;
}

int process_exec_plan_thread(Plan* plan)
{
        Dgraph* proc_graph = plan->processes;

        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

        Vec tdata_vec = { 0 };
        vec_construct_(&tdata_vec, struct thread_data);
        vec_resize(&tdata_vec, proc_graph->nodes->size);

        int i = 0;

        for (; i < tdata_vec.size; ++i) {
                struct thread_data* tdata = vec_at(&tdata_vec, i);
                Dnode** proc_node = vec_at(proc_graph->nodes, i);
                tdata->proc_node = *proc_node;
                tdata->proc_graph = proc_graph;
                Process* proc = (*proc_node)->data;
                if (pthread_create(&proc->thread, &attr, _thread_exec, tdata)) {
                        return FQL_FAIL;
                }
        }

        pthread_attr_destroy(&attr);

        void* status = NULL;
        for (i = 0; i < tdata_vec.size; ++i) {
                struct thread_data* tdata = vec_at(&tdata_vec, i);
                Process* proc = tdata->proc_node->data;
                if (pthread_join(proc->thread, &status)) {
                        return FQL_FAIL;
                }
        }

        pthread_exit(NULL);

        return FQL_GOOD;
}
