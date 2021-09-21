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
	ATOMIC_(struct fifo*) shared_mutex_fifo; /* for wait for either/both */
	pthread_mutex_t head_mutex;
	pthread_mutex_t tail_mutex;
	pthread_cond_t cond_add;
	pthread_cond_t cond_get;
	ATOMIC_ unsigned head;
	ATOMIC_ unsigned tail;
	ATOMIC_ unsigned _iter_head;
	unsigned input_count;
	bool is_open;
};
typedef struct fifo fifo;

struct fifo* fifo_construct(struct fifo* restrict, size_t elem_size, unsigned buf_size);
#define fifo_construct_(this_, T_, n_) fifo_construct(this_, sizeof(T_), n_)
void fifo_free(void* restrict);
void fifo_destroy(struct fifo* restrict);

void fifo_set_open(struct fifo* restrict, int);
void fifo_resize(struct fifo* restrict, unsigned);
unsigned fifo_available(const struct fifo* restrict);
bool fifo_is_empty(const struct fifo* restrict);
bool fifo_is_full(const struct fifo* restrict);
unsigned fifo_receivable(struct fifo* restrict);
void fifo_set_full(struct fifo* restrict);

void* fifo_get(struct fifo* restrict);
int fifo_nget(struct fifo* restrict,
              struct vec* restrict,
              unsigned block_size,
              unsigned max);
void* fifo_peek(const struct fifo* restrict);
void* fifo_look_ahead(const struct fifo* restrict);
void fifo_consume(struct fifo* restrict);

void fifo_add_eager(struct fifo* restrict, void* restrict);
void fifo_add(struct fifo* restrict, void* restrict);
void fifo_nadd(struct fifo* restrict, struct vec* restrict, unsigned start_idx);
void fifo_advance(struct fifo* restrict);


/* these iterators do not touch mutexes
 * and do not send signals. Up to user
 * to call update() after done iterating.
 */
void* fifo_begin(struct fifo* restrict);
void* fifo_iter(struct fifo* restrict);
void* fifo_end(const struct fifo* restrict);
void fifo_update(struct fifo* restrict);
//#define fifo_iter(f_) fifo_get(f_)

/* thread conditions */
void fifo_wait_for_add(struct fifo* restrict);
void fifo_wait_for_get(struct fifo* restrict);
void fifo_wait_for_add_either(struct fifo* restrict, struct fifo* restrict);
void fifo_wait_for_add_both(struct fifo* restrict, struct fifo* restrict);

#endif /* FIFO_H */
