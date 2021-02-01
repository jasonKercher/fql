#include "fqlplan.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "column.h"
#include "logic.h"
#include "process.h"
#include "query.h"
#include "util/dgraph.h"
#include "util/util.h"

#define PLAN_COLUMN_SEP " | "

/**
 * Implementation for Plan
 *
 * Plan is basically a decision graph
 * where each node represents a process
 */

Plan* plan_new(int source_total)
{
        Plan* new_plan = NULL;
        malloc_(new_plan, sizeof(*new_plan));

        return plan_construct(new_plan, source_total);
}

Plan* plan_construct(Plan* plan, int source_total)
{
        *plan = (Plan) {
                 dgraph_new()                                      /* processes */
                ,dnode_new(process_new("OP_TRUE", source_total))   /* op_true */
                ,dnode_new(process_new("OP_FALSE", source_total))  /* op_false */
                ,NULL                                              /* current */
                ,0                                                 /* rows_affected */
                ,0                                                 /* source_count */
        };

        /* source_count is a temporary variable used to keep track
         * of the current number of sources as the plan is built.
         * Setting the member (source_count) to 0 is correct here.
         */
        Process* start = process_new("start", 0);
        start->is_passive = true;
        plan->current = dgraph_add_data(plan->processes, start);
        dgraph_add_node(plan->processes, plan->op_true);
        dgraph_add_node(plan->processes, plan->op_false);

        return plan;
}

void plan_free(void* generic_plan)
{
        plan_destroy(generic_plan);
        free_(generic_plan);
}

void plan_destroy(void* generic_plan)
{
        Plan* plan = generic_plan;
        Dnode** it = vec_begin(plan->processes->nodes);
        for (; it != vec_end(plan->processes->nodes); ++it) {
                process_node_free(*it);
        }
        dgraph_free(plan->processes);
}

/* build process nodes from logic graph
 * assign processes for true and false
 * return beginning process
 */
void _logic_to_process(Process* logic_proc, LogicGroup* lg)
{
        switch (lg->type) {
        case LG_ROOT:
                string_cat(logic_proc->action_msg, "("); 
                break;
        case LG_AND:
                string_cat(logic_proc->action_msg, "AND("); 
                break;
        case LG_NOT:
                string_cat(logic_proc->action_msg, "IFNOT("); 
                if (lg->condition != NULL) {
                        logic_assign_process(lg->condition, logic_proc);
                }
                break;
        default:
                fprintf(stderr, "Unexpected logic group %d\n", lg->type);
                return;
        }

        LogicGroup** it = vec_begin(&lg->items);
        for (; it != vec_end(&lg->items); ++it) {
                _logic_to_process(logic_proc, *it);
        }

        string_cat(logic_proc->action_msg, ")"); 
}

void _logicgroup_process(Plan* plan, LogicGroup* lg)
{
        Process* logic_proc = process_new("", plan->source_count);
        logic_proc->action = &fql_logic;
        logic_proc->proc_data = lg;
        _logic_to_process(logic_proc, lg);
        Dnode* logic_node = dgraph_add_data(plan->processes, logic_proc);
                
        plan->current->out[0] = logic_node;
        logic_node->out[0] = plan->op_false;

        Process* logic_true = process_new("JOIN True", plan->source_count);
        logic_true->is_passive = true;
        Dnode* logic_true_node = dgraph_add_data(plan->processes, logic_true);

        logic_node->out[1] = logic_true_node;

        plan->current = logic_true_node;
}

Process* _new_join_proc(enum join_type type, int source_count)
{
        switch (type) {
        case JOIN_INNER:
                return process_new("INNER JOIN", source_count);
        case JOIN_LEFT:
                return process_new("LEFT JOIN", source_count);
        case JOIN_RIGHT:
                return process_new("RIGHT JOIN", source_count);
        case JOIN_FULL:
                return process_new("FULL JOIN", source_count);
        case JOIN_CROSS:
                return process_new("CROSS JOIN", source_count);
        case JOIN_FROM:
                return process_new("unexpected: JOIN_FROM", source_count);
        }
}

