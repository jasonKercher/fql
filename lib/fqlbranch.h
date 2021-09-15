#ifndef FQLBRANCH_H
#define FQLBRANCH_H

#include "query.h"
#include "fqlplan.h"
#include "util/vec.h"

struct fqlbranch {
	enum op oper_type;
	struct vec conditions;
	unsigned* next_query_idx_ref;
	unsigned next_idx;
};
typedef struct fqlbranch fqlbranch;

struct fqlbranch* fqlbranch_construct(struct fqlbranch*, struct query*);
void fqlbranch_destroy(struct fqlbranch*);

void fqlbranch_add_logicgroup(struct fqlbranch*, struct logicgroup*);

void fqlbranch_preop(struct fqlbranch*, struct query*);
int fqlbranch_apply_process(struct query*, struct fqlplan*);

#endif /* FQLBRANCH_H */
