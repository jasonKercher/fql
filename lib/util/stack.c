#include "stack.h"

stack_t* stack_push(stack_t** head, void* data)
{
        stack_t* newnode = NULL;
        malloc_(newnode, sizeof(*newnode));
        *newnode = (stack_t) {
                 data 
                ,NULL
                ,*head
        };

        if (*head)
                (*head)->prev = newnode;
        *head = newnode;

        return newnode;
}

void stack_delete(stack_t** head, stack_t* node)
{
        if (!node)
                return;

        void* data = stack_remove(head, node);
        free_(data);
}

void* stack_remove(stack_t** head, stack_t* node)
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

void* stack_pop(stack_t** head)
{
        return stack_remove(head, *head);
}

stack_t* stack_top(stack_t* node)
{
        if (!node)
                return NULL;

        stack_t* head = node;
        while (head && head->prev)
                head = head->prev;

        return head;
}

stack_t* stack_bottom(stack_t* node)
{
        if (!node)
                return NULL;

        stack_t* last = node;
        while (last && last->next)
                last = last->next;

        return last;
}

int stack_count(stack_t* head)
{
        if (!head)
                return 0;

        int count = 1;
        stack_t* last = head;
        while ((last = last->next))
                ++count;

        return count;
}

void stack_free_func(stack_t** head, generic_data_func free_func)
{
        for (; *head; stack_pop(head))
                free_func((*head)->data);
}

void stack_free_data(stack_t** head)
{
        for (; *head; stack_delete(head, *head));
}

void stack_free(stack_t** head)
{
        for (; *head; stack_pop(head));
}
