#include "dgraph.h"

#include "util.h"

dnode* dnode_construct(dnode* node, void* data)
{
	*node = (dnode) {
		 data           /* data */
		,{NULL, NULL}   /* out */
		,0              /* visit_count */
		,false          /* is_root */
	};

	return node;
}

void dnode_destroy(dnode* node) { }

dgraph* dgraph_construct(dgraph* graph)
{
	*graph = (dgraph) {
		 new_t_(vec, dnode*)    /* nodes */
		,NULL                   /* newest */
		,new_t_(fifo, dnode*, 5)/* _trav */
		,new_t_(vec, dnode*)    /* _roots */
		,0                      /* _root_idx */
		,false                  /* _roots_good */
	};

	return graph;
}

void dgraph_shallow_free(dgraph* graph)
{
	dgraph_shallow_destroy(graph);
	free_(graph);
}

void dgraph_shallow_destroy(dgraph* graph)
{
	delete_(vec, graph->nodes);
	delete_(vec, graph->_roots);
	delete_(fifo, graph->_trav);
}

void dgraph_destroy(dgraph* graph)
{
	unsigned i = 0;
	dnode** node = vec_begin(graph->nodes);
	for (; i < graph->nodes->size; ++i) {
		delete_(dnode, node[i]);
	}
	dgraph_shallow_destroy(graph);
}

/* making a copy here */
dnode* dgraph_add_node(dgraph* graph, dnode* node)
{
	vec_push_back(graph->nodes, &node);
	graph->newest = node;
	graph->_roots_good = false;
	return node;
}

dnode* dgraph_add_data(dgraph* graph, void* data)
{
	dnode* node = new_(dnode, data);
	return dgraph_add_node(graph, node);
}

void dgraph_consume(dgraph* dest, dgraph* src)
{
	dest->_roots_good = false;
	vec_extend(dest->nodes, src->nodes);
	dgraph_shallow_free(src);
}

void* dgraph_remove(dgraph* graph, dnode** node)
{
	void* data = (*node)->data;
	delete_(dnode, *node);
	vec_erase(graph->nodes, node);
	graph->_roots_good = false;
	return data;
}

dnode** _guess_root(dgraph* graph)
{
	if (vec_empty(graph->nodes)) {
		return NULL;
	}

	/* reset */
	dnode** it = vec_begin(graph->nodes);
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
	dnode** min = it++;
	for (; it != vec_end(graph->nodes); ++it) {
		if ((*it)->visit_count < (*min)->visit_count) {
			min = it;
		}
	}
	(*min)->is_root = true;

	return min;
}

vec* dgraph_get_roots(dgraph* graph)
{
	if (graph->_roots_good) {
		return graph->_roots;
	}

	vec_clear(graph->_roots);
	dnode** it = vec_begin(graph->nodes);
	for (; it != vec_end(graph->nodes); ++it) {
		if ((*it)->is_root) {
			vec_push_back(graph->_roots, it);
		}
	}

	/* if no roots found, try to figure it out */
	if (vec_empty(graph->_roots)) {
		vec_push_back(graph->_roots, _guess_root(graph));
	}
	graph->_roots_good = true;
	return graph->_roots;
}

void dgraph_traverse_reset(dgraph* graph)
{
	dgraph_get_roots(graph);

	fifo_resize(graph->_trav, graph->nodes->size);

	/* we are going to enter the first root manually */
	graph->_root_idx = 1;

	int i = 0;
	dnode** it = vec_begin(graph->nodes);
	for (; it != vec_end(graph->nodes); ++it) {
		(*it)->visit_count = 0;
	}

	/* start at first discovered root */
	it = vec_begin(graph->_roots);
	fifo_add(graph->_trav, it);
}

dnode* dgraph_traverse(dgraph* graph)
{
	while (fifo_is_empty(graph->_trav)) {
		if (graph->_root_idx >= graph->_roots->size) {
			return NULL;
		}
		dnode** node = vec_at(graph->_roots, graph->_root_idx++);
		if ((*node)->visit_count) {
			continue;
		}
		fifo_add(graph->_trav, node);
		return dgraph_traverse(graph);
	}

	dnode** node = fifo_get(graph->_trav);
	++(*node)->visit_count;

	if ((*node)->out[0] != NULL && !(*node)->out[0]->visit_count) {
		fifo_add(graph->_trav, &(*node)->out[0]);
	}
	if ((*node)->out[1] != NULL && !(*node)->out[1]->visit_count) {
		fifo_add(graph->_trav, &(*node)->out[1]);
	}

	return *node;
}
