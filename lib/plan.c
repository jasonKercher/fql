#include "plan.h"

#include <stdbool.h>
#include <stdint.h>

#include "column.h"
#include "query.h"
#include "util/util.h"

struct plan* _plan_new()
{
        struct plan* new_plan = NULL;
        malloc_(new_plan, sizeof(*new_plan));

        *new_plan = (struct plan) {
                 process_new("start")
                ,process_new("OP_TRUE")
                ,process_new("OP_FALSE")
                ,NULL
        };

        return new_plan;
}

void _traverse_logic(struct process* proc,
                     struct logic* logic,
                     struct process* proc_true,
                     struct process* proc_false)
{
        if (logic->proc == NULL) {
                logic_get_description(logic, proc->action_msg);
        } else {
                logic->proc = proc;
        }

        int branch = 0;
        for (; branch < 2; ++branch) {
                if (logic->out[branch]->comp_type == COMP_TRUE) {
                        proc->out[branch] = proc_true;
                } else if (logic->out[branch]->comp_type == COMP_FALSE) {
                        proc->out[branch] = proc_false;
                } else {
                        if (logic->out[branch]->proc == NULL) {
                                proc->out[branch] = process_new("");
                        } else {
                                proc->out[branch] = logic->out[branch]->proc;
                        }
                        _traverse_logic(proc->out[branch], 
                                        logic->out[branch], 
                                        proc_true,
                                        proc_false);
                }
        }
}

/* Returns logic beginning process
 * Assigns logic ending process
 */
struct process* _logic_to_process(struct process** proc_true,
                                  struct process** proc_false,
                                  struct logic* logic)
{
        struct process* proc_begin = process_new("");
        
        *proc_true = process_new("End logic: TRUE");
        *proc_false = process_new("End logic: FALSE");

        _traverse_logic(proc_begin, logic, *proc_true, *proc_false);

        /* logic_procs now points to the end of the logic.
         */
        return proc_begin;
}

void _plan_from(struct plan* plan, struct query* query)
{
        char action_msg[ACTION_MAX] = "";
        plan->current = plan->processes;

        if (query->sources->size) {
                struct source* src = query->sources->data_vec[0];
                sprintf(action_msg, "%s: %s", src->table->reader->file_name,
                                          "stream read");
                struct process* from_proc = process_new(action_msg);
                plan->current->out[0] = from_proc;
                plan->current = from_proc;
        }

        int i = 1;
        for (; i < query->sources->size; ++i) {
                struct source* src = query->sources->data_vec[i];

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
                        break;
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
                                                              src->condition->begin);
                        plan->current = proc_true;
                        /** TODO - Handle proc_false for non-inner joins **/
                } else { 
                        plan->current = join_proc;
                }

        }
}

void _plan_where(struct plan* plan, struct query* query) { }
void _plan_group(struct plan* plan, struct query* query) { }
void _plan_having(struct plan* plan, struct query* query) { }
void _plan_operation(struct plan* plan, struct query* query) 
{ 
        plan->current->out[0] = plan->op_true;
        /* TODO - Replace and free with actual operation */
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
        /* TODO - free process graph */
        free_(plan);
}

int build_plans(struct queue** plans, struct queue* query_list)
{
        struct queue* query_node = query_list;

        for (; query_node; query_node = query_node->next) {
                struct query* query = query_node->data;
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
                fprintf(stderr, "QUERY %d\n", i++);
                struct plan* plan = plans->data;
                struct stack* indents = NULL;
                _print_plan(plan->processes, &indents, 0);
                fputs("\n", stderr);
        }
}



