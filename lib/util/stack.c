#include "stack.h"
#include "util.h"

Stack* stack_push(Stack** head, void* data)
{
        Stack* newnode = NULL;
        malloc_(newnode, sizeof(*newnode));
        *newnode = (Stack) {
                 data
                ,NULL
                ,*head
        };

        if (*head)
                (*head)->prev = newnode;
        *head = newnode;

        return newnode;
}

void stack_delete(Stack** head, Stack* node)
{
        if (!node)
                return;

        void* data = stack_remove(head, node);
        free_(data);
}

void* stack_remove(Stack** head, Stack* node)
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

void* stack_pop(Stack** head)
{
        return stack_remove(head, *head);
}

Stack* stack_top(Stack* node)
{
        if (!node)
                return NULL;

        Stack* head = node;
        while (head && head->prev)
                head = head->prev;

        return head;
}

Stack* stack_bottom(Stack* node)
{
        if (!node)
                return NULL;

        Stack* last = node;
        while (last && last->next)
                last = last->next;

        return last;
}

int stack_count(Stack* head)
{
        if (!head)
                return 0;

        int count = 1;
        Stack* last = head;
        while ((last = last->next))
                ++count;

        return count;
}

void stack_free_func(Stack** head, generic_data_fn free_func)
{
        for (; *head; stack_pop(head))
                free_func((*head)->data);
}

void stack_free_data(Stack** head)
{
        for (; *head; stack_delete(head, *head));
}

void stack_free(Stack** head)
{
        for (; *head; stack_pop(head));
}
