#ifndef CHARNODE_H
#define CHARNODE_H
/**
 * Doubly-linked list that keeps track of char*
 */
struct charnode {
        const char* data;
        struct charnode* prev;
        struct charnode* next;
};

struct charnode* cn_push(struct charnode** head, const char*);
void cn_remove(struct charnode** head, struct charnode*);
const char* cn_pop(struct charnode** head);

#endif /* CHARNODE_H */
