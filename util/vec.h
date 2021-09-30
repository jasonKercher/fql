#ifndef VEC_H
#define VEC_H

/**
 * naive as fuck vector implementation
 *
 * vector is in contiguous memory and followed
 * by at least one extra allocation that is
 * referred to by vec_end
 *
 * this used to be a small, elegant struct for
 * storing void*. then, I wanted to have iterators.
 * now, it's fugly...
 *
 * also now... if a vector owns the data it is
 * given, don't keep references to its data
 * outside the vector, or realloc will fuck
 * your day all up.
 */


/* This is only for qsort_r used by vec_sort_r */
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdlib.h>
#include <stdbool.h>

struct vec {
	void* data;        /* the stuff */
	size_t size;       /* number of elements populated */
	size_t _alloc;     /* number of elements allocated */
	size_t _elem_size; /* size of a single element */
};
typedef struct vec vec;

typedef int (*qsort_r_cmp_fn)(const void*, const void*, void*);

struct vec* vec_construct(struct vec* restrict, size_t);
#define vec_construct_(v_, T_) vec_construct(v_, sizeof(T_))
void vec_destroy(struct vec* restrict);

bool vec_empty(const struct vec* restrict);
void* vec_at(const struct vec* restrict, size_t);
void* vec_begin(const struct vec* restrict);
void* vec_back(const struct vec* restrict);
void* vec_end(const struct vec* restrict);

void vec_reserve(struct vec* restrict, size_t);
void vec_resize(struct vec* restrict, size_t);
void vec_resize_and_zero(struct vec* restrict, size_t);
void vec_clear(struct vec* restrict);
void vec_shrink_to_fit(struct vec* restrict);
void* vec_pop_back(struct vec* restrict);

void* vec_add_one(struct vec* restrict);
void* vec_add_one_front(struct vec* restrict);
void vec_push_back(struct vec* restrict, const void* restrict);

void vec_set_iter(struct vec* restrict, void* pos, const void* begin, const void* back);
void vec_set_one(struct vec* restrict, void* pos, const void* restrict);
void vec_set_at(struct vec* restrict, size_t idx, const void* restrict, size_t len);
void vec_set(struct vec* restrict, void* pos, const void* restrict, size_t len);

void vec_insert_iter(struct vec* restrict,
                     void* pos,
                     const void* begin,
                     const void* back);
void vec_insert_one(struct vec* restrict, void* pos, const void* restrict data);
void vec_insert_at(struct vec* restrict, size_t idx, const void* begin, size_t len);
void vec_insert(struct vec* restrict, void* pos, const void* begin, size_t len);

void vec_erase_iter(struct vec* restrict, void* begin, const void* back);
void vec_erase_one(struct vec* restrict, void* elem);
void vec_erase_at(struct vec* restrict, size_t idx, size_t len);
void vec_erase(struct vec* restrict, void* begin, size_t len);

void vec_append(struct vec* restrict, const void* restrict src, size_t);
void vec_extend(struct vec* restrict dest, const struct vec* restrict src);

void vec_sort_r(struct vec* restrict, qsort_r_cmp_fn, void* context);

#define vec_iter_size_(v_, begin_, back_)                                      \
	(((const char*)back_ - (const char*)begin_) / v_->_elem_size) + 1

#define vec_get_idx_(v_, pos_) vec_iter_size_(v_, vec_begin(v_), pos_) - 1

typedef struct vec bitvec;
bitvec* bitvec_construct(bitvec* restrict);
void bitvec_destroy(bitvec* restrict);
void bitvec_resize(bitvec* restrict, size_t);
bool bitvec_at(const bitvec* restrict, size_t idx);
void bitvec_set(bitvec*, size_t idx, bool);
void bitvec_push_back(bitvec*, bool);

#endif /* VEC_H */
