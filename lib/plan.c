#include "plan.h"

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


/* Constructor (private)
 */
Plan* plan_new(int source_total)
{
        Plan* new_plan = NULL;
        malloc_(new_plan, sizeof(*new_plan));

        return plan_init(new_plan, source_total);
}

Plan* plan_init(Plan* plan, int source_total)
{
        *plan = (Plan) {
                 dgraph_new()                                      /* processes */
                ,dnode_new(process_new("OP_TRUE", source_total))   /* op_true */
                ,dnode_new(process_new("OP_FALSE", source_total))  /* op_false */
                ,NULL                                              /* current */
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
        Plan* plan = generic_plan;
        /* TODO */
        free_(plan);
}

/* build process nodes from logic graph
 * assign processes for true and false
 * return beginning process
 */
Dnode* _logic_to_process(Dgraph* proc_graph,
                         Dgraph* logic_graph,
                         Dnode** proc_node_true,
                         Dnode** proc_node_false,
                         int source_count)
{
        Dnode* return_node = NULL;

        /* build all process nodes */
        Dnode** it = vec_begin(logic_graph->nodes);
        for (; it != vec_end(logic_graph->nodes); ++it) {
                Logic* logic = (*it)->data;
                Process* proc = process_new("", source_count);
                logic_assign_process(logic, proc);

                Dnode* proc_node = dnode_new(proc);
                if (return_node == NULL) {
                        return_node = proc_node;
                }
                dgraph_add_node(proc_graph, proc_node);
                logic->proc_node = proc_node;
                if (logic->comp_type == COMP_TRUE) {
                        *proc_node_true = proc_node;
                } else {
                        *proc_node_false = proc_node;
                }
        }

        /* link process all nodes */
        for (it = vec_begin(logic_graph->nodes); it != vec_end(logic_graph->nodes); ++it) {
                Logic* logic = (*it)->data;
                if ((*it)->out[0] != NULL) {
                        logic->proc_node->out[0] = ((Logic*)(*it)->out[0]->data)->proc_node;
                }
                if ((*it)->out[1] != NULL) {
                        logic->proc_node->out[1] = ((Logic*)(*it)->out[1]->data)->proc_node;
                }
        }

        return return_node;
}



void _from(Plan* plan, Query* query)
{
        if (query->sources->size == 0) {
                return;
        }

        String* action_msg = string_new();
        Source* src = vec_begin(query->sources);

        string_sprintf(action_msg, "%s: %s", src->table->reader->file_name, "stream read");

        Process* from_proc = process_new(action_msg->data, ++plan->source_count);
        from_proc->action = &fql_read;
        from_proc->proc_data = src->table->reader;

        Dnode* from_node = dgraph_add_data(plan->processes, from_proc);
        from_node->is_root = true;

        plan->current->out[0] = from_node;
        plan->current = from_node;

        for (++src; src != vec_end(query->sources); ++src) {
                Process* join_proc = NULL;

                switch (src->join_type) {
                case JOIN_INNER:
                        join_proc = process_new("INNER JOIN", ++plan->source_count);
                        break;
                case JOIN_LEFT:
                        join_proc = process_new("LEFT JOIN", ++plan->source_count);
                        break;
                case JOIN_RIGHT:
                        join_proc = process_new("RIGHT JOIN", ++plan->source_count);
                        break;
                case JOIN_FULL:
                        join_proc = process_new("FULL JOIN", ++plan->source_count);
                        break;
                case JOIN_CROSS:
                        join_proc = process_new("CROSS JOIN", ++plan->source_count);
                        break;
                case JOIN_FROM:
                        join_proc = process_new("unexpected: JOIN_FROM", ++plan->source_count);
                }

                Dnode* join_proc_node = dgraph_add_data(plan->processes, join_proc);

                plan->current->out[0] = join_proc_node;

                string_sprintf(action_msg,
                               "%s: %s",
                               src->table->reader->file_name,
                               "mmap read");

                /* Root node only will only have one source */
                Process* read_proc = process_new(action_msg->data, 1);
                read_proc->proc_data = src->table->reader;
                read_proc->action = &fql_read;
                read_proc->is_secondary = true;

                Dnode* read_node = dgraph_add_data(plan->processes, read_proc);
                read_node->is_root = true;
                read_node->out[0] = join_proc_node;

                if (src->condition != NULL) {
                        Dnode* proc_true = NULL;
                        Dnode* proc_false = NULL;

                        join_proc_node->out[0] = _logic_to_process(plan->processes,
                                                                   src->condition->tree,
                                                                   &proc_true,
                                                                   &proc_false,
                                                                   plan->source_count
                                                                   );
                        plan->current = proc_true;
                        proc_false->out[0] = plan->op_false;
                } else {
                        plan->current = join_proc_node;
                }
        }
}

void _where(Plan* plan, Query* query)
{
        if (query->where == NULL) {
                return;
        }

        Dnode* proc_true = NULL;
        Dnode* proc_false = NULL;

        plan->current->out[0] = _logic_to_process(plan->processes,
                                                  query->where->tree,
                                                  &proc_true,
                                                  &proc_false,
                                                  plan->source_count);
        plan->current = proc_true;
        proc_false->out[0] = plan->op_false;
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
        op_apply_process(query->op, plan);
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
        /* In case we don't have a root process,
         * this will assign them.
         */
        dgraph_get_roots(plan->processes);

        Vec* node_vec = plan->processes->nodes;
        Dnode** nodes = vec_begin(node_vec);

        for (; nodes != vec_end(node_vec); ++nodes) {
                process_activate(*nodes);
        }
}

/* Run through processes and link up fifos. Input fifos are
 * owned by the process. Output fifos are just links to other
 * processes' fifos.
 */
void _make_pipes(Plan* plan)
{
        Vec* node_vec = plan->processes->nodes;
        Dnode** nodes = vec_begin(node_vec);

        for (; nodes != vec_end(node_vec); ++nodes) {
                Process* proc = (*nodes)->data;
                if ((*nodes)->out[0] == NULL) {
                        continue;
                }
                Process* proc0 = (*nodes)->out[0]->data;
                proc->fifo_out0 = (proc->is_secondary) ? proc0->fifo_in1 : proc0->fifo_in0;
                if (proc->fifo_out0 == NULL) {
                        fprintf (stderr, "fifo missing for `%s'\n", proc0->action_msg->data);
                        exit(EXIT_FAILURE);
                }

                if ((*nodes)->out[1] == NULL) {
                        continue;
                }
                Process* proc1 = (*nodes)->out[1]->data;
                proc->fifo_out1 = (proc->is_secondary) ? proc1->fifo_in1 : proc1->fifo_in0;
                if (proc->fifo_out1 == NULL) {
                        fprintf (stderr, "fifo missing for `%s'\n", proc1->action_msg->data);
                        exit(EXIT_FAILURE);
                }
        }

}

Plan* plan_build(Query* query)
{
        Plan* plan = plan_new(query->sources->size);

        /* Query */
        _from(plan, query);
        _where(plan, query);
        _group(plan, query);
        _having(plan, query);
        _operation(plan, query);
        _limit(plan, query);

        _clear_passive(plan);
        _activate_procs(plan);
        _make_pipes(plan);

        return plan;
}

int build_plans(Queue** plans, Queue* query_list)
{
        Queue* node = query_list;

        for (; node; node = node->next) {
                Query* query = node->data;
                queue_enqueue(plans, plan_build(query));
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

void print_plans(Queue* plans)
{
        int i = 0;
        for (; plans; plans = plans->next) {
                fprintf(stderr, "\nQUERY %d\n", ++i);
                Plan* plan = plans->data;
                _print_plan(plan);
                fputs("\n", stderr);
        }
}
