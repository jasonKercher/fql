#ifndef FQLSET_H
#define FQLSET_H

#include "query.h"
#include "variable.h"

struct fqlset {
	enum fql_operation oper_type;
	struct expression* init_expr;
	int variable_idx;
};
typedef struct fqlset fqlset;

struct fqlset* fqlset_construct(struct fqlset*);
void fqlset_destroy(struct fqlset*);
void fqlset_set_init_expression(struct fqlset*, struct expression*);
void fqlset_preop(struct fqlset*, struct query*);
int fqlset_apply_process(struct query*, struct fqlplan*);

#endif /* FQLSET_H */
