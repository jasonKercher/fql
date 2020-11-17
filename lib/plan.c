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
 * Implementation for struct plan
 *
 * struct plan is basically a decision graph
 * where each node represents a process
 */


/* Constructor (private)
 */
struct plan* _plan_new()
{
        struct plan* new_plan = NULL;
        malloc_(new_plan, sizeof(*new_plan));

        *new_plan = (struct plan) {
                 dgraph_new()                        /* processes */
                ,dnode_new(process_new("OP_TRUE"))   /* op_true */
                ,dnode_new(process_new("OP_FALSE"))  /* op_false */
                ,NULL                                /* current */
        };

        dgraph_add_data(new_plan->processes, process_new("start"));
        new_plan->current = new_plan->processes->newest;

        return new_plan;
}

/* build process nodes from logic graph
 * assign processes for true and false
 * return beginning process
 */
struct dnode* _logic_to_process(struct dgraph* proc_graph,
                                struct dgraph* logic_graph,
                                struct dnode** proc_node_true,
                                struct dnode** proc_node_false)

{
        struct dnode* return_node = NULL;

        /* build all process nodes */
        int i = 0;
        for (; i < logic_graph->nodes->size; ++i) {
                struct dnode* lnode = logic_graph->nodes->vector[i];
                struct logic* logic = lnode->data;
                struct process* proc = process_new("");
                logic_get_description(logic, proc->action_msg);

                struct dnode* proc_node = dnode_new(proc);
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
        for (i = 0; i < logic_graph->nodes->size; ++i) {
                struct dnode* lnode = logic_graph->nodes->vector[i];
                struct logic* logic = lnode->data;
                if (lnode->out[0] != NULL) {
                        logic->proc_node->out[0] = ((struct logic*)lnode->out[0]->data)->proc_node;
                }
                if (lnode->out[1] != NULL) {
                        logic->proc_node->out[1] = ((struct logic*)lnode->out[1]->data)->proc_node;
                }
        }

        return return_node;
}



void _plan_from(struct plan* plan, struct query* query)
{
        char action_msg[ACTION_MAX] = "";
        //plan->current = plan->processes;

        if (query->sources->size) {
                struct source* src = query->sources->vector[0];
                sprintf(action_msg, "%s: %s", src->table->reader->file_name, "stream read");
                struct dnode* from_proc = dgraph_add_data(plan->processes, process_new(action_msg));
                plan->current->out[0] = from_proc;
                plan->current = from_proc;
        }

        int i = 1;
        for (; i < query->sources->size; ++i) {
                struct source* src = query->sources->vector[i];

                struct process* join_proc = NULL;

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

                struct dnode* join_proc_node = dgraph_add_data(plan->processes, join_proc);

                plan->current->out[0] = join_proc_node;

                sprintf(action_msg,
                        "%s: %s",
                        src->table->reader->file_name,
                        "mmap read");
                struct dnode* read_proc = dgraph_add_data(plan->processes, process_new(action_msg));
                read_proc->out[0] = join_proc_node;

                if (src->condition != NULL) {
                        struct dnode* proc_true = NULL;
                        struct dnode* proc_false = NULL;

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

void _plan_where(struct plan* plan, struct query* query)
{
        if (query->where == NULL) {
                return;
        }

        struct dnode* proc_true = NULL;
        struct dnode* proc_false = NULL;

        plan->current->out[0] = _logic_to_process(plan->processes,
                                                  query->where->tree,
                                                  &proc_true,
                                                  &proc_false);
        plan->current = proc_true;

        /* TODO: handle proc_false */
}

void _plan_group(struct plan* plan, struct query* query) { }
void _plan_having(struct plan* plan, struct query* query) { }
void _plan_operation(struct plan* plan, struct query* query)
{
        /* TODO */
        plan->current->out[0] = plan->op_true;
}

void _plan_limit(struct plan* plan, struct query* query) { }

struct plan* _build_plan(struct query* query)
{
        struct plan* new_plan = _plan_new();

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
        struct plan* plan = generic_plan;
        /* TODO */
        free_(plan);
}

int build_plans(struct queue** plans, struct queue* query_list)
{
        struct queue* node = query_list;

        for (; node; node = node->next) {
                struct query* query = node->data;
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

void _print_plan(struct plan* plan)
{
        struct dgraph* proc_graph = plan->processes;
        struct vec* nodes = proc_graph->nodes;

        /* retrieve longest message */
        int max_len = strlen("BRANCH 0");
        int i = 0;
        for (; i < nodes->size; ++i) {
                struct dnode* node = nodes->vector[i];

                struct process* proc = node->data;
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
        for (i = 0; i < nodes->size; ++i) {
                fputc('\n', stderr);
                struct dnode* node = nodes->vector[i];

                struct process* proc = node->data;
                int len = strlen(proc->action_msg);
                fputs(proc->action_msg, stderr);
                _col_sep(max_len - len);
                len = 0;

                if (node->out[0] != NULL) {
                        proc = node->out[0]->data;
                        len = strlen(proc->action_msg);
                        fputs(proc->action_msg, stderr);
                }
                _col_sep(max_len - len);

                if (node->out[1] != NULL) {
                        proc = node->out[1]->data;
                        fputs(proc->action_msg, stderr);
                }
        }
}

void print_plans(struct queue* plans)
{
        int i = 0;
        for (; plans; plans = plans->next) {
                fprintf(stderr, "QUERY %d\n", ++i);
                struct plan* plan = plans->data;
                _print_plan(plan);
                fputs("\n", stderr);
        }
}
