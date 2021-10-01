#ifndef FQLBRANCH_H
#define FQLBRANCH_H

#include "fql.h"
#include "query.h"
#include "scope.h"
#include "fqlplan.h"
#include "util/vec.h"

struct fqlbranch {
	enum fql_operation oper_type;
	struct logicgroup* condition;
	int* next_query_idx_ref;
	struct query* last_true_block_query;
	struct scope* scope;
	struct scope* else_scope;
	int true_idx;
	int false_idx;
	bool expect_else;
	bool is_elseif;
};
typedef struct fqlbranch fqlbranch;

struct fqlbranch* fqlbranch_construct(struct fqlbranch*,
                                      struct fqlhandle*,
                                      struct query*,
                                      enum fql_operation);
void fqlbranch_destroy(struct fqlbranch*);

void fqlbranch_add_logicgroup(struct fqlbranch*, struct logicgroup*);

void fqlbranch_preop(struct fqlbranch*, struct query*);
int fqlbranch_apply_process(struct query*, struct fqlplan*);

#endif /* FQLBRANCH_H */
