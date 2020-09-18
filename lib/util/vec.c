#include "vec.h"
#include "util.h"

struct vector* vector_new_()
{
        struct vector* new_vec = NULL;
        malloc_(new_vec, sizeof(*new_vec));

        *new_vec = (struct vector) {
                 NULL
                ,0
                ,0
        };

        return new_vec;
}

struct vector* vector_new_s(size_t size)
{
        struct vector* new_vec = vector_new_();
        vector_reserve(new_vec, size);
        return new_vec;
}

void vector_free(struct vector* vec)
{
        free_(vec->data_vec);
        free_(vec);
}

void vector_reserve(struct vector* vec, size_t size)
{
        if (vec->_alloc > ++size) {
                return;
        }

        if (vec->_alloc) {
                realloc_(vec->data_vec, size * sizeof(void*));
        } else {
                malloc_(vec->data_vec, size * sizeof(void*));
        }

        vec->_alloc = size;
}

void vector_resize(struct vector* vec, size_t size)
{
        if (size == vec->size) {
                return;
        }

        if (size > vec->size) {
                vector_reserve(vec, size);
                size_t i = vec->size;

                /* TODO - just memset this */
                for (; i < size; ++i) {
                        vec->data_vec[i] = NULL;
                }
        } 
        /* TODO - else do we want to realloc smaller? */

        vec->data_vec[size] = NULL;
        vec->size = size;
}

void vector_push_back(struct vector* vec, void* item) 
{
        if (vec->_alloc <= ++vec->size)        
                vector_reserve(vec, vec->_alloc + VEC_BLOCK_SIZE);

        vec->data_vec[vec->size-1] = item;
}
