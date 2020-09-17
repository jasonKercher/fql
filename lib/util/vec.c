#include "vec.h"
#include "util.h"

vec_t* vec_new_()
{
        vec_t* new_vec = NULL;
        malloc_(new_vec, sizeof(*new_vec));

        *new_vec = (vec_t) {
                 NULL
                ,0
                ,0
        };

        return new_vec;
}

vec_t* vec_new_s(size_t size)
{
        vec_t* new_vec = vec_new_();
        vec_reserve(new_vec, size);
        return new_vec;
}

void vec_free(vec_t* vec)
{
        free_(vec->data);
        free_(vec);
}

void vec_reserve(vec_t* vec, size_t size)
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

void vec_resize(vec_t* vec, size_t size)
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

void vec_push_back(vec_t* vec, void* item) 
{
        if (vec->_alloc <= ++vec->size)        
                vec_reserve(vec, vec->_alloc + VEC_BLOCK_SIZE);

        vec->data[vec->size-1] = item;
}
