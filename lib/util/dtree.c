#include "dtree.h"

#include "util.h"
#include "vec.h"

struct dtree* dtree_new()
{
        struct dtree* new_dtree = NULL;
        malloc_(new_dtree, sizeof(*new_dtree));

        *new_dtree = (struct dtree) {
                 vec_new()      /* nodes */
                ,NULL           /* current */
                ,NULL           /* newest */
        };
        
        return new_dtree;
}

void dtree_free(struct dtree* tree)
{
        vec_free(tree->nodes);
        free_(tree);
}

void dtree_add(struct dtree* tree, void* data)
{

}

void dtree_traverse_start(struct dtree* tree)
{
        tree->current = tree->nodes->vector[0];
}

struct dnode* dtree_traverse(struct dtree* tree)
{
        return tree->current;
}
