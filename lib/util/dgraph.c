#include "dgraph.h"

#include "util.h"

Dnode* dnode_new(void* data)
{
        Dnode* new_node = NULL;
        malloc_(new_node, sizeof(*new_node));

        *new_node = (Dnode) {
                 data           /* data */
                ,{NULL, NULL}   /* out */
                ,false          /* visited */
        };

        return new_node;
}

void* dnode_free(Dnode* node)
{
        void* data = node->data;
        free_(node);
        return data;
}

Dgraph* dgraph_new()
{
        Dgraph* new_dgraph = NULL;
        malloc_(new_dgraph, sizeof(*new_dgraph));

        *new_dgraph = (Dgraph) {
                 vec_new()      /* nodes */
                ,NULL           /* newest */
                ,NULL           /* _trav */
                ,0              /* _trav_idx */
        };

        return new_dgraph;
}

void dgraph_shallow_free(Dgraph* graph)
{
        vec_free(graph->nodes);
        free_(graph);
}

void dgraph_free(Dgraph* graph)
{
        /* TODO: free nodes */
        dgraph_shallow_free(graph);
}

Dnode* dgraph_add_node(Dgraph* graph, Dnode* node)
{
        vec_push_back(graph->nodes, node);
        graph->newest = node;
        return node;
}

Dnode* dgraph_add_data(Dgraph* graph, void* data)
{
        Dnode* new_node = dnode_new(data);
        return dgraph_add_node(graph, new_node);
}

void dgraph_extend(Dgraph* dest, Dgraph* src)
{
        /* TODO */
}

void* dgraph_remove(Dgraph* graph, Dnode* node)
{
        /* TODO */
        return NULL;
}

Dnode* graph_traverse_begin(Dgraph* graph)
{
        stack_free(&graph->_trav);
        graph->_trav_idx = 0;

        int i = 0;
        for (; i < graph->nodes->size; ++i) {
                Dnode* node = graph->nodes->vector[i];
                node->visited = false;
        }

        return dgraph_traverse(graph);
}

/* depth first traverse by 1 */
Dnode* dgraph_traverse(Dgraph* graph)
{
        if (graph->_trav == NULL) {
                if (graph->_trav_idx >= graph->nodes->size) {
                        return NULL;
                }
                stack_push(&graph->_trav, graph->nodes->vector[graph->_trav_idx++]);
        }

        Dnode* node = graph->_trav->data;

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
