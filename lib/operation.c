#include "operation.h"

#include "select.h"
#include "util/util.h"


Schema* op_get_schema(void* op)
{
        enum op* type = op;

        switch(*type) {
        case OP_SELECT:
                return ((Select*) op)->schema;
        default:
                return NULL;
        }

        return NULL;
}

