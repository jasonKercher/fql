#include "fifo.h"
#include "util.h"

Fifo* fifo_new(size_t elem_size, unsigned buf_size)
{
        Fifo* new_fifo = NULL;
        malloc_(new_fifo, sizeof(*new_fifo));

        return fifo_construct(new_fifo, elem_size, buf_size);
}

Fifo* fifo_construct(Fifo* fifo, size_t elem_size, unsigned buf_size)
{
        /* Fifo requires a buffer of atleast size 2 */
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
                ,{ 0 }                  /* cond_work */
                ,0                      /* head */
                ,0                      /* tail */
                ,buf_size / 10          /* work_min */
                //,UINT_MAX               /* work_target */
                ,0                      /* input_count */
                ,true                   /* is_open */
        };

        //assert(fifo->work_min > 0);

        vec_resize(fifo->buf, buf_size);

        /* Build these no matter what
         * This makes all fifos interchangeable.
         */
        pthread_mutex_init(&fifo->head_mutex, NULL);
        pthread_mutex_init(&fifo->tail_mutex, NULL);
        pthread_mutex_init(&fifo->open_mutex, NULL);
        pthread_cond_init(&fifo->cond_add, NULL);
        pthread_cond_init(&fifo->cond_get, NULL);
        pthread_cond_init(&fifo->cond_work, NULL);

        return fifo;
}

void fifo_free(Fifo* fifo)
{
        fifo_destroy(fifo);
        free_(fifo);
}

void fifo_destroy(Fifo* fifo)
{
        vec_free(fifo->buf);
        pthread_mutex_destroy(&fifo->head_mutex);
        pthread_mutex_destroy(&fifo->tail_mutex);
        pthread_mutex_destroy(&fifo->open_mutex);
        pthread_cond_destroy(&fifo->cond_add);
        pthread_cond_destroy(&fifo->cond_get);
        pthread_cond_destroy(&fifo->cond_work);
}

void fifo_set_pipeline_size(Fifo* f, int pipeline_size)
{
        f->work_min = f->buf->size / 2 * pipeline_size;
        //assert(fifo->work_min > 0);
}

void fifo_set_open(Fifo* fifo, int is_open)
{
        fifo->is_open = is_open;
}

void fifo_set_open_ts(Fifo* fifo, int is_open)
{
        pthread_mutex_lock(&fifo->tail_mutex);
        pthread_mutex_lock(&fifo->head_mutex);
        pthread_mutex_lock(&fifo->open_mutex);
        fifo_set_open(fifo, is_open);

        pthread_cond_broadcast(&fifo->cond_get);
        pthread_cond_broadcast(&fifo->cond_add);
        pthread_cond_broadcast(&fifo->cond_work);

        pthread_mutex_unlock(&fifo->open_mutex);
        pthread_mutex_unlock(&fifo->head_mutex);
        pthread_mutex_unlock(&fifo->tail_mutex);
}

/* This struct was never meant to be resizable
 * But under the assumption that you are okay
 * losing all currently held data... yea...
 * go for it. (Definitely not thread-safe
 */
void fifo_resize(Fifo* fifo, unsigned n)
{
        /* Fifo requires a buffer of atleast size 2 */
        if (n <= 1) {
                n = 2;
        }
        vec_resize(fifo->buf, n);
        fifo->head = 0;
        fifo->tail = 0;
}

_Bool fifo_is_open(Fifo* fifo)
{
        return fifo->is_open;
}

_Bool fifo_is_open_ts(Fifo* fifo)
{
        pthread_mutex_lock(&fifo->open_mutex);
        _Bool ret = fifo->is_open;
        pthread_mutex_unlock(&fifo->open_mutex);

        return ret;
}

unsigned fifo_available(Fifo* f)
{
        size_t available = f->head - f->tail;
        if (f->head < f->tail) {
                available += f->buf->size;
        }
        return available;
}

unsigned fifo_available_ts(Fifo* f)
{
        pthread_mutex_lock(&f->tail_mutex);
        pthread_mutex_lock(&f->head_mutex);
        size_t available = f->head - f->tail;
        if (f->head < f->tail) {
                available += f->buf->size;
        }
        pthread_mutex_unlock(&f->head_mutex);
        pthread_mutex_unlock(&f->tail_mutex);
        return available;
}

_Bool fifo_is_empty(Fifo* f)
{
        return (f->head == f->tail);
}

_Bool fifo_is_empty_ts(Fifo* f)
{
        //pthread_mutex_lock(&f->tail_mutex);
        pthread_mutex_lock(&f->head_mutex);
        _Bool is_empty = (f->head == f->tail);
        pthread_mutex_unlock(&f->head_mutex);
        //pthread_mutex_unlock(&f->tail_mutex);
        return is_empty;
}

_Bool fifo_has_work(Fifo* f)
{
        return (fifo_available(f) >= f->work_min);
        //return (f->head == f->work_target);
}

//_Bool fifo_has_work_ts(Fifo* f)
//{
//        //pthread_mutex_lock(&f->tail_mutex);
//        pthread_mutex_lock(&f->head_mutex);
//        _Bool has_work = fifo_has_work(f);
//        pthread_mutex_unlock(&f->head_mutex);
//        //pthread_mutex_unlock(&f->tail_mutex);
//        return has_work;
//}

_Bool fifo_is_full(Fifo* f)
{
        return ((f->head + 1) % f->buf->size == f->tail);
}

