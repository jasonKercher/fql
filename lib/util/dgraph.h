#ifndef DGRAPH_H
#define DGRAPH_H

#include <stdbool.h>

#include "vec.h"
#include "stack.h"
#include "fifo.h"

struct dnode {
        void* data;
        struct dnode* out[2];
        int visit_count;
        _Bool is_root;
};
typedef struct dnode Dnode;

struct dnode* dnode_new(void*);
struct dnode* dnode_construct(struct dnode*, void*);
void* dnode_free(struct dnode*);

struct dgraph {
        struct vec* nodes;
        struct dnode* newest;
        struct fifo* _trav;
        struct vec* _roots;
        unsigned _root_idx;
        _Bool _roots_good;
};
typedef struct dgraph Dgraph;

struct dgraph* dgraph_new();
struct dgraph* dgraph_construct(struct dgraph*);
void dgraph_shallow_free(struct dgraph*);
void dgraph_free(struct dgraph*);

struct dnode* dgraph_add_node(struct dgraph*, struct dnode*);
struct dnode* dgraph_add_data(struct dgraph*, void*);
void dgraph_extend(struct dgraph* dest, struct dgraph* src);

void* dgraph_remove(struct dgraph*, struct dnode**);

struct vec* dgraph_get_roots(struct dgraph*);
void dgraph_traverse_reset(struct dgraph*);
struct dnode* dgraph_traverse(struct dgraph*);

#endif /* DGRAPH_H */
