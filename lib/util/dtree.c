#include "dtree.h"

#include "util.h"

struct dnode* dnode_new(void* data)
{
        struct dnode* new_node = NULL;
        malloc_(new_node, sizeof(*new_node));

        *new_node = (struct dnode) {
                 data           /* data */
                ,{NULL, NULL}   /* out */
                ,false          /* visited */
        };

        return new_node;
}

struct dtree* dtree_new()
{
        struct dtree* new_dtree = NULL;
        malloc_(new_dtree, sizeof(*new_dtree));

        *new_dtree = (struct dtree) {
                 vec_new()      /* nodes */
                ,NULL           /* newest */
                ,NULL           /* _trav */
                ,0              /* _trav_idx */
        };
        
        return new_dtree;
}

void dtree_free(struct dtree* tree)
{
        vec_free(tree->nodes);
        free_(tree);
}

struct dnode* dtree_add_node(struct dtree* tree, struct dnode* node)
{
        vec_push_back(tree->nodes, node);
        tree->newest = node;
        return node;
}

struct dnode* dtree_add_data(struct dtree* tree, void* data)
{
        struct dnode* new_node = dnode_new(data);
        return dtree_add_node(tree, new_node);
}

void dtree_traverse_begin(struct dtree* tree)
{
        stack_free(&tree->_trav);
        tree->_trav_idx = 0;

        int i = 0;
        for (; i < tree->nodes->size; ++i) {
                struct dnode* node = tree->nodes->vector[i];
                node->visited = false;
        }
}

/* depth first traverse by 1 */
struct dnode* dtree_traverse(struct dtree* tree)
{
        if (tree->_trav == NULL) {
                if (tree->_trav_idx >= tree->nodes->size) {
                        return NULL;
                }
                stack_push(&tree->_trav, tree->nodes->vector[tree->_trav_idx++]);
        }

        struct dnode* node = tree->_trav->data;

        if (node->out[0] != NULL && !node->out[0]->visited) {
                node = node->out[0];
                stack_push(&tree->_trav, node);
        } else if (node->out[1] != NULL && !node->out[1]->visited) {
                node = node->out[1];
                stack_push(&tree->_trav, node);
        } else {
                stack_pop(&tree->_trav);
                return dtree_traverse(tree);
        }
        
        node->visited = true;

        return node;
}
