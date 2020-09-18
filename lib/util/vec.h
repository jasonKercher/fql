#ifndef VEC_H
#define VEC_H

/** Naive as fuck vector implmentation */

#include <stdlib.h>

#define VEC_BLOCK_SIZE 128

struct vector {
        void** data_vec;
        size_t size;
        size_t _alloc;
};

#define vector_new(X) _Generic((X),                        \
                            int: vector_new_s,             \
                            unsigned: vector_new_s,        \
                            long: vector_new_s,            \
                            size_t: vector_new_s,          \
                            default: vector_new_           \
                            ) (X)

struct vector* vector_new_();
struct vector* vector_new_s(size_t);
void vector_free(struct vector*);

void vector_reserve(struct vector*, size_t);
void vector_resize(struct vector*, size_t);
void vector_push_back(struct vector*, void*);

#endif  /* VEC_H */
