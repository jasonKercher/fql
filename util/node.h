#ifndef NODE_H
#define NODE_H

#ifdef __cplusplus
extern "C" {
#define restrict
#endif

struct node {
	void* data;
	struct node* prev;
	struct node* next;
};
typedef struct node node;

/** NOTE: mutators need reference to node* **/

/* Treat nodes as a queue */
struct node* node_front(struct node* restrict node);
struct node* node_back(struct node* restrict node);
struct node* node_enqueue(struct node** head, void* retrict);
struct node* node_enqueue_import(struct node** head, struct node* restrict);
#define node_dequeue(head_)        node_pop(head_)
#define node_dequeue_export(head_) node_pop_export(head_)

/* Treat nodes as a stack */
struct node* node_top(struct node* restrict node);
struct node* node_bottom(struct node* restrict node);
struct node* node_push(struct node** head, void* restrict);
struct node* node_push_import(struct node** head, struct node* restrict);
void* node_pop(struct node** head);
struct node* node_pop_export(struct node** head);

void* node_data_at(struct node* restrict, unsigned);
struct node* node_at(struct node* restrict, unsigned);
int node_count(struct node* restrict head);

struct node* node_export(struct node* restrict);
void* node_remove(struct node** head, struct node* restrict node);
void node_delete(struct node** head, struct node* restrict node);
void node_free_func(struct node**, void (*)(void*));
void node_free_data(struct node** head);
void node_free(struct node** head);

#ifdef __cplusplus
}
#endif

#endif  /* NODE_H */
