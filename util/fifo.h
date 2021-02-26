#ifndef FIFO_H
#define FIFO_H

#include <stdbool.h>
#include <pthread.h>
#include "vec.h"

/**
 * Circular Buffer
 *
 */

struct fifo {
        Vec* buf;
        pthread_mutex_t head_mutex;
        pthread_mutex_t tail_mutex;
        pthread_mutex_t open_mutex;
        pthread_cond_t cond_add;
        pthread_cond_t cond_get;
        size_t head;
        size_t tail;
        unsigned input_count;
        _Bool is_open;
};
typedef struct fifo Fifo;

struct fifo* fifo_new(size_t, size_t);
#define fifo_new_(T_, n_) fifo_new(sizeof(T_), n_)
struct fifo* fifo_construct(struct fifo*, size_t, size_t);
void fifo_free(struct fifo*);

void fifo_close(struct fifo*);

void fifo_resize(struct fifo*, size_t);
size_t fifo_available(struct fifo*);
_Bool fifo_is_empty(struct fifo*);
_Bool fifo_is_full(struct fifo*);
_Bool fifo_is_receivable(struct fifo*);
//_Bool fifo_has_data(struct fifo*);
void* fifo_get(struct fifo*);
void* fifo_peek(struct fifo*);
void fifo_consume(struct fifo*);
int fifo_add(struct fifo*, void*);
int fifo_advance(struct fifo*);

void fifo_wait_for_add(struct fifo*);
void fifo_wait_for_get(struct fifo*);


#endif  /* CIRCLE_H */
