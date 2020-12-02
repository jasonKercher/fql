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
Plan* plan_new()
{
        Plan* new_plan = NULL;
        malloc_(new_plan, sizeof(*new_plan));

        return plan_init(new_plan);
}

Plan* plan_init(Plan* plan)
{
        *plan = (Plan) {
                 dgraph_new()                        /* processes */
                ,dnode_new(process_new("OP_TRUE"))   /* op_true */
                ,dnode_new(process_new("OP_FALSE"))  /* op_false */
                ,NULL                                /* current */
        };

        dgraph_add_data(plan->processes, process_new("start"));
        dgraph_add_node(plan->processes, plan->op_true);
        dgraph_add_node(plan->processes, plan->op_false);
        plan->current = plan->processes->newest;

        return plan;
}

/* build process nodes from logic graph
 * assign processes for true and false
 * return beginning process
 */
Dnode* _logic_to_process(Dgraph* proc_graph,
                         Dgraph* logic_graph,
                         Dnode** proc_node_true,
                         Dnode** proc_node_false)
{
        Dnode* return_node = NULL;

        /* build all process nodes */
        Dnode* it = vec_begin(logic_graph->nodes);
        for (; it != vec_end(logic_graph->nodes); ++it) {
                Logic* logic = it->data;
                Process* proc = process_new("");
                logic_get_description(logic, proc->action_msg);

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
                Logic* logic = it->data;
                if (it->out[0] != NULL) {
                        logic->proc_node->out[0] = ((Logic*)it->out[0]->data)->proc_node;
                }
                if (it->out[1] != NULL) {
                        logic->proc_node->out[1] = ((Logic*)it->out[1]->data)->proc_node;
                }
        }

        return return_node;
}



void _plan_from(Plan* plan, Query* query)
{
        char action_msg[ACTION_MAX] = "";
        //plan->current = plan->processes;

        Source* src = vec_begin(query->sources);
        if (query->sources->size) {
                sprintf(action_msg, "%s: %s", src->table->reader->file_name, "stream read");
                Dnode* from_proc = dgraph_add_data(plan->processes, process_new(action_msg));
                plan->current->out[0] = from_proc;
                plan->current = from_proc;
        }

        for (++src; src != vec_end(query->sources); ++src) {
                Process* join_proc = NULL;

                switch (src->join_type) {
                case JOIN_INNER:
                        join_proc = process_new("INNER JOIN");
                        break;
                case JOIN_LEFT:
                        join_proc = process_new("LEFT JOIN");
                        break;
                case JOIN_RIGHT:
                        join_proc = process_new("RIGHT JOIN");
                        break;
                case JOIN_FULL:
                        join_proc = process_new("FULL JOIN");
                        break;
                case JOIN_CROSS:
                        join_proc = process_new("CROSS JOIN");
                        break;
                case JOIN_FROM:
                        join_proc = process_new("unexpected: JOIN_FROM");
                }

                Dnode* join_proc_node = dgraph_add_data(plan->processes, join_proc);

                plan->current->out[0] = join_proc_node;

                sprintf(action_msg,
                        "%s: %s",
                        src->table->reader->file_name,
                        "mmap read");
                Dnode* read_proc = dgraph_add_data(plan->processes, process_new(action_msg));
                read_proc->out[0] = join_proc_node;

                if (src->condition != NULL) {
                        Dnode* proc_true = NULL;
                        Dnode* proc_false = NULL;

                        join_proc_node->out[0] = _logic_to_process(plan->processes,
                                                                   src->condition->tree,
                                                                   &proc_true,
                                                                   &proc_false);
                        plan->current = proc_true;
                        /* TODO: handle proc_false */
                } else {
                        plan->current = join_proc_node;
                }
        }
}

void _plan_where(Plan* plan, Query* query)
{
        if (query->where == NULL) {
                return;
        }

        Dnode* proc_true = NULL;
        Dnode* proc_false = NULL;

        plan->current->out[0] = _logic_to_process(plan->processes,
                                                  query->where->tree,
                                                  &proc_true,
                                                  &proc_false);
        plan->current = proc_true;

        /* TODO: handle proc_false */
}

void _plan_group(Plan* plan, Query* query) { }
void _plan_having(Plan* plan, Query* query) { }
void _plan_operation(Plan* plan, Query* query)
{
        /* TODO */
        plan->current->out[0] = plan->op_true;
}

void _plan_limit(Plan* plan, Query* query) { }

Plan* _build_plan(Query* query)
{
        Plan* new_plan = plan_new();

        _plan_from(new_plan, query);
        _plan_where(new_plan, query);
        _plan_group(new_plan, query);
        _plan_having(new_plan, query);
        _plan_operation(new_plan, query);
        _plan_limit(new_plan, query);

        return new_plan;
}

void plan_free(void* generic_plan)
{
        Plan* plan = generic_plan;
        /* TODO */
        free_(plan);
}

int build_plans(Queue** plans, Queue* query_list)
{
        Queue* node = query_list;

        for (; node; node = node->next) {
                Query* query = node->data;
                queue_enqueue(plans, _build_plan(query));
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

        Dnode* it = vec_begin(nodes);
        for (; it != vec_end(nodes); ++it) {
                Process* proc = it->data;
                int len = strlen(proc->action_msg);
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

                Process* proc = it->data;
                int len = strlen(proc->action_msg);
                fputs(proc->action_msg, stderr);
                _col_sep(max_len - len);
                len = 0;

                if (it->out[0] != NULL) {
                        proc = it->out[0]->data;
                        len = strlen(proc->action_msg);
                        fputs(proc->action_msg, stderr);
                }
                _col_sep(max_len - len);

                if (it->out[1] != NULL) {
                        proc = it->out[1]->data;
                        fputs(proc->action_msg, stderr);
                }
        }
}

void print_plans(Queue* plans)
{
        int i = 0;
        for (; plans; plans = plans->next) {
                fprintf(stderr, "QUERY %d\n", ++i);
                Plan* plan = plans->data;
                _print_plan(plan);
                fputs("\n", stderr);
        }
}