void _from(Plan* plan, Query* query)
{
        if (query->sources->size == 0) {
                return;
        }

        String action_msg;
        string_construct(&action_msg);
        Source* src = vec_begin(query->sources);

        string_sprintf(&action_msg,
                       "%s: %s",
                       src->table->reader->file_name.data,
                       "stream read");

        Process* from_proc = process_new(action_msg.data, ++plan->source_count);
        from_proc->action = &fql_read;
        from_proc->proc_data = src->table->reader;

        Dnode* from_node = dgraph_add_data(plan->processes, from_proc);
        from_node->is_root = true;

        plan->current->out[0] = from_node;
        plan->current = from_node;

        for (++src; src != vec_end(query->sources); ++src) {
                Process* join_proc = _new_join_proc(src->join_type, ++plan->source_count);

                Dnode* join_proc_node = dgraph_add_data(plan->processes, join_proc);

                plan->current->out[0] = join_proc_node;

                string_sprintf(&action_msg,
                               "%s: %s",
                               src->table->reader->file_name.data,
                               "mmap read");

                /* Root node only will only have one source */
                Process* read_proc = process_new(action_msg.data, 1);
                read_proc->proc_data = src->table->reader;
                read_proc->action = &fql_read;
                read_proc->is_secondary = true;

                Dnode* read_node = dgraph_add_data(plan->processes, read_proc);
                read_node->is_root = true;
                read_node->out[0] = join_proc_node;
                plan->current = join_proc_node;

                if (src->condition != NULL) {
                        _logicgroup_process(plan, src->condition);
                }
        }

        string_destroy(&action_msg);
}

void _where(Plan* plan, Query* query)
{
        if (query->where == NULL) {
                return;
        }

        _logicgroup_process(plan, query->where);
}

void _group(Plan* plan, Query* query)
{
        if (query->groups->size == 0) {
                return;
        }
        Process* group_proc = process_new("GROUP BY", plan->source_count);
        Dnode* group_node = dgraph_add_data(plan->processes, group_proc);
        plan->current->out[0] = group_node;
        plan->current = group_node;
}

void _having(Plan* plan, Query* query) { }

void _operation(Plan* plan, Query* query)
{
        plan->current->out[0] = plan->op_true;
        op_apply_process(query, plan);
}

void _limit(Plan* plan, Query* query) { }

/* In an effort to make building of the process graph easier
 * passive nodes are used as a sort of link between the steps.
 * Passive nodes always point to the next node on out[0]
 */
void _clear_passive(Plan* plan)
{
        Vec* node_vec = plan->processes->nodes;
        Dnode** nodes = vec_begin(node_vec);
        int i = 0;

        /* Re-link nodes so passive ones get skipped during traversal */
        for (; i < node_vec->size; ++i) {
                /* Check branch 0 */
                while (nodes[i]->out[0] != NULL) {
                        Process* proc = nodes[i]->out[0]->data;
                        if (proc->is_passive) {
                                nodes[i]->out[0] = nodes[i]->out[0]->out[0];
                        } else {
                                break;
                        }
                }

                /* Check branch 1 */
                while (nodes[i]->out[1] != NULL) {
                        Process* proc = nodes[i]->out[1]->data;
                        if (proc->is_passive) {
                                nodes[i]->out[1] = nodes[i]->out[1]->out[0];
                        } else {
                                break;
                        }
                }
        }

        /* At this point, all passive nodes should be unreachable */
        for (i = 0; i < node_vec->size;) {
                Process* proc = nodes[i]->data;
                if (proc->is_passive) {
                        process_free(proc);
                        dgraph_remove(plan->processes, &nodes[i]);
                } else {
                        ++i;
                }
        }
}

void _activate_procs(Plan* plan)
{
        Vec* node_vec = plan->processes->nodes;
        Dnode** nodes = vec_begin(node_vec);

        for (; nodes != vec_end(node_vec); ++nodes) {
                process_activate(*nodes);
        }
}

/* Run through processes and link up fifos. Input fifos are owned
 * by the process. Output fifos are just links to other processes'
 * fifos. This would read better if I called them pipes...
 */
