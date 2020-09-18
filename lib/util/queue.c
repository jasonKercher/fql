#include "queue.h"
#include "util.h"

struct queue* queue_enqueue(struct queue** head, void* data)
{
        struct queue* newnode = NULL;
        malloc_(newnode, sizeof(*newnode));
        *newnode = (struct queue) {
                 data
                ,NULL
                ,NULL
        };

        struct queue* back = queue_back(*head);

        if (back) {
                newnode->prev = back;
                back->next = newnode;
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

struct queue* queue_back(struct queue* node)
{
        if (!node)
                return NULL;

        struct queue* back = node;
        while (back && back->next)
                back = back->next;

        return back;
}

int queue_count(struct queue* head)
{
        if (!head)
                return 0;

        int count = 1;
        struct queue* back = head;
        while ((back = back->next))
                ++count;

        return count;
}

void queue_free_func(struct queue** head, generic_data_func free_func)
{
        for (; *head; queue_dequeue(head))
                free_func((*head)->data);
}

void queue_free_data(struct queue** head)
{
        for (; *head; queue_delete(head, *head));
}

void queue_free(struct queue** head)
{
        for (; *head; queue_dequeue(head));
}
