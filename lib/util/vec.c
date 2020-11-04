#include "vec.h"
#include "util.h"

void _vec_backfill(struct vec* vec, size_t size);

struct vec* vec_new()
{
        struct vec* new_vec = NULL;
        malloc_(new_vec, sizeof(*new_vec));

        *new_vec = (struct vec) {
                 NULL /* vector */
                ,0    /* size */
                ,0    /* _alloc */
        };

        _vec_backfill(new_vec, 0);

        return new_vec;
}

struct vec* vec_new_s(size_t size)
{
        struct vec* new_vec = vec_new();
        vec_resize(new_vec, size);
        return new_vec;
}

void vec_free(struct vec* vec)
{
        free_(vec->vector);
        free_(vec);
}

void* vec_end(struct vec* vec)
{
        if (vec->size == 0)
                return NULL;

        return vec->vector[vec->size-1];
}

void vec_reserve(struct vec* vec, size_t size)
{
        if (vec->_alloc > ++size) {
                return;
        }

        if (vec->_alloc) {
                realloc_(vec->vector, size * sizeof(void*));
        } else {
                malloc_(vec->vector, size * sizeof(void*));
        }

        vec->_alloc = size;
}

void _vec_backfill(struct vec* vec, size_t size)
{
        vec_reserve(vec, size);
        size_t i = vec->size;
        
        /* TODO - just memset this */
        for (; i < size; ++i) {
                vec->vector[i] = NULL;
        }
}

void vec_resize(struct vec* vec, size_t size)
{
        if (size == vec->size) {
                return;
        }

        if (size > vec->size) {
                _vec_backfill(vec, size);
        } 

        vec->vector[size] = NULL;
        vec->size = size;
}

void vec_push_back(struct vec* vec, void* item) 
{
        if (vec->_alloc <= ++vec->size)        
                vec_reserve(vec, vec->_alloc * 2 + 1);

        vec->vector[vec->size-1] = item;
}

void vec_extend(struct vec* dest, struct vec* src)
{
        vec_reserve(dest, dest->size + src->size);

        int i = 0;
        for (; i < src->size; ++i) {
                vec_push_back(dest, src->vector[i]);
        }
}
