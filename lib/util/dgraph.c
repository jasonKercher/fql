#include "dgraph.h"

#include "util.h"

Dnode* dnode_new(void* data)
{
        Dnode* new_node = NULL;
        malloc_(new_node, sizeof(*new_node));

        return dnode_init(new_node, data);
}

Dnode* dnode_init(Dnode* node, void* data)
{
        *node = (Dnode) {
                 data           /* data */
                ,{NULL, NULL}   /* out */
                ,false          /* was_visited */
                ,false          /* is_root */
        };

        return node;
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

        return dgraph_init(new_dgraph);
}

Dgraph* dgraph_init(Dgraph* dgraph)
{
        *dgraph = (Dgraph) {
                 vec_new_(Dnode*)       /* nodes */
                ,NULL                   /* newest */
                ,vec_new_(Dnode*)       /* _trav */
                ,0                      /* _trav_idx */
        };

        return dgraph;
}

void dgraph_shallow_free(Dgraph* graph)
{
        vec_free(graph->nodes);
        vec_free(graph->_trav);
        free_(graph);
}

void dgraph_free(Dgraph* graph)
{
        /* TODO: free the individual nodes */
        dgraph_shallow_free(graph);
}

/* making a copy here */
Dnode* dgraph_add_node(Dgraph* graph, Dnode* node)
{
        vec_push_back(graph->nodes, &node);
        graph->newest = node;
        return node;
}

Dnode* dgraph_add_data(Dgraph* graph, void* data)
{
        Dnode* node = dnode_new(data);
        return dgraph_add_node(graph, node);
}

void dgraph_extend(Dgraph* dest, Dgraph* src)
{
        /* TODO */
}

void* dgraph_remove(Dgraph* graph, Dnode** node)
{
        void* data = (*node)->data;
        vec_erase(graph->nodes, node);
        return data;
}

/* TODO: start at root 
 *       keep track of root nodes
 *       update_roots()
 */
Dnode* graph_traverse_begin(Dgraph* graph)
{
        vec_clear(graph->_trav);
        graph->_trav_idx = 0;

        int i = 0;
        Dnode** node = vec_begin(graph->nodes);
        for (; node != vec_end(graph->nodes); ++node) {
                (*node)->was_visited = false;
        }

        return dgraph_traverse(graph);
}

/* depth first traverse by 1 */
/* TODO: change to BFS traversal... BFS will use Vec more efficiently */
Dnode* dgraph_traverse(Dgraph* graph)
{
        Dnode* node = NULL;
        if (vec_empty(graph->_trav)) {
                if (graph->_trav_idx >= graph->nodes->size) {
                        return NULL;
                }

                node = vec_at(graph->nodes, graph->_trav_idx++);
                vec_push_back(graph->_trav, &node);
                node->was_visited = true;
                return node;
        }

        node = vec_back(graph->_trav);

        if (node->out[0] != NULL && !node->out[0]->was_visited) {
                node = node->out[0];
                vec_push_back(graph->_trav, &node);
        } else if (node->out[1] != NULL && !node->out[1]->was_visited) {
                node = node->out[1];
                vec_push_back(graph->_trav, &node);
        } else {
                vec_remove(graph->_trav, 0);
                return dgraph_traverse(graph);
        }

        node->was_visited = true;

        return node;
}
