#include "plan.h"

#include "query.h"
#include "util/util.h"

plan_t* plan_new(query_t* query) 
{
        plan_t* new_plan = NULL;
        malloc_(new_plan, sizeof(*new_plan));

        *new_plan = (plan_t) {
                NULL
        };

        return new_plan;
}

void plan_free(void* generic_plan)
{
        plan_t* plan = generic_plan;
        queue_free_data(&plan->process_queue);
        free_(plan);
}

int build_plans(queue_t** plans, queue_t* query_list)
{
        queue_t* query_node = query_list;
        
        for (; query_node; query_node = query_node->next) {
                query_t* query = query_node->data;
                queue_enqueue(plans, plan_new(query));
        }
        return 0;
}
