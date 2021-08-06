#ifndef FQLUPDATE_H
#define FQLUPDATE_H

#include <stdlib.h>

#include "operation.h"

struct node;
struct fqlupdate;
struct expression;

typedef int (*update_fn)(struct fqlupdate*, struct node*);

struct fqlupdate {
	enum op oper_type;
	struct schema* schema;
	struct writer* writer;
	update_fn update__;
	update_fn noupdate__;
	size_t update_idx;
	size_t rows_affected;
	size_t top_count;
	struct vec update_expressions;
	struct vec set_columns;
	struct vec value_expressions;
	unsigned table_idx;
	unsigned rownum;
	enum filter_state state;
	bool has_matched_alias;
};
typedef struct fqlupdate fqlupdate;

struct fqlupdate* fqlupdate_construct(struct fqlupdate*);
void fqlupdate_destroy(struct fqlupdate*);

int fqlupdate_add_expression(struct fqlupdate*, const struct expression*);
int fqlupdate_resolve_additional(struct fqlupdate*, struct query*);
int fqlupdate_apply_process(struct query*, struct fql_plan*);
void fqlupdate_preop(struct fqlupdate*, struct query*);

/* operation.c? */
int fqlupdate_resolve_final_types(struct fqlupdate*);

#endif /* FQLUPDATE_H */
