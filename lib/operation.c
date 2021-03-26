#include "operation.h"

#include "fql.h"
#include "select.h"
#include "util/util.h"

Vec* op_get_validation_list(void* op)
{
        enum op* type = op;

        switch (*type) {
        case OP_SELECT:
                return ((Select*) op)->schema->columns;
        default:
                return NULL;
        }

        return NULL;
}

void op_preop(struct fql_handle* fql)
{
        Query* query = fql->query_list->data;
        enum op* type = query->op;

        switch (*type) {
        case OP_SELECT:
                if (fql->props.print_header) {
                        select_preop(query->op, query);
                }
                break;
        default:
                ;
        }
}

void op_apply_process(Query* query, Plan* plan)
{
        enum op* type = query->op;

        switch (*type) {
        case OP_SELECT:
                select_apply_process(query, plan);
                break;
        default:
                ;
        }
}

void op_free(void* op)
{
        enum op* type = op;

        switch (*type) {
        case OP_SELECT:
                select_free(op);
                break;
        default:
                ;
        }
}
