#include "dgraph.h"

#include "util.h"

dnode* dnode_construct(dnode* restrict node, void* restrict data)
{
	*node = (dnode) {
	        .data = data,
	};

	return node;
}

void dnode_destroy(dnode* restrict _unused)
{
	_unused = NULL;
}

dgraph* dgraph_construct(dgraph* restrict self)
{
	*self = (dgraph) {
	        .nodes = new_t_(vec, dnode*),
	        ._trav = new_t_(fifo, dnode*, 5),
	        ._roots = new_t_(vec, dnode*),
	        ._root_idx = 0,
	};

	return self;
}

void dgraph_shallow_free(dgraph* restrict self)
{
	dgraph_shallow_destroy(self);
	free_(self);
}

void dgraph_shallow_destroy(dgraph* restrict self)
{
	delete_(vec, self->nodes);
	delete_(vec, self->_roots);
	delete_(fifo, self->_trav);
}

void dgraph_destroy(dgraph* restrict self)
{
	unsigned i = 0;
	dnode** node = vec_begin(self->nodes);
	for (; i < self->nodes->size; ++i) {
		delete_(dnode, node[i]);
	}
	dgraph_shallow_destroy(self);
}

/* making a copy here */
dnode* dgraph_add_node(dgraph* restrict self, dnode* node)
{
	vec_push_back(self->nodes, &node);
	self->newest = node;
	self->_roots_good = false;
	return node;
}

dnode* dgraph_add_data(dgraph* restrict self, void* restrict data)
{
	dnode* node = new_(dnode, data);
	return dgraph_add_node(self, node);
}

void dgraph_consume(dgraph* restrict dest, dgraph* restrict src)
{
	dest->_roots_good = false;
	vec_extend(dest->nodes, src->nodes);
	dgraph_shallow_free(src);
}

void* dgraph_remove(dgraph* restrict self, dnode** node)
{
	dnode** it = vec_begin(self->nodes);
	for (; it != vec_end(self->nodes); ++it) {
		if ((*it)->out[0] == *node) {
			(*it)->out[0] = NULL;
		}
		if ((*it)->out[1] == *node) {
			(*it)->out[1] = NULL;
		}
	}
	void* data = (*node)->data;
	delete_(dnode, *node);
	vec_erase_one(self->nodes, node);
	self->_roots_good = false;
	return data;
}

int _assume_roots(dgraph* restrict self)
{
	if (vec_empty(self->nodes)) {
		return 0;
	}

	/* reset */
	dnode** it = vec_begin(self->nodes);
	for (; it != vec_end(self->nodes); ++it) {
		(*it)->visit_count = 0;
	}

	/* get counts */
	it = vec_begin(self->nodes);
	for (; it != vec_end(self->nodes); ++it) {
		if ((*it)->out[0] != NULL) {
			++(*it)->out[0]->visit_count;
		}
		if ((*it)->out[1] != NULL) {
			++(*it)->out[1]->visit_count;
		}
	}

	it = vec_begin(self->nodes);
	for (; it != vec_end(self->nodes); ++it) {
		if ((*it)->visit_count == 0) {
			(*it)->is_root = true;
			vec_push_back(self->_roots, it);
			continue;
		}
		(*it)->visit_count = 0;
	}

	return self->_roots->size;
}

unsigned dgraph_root_count(dgraph* restrict self)
{
	return self->_roots->size;
}

vec* dgraph_get_roots(dgraph* restrict self)
{
	if (self->_roots_good) {
		return self->_roots;
	}

	vec_clear(self->_roots);
	dnode** it = vec_begin(self->nodes);
	for (; it != vec_end(self->nodes); ++it) {
		if ((*it)->is_root) {
			vec_push_back(self->_roots, it);
		}
	}

	/* if no roots found, try to figure it out */
	if (vec_empty(self->_roots)) {
		/* TODO: handle return of 0 */
		_assume_roots(self);
	}
	self->_roots_good = true;
	return self->_roots;
}

void dgraph_traverse_reset(dgraph* restrict self)
{
	dgraph_get_roots(self);

	fifo_resize(self->_trav, self->nodes->size);

	/* we are going to enter the first root manually */
	self->_root_idx = 1;

	dnode** it = vec_begin(self->nodes);
	for (; it != vec_end(self->nodes); ++it) {
		(*it)->visit_count = 0;
	}

	/* start at first discovered root */
	it = vec_begin(self->_roots);
	fifo_add(self->_trav, it);
}

dnode* dgraph_traverse(dgraph* restrict self)
{
	while (fifo_is_empty(self->_trav)) {
		if (self->_root_idx >= self->_roots->size) {
			return NULL;
		}
		dnode** node = vec_at(self->_roots, self->_root_idx++);
		if ((*node)->visit_count) {
			continue;
		}
		fifo_add(self->_trav, node);
		return dgraph_traverse(self);
	}

	dnode** node = fifo_get(self->_trav);
	++(*node)->visit_count;

	if ((*node)->out[0] != NULL && !(*node)->out[0]->visit_count) {
		fifo_add(self->_trav, &(*node)->out[0]);
	}
	if ((*node)->out[1] != NULL && !(*node)->out[1]->visit_count) {
		fifo_add(self->_trav, &(*node)->out[1]);
	}

	return *node;
}
