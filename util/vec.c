#include "vec.h"

#include <stdint.h>
#include "util.h"

#define VEC_ALLOC_DEFAULT 2

vec* vec_construct(vec* restrict self, size_t elem_size)
{
	*self = (vec) {
	        ._alloc = VEC_ALLOC_DEFAULT,
	        ._elem_size = elem_size,
	};

	self->data = malloc_(VEC_ALLOC_DEFAULT * self->_elem_size);

	return self;
}

void vec_destroy(vec* restrict self)
{
	free_(self->data);
}

bool vec_empty(const vec* restrict self)
{
	return self->size == 0;
}

void* vec_at(const vec* restrict self, size_t index)
{
	return (char*)self->data + self->_elem_size * index;
}

void* vec_begin(const vec* restrict self)
{
	return self->data;
}

void* vec_back(const vec* restrict self)
{
	if (self->size == 0) {
		return self->data;
	}
	return vec_at(self, self->size - 1);
}

void* vec_end(const vec* restrict self)
{
	return vec_at(self, self->size);
}

/** allocate alloc + 1 for iterator end **/
void vec_reserve(vec* restrict self, size_t alloc)
{
	if (self->_alloc >= ++alloc) {
		return;
	}
	void* new_dest_ = realloc(self->data, alloc * self->_elem_size);
	if (!new_dest_) {
		perror("realloc");
		exit(EXIT_FAILURE);
	}
	self->data = new_dest_;
	new_dest_ = NULL;
	self->_alloc = alloc;
}

void vec_resize(vec* restrict self, size_t size)
{
	if (size == self->size) {
		return;
	}

	if (size > self->size) {
		vec_reserve(self, size);
	}

	self->size = size;
}

void vec_resize_and_zero(vec* restrict self, size_t size)
{
	size_t org_size = self->size;
	size_t org_alloc = self->_alloc;
	vec_resize(self, size);
	if (org_alloc == self->_alloc) {
		return;
	}
	void* new_shit = vec_at(self, org_size);
	size_t new_shit_size = self->_alloc - org_size;
	memset(new_shit, 0, new_shit_size * self->_elem_size);
}

void vec_clear(vec* restrict self)
{
	vec_resize(self, 0);
}

void vec_shrink_to_fit(vec* restrict self)
{
	realloc_(self->data, (self->size + 1) * self->_elem_size);
	self->_alloc = self->size + 1;
}

void* vec_pop_back(vec* restrict self)
{
	if (self->size == 0) {
		return NULL;
	}

	--self->size;
	return vec_end(self);
}

void* vec_add_one(vec* restrict self)
{
	if (self->_alloc <= ++self->size) {
		vec_reserve(self, self->_alloc * 2);
	}

	return vec_back(self);
}

void* vec_add_one_front(vec* restrict self)
{
	size_t move_bytes = self->_elem_size * (self->_alloc - 1);
	vec_add_one(self);

	memmove((char*)self->data + self->_elem_size, self->data, move_bytes);
	return vec_begin(self);
}

void vec_set_iter(vec* restrict _unused, void* pos, const void* begin, const void* back)
{
	memcpy(pos, begin, (char*)back - (char*)begin + 1);
}

void vec_set_one(vec* restrict self, void* pos, const void* restrict src)
{
	memcpy(pos, src, self->_elem_size);
}

void vec_set_at(vec* restrict self, size_t n, const void* restrict src, size_t len)
{
	void* restrict dest = vec_at(self, n);
	memcpy(dest, src, self->_elem_size * len);
}

void vec_set(vec* restrict self, void* pos, const void* restrict src, size_t len)
{
	memcpy(pos, src, self->_elem_size * len);
}

void vec_push_back(vec* restrict self, const void* restrict item)
{
	memcpy(vec_add_one(self), item, self->_elem_size);
}

void vec_insert_iter(vec* restrict self, void* pos, const void* begin, const void* back)
{
	size_t idx = vec_get_idx_(self, pos);
	size_t iter_size = vec_iter_size_(self, begin, back);
	size_t iter_bytes = self->_elem_size * iter_size;

	vec_resize(self, self->size + iter_size);
	size_t move_bytes = self->_elem_size * (self->_alloc - idx - iter_size);

	pos = vec_at(self, idx);

	memmove((char*)pos + iter_bytes, pos, move_bytes);
	memcpy(pos, begin, iter_bytes);
}

