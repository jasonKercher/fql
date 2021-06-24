#ifndef OPERATION_H
#define OPERATION_H

#include "fqlhandle.h"
#include "fqlplan.h"
#include "util/queue.h"

struct schema;

/** operation **/
enum op {
	OP_NONE,
	OP_SELECT,
	OP_DELETE,
	OP_UPDATE,
};

struct vec* op_get_validation_list(void* op);
void op_preop(struct fql_handle*);
void op_set_schema(enum op*, const struct schema*);
void op_set_delim(enum op*, const char* delim);
void op_apply_process(struct query*, struct fql_plan*);
void op_preflight(struct query*);
void op_destroy(enum op*);

#endif /* OPERATION_H */
