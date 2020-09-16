#include "queue.h"

queue_t* queue_enqueue(queue_t** head, void* data)
{
        queue_t* newnode = NULL;
        malloc_(newnode, sizeof(*newnode));
        *newnode = (queue_t) {
                 data
                ,NULL
                ,NULL
        };

        queue_t* back = queue_back(*head);

        if (back) {
                newnode->prev = back;
                back->next = newnode;
        }

        if (!*head)
                *head = newnode;

        return newnode;
}

void queue_delete(queue_t** head, queue_t* node)
{
        if (!node)
                return;

        void* data = queue_remove(head, node);
        free_(data);
}

void* queue_remove(queue_t** head, queue_t* node)
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

void* queue_dequeue(queue_t** head)
{
        return queue_remove(head, *head);
}

queue_t* queue_front(queue_t* node)
{
        if (!node)
                return NULL;

        queue_t* head = node;
        while (head && head->prev)
                head = head->prev;

        return head;
}

queue_t* queue_back(queue_t* node)
{
        if (!node)
                return NULL;

        queue_t* back = node;
        while (back && back->next)
                back = back->next;

        return back;
}

int queue_count(queue_t* head)
{
        if (!head)
                return 0;

        int count = 1;
        queue_t* back = head;
        while ((back = back->next))
                ++count;

        return count;
}

void queue_free_func(queue_t** head, generic_data_func free_func)
{
        void* data = NULL;
        for (; *head; data = queue_dequeue(head))
                free_func(data);
}

void queue_free_data(queue_t** head)
{
        for (; *head; queue_delete(head, *head));
}

void queue_free(queue_t** head)
{
        for (; *head; queue_dequeue(head));
}
