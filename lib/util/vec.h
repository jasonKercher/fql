#ifndef VEC_H
#define VEC_H

/** Naive as fuck vec implementation */

#include <stdlib.h>

struct vec {
        void** vector;
        size_t size;
        size_t _alloc;
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

#endif  /* VEC_H */
