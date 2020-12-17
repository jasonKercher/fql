#include "fifo.h"
#include "util.h"

Fifo* fifo_new(size_t elem_size)
{
        Fifo* new_fifo = NULL;
        malloc_(new_fifo, sizeof(*new_fifo));

        return fifo_init(new_fifo, elem_size);
}

Fifo* fifo_init(Fifo* fifo, size_t elem_size)
{
        *fifo = (Fifo) {
                 vec_new(elem_size)     /* buf */
                ,0                      /* front */
                ,0                      /* back */
        };

        vec_resize(fifo->buf, UCHAR_MAX);

        return fifo;
}

void fifo_free(Fifo* fifo)
{
        vec_free(fifo->buf);
        free_(fifo);
}

unsigned char fifo_available(Fifo* f)
{
        return (unsigned char) f->front - f->back;
}

void* fifo_get(Fifo* f)
{
        return vec_at(f->buf, f->back++);
}

int fifo_add(Fifo* f, void* data)
{
        vec_set(f->buf, f->front++, data);
        return 0;
}
