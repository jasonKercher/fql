#ifndef OPERATION_H
#define OPERATION_H


#ifdef __cplusplus
extern "C" {
#endif

#include "plan.h"
#include "util/queue.h"

/** Operation **/
enum op {
        OP_NONE,
        OP_SELECT,
        OP_DELETE,
        OP_UPDATE,
};

struct vec* op_get_validation_list(void* op);
void op_apply_process(void* op, struct plan*);

#ifdef __cplusplus
}
#endif

#endif /* OPERATION_H */


