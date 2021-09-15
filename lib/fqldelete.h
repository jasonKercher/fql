#ifndef FQLDELETE_H
#define FQLDELETE_H

#include <stdlib.h>

#include "operation.h"
#include "query.h"

struct node;
struct fqldelete;

typedef int (*delete_fn)(struct fqldelete*, struct node*);

struct fqldelete {
	enum op oper_type;
	struct schema* schema;
	struct writer* writer;
	delete_fn undelete__;
	size_t delete_idx;
	node* delete_node;
	size_t rows_affected;
	size_t top_count;
	unsigned table_idx;
	enum filter_state state;
	bool has_matched_alias;
};
typedef struct fqldelete fqldelete;

struct fqldelete* fqldelete_construct(struct fqldelete*);
void fqldelete_destroy(struct fqldelete*);

int fqldelete_apply_process(struct query*, struct fqlplan*);
void fqldelete_preop(struct fqldelete*, struct query*);


#endif  /* FQLDELETE_H */
