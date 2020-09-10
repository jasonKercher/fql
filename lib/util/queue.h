#ifndef QUEUE_H
#define QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

/**
 * Doubly-linked list that can be treated as a
 * stack (pop) or a queue (dequeue)
 */
struct queue_t {
        void* data;
        struct queue_t* prev;
        struct queue_t* next;
};

typedef struct queue_t queue_t;

queue_t* queue_enqueue(queue_t** head, void*);
void* queue_remove(queue_t** head, queue_t* node);
void queue_delete(queue_t** head, queue_t* node);
void* queue_dequeue(queue_t** head);
queue_t* queue_front(queue_t* node);
queue_t* queue_tail(queue_t* node);
int queue_count(queue_t* head);
void queue_free_data(queue_t* head);
void queue_free(queue_t* head);

#ifdef __cplusplus
}
#endif

#endif /* QUEUE_H */
