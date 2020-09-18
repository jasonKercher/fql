#include "plan.h"

#include "query.h"
#include "util/util.h"

struct plan* plan_new(struct query* query) 
{
        struct plan* new_plan = NULL;
        malloc_(new_plan, sizeof(*new_plan));

        *new_plan = (struct plan) {
                NULL
        };

        return new_plan;
}

void plan_free(void* generic_plan)
{
        struct plan* plan = generic_plan;
        queue_free_data(&plan->process_queue);
        free_(plan);
}

int build_plans(struct queue** plans, struct queue* query_list)
{
        struct queue* query_node = query_list;
        
        for (; query_node; query_node = query_node->next) {
                struct query* query = query_node->data;
                queue_enqueue(plans, plan_new(query));
        }
        return 0;
}
