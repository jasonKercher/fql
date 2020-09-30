#ifndef VEC_H
#define VEC_H

/** Naive as fuck vector implementation */

#include <stdlib.h>

struct vector {
        void** data_vec;
        size_t size;
        size_t _alloc;
};

struct vector* vector_new();
struct vector* vector_new_s(size_t);
void vector_free(struct vector*);

void* vector_end(struct vector*);
void vector_reserve(struct vector*, size_t);
void vector_resize(struct vector*, size_t);
void vector_push_back(struct vector*, void*);

#endif  /* VEC_H */
