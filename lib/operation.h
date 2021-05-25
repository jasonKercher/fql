#ifndef OPERATION_H
#define OPERATION_H


#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "fqlhandle.h"
#include "fqlplan.h"
#include "util/queue.h"

/** operation **/
enum op {
	OP_NONE,
	OP_SELECT,
	OP_DELETE,
	OP_UPDATE,
};

struct vec* op_get_validation_list(void* op);
void op_preop(struct fql_handle*);
_Bool op_has_delim(enum op*);
void op_set_delim(enum op*, const char*);
void op_apply_process(struct query*, struct fql_plan*);
//int op_finish(enum op*);
void op_preflight(struct query*);
void op_destroy(enum op*);

#ifdef __cplusplus
}
#endif

#endif /* OPERATION_H */


