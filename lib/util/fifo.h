#ifndef FIFO_H
#define FIFO_H

#include "vec.h"

/**
 * 
 */

struct fifo {
        Vec* buf;
        unsigned char front; 
        unsigned char back;
};
typedef struct fifo Fifo;

struct fifo* fifo_new(size_t);
#define fifo_new_(T_) fifo_new(sizeof(T_))
struct fifo* fifo_init(struct fifo*, size_t);
void fifo_free(struct fifo*);

unsigned char fifo_available(struct fifo*);
void* fifo_get(struct fifo*);
int fifo_add(struct fifo*, void*);


#endif  /* CIRCLE_H */
