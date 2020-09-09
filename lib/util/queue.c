#include "queue.h"
#include "util.h"

queue_t* queue_push(queue_t** head, void* data)
{
        queue_t* newnode = NULL;
        malloc_(newnode, sizeof(*newnode));
        *newnode = (queue_t) {
                 data
                ,NULL
                ,NULL
        };

        queue_t* last = queue_tail(*head);

        if (last) {
                newnode->prev = last;
                last->next = newnode;
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

queue_t* queue_tail(queue_t* node)
{
        if (!node)
                return NULL;

        queue_t* last = node;
        while (last && last->next)
                last = last->next;

        return last;
}

int queue_count(queue_t* head)
{
        if (!head)
                return 0;

        int count = 1;
        queue_t* last = head;
        while ((last = last->next))
                ++count;

        return count;
}

void queue_free_data(queue_t* head)
{
        for (; head; queue_delete(&head, head));
}

void queue_free(queue_t* head)
{
        for (; head; queue_dequeue(&head));
}
