#ifndef DTREE_H
#define DTREE_H

#include <stdbool.h>

#include "vec.h"

struct dnode {
        void* data;
        struct dnode* node[2];
        _Bool visited;
};

struct dtree {
        struct vec* nodes;
        struct dnode* current;
        struct dnode* newest;
};

struct dtree* dtree_new();
void dtree_free(struct dtree*);

void dtree_add(struct dtree*, void*);

/* depth first */
void dtree_traverse_start(struct dtree*);
struct dnode* dtree_traverse(struct dtree*);

#endif /* DTREE_H */
