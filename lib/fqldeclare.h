#ifndef FQLDECLARE_H
#define FQLDECLARE_H

#include "query.h"
#include "variable.h"

struct fqldeclare {
	enum op oper_type;
	struct expression* init_expr;
	int variable_idx;
};
typedef struct fqldeclare fqldeclare;

struct fqldeclare* fqldeclare_construct(struct fqldeclare*);
void fqldeclare_destroy(struct fqldeclare*);
void fqldeclare_set_init_expression(struct fqldeclare*, struct expression*);
void fqldeclare_preop(struct fqldeclare*, struct query*);
int fqldeclare_apply_process(struct query*, struct fqlplan*);

#endif  /* FQLDECLARE_H */
