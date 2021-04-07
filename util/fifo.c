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
		,0                      /* input_count */
		,true                   /* is_open */
	};

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

void fifo_set_open(Fifo* fifo, int is_open)
{
	pthread_mutex_lock(&fifo->tail_mutex);
	pthread_mutex_lock(&fifo->head_mutex);
	pthread_mutex_lock(&fifo->open_mutex);

	fifo->is_open = is_open;

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

_Bool fifo_is_empty(Fifo* f)
{
	return (f->head == f->tail);
}

_Bool fifo_has_work(Fifo* f)
{
	return (fifo_available(f) >= (3 * f->buf->size) / 4);
}

_Bool fifo_is_full(Fifo* f)
{
	return ((f->head + 1) % f->buf->size == f->tail);
}

/* Add input_count to current number of current
 * available elements to avoid clobber
 */
_Bool fifo_is_receivable(Fifo* f)
{
	return (fifo_available(f) + f->input_count + 1 < f->buf->size);
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

void* fifo_peek(Fifo* f)
{
	return vec_at(f->buf, f->tail);
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
	if (fifo_has_work(f)) {
		pthread_cond_signal(&f->cond_work);
	}
	pthread_mutex_unlock(&f->head_mutex);

	return 0;
}

int fifo_advance(Fifo* f)
{
	pthread_mutex_lock(&f->head_mutex);
	++f->head;
	f->head %= f->buf->size;
	pthread_cond_signal(&f->cond_add);
	if (fifo_has_work(f)) {
		pthread_cond_signal(&f->cond_work);
	}
	pthread_mutex_unlock(&f->head_mutex);

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
	pthread_mutex_lock(&f->head_mutex);
	while (fifo_is_open(f) && !fifo_has_work(f)) {
		pthread_cond_wait(&f->cond_work, &f->head_mutex);
	}
	pthread_mutex_unlock(&f->head_mutex);
}
