#include "vec.h"

#include <limits.h>
#include "util.h"

Vec* vec_new(size_t elem_size)
{
        Vec* new_vec = NULL;
        malloc_(new_vec, sizeof(*new_vec));

        return vec_init(new_vec, elem_size);
}

Vec* vec_init(Vec* vec, size_t elem_size)
{
        *vec = (Vec) {
                 NULL           /* data */
                ,0              /* size */
                ,0              /* _alloc */
                ,elem_size      /* _elem_s */
        };

        /* Allocate space for first element and
         * trailing NULL pointer
         */
        malloc_(vec->data, 2 * vec->_elem_size);

        return vec;
}

void vec_destroy(Vec* vec)
{
        free_(vec->data);
}

void vec_free(Vec* vec)
{
        vec_destroy(vec);
        free_(vec);
}

_Bool vec_empty(const Vec* vec)
{
        return vec->size == 0;
}

void* vec_at(const Vec* vec, size_t index)
{
        return (char*) vec->data + vec->_elem_size * index;
}

void* vec_begin(const Vec* vec)
{
        return vec->data;
}

void* vec_back(const Vec* vec)
{
        if (vec->size == 0) {
                return vec->data;
        }
        return vec_at(vec, vec->size - 1);
}

void* vec_end(const Vec* vec)
{
        return vec_at(vec, vec->size);
}

/** Allocate size + 1 for iterator end **/
void vec_reserve(Vec* vec, size_t size)
{
        if (vec->_alloc > ++size) {
                return;
        }
        realloc_(vec->data, size * vec->_elem_size);
        vec->_alloc = size;
}

void vec_resize(Vec* vec, size_t size)
{
        if (size == vec->size) {
                return;
        }

        if (size > vec->size) {
                vec_reserve(vec, size);
        }

        vec->size = size;
}

void vec_clear(Vec* vec)
{
        vec_resize(vec, 0);
}

void vec_shrink_to_fit(Vec* vec)
{
        realloc_(vec->data, (vec->size + 1) * vec->_elem_size);
        vec->_alloc = vec->size + 1;
}

void* vec_add_one(Vec* vec)
{
        if (vec->_alloc <= ++vec->size) {
                vec_reserve(vec, vec->_alloc * 2);
        }

        return vec_back(vec);
}

void vec_set(Vec* vec, size_t n, void* src)
{
        void* dest = vec_at(vec, n);
        memcpy(dest, src, vec->_elem_size);
}

void vec_push_back(Vec* vec, void* item)
{
        memcpy(vec_add_one(vec), item, vec->_elem_size);
}

void vec_extend(Vec* dest, const Vec* src)
{
        int index = dest->size;
        vec_resize(dest, dest->size + src->size);
        void* end = vec_at(dest, index);
        size_t bytes = src->_elem_size * (src->size + 1);
        memcpy(end, vec_begin(src), bytes);
}

void vec_erase(Vec* vec, void* elem)
{
        size_t bytes = (char*)vec_end(vec) - (char*)elem;
        memmove(elem, (char*)elem + vec->_elem_size, bytes);
        --vec->size;
}

void vec_remove(Vec* vec, size_t index)
{
        if (index >= vec->size) {
                return; /* abort? */
        }

        vec_erase(vec, vec_at(vec, index));
}
