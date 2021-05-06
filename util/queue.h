#ifndef QUEUE_H
#define QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

/**
 * doubly-linked list treated as a queue
 */
struct queue {
	void* data;
	struct queue* prev;
	struct queue* next;
};
typedef struct queue queue;

struct queue* queue_enqueue(struct queue** head, void*);
void* queue_remove(struct queue** head, struct queue* node);
void queue_delete(struct queue** head, struct queue* node);
void* queue_dequeue(struct queue** head);
struct queue* queue_front(struct queue* node);
struct queue* queue_back(struct queue* node);
int queue_count(struct queue* head);
void queue_free_func(struct queue**, void(*)(void*));
void queue_free_data(struct queue** head);
void queue_free(struct queue** head);

#ifdef __cplusplus
}
#endif

#endif /* QUEUE_H */
