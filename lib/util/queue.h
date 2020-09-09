#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>

/**
 * Doubly-linked list that can be treated as a
 * stack (pop) or a queue (dequeue)
 */
struct queue {
        void* data;
        struct queue* prev;
        struct queue* next;
};

typedef struct queue queue_t;

struct queue* queue_push(struct queue** head, void*);
void* queue_remove(struct queue** head, struct queue*);
void queue_delete(struct queue** head, struct queue*);
void* queue_dequeue(struct queue** head);
struct queue* queue_front(struct queue* node);
struct queue* queue_tail(struct queue* node);
int queue_count(struct queue* head);
void queue_free_data(struct queue* head);
void queue_free(struct queue* head);

#endif /* QUEUE_H */
