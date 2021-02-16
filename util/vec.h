#ifndef VEC_H
#define VEC_H

/**
 * Naive as fuck vector implementation
 *
 * vector is in contiguous memory and followed
 * by at least one extra allocation that is
 * referred to by vec_end
 *
 * This used to be a small, elegant struct for
 * storing void*. Then, I wanted to have iterators.
 * Now, it's fugly...
 *
 * Also now... if a vector owns the data it is
 * given, don't keep references to its data
 * outside the vector, or realloc will fuck
 * your day all up.
 */

#include <stdlib.h>
#include <stdbool.h>

struct vec {
        void* data;             /* the data */
        size_t size;            /* number of elements populated */
        size_t _alloc;          /* number of allocated elements */
        size_t _elem_size;      /* size of a single element */
};
typedef struct vec Vec;

struct vec* vec_new(size_t);
#define vec_new_(T_) vec_new(sizeof(T_))
struct vec* vec_construct(struct vec*, size_t);
#define vec_construct_(v_, T_) vec_construct(v_, sizeof(T_))
void vec_destroy(struct vec*);
void vec_free(struct vec*);

_Bool vec_empty(const struct vec*);
void* vec_at(const struct vec*, size_t);
void* vec_begin(const struct vec*);
void* vec_back(const struct vec*);
void* vec_end(const struct vec*);

void vec_reserve(struct vec*, size_t);
void vec_resize(struct vec*, size_t);
void vec_resize_and_zero(struct vec*, size_t);
void vec_clear(struct vec*);
void vec_shrink_to_fit(struct vec*);

void* vec_add_one(struct vec*);
void vec_set(struct vec*, size_t, void*);
void vec_insert(struct vec*, size_t, void*);
void vec_push_back(struct vec*, void*);

void vec_extend(struct vec* dest, const struct vec* src);
void vec_erase(struct vec*, void*);
void vec_remove(struct vec*, size_t);

#endif  /* VEC_H */
