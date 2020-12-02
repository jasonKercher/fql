#ifndef VEC_H
#define VEC_H

/**
 * Naive as fuck vector implementation
 *
 * data vector is in contiguous memory always
 * ending in a iterator end. Therefore, _alloc
 * is guaranteed to be > size
 *
 * This used to be a small, elegant struct for
 * storing void*. Then, I wanted to have itertors.
 * Now, it's fugly...
 *
 * Also now... This vector owns the data it is
 * given. Don't keep references to its data 
 * outside the vector, or realloc will fuck
 * your day all up.
 */

#include <stdlib.h>

struct vec {
        void* vector;           /* the data */
        size_t size;            /* number of elements populated */
        size_t _alloc;          /* number of allocated elements */
        size_t _elem_size;      /* size of a single element */
};
typedef struct vec Vec;

struct vec* vec_new(size_t);
#define vec_new_(T_) vec_new(sizeof(T_))
struct vec* vec_init(struct vec*, size_t);
void vec_free(struct vec*);

void* vec_at(struct vec*, size_t);
void* vec_begin(struct vec*);
void* vec_back(struct vec*);
void* vec_end(struct vec*);

void vec_reserve(struct vec*, size_t);
void vec_resize(struct vec*, size_t);
void vec_shrink_to_fit(struct vec*);

void* vec_add(struct vec*);
void vec_push_back(struct vec*, void*);

void vec_extend(struct vec* dest, struct vec* src);
void vec_erase(struct vec*, void*);
void vec_remove(struct vec*, size_t);

#endif  /* VEC_H */
