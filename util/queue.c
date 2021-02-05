#include "queue.h"
#include "util.h"

Queue* queue_enqueue(Queue** head, void* data)
{
        Queue* newnode = NULL;
        malloc_(newnode, sizeof(*newnode));
        *newnode = (Queue) {
                 data
                ,NULL
                ,NULL
        };

        Queue* back = queue_back(*head);

        if (back) {
                newnode->prev = back;
                back->next = newnode;
        }

        if (!*head)
                *head = newnode;

        return newnode;
}

void queue_delete(Queue** head, Queue* node)
{
        if (!node)
                return;

        void* data = queue_remove(head, node);
        free_(data);
}

void* queue_remove(Queue** head, Queue* node)
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

void* queue_dequeue(Queue** head)
{
        return queue_remove(head, *head);
}

Queue* queue_front(Queue* node)
{
        if (!node)
                return NULL;

        Queue* head = node;
        while (head && head->prev)
                head = head->prev;

        return head;
}

Queue* queue_back(Queue* node)
{
        if (!node)
                return NULL;

        Queue* back = node;
        while (back && back->next)
                back = back->next;

        return back;
}

int queue_count(Queue* head)
{
        if (!head)
                return 0;

        int count = 1;
        Queue* back = head;
        while ((back = back->next))
                ++count;

        return count;
}

void queue_free_func(Queue** head, generic_data_fn free_func)
{
        for (; *head; queue_dequeue(head))
                free_func((*head)->data);
}

void queue_free_data(Queue** head)
{
        for (; *head; queue_delete(head, *head));
}

void queue_free(Queue** head)
{
        for (; *head; queue_dequeue(head));
}
