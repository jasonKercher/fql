#include "plan.h"

#include <stdbool.h>
#include <stdint.h>

#include "column.h"
#include "logic.h"
#include "process.h"
#include "query.h"
#include "util/dgraph.h"
#include "util/util.h"

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

/* return logic beginning process
 * allocate and assign ending
 * processes for true and false
 */
struct dgraph* _logic_to_process(struct dnode** proc_node_true,
                                 struct dnode** proc_node_false,
                                 struct dgraph* logic_graph)
{
        struct dgraph* proc_graph = dgraph_new();

        /* build all process nodes */
        int i = 0;
        for (; i < logic_graph->nodes->size; ++i) {
                struct dnode* lnode = logic_graph->nodes->vector[i];
                struct logic* logic = lnode->data;
                struct process* proc = process_new("");
                logic_get_description(logic, proc->action_msg);

                struct dnode* proc_node = dnode_new(proc);
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

        return proc_graph;
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
                struct process* new_table = process_new(action_msg);

                if (src->condition != NULL) {
                        struct dnode* proc_true = NULL;
                        struct dnode* proc_false = NULL;
                        struct dgraph* lgraph = _logic_to_process(&proc_true, &proc_false, src->condition->tree);
                        join_proc_node->out[0] = lgraph->nodes->vector[0];
                        plan->current = proc_true;
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

        struct dgraph* lgraph = _logic_to_process(&proc_true,
                                                 &proc_false,
                                                 query->where->tree);
        plan->current->out[0] = lgraph->nodes->vector[0];
        plan->current = proc_true;
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

void _print_indent(struct stack* indents)
{
        fputs("\n", stderr);
        struct stack* btm = stack_bottom(indents);
        int first_column = 0;

        for (; btm; btm = btm->prev) {
                int i = 0;
                uintptr_t width = (uintptr_t) indents->data;
                for (; i < width - first_column; ++i) {
                        fputc(' ', stderr);
                }
                first_column = 1;
                if (btm->prev) {
                        fputc('|', stderr);
                }
        }
}

void _print_plan(struct dnode* proc_node, struct stack** indents, int branch)
{
        if (branch == 0) {
                _print_indent(*indents);
        }

        struct process* proc = proc_node->data;

        fputs(proc->action_msg, stderr);

        if (proc_node->out[1] != NULL) {
                fputs(" --> ", stderr);
                stack_push(indents, (void*) 5 + strlen(proc->action_msg));
                _print_plan(proc_node->out[1], indents, 1);
                stack_pop(indents);
        }

        if (proc_node->out[0] != NULL) {
                _print_indent(*indents);
                fputc('|', stderr);
                _print_plan(proc_node->out[0], indents, 0);
        }
}

void print_plans(struct queue* plans)
{
        int i = 0;
        for (; plans; plans = plans->next) {
                fprintf(stderr, "QUERY %d\n", ++i);
                struct plan* plan = plans->data;
                struct stack* indents = NULL;
                _print_plan(plan->processes->nodes->vector[0], &indents, 0);
                fputs("\n", stderr);
        }
}
