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
                ,0                      /* head */
                ,0                      /* tail */
                ,true                   /* is_open */
        };

        vec_resize(fifo->buf, buf_size);

        return fifo;
}

void fifo_free(Fifo* fifo)
{
        vec_free(fifo->buf);
        free_(fifo);
}

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
        if (f->head >= f->tail) {
                return f->head - f->tail;
        }
        return f->buf->size + f->head - f->tail;
}

_Bool fifo_is_full(Fifo* f)
{
        return ((f->head + 1) % f->buf->size == f->tail);
}

_Bool fifo_is_empty(Fifo* f)
{
        return (f->head == f->tail);
}

_Bool fifo_is_receivable(Fifo* f)
{
        return (f->is_open && !fifo_is_full(f));
}

_Bool fifo_has_data(Fifo* f)
{
        return (f->is_open && fifo_available(f));
}

void* fifo_get(Fifo* f)
{
        void* data = fifo_peek(f);
        fifo_consume(f);
        return data;
}

void* fifo_peek(Fifo* f)
{
        return vec_at(f->buf, f->tail);
}

void fifo_consume(Fifo* f)
{
        ++f->tail;
        f->tail %= f->buf->size;
}

int fifo_add(Fifo* f, void* data)
{
        if (fifo_is_full(f)) {
                return 1;
        }
        vec_set(f->buf, f->head, data);
        ++f->head;
        f->head %= f->buf->size;

        return 0;
}

int fifo_advance(Fifo* f)
{
        if (fifo_is_full(f)) {
                return 1;
        }
        ++f->head;
        f->head %= f->buf->size;

        return 0;
}


