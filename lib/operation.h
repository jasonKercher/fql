#ifndef OPERATION_H
#define OPERATION_H


#ifdef __cplusplus
extern "C" {
#endif

#include "fqlplan.h"
#include "util/queue.h"

/** Operation **/
enum op {
        OP_NONE,
        OP_SELECT,
        OP_DELETE,
        OP_UPDATE,
};

struct vec* op_get_validation_list(void* op);
void op_apply_process(void* op, struct fql_plan*);
void op_use_non_api(void* op);

#ifdef __cplusplus
}
#endif

#endif /* OPERATION_H */


