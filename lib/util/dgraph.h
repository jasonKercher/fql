#ifndef DGRAPH_H
#define DGRAPH_H

#include <stdbool.h>

#include "vec.h"
#include "stack.h"

struct dnode {
        void* data;
        struct dnode* out[2];
        _Bool visited;
};
typedef struct dnode Dnode;

struct dnode* dnode_new(void*);
void* dnode_free(struct dnode*);

struct dgraph {
        struct vec* nodes;
        struct dnode* newest;
        struct stack* _trav;
        int _trav_idx;
};
typedef struct dgraph Dgraph;

struct dgraph* dgraph_new();
void dgraph_shallow_free(struct dgraph*);
void dgraph_free(struct dgraph*);

struct dnode* dgraph_add_node(struct dgraph*, struct dnode*);
struct dnode* dgraph_add_data(struct dgraph*, void*);
void dgraph_extend(struct dgraph* dest, struct dgraph* src);

void* dgraph_remove(struct dgraph*, struct dnode*);

struct dnode* dgraph_traverse_begin(struct dgraph*);
struct dnode* dgraph_traverse(struct dgraph*);

#endif /* DGRAPH_H */
