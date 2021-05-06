#include "fifo.h"
#include "util.h"

fifo* fifo_construct(fifo* self, size_t elem_size, unsigned buf_size)
{
	/* fifo requires a buffer of atleast size 2 */
	if (buf_size <= 1) {
		buf_size = 2;
	}
	*self = (fifo) {
		 new_(vec, elem_size)   /* buf */
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

	vec_resize(self->buf, buf_size);

	/* build these no matter what
	 * this makes all fifos interchangeable.
	 */
	pthread_mutex_init(&self->head_mutex, NULL);
	pthread_mutex_init(&self->tail_mutex, NULL);
	pthread_mutex_init(&self->open_mutex, NULL);
	pthread_cond_init(&self->cond_add, NULL);
	pthread_cond_init(&self->cond_get, NULL);
	pthread_cond_init(&self->cond_work, NULL);

	return self;
}

void fifo_destroy(fifo* self)
{
	delete_(vec, self->buf);
	pthread_mutex_destroy(&self->head_mutex);
	pthread_mutex_destroy(&self->tail_mutex);
	pthread_mutex_destroy(&self->open_mutex);
	pthread_cond_destroy(&self->cond_add);
	pthread_cond_destroy(&self->cond_get);
	pthread_cond_destroy(&self->cond_work);
}

void fifo_set_open(fifo* self, int is_open)
{
	pthread_mutex_lock(&self->tail_mutex);
	pthread_mutex_lock(&self->head_mutex);
	pthread_mutex_lock(&self->open_mutex);

	self->is_open = is_open;

	pthread_cond_broadcast(&self->cond_get);
	pthread_cond_broadcast(&self->cond_add);
	pthread_cond_broadcast(&self->cond_work);

	pthread_mutex_unlock(&self->open_mutex);
	pthread_mutex_unlock(&self->head_mutex);
	pthread_mutex_unlock(&self->tail_mutex);
}

/* this struct was never meant to be resizable
 * but under the assumption that you are okay
 * losing all currently held data... yea...
 * go for it. (definitely not thread-safe
 */
void fifo_resize(fifo* self, unsigned n)
{
	/* fifo requires a buffer of atleast size 2 */
	if (n <= 1) {
		n = 2;
	}
	vec_resize(self->buf, n);
	self->head = 0;
	self->tail = 0;
}

_Bool fifo_is_open(fifo* self)
{
	pthread_mutex_lock(&self->open_mutex);
	_Bool ret = self->is_open;
	pthread_mutex_unlock(&self->open_mutex);
	return ret;
}

unsigned fifo_available(fifo* f)
{
	size_t available = f->head - f->tail;
	if (f->head < f->tail) {
		available += f->buf->size;
	}
	return available;
}

_Bool fifo_is_empty(fifo* f)
{
	return (f->head == f->tail);
}

_Bool fifo_has_work(fifo* f)
{
	return (fifo_available(f) >= (3 * f->buf->size) / 4);
}

_Bool fifo_is_full(fifo* f)
{
	return ((f->head + 1) % f->buf->size == f->tail);
}

/* add input_count to current number of current
 * available elements to avoid clobber
 */
_Bool fifo_is_receivable(fifo* f)
{
	return (fifo_available(f) + f->input_count + 1 < f->buf->size);
}


void fifo_set_full(fifo* f)
{
	f->tail = 0;
	f->head = f->buf->size - 1;
}

void* fifo_get(fifo* f)
{
	void* data = fifo_peek(f);
	fifo_consume(f);
	return data;
}

void* fifo_peek(fifo* f)
{
	return vec_at(f->buf, f->tail);
}

void fifo_consume(fifo* f)
{
	pthread_mutex_lock(&f->tail_mutex);
	++f->tail;
	f->tail %= f->buf->size;
	pthread_cond_signal(&f->cond_get);
	pthread_mutex_unlock(&f->tail_mutex);
}

int fifo_add(fifo* f, void* data)
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

int fifo_advance(fifo* f)
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

void fifo_wait_for_add(fifo* f)
{
	pthread_mutex_lock(&f->head_mutex);
	while (fifo_is_open(f) && fifo_is_empty(f)) {
		pthread_cond_wait(&f->cond_add, &f->head_mutex);
	}
	pthread_mutex_unlock(&f->head_mutex);
}

void fifo_wait_for_get(fifo* f)
{
	pthread_mutex_lock(&f->tail_mutex);
	while (!fifo_is_receivable(f)) {
		pthread_cond_wait(&f->cond_get, &f->tail_mutex);
	}
	pthread_mutex_unlock(&f->tail_mutex);
}

void fifo_wait_for_work(fifo* f)
{
	pthread_mutex_lock(&f->head_mutex);
	while (fifo_is_open(f) && !fifo_has_work(f)) {
		pthread_cond_wait(&f->cond_work, &f->head_mutex);
	}
	pthread_mutex_unlock(&f->head_mutex);
}
