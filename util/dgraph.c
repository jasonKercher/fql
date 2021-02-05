#include "dgraph.h"

#include "util.h"

Dnode* dnode_new(void* data)
{
        Dnode* new_node = NULL;
        malloc_(new_node, sizeof(*new_node));

        return dnode_construct(new_node, data);
}

Dnode* dnode_construct(Dnode* node, void* data)
{
        *node = (Dnode) {
                 data           /* data */
                ,{NULL, NULL}   /* out */
                ,0              /* visit_count */
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

        return dgraph_construct(new_dgraph);
}

Dgraph* dgraph_construct(Dgraph* dgraph)
{
        *dgraph = (Dgraph) {
                 vec_new_(Dnode*)       /* nodes */
                ,NULL                   /* newest */
                ,fifo_new_(Dnode*, 5)   /* _trav */
                ,vec_new_(Dnode*)       /* _roots */
                ,0                      /* _root_idx */
                ,false                  /* _roots_good */
        };

        return dgraph;
}

void dgraph_shallow_free(Dgraph* graph)
{
        vec_free(graph->nodes);
        vec_free(graph->_roots);
        fifo_free(graph->_trav);
        free_(graph);
}

void dgraph_free(Dgraph* graph)
{
        unsigned i = 0;
        Dnode** node = vec_begin(graph->nodes);
        for (; i < graph->nodes->size; ++i) {
                dnode_free(node[i]);
        }
        dgraph_shallow_free(graph);
}

/* making a copy here */
Dnode* dgraph_add_node(Dgraph* graph, Dnode* node)
{
        vec_push_back(graph->nodes, &node);
        graph->newest = node;
        graph->_roots_good = false;
        return node;
}

Dnode* dgraph_add_data(Dgraph* graph, void* data)
{
        Dnode* node = dnode_new(data);
        return dgraph_add_node(graph, node);
}

void dgraph_extend(Dgraph* dest, Dgraph* src)
{
        dest->_roots_good = false;
        /* TODO */
}

void* dgraph_remove(Dgraph* graph, Dnode** node)
{
        void* data = (*node)->data;
        dnode_free(*node);
        vec_erase(graph->nodes, node);
        graph->_roots_good = false;
        return data;
}

Dnode** _guess_root(Dgraph* graph)
{
        /* reset */
        Dnode** it = vec_begin(graph->nodes);
        for (; it != vec_end(graph->nodes); ++it) {
                (*it)->visit_count = 0;
        }

        /* get counts */ 
        it = vec_begin(graph->nodes);
        for (; it != vec_end(graph->nodes); ++it) {
                if ((*it)->out[0] != NULL) {
                        ++(*it)->out[0]->visit_count;
                }
                if ((*it)->out[1] != NULL) {
                        ++(*it)->out[1]->visit_count;
                }
        }

        /* find min */
        it = vec_begin(graph->nodes);
        Dnode** min = it++;
        for (; it != vec_end(graph->nodes); ++it) {
                if ((*it)->visit_count < (*min)->visit_count) {
                        min = it;
                }
        }
        (*min)->is_root = true;

        return min;
}

Vec* dgraph_get_roots(Dgraph* graph)
{
        if (graph->_roots_good) {
                return graph->_roots;
        }

        vec_clear(graph->_roots);
        Dnode** it = vec_begin(graph->nodes);
        for (; it != vec_end(graph->nodes); ++it) {
                if ((*it)->is_root) {
                        vec_push_back(graph->_roots, it);
                }
        }

        /* If no roots found, try to figure it out */
        if (vec_empty(graph->_roots)) {
                vec_push_back(graph->_roots, _guess_root(graph));
        }
        graph->_roots_good = true;
        return graph->_roots;
}

void dgraph_traverse_reset(Dgraph* graph)
{
        dgraph_get_roots(graph);

        fifo_resize(graph->_trav, graph->nodes->size);

        /* we are going to enter the first root manually */
        graph->_root_idx = 1;

        int i = 0;
        Dnode** it = vec_begin(graph->nodes);
        for (; it != vec_end(graph->nodes); ++it) {
                (*it)->visit_count = 0;
        }

        /* Start at first discovered root */
        it = vec_begin(graph->_roots);
        fifo_add(graph->_trav, it);
}

Dnode* dgraph_traverse(Dgraph* graph)
{
        if (fifo_is_empty(graph->_trav)) {
                if (graph->_root_idx >= graph->_roots->size) {
                        return NULL;
                }
                Dnode** node = vec_at(graph->_roots, graph->_root_idx++);
                /* Assert node not visited */
                fifo_add(graph->_trav, node);
                return dgraph_traverse(graph);
        }

        Dnode** node = fifo_get(graph->_trav);
        ++(*node)->visit_count;

        if ((*node)->out[0] != NULL && !(*node)->out[0]->visit_count) {
                fifo_add(graph->_trav, &(*node)->out[0]);
        }
        if ((*node)->out[1] != NULL && !(*node)->out[1]->visit_count) {
                fifo_add(graph->_trav, &(*node)->out[1]);
        }

        return *node;
}