void vec_insert_one(vec* restrict self, void* pos, const void* restrict data)
{
	vec_add_one(self);
	size_t move_bytes =
	        self->_elem_size * ((self->_alloc - 1) - vec_get_idx_(self, pos));

	memmove((char*)pos + self->_elem_size, pos, move_bytes);
	memcpy(pos, data, self->_elem_size);
}

void vec_insert_at(vec* restrict self, size_t idx, const void* begin, size_t len)
{
	void* pos = vec_at(self, idx);
	const void* back = (const char*)begin + ((len - 1) * self->_elem_size);
	vec_insert_iter(self, pos, begin, back);
}

void vec_insert(vec* restrict self, void* pos, const void* begin, size_t len)
{
	if (len == 0) {
		return;
	}
	const void* back = begin + self->_elem_size * (len - 1);
	vec_insert_iter(self, pos, begin, back);
}

void vec_erase_iter(vec* restrict self, void* begin, const void* back)
{
	size_t bytes = (const char*)vec_at(self, self->_alloc - 1) - (const char*)back;
	self->size -= vec_iter_size_(self, begin, back);
	memmove(begin, (char*)back + self->_elem_size, bytes);
}

void vec_erase_one(vec* restrict self, void* elem)
{
	vec_erase_iter(self, elem, elem);
}

void vec_erase_at(vec* restrict self, size_t index, size_t len)
{
	void* begin = vec_at(self, index);
	const void* back = vec_at(self, index + len - 1);
	vec_erase_iter(self, begin, back);
}

void vec_erase(vec* restrict self, void* begin, size_t len)
{
	if (len == 0) {
		return;
	}
	const void* back = begin + self->_elem_size * (len - 1);
	vec_erase_iter(self, begin, back);
}

void vec_append(vec* restrict self, const void* restrict src, size_t n)
{
	size_t old_size = self->size;
	vec_resize(self, self->size + n);
	void* end = vec_at(self, old_size);
	memcpy(end, src, n * self->_elem_size);
}

void vec_extend(vec* restrict dest, const vec* restrict src)
{
	int index = dest->size;
	vec_resize(dest, dest->size + src->size);
	void* end = vec_at(dest, index);
	size_t bytes = src->_elem_size * (src->size + 1);
	memmove(end, vec_begin(src), bytes);
}

void vec_sort_r(vec* restrict self, qsort_r_cmp_fn cmp__, void* context)
{
	qsort_r(self->data, self->size, self->_elem_size, cmp__, context);
}


bitvec* bitvec_construct(bitvec* restrict self)
{
	vec_construct_(self, uint32_t);
	memset(self->data, 0, self->_elem_size * self->_alloc);
	return self;
}

void bitvec_destroy(bitvec* restrict self)
{
	vec_destroy(self);
}

bool bitvec_at(const bitvec* restrict self, size_t idx)
{
	uint32_t* num = vec_at(self, idx / 32);
	uint8_t offset = idx % 32;
	return (*num >> offset) & 1;
}

void bitvec_set(bitvec* restrict self, size_t idx, bool newval)
{
	uint32_t* num = vec_at(self, idx / 32);
	uint32_t offset = idx % 32;
	if (newval) {
		*num |= (1 << offset);
	} else {
		*num &= ~(1 << offset);
	}
}

void bitvec_reserve(bitvec* restrict self, size_t size)
{
	size_t alloc = size / 32 + 1;
	if (self->_alloc >= ++alloc) {
		return;
	}
	void* new_dest_ = realloc(self->data, alloc * self->_elem_size);
	if (!new_dest_) {
		perror("realloc");
		exit(EXIT_FAILURE);
	}
	self->data = new_dest_;
	new_dest_ = NULL;
	self->_alloc = alloc;
}

void bitvec_resize(bitvec* restrict self, size_t size)
{
	size_t org_size = self->size / 32 + 1;
	size_t org_alloc = self->_alloc;

	if (size > self->size) {
		bitvec_reserve(self, size);
	}
	self->size = size;

	if (org_alloc == self->_alloc) {
		return;
	}
	void* new_shit = vec_at(self, org_size);
	size_t new_shit_size = self->_alloc - org_size;
	memset(new_shit, 0, new_shit_size * self->_elem_size);
}

void bitvec_push_back(bitvec* restrict self, bool val)
{
	if (++self->size > (self->_alloc - 1) * 32) {
		bitvec_reserve(self, self->size * 2);
	}

	bitvec_set(self, self->size - 1, val);
}
