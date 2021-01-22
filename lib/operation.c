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

void op_apply_process(void* op, Plan* plan)
{
        enum op* type = op;

        switch (*type) {
        case OP_SELECT:
                select_apply_process(op, plan);
        default:
                ;
        }
}

void op_use_non_api(void* op)
{
        enum op* type = op;

        switch (*type) {
        case OP_SELECT:
                select_use_non_api(op);
        default:
                ;
        }
}

void op_connect_api(void* op, Vec* api)
{
        enum op* type = op;

        switch (*type) {
        case OP_SELECT:
                select_connect_api(op, api);
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
        default:
                ;
        }
}
