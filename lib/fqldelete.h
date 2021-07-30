#ifndef FQLDELETE_H
#define FQLDELETE_H

#include <stdlib.h>

#include "operation.h"

struct node;
struct fqldelete;

typedef int (*delete_fn)(struct fqldelete*, struct node*);

struct fqldelete {
	enum op oper_type;
	struct schema* schema;
	struct writer* writer;
	delete_fn delete__;
	struct table* delete_table;
	size_t rows_affected;
	size_t top_count;
	bool has_matched_alias;
};
typedef struct fqldelete fqldelete;

struct fqldelete* fqldelete_construct(struct fqldelete*);
void fqldelete_destroy(struct fqldelete*);

void fqldelete_apply_process(struct query*, struct fql_plan*);
void fqldelete_preop(struct fqldelete*, struct query*);


#endif  /* FQLDELETE_H */
