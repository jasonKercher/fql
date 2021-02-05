#include "charnode.h"
#include "misc.h"
#include "util.h"

struct charnode* cn_push(struct charnode** head, const char* s)
{
        struct charnode* newnode = NULL;
        malloc_(newnode, sizeof(*newnode));
        *newnode = (struct charnode) {
                s
                ,*head
                ,NULL
        };

        if (*head)
                (*head)->next = newnode;
        *head = newnode;

        return newnode;
}

void cn_remove(struct charnode** head, struct charnode* node)
{
        if (!node)
                return;

        if (node->prev)
                node->prev->next = node->next;
        if (node->next)
                node->next->prev = node->prev;
        else
                *head = node->prev;

        free_(node->data);
        free_(node);
}

const char* cn_pop(struct charnode** head)
{
        struct charnode* newhead = (*head)->prev;
        const char* data = (*head)->data;
        free_(*head);
        if (newhead)
                newhead->next = NULL;
        (*head) = newhead;
        return data;
}

