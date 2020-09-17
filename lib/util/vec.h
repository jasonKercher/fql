#ifndef VEC_H
#define VEC_H

/** Naive as fuck vector implmentation */

#include <stdlib.h>

#define VEC_BLOCK_SIZE 128

struct vec_t {
        void** data;
        size_t size;
        size_t _alloc;
};

typedef struct vec_t vec_t;

#define vec_new(X) _Generic((X),                        \
                            int: vec_new_s,             \
                            unsigned: vec_new_s,        \
                            long: vec_new_s,            \
                            size_t: vec_new_s,          \
                            default: vec_new_           \
                            ) (X)

vec_t* vec_new_();
vec_t* vec_new_s(size_t);
void vec_free(vec_t*);

void vec_reserve(vec_t*, size_t);
void vec_resize(vec_t*, size_t);
void vec_push_back(vec_t*, void*);

#endif  /* VEC_H */
