#include "plan.h"

#include "query.h"

plan_t* build_plan(query_t* query) 
{
        plan_t* new_plan = NULL;
        malloc_(new_plan, sizeof(*new_plan));

        return new_plan;
}

int build_plans(queue_t** plans, queue_t* query_list)
{
        queue_t* query_node = query_list;
        
        for (; query_node; query_node = query_node->next) {
                query_t* query = query_node->data;
                queue_enqueue(plans, build_plan(query));
        }
        return 0;
}
