#ifndef FIFO_H
#define FIFO_H

#include <stdbool.h>
#include <pthread.h>
#include "vec.h"

/**
 * Naive Thread-safe Circular Buffer
 * Bounds checking is the responsibilty user
 */

struct fifo {
        Vec* buf;
        pthread_mutex_t head_mutex;
        pthread_mutex_t tail_mutex;
        pthread_mutex_t open_mutex;
        pthread_cond_t cond_add;
        pthread_cond_t cond_get;
        pthread_cond_t cond_half;
        size_t head;
        size_t tail;
        unsigned input_count;
        _Bool is_open;
};
typedef struct fifo Fifo;

struct fifo* fifo_new(size_t elem_size, size_t buf_size);
#define fifo_new_(T_, n_) fifo_new(sizeof(T_), n_)

struct fifo* fifo_construct(struct fifo*, size_t, size_t);
#define fifo_construct_(this_, T_, n_) fifo_construct(this_, sizeof(T_), n_)

void fifo_free(struct fifo*);
void fifo_destroy(struct fifo*);

void fifo_set_open(struct fifo*, int);
void fifo_set_open_ts(struct fifo*, int);
void fifo_resize(struct fifo*, size_t);
// meh
//void fifo_resize_ts(struct fifo*, size_t);
size_t fifo_available(struct fifo*);
size_t fifo_available_ts(struct fifo*);
_Bool fifo_is_empty(struct fifo*);
_Bool fifo_is_empty_ts(struct fifo*);
_Bool fifo_is_over_half(struct fifo*);
_Bool fifo_is_over_half_ts(struct fifo*);
_Bool fifo_is_full(struct fifo*);
_Bool fifo_is_full_ts(struct fifo*);
_Bool fifo_is_open(struct fifo*);
_Bool fifo_is_open_ts(struct fifo*);
_Bool fifo_is_receivable(struct fifo*);
_Bool fifo_is_receivable_ts(struct fifo*);
void fifo_set_full(struct fifo*);
void* fifo_get(struct fifo*);
void* fifo_get_ts(struct fifo*);
void* fifo_peek(struct fifo*);
void* fifo_peek_ts(struct fifo*);
void fifo_consume(struct fifo*);
void fifo_consume_ts(struct fifo*);
int fifo_recycle(struct fifo*, void*);
int fifo_recycle_ts(struct fifo*, void*);
int fifo_add(struct fifo*, void*);
int fifo_add_ts(struct fifo*, void*);
int fifo_advance(struct fifo*);
int fifo_advance_ts(struct fifo*);


/* thread conditions */
void fifo_wait_for_add(struct fifo*);
void fifo_wait_for_get(struct fifo*);
void fifo_wait_for_half(struct fifo*);


#endif  /* CIRCLE_H */
