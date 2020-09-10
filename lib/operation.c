#include "query.h"

#include "util/util.h"

operation_t* operation_new()
{
        operation_t* new_operation = NULL;
        malloc_(new_operation, sizeof(*new_operation));

        *new_operation = (operation_t) {
                 table_new()    /* output_table */
                ,OP_NONE        /* type */
        };

        return new_operation;
}

