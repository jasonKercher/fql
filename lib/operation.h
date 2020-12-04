#ifndef OPERATION_H
#define OPERATION_H


#ifdef __cplusplus
extern "C" {
#endif

#include "table.h"
#include "util/queue.h"


/** Operation **/
enum op {
        OP_NONE,
        OP_SELECT,
        OP_DELETE,
        OP_UPDATE,
};

struct vec* op_get_columns(void*);

#ifdef __cplusplus
}
#endif

#endif /* OPERATION_H */


