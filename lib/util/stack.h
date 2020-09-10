#ifndef STACK_H
#define STACK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

/**
 * Doubly-linked list that can be treated as a
 * stack (pop) or a stack (destack)
 */
struct stack_t {
        void* data;
        struct stack_t* prev;
        struct stack_t* next;
};

typedef struct stack_t stack_t;

stack_t* stack_push(stack_t** head, void*);
void* stack_remove(stack_t** head, stack_t*);
void stack_delete(stack_t** head, stack_t*);
void* stack_pop(stack_t** head);
stack_t* stack_top(stack_t* node);
stack_t* stack_bottom(stack_t* node);
int stack_count(stack_t* head);
void stack_free_data(stack_t* head);
void stack_free(stack_t* head);

#ifdef __cplusplus
}
#endif

#endif /* STACK_H */
