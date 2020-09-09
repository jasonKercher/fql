#ifndef STACK_H
#define STACK_H

#include <stdlib.h>

/**
 * Doubly-linked list that can be treated as a
 * stack (pop) or a stack (destack)
 */
struct stack {
        void* data;
        struct stack* prev;
        struct stack* next;
};

typedef struct stack stack_t;

struct stack* stack_push(struct stack** head, void*);
void* stack_remove(struct stack** head, struct stack*);
void stack_delete(struct stack** head, struct stack*);
void* stack_pop(struct stack** head);
struct stack* stack_top(struct stack* node);
struct stack* stack_bottom(struct stack* node);
int stack_count(struct stack* head);
void stack_free_data(struct stack* head);
void stack_free(struct stack* head);

#endif /* STACK_H */
