#ifndef FIFO_H
#define FIFO_H

#include <stdbool.h>
#include "vec.h"

/**
 *
 */

struct fifo {
        Vec* buf;
        size_t head;
        size_t tail;
        _Bool is_full;
        _Bool is_open;
};
typedef struct fifo Fifo;

struct fifo* fifo_new(size_t, size_t);
#define fifo_new_(T_, n_) fifo_new(sizeof(T_), n_)
struct fifo* fifo_construct(struct fifo*, size_t, size_t);
void fifo_free(struct fifo*);

void fifo_resize(struct fifo*, size_t);
size_t fifo_available(struct fifo*);
_Bool fifo_is_empty(struct fifo*);
_Bool fifo_is_full(struct fifo*);
void* fifo_get(struct fifo*);
void* fifo_peek(struct fifo*);
void fifo_consume(struct fifo*);
int fifo_add(struct fifo*, void*);
int fifo_advance(struct fifo*);


#endif  /* CIRCLE_H */
