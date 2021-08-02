#include "vec.h"

#include <stdint.h>
#include "util.h"


vec* vec_construct(vec* self, size_t elem_size)
{
	*self = (vec) {
	        NULL,     /* data */
	        0,        /* size */
	        2,        /* _alloc */
	        elem_size /* _elem_s */
	};

	/* allocate space for first element and
	 * trailing NULL pointer
	 */
	self->data = malloc_(2 * self->_elem_size);

	return self;
}

void vec_destroy(vec* self)
{
	free_(self->data);
}

bool vec_empty(const vec* self)
{
	return self->size == 0;
}

void* vec_at(const vec* self, size_t index)
{
	return (char*)self->data + self->_elem_size * index;
}

void* vec_begin(const vec* self)
{
	return self->data;
}

void* vec_back(const vec* self)
{
	if (self->size == 0) {
		return self->data;
	}
	return vec_at(self, self->size - 1);
}

void* vec_end(const vec* self)
{
	return vec_at(self, self->size);
}

/** allocate alloc + 1 for iterator end **/
void vec_reserve(vec* self, size_t alloc)
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

void vec_resize(vec* self, size_t size)
{
	if (size == self->size) {
		return;
	}

	if (size > self->size) {
		vec_reserve(self, size);
	}

	self->size = size;
}


void vec_resize_and_zero(vec* self, size_t size)
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

void vec_clear(vec* self)
{
	vec_resize(self, 0);
}

void vec_shrink_to_fit(vec* self)
{
	realloc_(self->data, (self->size + 1) * self->_elem_size);
	self->_alloc = self->size + 1;
}

void* vec_pop_back(vec* self)
{
	if (self->size == 0) {
		return NULL;
	}

	--self->size;
	return vec_end(self);
}

void* vec_add_one(vec* self)
{
	if (self->_alloc <= ++self->size) {
		vec_reserve(self, self->_alloc * 2);
	}

	return vec_back(self);
}

void* vec_add_one_front(vec* self)
{
	size_t move_bytes = self->_elem_size * (self->_alloc - 1);
	vec_add_one(self);

	memmove((char*)self->data + self->_elem_size, self->data, move_bytes);
	return vec_begin(self);
}

void vec_set(vec* self, size_t n, const void* src)
{
	void* dest = vec_at(self, n);
	memcpy(dest, src, self->_elem_size);
}

void vec_append(vec* self, const void* src, size_t n)
{
	size_t old_size = self->size;
	vec_resize(self, self->size + n);
	void* end = vec_at(self, old_size);
	memcpy(end, src, n * self->_elem_size);
}

void vec_push_back(vec* self, const void* item)
{
	memcpy(vec_add_one(self), item, self->_elem_size);
}

void vec_extend(vec* dest, const vec* src)
{
	int index = dest->size;
	vec_resize(dest, dest->size + src->size);
	void* end = vec_at(dest, index);
	size_t bytes = src->_elem_size * (src->size + 1);
	memmove(end, vec_begin(src), bytes);
}

void vec_insert_one(vec* self, void* pos, const void* src)
{
	vec_add_one(self);
	size_t move_bytes =
	        self->_elem_size * ((self->_alloc - 1) - vec_get_idx_(self, pos));

	memmove((char*)pos + self->_elem_size, pos, move_bytes);
	memcpy(pos, src, self->_elem_size);
}

void vec_insert_at(vec* self, size_t idx, const void* data, size_t len)
{
	void* pos = vec_at(self, idx);
	const void* back = (const char*)data + ((len - 1) * self->_elem_size);
	vec_insert(self, pos, data, back);
}

void vec_insert(vec* self, void* pos, const void* begin, const void* back)
{
	size_t idx = vec_get_idx_(self, pos);
	size_t iter_size = vec_iter_size_(self, begin, back);
	size_t iter_bytes = self->_elem_size * iter_size;
	size_t move_bytes = self->_elem_size * (self->_alloc - idx - iter_size);
	vec_resize(self, self->size + iter_size);

	pos = vec_at(self, idx);

	memmove((char*)pos + iter_bytes, pos, move_bytes);
	memcpy(pos, begin, iter_bytes);
}

void vec_erase_one(vec* self, void* elem)
{
	vec_erase(self, elem, elem);
}

void vec_erase_at(vec* self, size_t index, size_t len)
{
	void* begin = vec_at(self, index);
	void* back = vec_at(self, index + len - 1);
	vec_erase(self, begin, back);
}

void vec_erase(vec* self, void* begin, void* back)
{
	size_t bytes = (const char*)vec_at(self, self->_alloc - 1) - (const char*)back;
	self->size -= vec_iter_size_(self, begin, back);
	memmove(begin, (char*)back + self->_elem_size, bytes);
}

void vec_sort_r(vec* self, qsort_r_cmp_fn cmp__, void* context)
{
	qsort_r(self->data, self->size, self->_elem_size, cmp__, context);
}


bitvec* bitvec_construct(bitvec* self)
{
	return vec_construct_(self, uint64_t);
}

void bitvec_destroy(bitvec* self)
{
	vec_destroy(self);
}

bool bitvec_at(const bitvec* self, size_t idx)
{
	uint64_t* num = vec_at(self, idx / 64);
	uint8_t offset = idx % 64;
	return (*num >> offset) & 1;
}

void bitvec_set(bitvec* self, size_t idx, bool newval)
{
	uint64_t* num = vec_at(self, idx / 64);
	uint8_t offset = idx % 64;
	if (newval) {
		*num |= (1 << offset);
	} else {
		*num &= ~(1 << offset);
	}
}

void bitvec_resize(bitvec* self, size_t size)
{
	vec_resize(self, size / 64 + 1);
}
