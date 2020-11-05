#include "plan.h"

#include <stdbool.h>
#include <stdint.h>

#include "column.h"
#include "query.h"
#include "util/util.h"

/**
 * Implementation for struct plan
 *
 * struct plan is basically a decision tree
 * where each node represents a process
 */


/* Constructor (private)
 */
struct plan* _plan_new()
{
        struct plan* new_plan = NULL;
        malloc_(new_plan, sizeof(*new_plan));

        *new_plan = (struct plan) {
                 process_new("start")     /* processes */
                ,process_new("OP_TRUE")   /* op_true */
                ,process_new("OP_FALSE")  /* op_false */
                ,NULL                     /* current */
        };

        return new_plan;
}

/* recursively traverse logic tree and
 * rebuild it as a process tree
 */
void _traverse_logic(struct process* proc,
                     struct dnode* node,
                     struct process* proc_true,
                     struct process* proc_false)
{
        struct logic* logic = node->data;
        if (logic->proc == NULL) {
                logic_get_description(logic, proc->action_msg);
        } else {
                logic->proc = proc;
        }

        int branch = 0;
        for (; branch < 2; ++branch) {
                struct logic* next = node->out[branch]->data;
                if (next->comp_type == COMP_TRUE) {
                        proc->out[branch] = proc_true;
                } else if (next->comp_type == COMP_FALSE) {
                        proc->out[branch] = proc_false;
                } else {
                        if (next->proc == NULL) {
                                proc->out[branch] = process_new("");
                        } else {
                                proc->out[branch] = next->proc;
                        }
                        _traverse_logic(proc->out[branch],
                                        node->out[branch],
                                        proc_true,
                                        proc_false);
                }
        }
}

/* return logic beginning process
 * allocate and assign ending 
 * processes for true and false
 */
struct process* _logic_to_process(struct process** proc_true,
                                  struct process** proc_false,
                                  struct dgraph* tree)
{
        struct process* proc_begin = process_new("");

        *proc_true = process_new("End logic: TRUE");
        *proc_false = process_new("End logic: FALSE");

        struct dnode* begin = tree->nodes->vector[0];

        _traverse_logic(proc_begin, begin, *proc_true, *proc_false);

        return proc_begin;
}



void _plan_from(struct plan* plan, struct query* query)
{
        char action_msg[ACTION_MAX] = "";
        plan->current = plan->processes;

        if (query->sources->size) {
                struct source* src = query->sources->vector[0];
                sprintf(action_msg, "%s: %s", src->table->reader->file_name,
                                              "stream read");
                struct process* from_proc = process_new(action_msg);
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

                plan->current->out[0] = join_proc;

                sprintf(action_msg, "%s: %s", src->table->reader->file_name,
                                              "mmap read");
                struct process* new_table = process_new(action_msg);

                if (src->condition != NULL) {
                        struct process* proc_true = NULL;
                        struct process* proc_false = NULL;
                        join_proc->out[0] = _logic_to_process(&proc_true,
                                                              &proc_false,
                                                              src->condition->tree);
                        plan->current = proc_true;
                } else {
                        plan->current = join_proc;
                }
        }
}

void _plan_where(struct plan* plan, struct query* query)
{
        if (query->where == NULL) {
                return;
        }

        struct process* proc_true = NULL;
        struct process* proc_false = NULL;

        plan->current->out[0] = _logic_to_process(&proc_true,
                                                  &proc_false,
                                                  query->where->tree);
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

void _print_plan(struct process* proc, struct stack** indents, int branch)
{
        if (branch == 0) {
                _print_indent(*indents);
        }

        fputs(proc->action_msg, stderr);

        if (proc->out[1] != NULL) {
                fputs(" --> ", stderr);
                stack_push(indents, (void*) 5 + strlen(proc->action_msg));
                _print_plan(proc->out[1], indents, 1);
                stack_pop(indents);
        }

        if (proc->out[0] != NULL) { 
                _print_indent(*indents);
                fputc('|', stderr);
                _print_plan(proc->out[0], indents, 0);
        }
}

void print_plans(struct queue* plans)
{
        int i = 0;
        for (; plans; plans = plans->next) {
                fprintf(stderr, "QUERY %d\n", ++i);
                struct plan* plan = plans->data;
                struct stack* indents = NULL;
                _print_plan(plan->processes, &indents, 0);
                fputs("\n", stderr);
        }
}
