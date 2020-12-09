#include "operation.h"

#include "fql.h"
#include "select.h"
#include "util/util.h"

Vec* op_get_validation_list(void* op)
{
        enum op* type = op;

        switch(*type) {
        case OP_SELECT:
                return ((Select*) op)->validation_list;
        default:
                return NULL;
        }

        return NULL;
}

void op_apply_process(void* op, Plan* plan)
{
        enum op* type = op;

        switch(*type) {
        case OP_SELECT:
                select_apply_process(op, plan);
        default:
                ;
        }
}