void _make_pipes(Plan* plan)
{
        Vec* node_vec = plan->processes->nodes;
        Dnode** nodes = vec_begin(node_vec);

        for (; nodes != vec_end(node_vec); ++nodes) {
                Process* proc = (*nodes)->data;
                if ((*nodes)->out[0] != NULL) {
                        Process* proc0 = (*nodes)->out[0]->data;
                        proc->fifo_out0 = (proc->is_secondary) ? proc0->fifo_in1 : proc0->fifo_in0;
                        if (proc->fifo_out0 == NULL) {
                                fprintf (stderr, "fifo missing for `%s'\n", proc0->action_msg->data);
                                exit(EXIT_FAILURE);
                        }
                }

                if ((*nodes)->out[1] != NULL) {
                        Process* proc1 = (*nodes)->out[1]->data;
                        proc->fifo_out1 = (proc->is_secondary) ? proc1->fifo_in1 : proc1->fifo_in0;
                        if (proc->fifo_out1 == NULL) {
                                fprintf (stderr, "fifo missing for `%s'\n", proc1->action_msg->data);
                                exit(EXIT_FAILURE);
                        }
                }
        }

}

Plan* plan_build(Plan* plan, Query* query)
{
        //Plan* plan = plan_new(query->sources->size);

        /* Query */
        _from(plan, query);
        _where(plan, query);
        _group(plan, query);
        _having(plan, query);
        _operation(plan, query);
        _limit(plan, query);

        /* In case we don't have root process(es),
         * this will assign *one*.
         */
        //dgraph_get_roots(plan->processes);
        _clear_passive(plan);

        /* Reset root vec after passive removed */
        dgraph_get_roots(plan->processes);
        _activate_procs(plan);
        _make_pipes(plan);

        return plan;
}

int build_plans(Vec* plans, Queue* query_list)
{
        Queue* node = query_list;

        for (; node; node = node->next) {
                Query* query = node->data;
                Plan* plan = vec_add_one(plans);
                plan_construct(plan, query->sources->size);
                plan_build(plan, query);
        }

        return 0;
}

void _col_sep(int n)
{
        for (; n > 0; --n) {
                fputc(' ', stderr);
        }
        fputs(PLAN_COLUMN_SEP, stderr);
}

void _print_plan(Plan* plan)
{
        Dgraph* proc_graph = plan->processes;
        Vec* nodes = proc_graph->nodes;

        /* retrieve longest message */
        int max_len = strlen("BRANCH 0");

        Dnode** it = vec_begin(nodes);
        for (; it != vec_end(nodes); ++it) {
                Process* proc = (*it)->data;
                int len = proc->action_msg->size;
                if (len > max_len) {
                        max_len = len;
                }
        }


        /* Print Header */
        fputs("NODE", stderr);
        _col_sep(max_len - strlen("NODE"));
        fputs("BRANCH 0", stderr);
        _col_sep(max_len - strlen("BRANCH 0"));
        fputs("BRANCH 1\n", stderr);

        int i = 0;
        for (i = 0; i < max_len; ++i) {
                fputc('=', stderr);
        }
        _col_sep(0);
        for (i = 0; i < max_len; ++i) {
                fputc('=', stderr);
        }
        _col_sep(0);
        for (i = 0; i < max_len; ++i) {
                fputc('=', stderr);
        }


        /* Print adjacency list */
        for (it = vec_begin(nodes); it != vec_end(nodes); ++it) {
                fputc('\n', stderr);

                Process* proc = (*it)->data;
                int len = proc->action_msg->size;
                fputs(proc->action_msg->data, stderr);
                _col_sep(max_len - len);
                len = 0;

                if ((*it)->out[0] != NULL) {
                        proc = (*it)->out[0]->data;
                        len = proc->action_msg->size;
                        fputs(proc->action_msg->data, stderr);
                }
                _col_sep(max_len - len);

                if ((*it)->out[1] != NULL) {
                        proc = (*it)->out[1]->data;
                        fputs(proc->action_msg->data, stderr);
                }
        }
}

void print_plans(Vec* plans)
{
        Plan* it = vec_begin(plans);
        int i = 0;
        for (; it != vec_end(plans); ++it) {
                fprintf(stderr, "\nQUERY %d\n", ++i);
                _print_plan(it);
                fputs("\n", stderr);
        }
}
