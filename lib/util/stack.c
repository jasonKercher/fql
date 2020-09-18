#include "stack.h"
#include "util.h"

struct stack* stack_push(struct stack** head, void* data)
{
        struct stack* newnode = NULL;
        malloc_(newnode, sizeof(*newnode));
        *newnode = (struct stack) {
                 data 
                ,NULL
                ,*head
        };

        if (*head)
                (*head)->prev = newnode;
        *head = newnode;

        return newnode;
}

void stack_delete(struct stack** head, struct stack* node)
{
        if (!node)
                return;

        void* data = stack_remove(head, node);
        free_(data);
}

void* stack_remove(struct stack** head, struct stack* node)
{
        if (!node)
                return NULL;

        if (node->next)
                node->next->prev = node->prev;
        if (node->prev)
                node->prev->next = node->next;
        else
                *head = node->next;

        void* data = node->data;
        free_(node);

        return data;
}

void* stack_pop(struct stack** head)
{
        return stack_remove(head, *head);
}

struct stack* stack_top(struct stack* node)
{
        if (!node)
                return NULL;

        struct stack* head = node;
        while (head && head->prev)
                head = head->prev;

        return head;
}

struct stack* stack_bottom(struct stack* node)
{
        if (!node)
                return NULL;

        struct stack* last = node;
        while (last && last->next)
                last = last->next;

        return last;
}

int stack_count(struct stack* head)
{
        if (!head)
                return 0;

        int count = 1;
        struct stack* last = head;
        while ((last = last->next))
                ++count;

        return count;
}

void stack_free_func(struct stack** head, generic_data_func free_func)
{
        for (; *head; stack_pop(head))
                free_func((*head)->data);
}

void stack_free_data(struct stack** head)
{
        for (; *head; stack_delete(head, *head));
}

void stack_free(struct stack** head)
{
        for (; *head; stack_pop(head));
}
