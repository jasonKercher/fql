#include "util.h"
#include "node.h"

/* STACK FUNCTIONS */
node* node_top(node* self)
{
	if (!self)
		return NULL;

	node* head = self;
	while (head && head->prev)
		head = head->prev;

	return head;
}

node* node_bottom(node* self)
{
	if (!self)
		return NULL;

	node* last = self;
	while (last && last->next)
		last = last->next;

	return last;
}

node* node_pop_export(node** head)
{
	return node_export(head, *head);
}

void* node_pop(node** head)
{
	return node_remove(head, *head);
}

node* node_push(node** head, void* data)
{
	node* newnode = malloc_(sizeof(*newnode));
	*newnode = (node) {
	        data, /* data */
	        NULL, /* prev */
	        *head /* next */
	};

	node_push_import(head, newnode);
	return newnode;
}

node* node_push_import(node** head, node* import)
{
	import->next = *head;

	if (*head)
		(*head)->prev = import;
	*head = import;

	import->prev = NULL;

	return import;
}

/* QUEUE FUNCTIONS */

node* node_front(node* self)
{
	if (!self)
		return NULL;

	node* head = self;
	while (head && head->prev)
		head = head->prev;

	return head;
}

node* node_back(node* self)
{
	if (!self)
		return NULL;

	node* back = self;
	while (back && back->next)
		back = back->next;

	return back;
}

node* node_enqueue_import(node** head, node* import)
{
	node* back = node_back(*head);

	if (back) {
		import->prev = back;
		back->next = import;
	}

	if (!*head)
		*head = import;

	import->next = NULL;

	return import;
}

node* node_enqueue(node** head, void* data)
{
	node* newnode = malloc_(sizeof(*newnode));
	*newnode = (node) {
	        data, /* data */
	        NULL, /* prev */
	        NULL  /* next */
	};

	node_enqueue_import(head, newnode);
	return newnode;
}

node* node_dequeue_export(node** head)
{
	return node_export(head, *head);
}

void* node_dequeue(node** head)
{
	return node_remove(head, *head);
}


/** generic linked list functions **/
node* node_at(node* head, unsigned idx)
{
	unsigned i = 0;
	for (; head && i < idx; ++i) {
		head = head->next;
	}

	return head;
}

void* node_data_at(node* head, unsigned idx)
{
	node* node = node_at(head, idx);
	if (node == NULL) {
		return NULL;
	}
	return node->data;
}

int node_count(node* head)
{
	if (!head)
		return 0;

	int count = 1;
	node* last = head;
	while ((last = last->next))
		++count;

	return count;
}

void node_delete(node** head, node* node)
{
	if (!node)
		return;

	void* data = node_remove(head, node);
	free_(data);
}

node* node_export(node** head, node* export)
{
	if (!export)
		return NULL;

	if (export->next)
		export->next->prev = export->prev;
	if (export->prev)
		export->prev->next = export->next;
	else
		*head = export->next;

	export->next = NULL;
	export->prev = NULL;

	return export;
}

void* node_remove(node** head, node* node)
{
	if (!node)
		return NULL;

	node_export(head, node);
	void* data = node->data;
	free_(node);
	return data;
}

void node_free_func(node** head, generic_data_fn free_func)
{
	*head = node_top(*head);
	for (; *head; node_pop(head))
		free_func((*head)->data);
}

void node_free_data(node** head)
{
	*head = node_top(*head);
	for (; *head; node_delete(head, *head))
		;
}

void node_free(node** head)
{
	*head = node_top(*head);
	for (; *head; node_pop(head))
		;
}
