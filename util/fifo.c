#include "fifo.h"
#include "util.h"
#include <stdbool.h>

Fifo* fifo_new(size_t elem_size, size_t buf_size)
{
        Fifo* new_fifo = NULL;
        malloc_(new_fifo, sizeof(*new_fifo));

        return fifo_construct(new_fifo, elem_size, buf_size);
}

Fifo* fifo_construct(Fifo* fifo, size_t elem_size, size_t buf_size)
{
        /* Fifo requires a buffer of atleast size 1 */
        if (buf_size <= 1) {
                buf_size = 2;
        }
        *fifo = (Fifo) {
                 vec_new(elem_size)     /* buf */
                ,{ 0 }                  /* head_mutex */
                ,{ 0 }                  /* tail_mutex */
                ,{ 0 }                  /* open_mutex */
                ,{ 0 }                  /* cond_add */
                ,{ 0 }                  /* cond_get */
                ,0                      /* head */
                ,0                      /* tail */
                ,0                      /* input_count */
                ,true                   /* is_open */
        };

        vec_resize(fifo->buf, buf_size);
        pthread_mutex_init(&fifo->head_mutex, NULL);
        pthread_mutex_init(&fifo->tail_mutex, NULL);
        pthread_mutex_init(&fifo->open_mutex, NULL);
        pthread_cond_init(&fifo->cond_add, NULL);
        pthread_cond_init(&fifo->cond_get, NULL);

        return fifo;
}

void fifo_free(Fifo* fifo)
{
        vec_free(fifo->buf);
        pthread_mutex_destroy(&fifo->head_mutex);
        pthread_mutex_destroy(&fifo->tail_mutex);
        pthread_mutex_destroy(&fifo->open_mutex);
        pthread_cond_destroy(&fifo->cond_add);
        pthread_cond_destroy(&fifo->cond_get);
        free_(fifo);
}

void fifo_close(Fifo* fifo)
{
        pthread_mutex_lock(&fifo->head_mutex);
        pthread_mutex_lock(&fifo->tail_mutex);
        pthread_mutex_lock(&fifo->open_mutex);
        fifo->is_open = false;

        pthread_cond_broadcast(&fifo->cond_add);
        pthread_cond_broadcast(&fifo->cond_get);

        pthread_mutex_unlock(&fifo->open_mutex);
        pthread_mutex_unlock(&fifo->tail_mutex);
        pthread_mutex_unlock(&fifo->head_mutex);
}

_Bool fifo_is_open(Fifo* fifo)
{
        pthread_mutex_lock(&fifo->open_mutex);
        _Bool ret = fifo->is_open;
        pthread_mutex_unlock(&fifo->open_mutex);

        return ret;
}

/** NOT Thread-safe **/
void fifo_resize(Fifo* fifo, size_t n)
{
        /* Fifo requires a buffer of atleast size 1 */
        if (n <= 1) {
                n = 2;
        }
        vec_resize(fifo->buf, n);
        fifo->head = 0;
        fifo->tail = 0;
}

size_t fifo_available(Fifo* f)
{
        pthread_mutex_lock(&f->head_mutex);
        pthread_mutex_lock(&f->tail_mutex);
        size_t available = f->head - f->tail;
        if (f->head < f->tail) {
                available += f->buf->size;
        }
        pthread_mutex_unlock(&f->tail_mutex);
        pthread_mutex_unlock(&f->head_mutex);
        return available;
}

_Bool fifo_is_full(Fifo* f)
{
        pthread_mutex_lock(&f->head_mutex);
        pthread_mutex_lock(&f->tail_mutex);
        _Bool is_full = ((f->head + 1) % f->buf->size == f->tail);
        pthread_mutex_unlock(&f->tail_mutex);
        pthread_mutex_unlock(&f->head_mutex);
        return is_full;
}

_Bool fifo_is_empty(Fifo* f)
{
        pthread_mutex_lock(&f->head_mutex);
        pthread_mutex_lock(&f->tail_mutex);
        _Bool is_empty = (f->head == f->tail);
        pthread_mutex_unlock(&f->tail_mutex);
        pthread_mutex_unlock(&f->head_mutex);
        return is_empty;
}

/* Add input_count to current number of current
 * available elements to avoid clobber
 */
_Bool fifo_is_receivable(Fifo* f)
{
        return (fifo_available(f) + f->input_count + 1 < f->buf->size);
}

//_Bool fifo_has_data(Fifo* f)
//{
//        return (f->is_open && fifo_available(f));
//}

void* fifo_get(Fifo* f)
{
        void* data = fifo_peek(f);
        fifo_consume(f);
        return data;
}

void* fifo_peek(Fifo* f)
{
        pthread_mutex_lock(&f->tail_mutex);
        void* data = vec_at(f->buf, f->tail);
        pthread_mutex_unlock(&f->tail_mutex);
        return data;
}

void fifo_consume(Fifo* f)
{
        pthread_mutex_lock(&f->tail_mutex);
        ++f->tail;
        f->tail %= f->buf->size;
        pthread_cond_signal(&f->cond_get);
        pthread_mutex_unlock(&f->tail_mutex);
}

int fifo_add(Fifo* f, void* data)
{
        pthread_mutex_lock(&f->head_mutex);
        vec_set(f->buf, f->head, data);
        ++f->head;
        f->head %= f->buf->size;
        pthread_cond_signal(&f->cond_add);
        pthread_mutex_unlock(&f->head_mutex);

        return 0;
}

int fifo_advance(Fifo* f)
{
        pthread_mutex_lock(&f->head_mutex);
        ++f->head;
        f->head %= f->buf->size;
        pthread_cond_signal(&f->cond_add);
        pthread_mutex_unlock(&f->head_mutex);

        return 0;
}

void fifo_wait_for_add(Fifo* f)
{
        pthread_mutex_lock(&f->head_mutex);
        pthread_mutex_lock(&f->tail_mutex);
        pthread_mutex_lock(&f->open_mutex);
        while (f->is_open && f->head == f->tail) {
                pthread_cond_wait(&f->cond_add, &f->tail_mutex);
        }
        pthread_mutex_unlock(&f->open_mutex);
        pthread_mutex_unlock(&f->tail_mutex);
        pthread_mutex_unlock(&f->head_mutex);
}

void fifo_wait_for_get(Fifo* f)
{
        pthread_mutex_lock(&f->head_mutex);
        pthread_mutex_lock(&f->tail_mutex);
        while ((f->head + 1) % f->buf->size == f->tail) {
                pthread_cond_wait(&f->cond_get, &f->head_mutex);
        }
        pthread_mutex_unlock(&f->tail_mutex);
        pthread_mutex_unlock(&f->head_mutex);
}
