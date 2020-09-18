#ifndef VEC_H
#define VEC_H

/** Naive as fuck vector implmentation */

#include <stdlib.h>

#define VEC_BLOCK_SIZE 128

struct vec {
        void** data;
        size_t size;
        size_t _alloc;
};

#define vec_new(X) _Generic((X),                        \
                            int: vec_new_s,             \
                            unsigned: vec_new_s,        \
                            long: vec_new_s,            \
                            size_t: vec_new_s,          \
                            default: vec_new_           \
                            ) (X)

struct vec* vec_new_();
struct vec* vec_new_s(size_t);
void vec_free(struct vec*);

void vec_reserve(struct vec*, size_t);
void vec_resize(struct vec*, size_t);
void vec_push_back(struct vec*, void*);

#endif  /* VEC_H */
