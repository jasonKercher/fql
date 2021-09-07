#include "fifo.h"
#include "util.h"

#define _idx_adv_(idx_) idx_ = (idx_ + 1) % self->buf->size;

fifo* fifo_construct(fifo* self, size_t elem_size, unsigned buf_size)
{
	/* fifo requires a buffer of atleast size 2 */
	if (buf_size <= 1) {
		buf_size = 2;
	}
	memset(self, 0, sizeof(*self));
	self->buf = new_(vec, elem_size);
	self->is_open = true;
	self->shared_mutex_fifo = NULL;

	vec_resize_and_zero(self->buf, buf_size);

	pthread_mutex_init(&self->head_mutex, NULL);
	pthread_mutex_init(&self->tail_mutex, NULL);
	pthread_mutex_init(&self->open_mutex, NULL);
	pthread_cond_init(&self->cond_add, NULL);
	pthread_cond_init(&self->cond_get, NULL);

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
}

void fifo_free(void* self)
{
	fifo_destroy(self);
	free_(self);
}

void fifo_set_open(fifo* self, int is_open)
{
	pthread_mutex_lock(&self->tail_mutex);
	pthread_mutex_lock(&self->head_mutex);
	pthread_mutex_lock(&self->open_mutex);

	self->is_open = is_open;

	pthread_cond_broadcast(&self->cond_get);
	pthread_cond_broadcast(&self->cond_add);

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

bool fifo_is_open(fifo* self)
{
	pthread_mutex_lock(&self->open_mutex);
	bool ret = self->is_open;
	pthread_mutex_unlock(&self->open_mutex);
	return ret;
}

unsigned fifo_available(const fifo* self)
{
	size_t available = self->head - self->tail;
	if (self->head < self->tail) {
		available += self->buf->size;
	}
	return available;
}

bool fifo_is_empty(const fifo* self)
{
	return (self->head == self->tail);
}

bool fifo_is_full(const fifo* self)
{
	return ((self->head + 1) % self->buf->size == self->tail);
}

unsigned fifo_receivable(fifo* self)
{
	unsigned avail = fifo_available(self);
	return self->buf->size - avail - self->input_count;
}

void fifo_set_full(fifo* self)
{
	self->tail = 0;
	self->head = self->buf->size - 1;
}

void* fifo_get(fifo* self)
{
	pthread_mutex_lock(&self->tail_mutex);
	void* data = fifo_peek(self);
	_idx_adv_(self->tail);
	pthread_cond_signal(&self->cond_get);
	pthread_mutex_unlock(&self->tail_mutex);
	return data;
}

int fifo_nget(fifo* self, vec* buffer, unsigned block_size, unsigned max)
{
	pthread_mutex_lock(&self->tail_mutex);
	unsigned available = fifo_available(self);
	if (available == 0) {
		pthread_mutex_unlock(&self->tail_mutex);
		return 0;
	}
	unsigned transfer_count = (available > max) ? max : available;
	transfer_count -= transfer_count % block_size;
	unsigned new_tail = (self->tail + transfer_count) % self->buf->size;

	if (new_tail > self->tail) {
		vec_append(buffer, vec_at(self->buf, self->tail), new_tail - self->tail);
	} else {
		vec_append(buffer,
		           vec_at(self->buf, self->tail),
		           self->buf->size - self->tail);
		vec_append(buffer, vec_begin(self->buf), new_tail);
	}

	self->tail = new_tail;

	pthread_cond_signal(&self->cond_get);
	pthread_mutex_unlock(&self->tail_mutex);

	/* Return what we *know* is available */
	return available - transfer_count;
}

void* fifo_peek(const fifo* self)
{
	return vec_at(self->buf, self->tail);
}

void* fifo_look_ahead(const fifo* self)
{
	if (fifo_available(self) < 2) {
		return NULL;
	}
	unsigned look_ahead_idx = self->tail;
	_idx_adv_(look_ahead_idx);

	return vec_at(self->buf, look_ahead_idx);
}

void fifo_consume(fifo* self)
{
	pthread_mutex_lock(&self->tail_mutex);
	_idx_adv_(self->tail);
	pthread_cond_signal(&self->cond_get);
	pthread_mutex_unlock(&self->tail_mutex);
}

void fifo_add(fifo* self, void* data)
{
	pthread_mutex_lock(&self->head_mutex);
	vec_set(self->buf, self->head, data);
	pthread_cond_signal(&self->cond_add);
	if (self->shared_mutex_fifo != NULL) {
		pthread_cond_signal(&self->shared_mutex_fifo->cond_add);
		self->shared_mutex_fifo = NULL;
	}
	_idx_adv_(self->head);
	pthread_mutex_unlock(&self->head_mutex);
}

void fifo_nadd(fifo* self, vec* src, unsigned start_idx)
{
	pthread_mutex_lock(&self->head_mutex);

	void* it = vec_at(src, start_idx);
	for (; it != vec_end(src); ++it, ++start_idx) {
		vec_set(self->buf, self->head, it);
		_idx_adv_(self->head);
	}

	pthread_cond_signal(&self->cond_add);
	pthread_mutex_unlock(&self->head_mutex);

	if (start_idx == src->size) {
		vec_clear(src);
	}
}

void fifo_advance(fifo* self)
{
	pthread_mutex_lock(&self->head_mutex);
	_idx_adv_(self->head);
	pthread_cond_signal(&self->cond_add);
	pthread_mutex_unlock(&self->head_mutex);
}


void* fifo_begin(fifo* self)
{
	self->_iter_head = self->head % self->buf->size;
	return fifo_peek(self);
}

void* fifo_iter(fifo* self)
{
	/* consume without mutexes */
	_idx_adv_(self->tail);
	return fifo_peek(self);
}

void* fifo_end(const fifo* self)
{
	return vec_at(self->buf, self->_iter_head);
}

void fifo_update(fifo* self)
{
	pthread_mutex_lock(&self->tail_mutex);
	pthread_cond_signal(&self->cond_get);
	pthread_mutex_unlock(&self->tail_mutex);
}


void fifo_wait_for_add(fifo* self)
{
	pthread_mutex_lock(&self->head_mutex);
	while (fifo_is_open(self) && fifo_is_empty(self)) {
		pthread_cond_wait(&self->cond_add, &self->head_mutex);
	}
	pthread_mutex_unlock(&self->head_mutex);
}

void fifo_wait_for_add_either(fifo* f0, fifo* f1)
{
	pthread_mutex_lock(&f0->head_mutex);
	pthread_mutex_lock(&f1->head_mutex);
	while (fifo_is_open(f1) && fifo_is_empty(f1) && fifo_is_open(f0)
	       && fifo_is_empty(f0)) {
		f0->shared_mutex_fifo = f1;
		pthread_cond_wait(&f1->cond_add, &f1->head_mutex);
	}
	pthread_mutex_unlock(&f1->head_mutex);
	pthread_mutex_unlock(&f0->head_mutex);
}

void fifo_wait_for_add_both(fifo* f0, fifo* f1)
{
	while (fifo_is_open(f1) && fifo_is_open(f0)
	       && (fifo_is_empty(f0) || fifo_is_empty(f1))) {
		fifo_wait_for_add(f0);
		fifo_wait_for_add(f1);
	}
}

void fifo_wait_for_get(fifo* self)
{
	pthread_mutex_lock(&self->tail_mutex);
	while (!fifo_receivable(self)) {
		pthread_cond_wait(&self->cond_get, &self->tail_mutex);
	}
	pthread_mutex_unlock(&self->tail_mutex);
}
