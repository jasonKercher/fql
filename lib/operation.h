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

void op_preop(struct fql_handle*);
struct vec* op_get_expressions(void* op);
struct schema* op_get_schema(enum op*);
void op_set_top_count(enum op*, size_t);
void op_set_schema(enum op*, const struct schema*);
void op_set_rec_terminator(enum op*, const char* term);
void op_set_delim(enum op*, const char* delim);
void op_apply_process(struct query*, struct fql_plan*, bool is_subquery);
int op_resolve_final_types(enum op*);
int op_writer_init(struct query*, struct fql_handle*);

void op_destroy(enum op*);

#endif /* OPERATION_H */
