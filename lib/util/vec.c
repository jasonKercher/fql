#include "vec.h"
#include "util.h"

struct vec* vec_new_()
{
        struct vec* new_vec = NULL;
        malloc_(new_vec, sizeof(*new_vec));

        *new_vec = (struct vec) {
                 NULL
                ,0
                ,0
        };

        return new_vec;
}

struct vec* vec_new_s(size_t size)
{
        struct vec* new_vec = vec_new_();
        vec_reserve(new_vec, size);
        return new_vec;
}

void vec_free(struct vec* vec)
{
        free_(vec->data);
        free_(vec);
}

void vec_reserve(struct vec* vec, size_t size)
{
        if (vec->_alloc > ++size) {
                return;
        }

        if (vec->_alloc) {
                realloc_(vec->data, size * sizeof(void*));
        } else {
                malloc_(vec->data, size * sizeof(void*));
        }

        vec->_alloc = size;
}

void vec_resize(struct vec* vec, size_t size)
{
        if (size == vec->size) {
                return;
        }

        if (size > vec->size) {
                vec_reserve(vec, size);
                size_t i = vec->size;

                /* TODO - just memset this */
                for (; i < size; ++i) {
                        vec->data[i] = NULL;
                }
        } 
        /* TODO - else do we want to realloc smaller? */

        vec->data[size] = NULL;
        vec->size = size;
}

void vec_push_back(struct vec* vec, void* item) 
{
        if (vec->_alloc <= ++vec->size)        
                vec_reserve(vec, vec->_alloc + VEC_BLOCK_SIZE);

        vec->data[vec->size-1] = item;
}
