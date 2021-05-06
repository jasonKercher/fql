#include "vec.h"

#include <limits.h>
#include "util.h"

//vec* vec_new(size_t elem_size)
//{
//	vec* new_vec = NULL;
//	malloc_(new_vec, sizeof(*new_vec));
//
//	return vec_construct(new_vec, elem_size);
//}

vec* vec_construct_s(vec* v, size_t elem_size)
{
	*v = (vec) {
		 NULL           /* data */
		,0              /* size */
		,2              /* _alloc */
		,elem_size      /* _elem_s */
	};

	/* allocate space for first element and
	 * trailing NULL pointer
	 */
	v->data = malloc_(2 * v->_elem_size);

	return v;
}

void vec_destroy(vec* v)
{
	free_(v->data);
}

void vec_free(vec* v)
{
	vec_destroy(v);
	free_(v);
}

_Bool vec_empty(const vec* v)
{
	return v->size == 0;
}

void* vec_at(const vec* v, size_t index)
{
	return (char*) v->data + v->_elem_size * index;
}

void* vec_begin(const vec* v)
{
	return v->data;
}

void* vec_back(const vec* v)
{
	if (v->size == 0) {
		return v->data;
	}
	return vec_at(v, v->size - 1);
}

void* vec_end(const vec* v)
{
	return vec_at(v, v->size);
}

/** allocate alloc + 1 for iterator end **/
void vec_reserve(vec* v, size_t alloc)
{
	if (v->_alloc > ++alloc) {
		return;
	}
	//realloc_(v->data, alloc * v->_elem_size);
	void* new_dest_ = realloc(v->data, alloc * v->_elem_size);
	if (!new_dest_) {
		perror("realloc");
		exit(EXIT_FAILURE);
	}
	v->data = new_dest_;
	new_dest_ = NULL;
	v->_alloc = alloc;
}

void vec_resize(vec* v, size_t size)
{
	if (size == v->size) {
		return;
	}

	if (size > v->size) {
		vec_reserve(v, size);
	}

	v->size = size;
}

/* it is assumed that a vector resized with
 * this function should only be resized with
 * this function.
 */
void vec_resize_and_zero(vec* v, size_t size)
{
	size_t org_alloc = v->_alloc;
	vec_resize(v, size);
	if (org_alloc == v->_alloc) {
		return;
	}
	void* new_alloc = vec_at(v, org_alloc);
	size_t new_alloc_total = v->_alloc - org_alloc;
	memset(new_alloc, 0, new_alloc_total * v->_elem_size);
}

void vec_clear(vec* v)
{
	vec_resize(v, 0);
}

void vec_shrink_to_fit(vec* v)
{
	realloc_(v->data, (v->size + 1) * v->_elem_size);
	v->_alloc = v->size + 1;
}

void* vec_add_one(vec* v)
{
	if (v->_alloc <= ++v->size) {
		vec_reserve(v, v->_alloc * 2);
	}

	return vec_back(v);
}

void vec_set(vec* v, size_t n, const void* src)
{
	void* dest = vec_at(v, n);
	memcpy(dest, src, v->_elem_size);
}

void vec_append(vec* v, const void* src, size_t n)
{
	size_t old_size = v->size;
	vec_resize(v, v->size + n);
	void* end = vec_at(v, old_size);
	memcpy(end, src, n * v->_elem_size);
}

void vec_insert(vec* v, size_t n, const void* src)
{
	vec_add_one(v);
	char* dest = vec_at(v, n);
	size_t shift_size = v->_elem_size * (v->size - n);

	memmove(dest + v->_elem_size, dest, shift_size);
	vec_set(v, n, src);
}

void vec_push_back(vec* v, const void* item)
{
	memcpy(vec_add_one(v), item, v->_elem_size);
}

void vec_extend(vec* dest, const vec* src)
{
	int index = dest->size;
	vec_resize(dest, dest->size + src->size);
	void* end = vec_at(dest, index);
	size_t bytes = src->_elem_size * (src->size + 1);
	memcpy(end, vec_begin(src), bytes);
}

void vec_erase(vec* v, void* elem)
{
	size_t bytes = (char*)vec_end(v) - (char*)elem;
	memmove(elem, (char*)elem + v->_elem_size, bytes);
	--v->size;
}

void vec_remove(vec* v, size_t index)
{
	if (index >= v->size) {
		return; /* abort? */
	}

	vec_erase(v, vec_at(v, index));
}
