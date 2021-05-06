#ifndef FIFO_H
#define FIFO_H

#include <stdbool.h>
#include <pthread.h>
#include "vec.h"

#if __STDC_VERSION__ < 201112L
#define ATOMIC_
#else
#include <stdatomic.h>
#define ATOMIC_ _Atomic
#endif

/**
 * naive thread-safe circular buffer
 * this is designed for multiple producer/
 * single consumer. it is not thread-safe
 * for multiple consumers.
 */

struct fifo {
	vec* buf;
	pthread_mutex_t head_mutex;
	pthread_mutex_t tail_mutex;
	pthread_mutex_t open_mutex;
	pthread_cond_t cond_add;
	pthread_cond_t cond_get;
	pthread_cond_t cond_work;
	ATOMIC_ unsigned head;
	ATOMIC_ unsigned tail;
	unsigned input_count;
	_Bool is_open;
};
typedef struct fifo fifo;

struct fifo* fifo_construct(struct fifo*, size_t elem_size, unsigned buf_size);
#define fifo_construct_(this_, T_, n_) fifo_construct(this_, sizeof(T_), n_)
void fifo_destroy(struct fifo*);

void fifo_set_open(struct fifo*, int);
void fifo_resize(struct fifo*, unsigned);
unsigned fifo_available(struct fifo*);
_Bool fifo_is_empty(struct fifo*);
//_Bool fifo_has_work(struct fifo*);
_Bool fifo_is_full(struct fifo*);
_Bool fifo_is_open(struct fifo*);
_Bool fifo_is_receivable(struct fifo*);
void fifo_set_full(struct fifo*);
void* fifo_get(struct fifo*);
void* fifo_peek(struct fifo*);
void fifo_consume(struct fifo*);
int fifo_add(struct fifo*, void*);
int fifo_advance(struct fifo*);


/* thread conditions */
void fifo_wait_for_add(struct fifo*);
void fifo_wait_for_get(struct fifo*);
void fifo_wait_for_work(struct fifo*);


#endif  /* FIFO_H */