_Bool fifo_is_full_ts(Fifo* f)
{
        pthread_mutex_lock(&f->tail_mutex);
        pthread_mutex_lock(&f->head_mutex);
        _Bool is_full = fifo_is_full(f);
        pthread_mutex_unlock(&f->head_mutex);
        pthread_mutex_unlock(&f->tail_mutex);
        return is_full;
}

/* Add input_count to current number of current
 * available elements to avoid clobber
 */
_Bool fifo_is_receivable(Fifo* f)
{
        return (fifo_available(f) + f->input_count + 1 < f->buf->size);
}

_Bool fifo_is_receivable_ts(Fifo* f)
{
        return (fifo_available_ts(f) + f->input_count + 1 < f->buf->size);
}

void fifo_set_full(Fifo* f)
{
        f->tail = 0;
        f->head = f->buf->size - 1;
}

void* fifo_get(Fifo* f)
{
        void* data = fifo_peek(f);
        fifo_consume(f);
        return data;
}

void* fifo_get_ts(Fifo* f)
{
        void* data = fifo_peek_ts(f);
        fifo_consume_ts(f);
        return data;
}

void* fifo_peek(Fifo* f)
{
        return vec_at(f->buf, f->tail);
}

void* fifo_peek_ts(Fifo* f)
{
        pthread_mutex_lock(&f->tail_mutex);
        void* data = fifo_peek(f);
        pthread_mutex_unlock(&f->tail_mutex);
        return data;
}

void fifo_consume(Fifo* f)
{
        ++f->tail;
        f->tail %= f->buf->size;
}

void fifo_consume_ts(Fifo* f)
{
        pthread_mutex_lock(&f->tail_mutex);
        fifo_consume(f);
        pthread_cond_signal(&f->cond_get);
        pthread_mutex_unlock(&f->tail_mutex);
}

int fifo_recycle(Fifo* f, void* data)
{
        size_t waste_idx = f->tail - 1;
        waste_idx %= f->buf->size;
        vec_set(f->buf, waste_idx, data);
        fifo_advance(f);

        return 0;
}

int fifo_recycle_ts(Fifo* f, void* data)
{
        pthread_mutex_lock(&f->tail_mutex);
        //pthread_mutex_lock(&f->head_mutex);
        fifo_recycle(f, data);
        pthread_cond_signal(&f->cond_add);
        /* No need to signal here in this application */
        //if (!fifo_is_receivable(f)) {
        //        pthread_cond_signal(&f->cond_work);
        //}
        //pthread_mutex_unlock(&f->head_mutex);
        pthread_mutex_unlock(&f->tail_mutex);

        return 0;
}

int fifo_add(Fifo* f, void* data)
{
        vec_set(f->buf, f->head, data);
        fifo_advance(f);

        return 0;
}

int fifo_add_ts(Fifo* f, void* data)
{
        //pthread_mutex_lock(&f->tail_mutex);
        pthread_mutex_lock(&f->head_mutex);
        fifo_add(f, data);
        pthread_cond_signal(&f->cond_add);
        if (fifo_has_work(f)) {
                pthread_cond_signal(&f->cond_work);
        }
        pthread_mutex_unlock(&f->head_mutex);
        //pthread_mutex_unlock(&f->tail_mutex);

        return 0;
}

int fifo_advance(Fifo* f)
{
        ++f->head;
        f->head %= f->buf->size;

        return 0;
}

int fifo_advance_ts(Fifo* f)
{
        //pthread_mutex_lock(&f->tail_mutex);
        pthread_mutex_lock(&f->head_mutex);
        fifo_advance(f);
        pthread_cond_signal(&f->cond_add);
        if (fifo_has_work(f)) {
                pthread_cond_signal(&f->cond_work);
        }
        pthread_mutex_unlock(&f->head_mutex);
        //pthread_mutex_unlock(&f->tail_mutex);

        return 0;
}

void fifo_wait_for_add(Fifo* f)
{
        pthread_mutex_lock(&f->head_mutex);
        while (fifo_is_open(f) && fifo_is_empty(f)) {
                pthread_cond_wait(&f->cond_add, &f->head_mutex);
        }
        pthread_mutex_unlock(&f->head_mutex);
}

void fifo_wait_for_get(Fifo* f)
{
        pthread_mutex_lock(&f->tail_mutex);
        while (!fifo_is_receivable(f)) {
                pthread_cond_wait(&f->cond_get, &f->tail_mutex);
        }
        pthread_mutex_unlock(&f->tail_mutex);
}

void fifo_wait_for_work(Fifo* f)
{
        /* get work target, so we don't need
         * to lock tail while checking for work
         */
        //pthread_mutex_lock(&f->tail_mutex);
        //pthread_mutex_lock(&f->head_mutex);
        //unsigned available = fifo_available(f);
        //f->work_target = (f->work_min - available) + f->head;
        //f->work_target %= f->buf->size;
        //pthread_mutex_unlock(&f->head_mutex);
        //pthread_mutex_unlock(&f->tail_mutex);

        //if (available >= f->work_min) {
        //        return;
        //}

        pthread_mutex_lock(&f->head_mutex);
        /* No loop here in case we never fill. */
        //if (!fifo_has_work(f)) {
        while (fifo_is_open(f) && !fifo_has_work(f)) {
                pthread_cond_wait(&f->cond_work, &f->head_mutex);
                //return (fifo_available(f) + f->input_count + 1 < f->buf->size);
        }
        pthread_mutex_unlock(&f->head_mutex);
}
