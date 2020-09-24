#include "plan.h"

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



struct process* _search_to_process(struct search* search)
{
        /* TODO */
        return NULL;
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
                from_proc->in[0] = plan->current;
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
                join_proc->in[0] = plan->current;

                sprintf(action_msg, "%s: %s", src->table->reader->file_name,
                                "mmap read");
                struct process* new_table = process_new(action_msg);

                join_proc->in[1] = new_table;

                if (src->condition != NULL) {
                        plan->current = _search_to_process(src->condition);
                        join_proc->out[0] = plan->current;
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

void _print_plan(struct plan* plan)
{
        struct process* proc = plan->processes;

        int i = 0;
        for (; proc; proc = proc->out[0]) {
                if (i) {
                        fputs("  V\n", stderr);
                }
                fputs(proc->action_msg, stderr);
                if (proc->in[1]) {
                        struct process* in1 = proc->in[1];
                        fprintf(stderr, " <-- %s", in1->action_msg);
                        if (in1->out[1]) {
                                struct process* out1 = in1->out[1];
                                fprintf(stderr, " --> %s", out1->action_msg);
                        }
                }
                fputs("\n", stderr);
                ++i;
        }
}

void print_plans(struct queue* plans)
{
        for (; plans; plans = plans->next) {
                _print_plan(plans->data);
        }
}



