#include "flex.h"

/* NOTE: I'm too lazy to re-record the indicies
 *       for self->_raw. Therefore, self->_raw
 *       can never shrink unless totally cleared!
 */

/* Internal pair uses size_t instead of void* */
struct _pair {
	size_t idx;
	unsigned len;
};

flex* flex_construct(flex* self)
{
	vec_construct_(&self->_pairs, struct _pair);
	vec_construct_(&self->_raw, char);
	return self;
}

void flex_destroy(flex* self)
{
	vec_destroy(&self->_pairs);
	vec_destroy(&self->_raw);
}

_Bool flex_empty(const flex* self)
{
	return vec_empty(&self->_pairs);
}

void* flex_at(const flex* self, size_t idx)
{
	struct _pair* pair = vec_at(&self->_pairs, idx);
	return vec_at(&self->_raw, pair->idx);
}

flexpair flex_pair_at(const flex* self, size_t idx)
{
	struct _pair* pair = vec_at(&self->_pairs, idx);
	return (flexpair) {
		 vec_at(&self->_raw, pair->idx)
		,pair->len	
	};
}

void flex_reserve(flex* self, size_t size)
{
	vec_reserve(&self->_pairs, size);
	vec_reserve(&self->_raw, size);
}

void flex_resize(flex* self, size_t size)
{
	vec_resize(&self->_pairs, size);
}

void flex_clear(flex* self)
{
	vec_clear(&self->_pairs);
	vec_clear(&self->_raw);
}

void flex_shrink_to_fit(flex* self)
{
	vec_shrink_to_fit(&self->_pairs);
}

void flex_pop_back(flex* self)
{
	struct _pair* pair = vec_back(&self->_pairs);
	/* Is the last pair also last in _raw? */
	if (pair->idx + pair->len == self->_raw.size) {
		vec_resize(&self->_raw, pair->idx-1);
	}
	vec_pop_back(&self->_pairs);
}

void flex_insert(flex* self, size_t idx, void* data, unsigned datalen)
{
	struct _pair pair = {
		 self->_raw.size
		,datalen
	};
	vec_insert(&self->_pairs, idx, &pair);
	vec_append(&self->_raw, data, datalen);
}

void flex_push_back(flex* self, void* data, unsigned datalen)
{
	struct _pair pair = {
		 self->_raw.size
		,datalen
	};
	vec_push_back(&self->_pairs, &pair);
	vec_append(&self->_raw, data, datalen);
}

void flex_remove(flex* self, size_t idx)
{
	vec_remove(&self->_pairs, idx);
}




