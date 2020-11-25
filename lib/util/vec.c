#include "vec.h"
#include "util.h"

void _vec_backfill(Vec* vec, size_t size);

Vec* vec_new()
{
        Vec* new_vec = NULL;
        malloc_(new_vec, sizeof(*new_vec));

        *new_vec = (Vec) {
                 NULL /* vector */
                ,0    /* size */
                ,0    /* _alloc */
        };

        _vec_backfill(new_vec, 0);

        return new_vec;
}

Vec* vec_new_s(size_t size)
{
        Vec* new_vec = vec_new();
        vec_resize(new_vec, size);
        return new_vec;
}

void vec_free(Vec* vec)
{
        free_(vec->vector);
        free_(vec);
}

void* vec_begin(Vec* vec)
{
        return vec->vector[0];
}

void* vec_end(Vec* vec)
{
        if (vec->size == 0)
                return NULL;

        return vec->vector[vec->size-1];
}

void vec_reserve(Vec* vec, size_t size)
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

void _vec_backfill(Vec* vec, size_t size)
{
        vec_reserve(vec, size);
        size_t i = vec->size;

        /* TODO - just memset this */
        for (; i < size; ++i) {
                vec->vector[i] = NULL;
        }
}

void vec_resize(Vec* vec, size_t size)
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

void vec_push_back(Vec* vec, void* item)
{
        if (vec->_alloc <= ++vec->size)
                vec_reserve(vec, vec->_alloc * 2 + 1);

        vec->vector[vec->size-1] = item;
}

void vec_extend(Vec* dest, Vec* src)
{
        vec_reserve(dest, dest->size + src->size);

        int i = 0;
        for (; i < src->size; ++i) {
                vec_push_back(dest, src->vector[i]);
        }
}

void* vec_remove(Vec* vec, size_t index)
{
        if (index >= vec->size) {
                return NULL;
        }
        void* data = vec->vector[index];

        int i = index;
        for (; i < vec->size; ++i) {
               vec->vector[i] = vec->vector[i+1];
        }

        --vec->size;

        return data;
}
