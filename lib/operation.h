#ifndef OPERATION_H
#define OPERATION_H

#include "fqlhandle.h"
#include "fqlplan.h"

struct table;
struct schema;
struct writer;
struct expression;

enum filter_state {
	FILTER_OPEN,
	FILTER_PASSTHROUGH,
	FILTER_FILTERING,
};

/** operation **/
enum op {
	OP_NONE,
	OP_SELECT,
	OP_DELETE,
	OP_UPDATE,
};

void op_preop(struct fql_handle*);
struct vec* op_get_expressions(enum op*);
struct vec* op_get_additional_exprs(enum op*);
struct schema* op_get_schema(enum op*);
const char* op_get_table_name(enum op*);
void op_match_table_alias(enum op*, struct table*);
void op_set_table(enum op*, struct table*);
void op_set_table_name(enum op*, const char*);
void op_set_top_count(enum op*, size_t);
void op_set_writer(enum op*, struct writer*);
void op_set_schema(enum op*, const struct schema*);
void op_set_rec_terminator(enum op*, const char* term);
void op_set_delim(enum op*, const char* delim);
void op_assign_rownum_ref(enum op*, struct expression*);
int op_apply_process(struct query*, struct fql_plan*, bool is_subquery);
int op_resolve_final_types(enum op*);
int op_writer_init(struct query*, struct fql_handle*);
void op_expand_asterisks(struct query*, bool force_expansion);

void op_destroy(enum op*);

#endif /* OPERATION_H */
