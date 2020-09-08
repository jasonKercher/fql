#include "queue.h"
#include "util.h"

struct queue* queue_push(struct queue** head, void* data)
{
        struct queue* newnode = NULL;
        malloc_(newnode, sizeof(*newnode));
        *newnode = (struct queue) {
                 data
                ,NULL
                ,NULL
        };

        struct queue* last = queue_tail(*head);

        if (last) {
                newnode->prev = last;
                last->next = newnode;
        }

        if (!*head)
                *head = newnode;

        return newnode;
}

void queue_delete(struct queue** head, struct queue* node)
{
        if (!node)
                return;

        void* data = queue_remove(head, node);
        free_(data);
}

void* queue_remove(struct queue** head, struct queue* node)
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

void* queue_dequeue(struct queue** head)
{
        return queue_remove(head, *head);
}

struct queue* queue_front(struct queue* node)
{
        if (!node)
                return NULL;

        struct queue* head = node;
        while (head && head->prev)
                head = head->prev;

        return head;
}

struct queue* queue_tail(struct queue* node)
{
        if (!node)
                return NULL;

        struct queue* last = node;
        while (last && last->next)
                last = last->next;

        return last;
}

int queue_count(struct queue* head)
{
        if (!head)
                return 0;

        int count = 1;
        struct queue* last = head;
        while ((last = last->next))
                ++count;

        return count;
}

void queue_free_data(struct queue* head)
{
        for (; head; queue_delete(&head, head));
}

void queue_free(struct queue* head)
{
        for (; head; queue_dequeue(&head));
}
