#ifndef DTREE_H
#define DTREE_H

#include <stdbool.h>

#include "vec.h"
#include "stack.h"

struct dnode {
        void* data;
        struct dnode* out[2];
        _Bool visited;
};

struct dnode* dnode_new(void*);

struct dtree {
        struct vec* nodes;
        struct dnode* newest;
        struct stack* _trav;
        int _trav_idx;
};

struct dtree* dtree_new();
void dtree_free(struct dtree*);

struct dnode* dtree_add_node(struct dtree*, struct dnode*);
struct dnode* dtree_add_data(struct dtree*, void*);

void dtree_traverse_begin(struct dtree*);
struct dnode* dtree_traverse(struct dtree*);

#endif /* DTREE_H */
