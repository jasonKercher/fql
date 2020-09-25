#include "plan.h"

#include <stdbool.h>

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

void _traverse_searches(struct process* proc,
                        struct hmap* map,
                        struct search* search,
                        struct process* proc_true,
                        struct process* proc_false)
{
        if (hmap_get_a(map, search) != NULL) {
                return;
        }

        search_get_description(search, proc->action_msg);
        hmap_set_a(map, search, proc); 

        int branch = 0;
        for (; branch < 2; ++branch) {
                if (search->out[branch] == NULL) {
                        proc->out[branch] = proc_true;
                } else {
                        proc->out[branch] = process_new("");
                        _traverse_searches(proc->out[branch], 
                                           map, 
                                           search->out[branch], 
                                           proc_true,
                                           proc_false);
                }
        }
}

/* Returns search beginning process
 * Assigns search ending process
 */
struct process* _search_to_process(struct process** proc_true,
                                   struct process** proc_false,
                                   struct search* search)
{
        /* We are traversing the whole map and may
         * reach the same node more than once so
         * hash the address of each node so it isn't 
         * allocated twice.
         */
        struct hmap* proc_map = hmap_new(100, 0); /* TODO - keep a count */

        struct process* proc_begin = process_new("");
        
        *proc_true = process_new("End search: TRUE");
        *proc_false = process_new("End search: FALSE");

        _traverse_searches(proc_begin, proc_map, search, *proc_true, *proc_false);

        /* search_procs now points to the end of the search.
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
                        struct process* proc_true = NULL;
                        struct process* proc_false = NULL;
                        join_proc->out[0] = _search_to_process(&proc_true,
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



