#include "fifo.h"
#include "util.h"

Fifo* fifo_new(size_t elem_size, size_t buf_size)
{
        Fifo* new_fifo = NULL;
        malloc_(new_fifo, sizeof(*new_fifo));

        return fifo_init(new_fifo, elem_size, buf_size);
}

Fifo* fifo_init(Fifo* fifo, size_t elem_size, size_t buf_size)
{
        *fifo = (Fifo) {
                 vec_new(elem_size)     /* buf */
                ,0                      /* head */
                ,0                      /* tail */
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
        vec_resize(fifo->buf, n);
        fifo->head = 0;
        fifo->tail = 0;
}

size_t fifo_available(Fifo* f)
{
        if (f->is_full) {
                return f->buf->size;
        }

        if (f->head >= f->tail) {
                return f->head - f->tail;
        }
        return f->buf->size + f->head - f->tail;
}

_Bool fifo_is_full(Fifo* f)
{
        return f->is_full;
}

_Bool fifo_is_empty(Fifo* f)
{
        return (!f->is_full && (f->head == f->tail));
}

void* fifo_get(Fifo* f)
{
        void* data = vec_at(f->buf, f->tail++);
        f->tail %= f->buf->size;
        f->is_full = false;

        return data;
}

void* fifo_peek(Fifo* f)
{
        return vec_at(f->buf, f->tail);
}

int fifo_add(Fifo* f, void* data)
{
        if (f->is_full) {
                return 1;
        }
        vec_set(f->buf, f->head++, data);
        f->head %= f->buf->size;
        f->is_full = (f->head == f->tail);
         
        return 0;
}
