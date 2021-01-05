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
};
typedef struct fifo Fifo;

struct fifo* fifo_new(size_t, size_t);
#define fifo_new_(T_, n_) fifo_new(sizeof(T_), n_)
struct fifo* fifo_init(struct fifo*, size_t, size_t);
void fifo_free(struct fifo*);

void fifo_resize(struct fifo*, size_t);
size_t fifo_available(struct fifo*);
_Bool fifo_is_empty(struct fifo*);
_Bool fifo_is_full(struct fifo*);
void* fifo_get(struct fifo*);
void* fifo_peek(struct fifo*);
int fifo_add(struct fifo*, void*);
int fifo_advance(struct fifo*);


#endif  /* CIRCLE_H */
