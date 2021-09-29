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

enum fql_operation;

int op_preop(struct query*, struct fqlhandle*);
struct vec* op_get_expressions(enum fql_operation*);
struct vec* op_get_additional_exprs(enum fql_operation*);
struct schema* op_get_schema(enum fql_operation*);
const char* op_get_table_name(enum fql_operation*);
void op_match_table_alias(enum fql_operation*, struct table*);
void op_set_table(enum fql_operation*, struct table*);
void op_set_table_name(enum fql_operation*, const char*);
void op_set_top_count(enum fql_operation*, size_t);
void op_set_writer(enum fql_operation*, struct writer*);
void op_set_schema(enum fql_operation*, const struct schema*);
void op_set_rec_terminator(enum fql_operation*, const char* term);
void op_set_delim(enum fql_operation*, const char* delim);
void op_assign_rownum_ref(enum fql_operation*, struct expression*);
int op_apply_process(struct query*, struct fqlplan*, bool is_subquery);
int op_resolve_final_types(enum fql_operation*);
int op_writer_init(struct query*, struct fqlhandle*);
int op_reset(enum fql_operation*);
void op_expand_asterisks(struct query*, bool force_expansion);

void op_destroy(enum fql_operation*);

#endif /* OPERATION_H */
