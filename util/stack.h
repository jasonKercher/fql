#ifndef STACK_H
#define STACK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

/**
 * doubly-linked list treated as a stack.
 */
struct stack {
	void* data;
	struct stack* prev;
	struct stack* next;
};
typedef struct stack stack;

struct stack* stack_push(struct stack** head, void*);
void* stack_remove(struct stack** head, struct stack*);
void stack_delete(struct stack** head, struct stack*);
void* stack_pop(struct stack** head);
struct stack* stack_top(struct stack* node);
struct stack* stack_bottom(struct stack* node);
int stack_count(struct stack* head);
void stack_free_func(struct stack**, void(*)(void*));
void stack_free_data(struct stack** head);
void stack_free(struct stack** head);

#ifdef __cplusplus
}
#endif

#endif /* STACK_H */
