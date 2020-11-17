#include "dgraph.h"

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

struct dgraph* dgraph_new()
{
        struct dgraph* new_dgraph = NULL;
        malloc_(new_dgraph, sizeof(*new_dgraph));

        *new_dgraph = (struct dgraph) {
                 vec_new()      /* nodes */
                ,NULL           /* newest */
                ,NULL           /* _trav */
                ,0              /* _trav_idx */
        };

        return new_dgraph;
}

void dgraph_shallow_free(struct dgraph* graph)
{
        vec_free(graph->nodes);
        free_(graph);
}

void dgraph_free(struct dgraph* graph)
{
        /* TODO: free nodes */
        dgraph_shallow_free(graph);
}

struct dnode* dgraph_add_node(struct dgraph* graph, struct dnode* node)
{
        vec_push_back(graph->nodes, node);
        graph->newest = node;
        return node;
}

struct dnode* dgraph_add_data(struct dgraph* graph, void* data)
{
        struct dnode* new_node = dnode_new(data);
        return dgraph_add_node(graph, new_node);
}

void dgraph_extend(struct dgraph* dest, struct dgraph* src)
{
        /* TODO */
}

struct dnode* graph_traverse_begin(struct dgraph* graph)
{
        stack_free(&graph->_trav);
        graph->_trav_idx = 0;

        int i = 0;
        for (; i < graph->nodes->size; ++i) {
                struct dnode* node = graph->nodes->vector[i];
                node->visited = false;
        }

        return dgraph_traverse(graph);
}

/* depth first traverse by 1 */
struct dnode* dgraph_traverse(struct dgraph* graph)
{
        if (graph->_trav == NULL) {
                if (graph->_trav_idx >= graph->nodes->size) {
                        return NULL;
                }
                stack_push(&graph->_trav, graph->nodes->vector[graph->_trav_idx++]);
        }

        struct dnode* node = graph->_trav->data;

        if (node->out[0] != NULL && !node->out[0]->visited) {
                node = node->out[0];
                stack_push(&graph->_trav, node);
        } else if (node->out[1] != NULL && !node->out[1]->visited) {
                node = node->out[1];
                stack_push(&graph->_trav, node);
        } else {
                stack_pop(&graph->_trav);
                return dgraph_traverse(graph);
        }

        node->visited = true;

        return node;
}
