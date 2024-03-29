#include "flex.h"
#include <stdio.h>

/* NOTE: I'm too lazy to re-record the indicies
 *       for _raw. Therefore, _raw can only
 *       shrink if the back _pair is referencing
 *       the back data in _raw.
 */

/* Internal pair uses size_t instead of void* */
struct _pair {
	size_t idx;
	unsigned len;
};

flex* flex_construct(flex* restrict self)
{
	vec_construct_(&self->_pairs, struct _pair);
	vec_construct_(&self->_raw, char);
	return self;
}

void flex_destroy(flex* restrict self)
{
	vec_destroy(&self->_pairs);
	vec_destroy(&self->_raw);
}

bool flex_empty(const flex* restrict self)
{
	return vec_empty(&self->_pairs);
}

size_t flex_size(const flex* restrict self)
{
	return self->_pairs.size;
}

void* flex_at(const flex* restrict self, size_t idx)
{
	struct _pair* pair = vec_at(&self->_pairs, idx);
	return vec_at(&self->_raw, pair->idx);
}

stringview flex_pair_at(const flex* restrict self, size_t idx)
{
	struct _pair* pair = vec_at(&self->_pairs, idx);
	return (stringview) {
		 vec_at(&self->_raw, pair->idx)
		,pair->len
	};
}

void flex_reserve(flex* restrict self, size_t size)
{
	vec_reserve(&self->_pairs, size);
	vec_reserve(&self->_raw, size);
}

void flex_resize(flex* restrict self, size_t size)
{
	if (size > self->_pairs.size) {
		vec_resize(&self->_pairs, size);
		return;
	}
	for (; self->_pairs.size > size;
	       flex_pop_back(self));
}

void flex_clear(flex* restrict self)
{
	vec_clear(&self->_pairs);
	vec_clear(&self->_raw);
}

void flex_shrink_to_fit(flex* restrict self)
{
	vec_shrink_to_fit(&self->_pairs);
}

void flex_pop_back(flex* restrict self)
{
	struct _pair* pair = vec_back(&self->_pairs);
	/* Is the last pair also last in _raw? */
	if (pair->idx + pair->len == self->_raw.size) {
		vec_resize(&self->_raw, pair->idx);
	}
	vec_pop_back(&self->_pairs);
}

void flex_insert(flex* restrict self,
                 size_t idx,
                 const void* restrict data,
                 unsigned datalen)
{
	struct _pair pair = {
		 self->_raw.size
		,datalen
	};
	vec_insert_at(&self->_pairs, idx, &pair, 1);
	vec_append(&self->_raw, data, datalen);
}

void flex_push_back(flex* restrict self, const void* restrict data, unsigned datalen)
{
	struct _pair pair = {
		 self->_raw.size
		,datalen
	};
	vec_push_back(&self->_pairs, &pair);
	vec_append(&self->_raw, data, datalen);
}

void flex_push_back_str_int(struct flex* restrict self, long num)
{
	struct _pair pair = {
		 self->_raw.size
		,snprintf(NULL, 0, "%ld", num)
	};
	vec_push_back(&self->_pairs, &pair);
	vec_resize(&self->_raw, pair.idx + pair.len);
	char* end = vec_at(&self->_raw, pair.idx);
	snprintf(end, pair.len+1, "%ld", num);
}
void flex_push_back_str_float(struct flex* restrict self, double num)
{
	struct _pair pair = {
		 self->_raw.size
		,snprintf(NULL, 0, "%f", num)
	};
	vec_push_back(&self->_pairs, &pair);
	vec_resize(&self->_raw, pair.idx + pair.len);
	char* end = vec_at(&self->_raw, pair.idx);
	snprintf(end, pair.len+1, "%f", num);
}

void flex_remove(flex* restrict self, size_t idx)
{
	vec_erase_at(&self->_pairs, idx, 1);
}
