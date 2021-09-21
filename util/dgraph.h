#ifndef DGRAPH_H
#define DGRAPH_H

#include <stdbool.h>

#include "vec.h"
#include "fifo.h"

struct dnode {
	void* data;
	struct dnode* out[2];
	int visit_count;
	bool is_root;
};
typedef struct dnode dnode;

struct dnode* dnode_construct(struct dnode* restrict, void* restrict);
void dnode_destroy(struct dnode* restrict);

struct dgraph {
	struct vec* nodes;
	struct dnode* newest;
	struct fifo* _trav;
	struct vec* _roots;
	unsigned _root_idx;
	bool _roots_good;
};
typedef struct dgraph dgraph;

struct dgraph* dgraph_construct(struct dgraph* restrict);
void dgraph_shallow_free(struct dgraph* restrict);
void dgraph_shallow_destroy(struct dgraph* restrict);
void dgraph_destroy(struct dgraph* restrict);

struct dnode* dgraph_add_node(struct dgraph* restrict, struct dnode*);
struct dnode* dgraph_add_data(struct dgraph* restrict, void* restrict);
void dgraph_consume(struct dgraph* restrict dest, struct dgraph* restrict src);

void* dgraph_remove(struct dgraph* restrict, struct dnode**);

unsigned dgraph_root_count(struct dgraph* restrict);
struct vec* dgraph_get_roots(struct dgraph* restrict);
void dgraph_traverse_reset(struct dgraph* restrict);
struct dnode* dgraph_traverse(struct dgraph* restrict);

#endif /* DGRAPH_H */
