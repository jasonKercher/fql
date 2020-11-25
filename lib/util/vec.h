#ifndef VEC_H
#define VEC_H

/**
 * Naive as fuck vector implementation
 *
 * data vector is in contiguous memory always
 * ending in a NULL pointer. Therefor, _alloc
 * is guaranteed to be > size
 */

#include <stdlib.h>

struct vec {
        void** vector;  /* the data */
        size_t size;    /* number of elements populated */
        size_t _alloc;  /* number of allocated void* */
};
typedef struct vec Vec;

struct vec* vec_new();
struct vec* vec_new_s(size_t);
void vec_free(struct vec*);

void* vec_begin(struct vec*);
void* vec_end(struct vec*);
void vec_reserve(struct vec*, size_t);
void vec_resize(struct vec*, size_t);
void vec_push_back(struct vec*, void*);
void vec_extend(struct vec* dest, struct vec* src);
void* vec_remove(struct vec*, size_t);

#endif  /* VEC_H */
