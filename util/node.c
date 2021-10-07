#include "util.h"
#include "node.h"


void* _node_remove(node* node);

/* STACK FUNCTIONS */
node* node_top(node* restrict self)
{
	if (!self)
		return NULL;

	while (self && self->prev)
		self = self->prev;

	return self;
}

node* node_bottom(node* restrict self)
{
	if (!self)
		return NULL;

	while (self && self->next)
		self = self->next;

	return self;
}

node* _node_pop(node** head)
{
	if (*head == NULL) {
		return NULL;
	}
	node* oldhead = *head;
	*head = oldhead->next;
	return oldhead;
}

node* node_pop_export(node** head)
{
	node* oldhead = _node_pop(head);
	return node_export(oldhead);
}

void* node_pop(node** head)
{
	node* oldhead = _node_pop(head);
	return _node_remove(oldhead);
}

node* node_push(node** head, void* restrict data)
{
	node* newnode = malloc_(sizeof(*newnode));
	*newnode = (node) {
	        .data = data,
	        .next = *head,
	};

	node_push_import(head, newnode);
	return newnode;
}

node* node_push_import(node** head, node* restrict import)
{
	import->next = *head;

	if (*head)
		(*head)->prev = import;

	import->prev = NULL;

	*head = import;
	return import;
}

/* QUEUE FUNCTIONS */
node* node_front(node* restrict self)
{
	if (!self)
		return NULL;

	while (self && self->prev)
		self = self->prev;

	return self;
}

node* node_back(node* restrict self)
{
	if (!self)
		return NULL;

	while (self && self->next)
		self = self->next;

	return self;
}

node* node_enqueue_import(node** head, node* restrict import)
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

node* node_enqueue(node** head, void* restrict data)
{
	node* newnode = malloc_(sizeof(*newnode));
	*newnode = (node) {
	        .data = data,
	};

	node_enqueue_import(head, newnode);
	return newnode;
}

/** generic linked list functions **/
node* node_at(node* restrict head, unsigned idx)
{
	unsigned i = 0;
	for (; head && i < idx; ++i) {
		head = head->next;
	}

	return head;
}

void* node_data_at(node* restrict head, unsigned idx)
{
	node* node = node_at(head, idx);
	if (node == NULL) {
		return NULL;
	}
	return node->data;
}

int node_count(node* restrict head)
{
	if (!head)
		return 0;

	int count = 1;
	while ((head = head->next))
		++count;

	return count;
}

node* node_export(node* restrict export)
{
	if (!export)
		return NULL;

	if (export->next)
		export->next->prev = export->prev;
	if (export->prev)
		export->prev->next = export->next;

	export->next = NULL;
	export->prev = NULL;

	return export;
}

void* _node_remove(node* node)
{
	if (node == NULL) {
		return NULL;
	}
	void* data = node->data;
	node_export(node);
	free_(node);
	return data;
}

void* node_remove(node** head, node* node)
{
	if (!node)
		return NULL;

	if (*head == node) {
		return node_pop(head);
	}
	return _node_remove(node);
}

void node_delete(node** head, node* node)
{
	void* data = node_remove(head, node);
	free_if_exists_(data);
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
	while (*head) {
		void* data = node_pop(head);
		free_(data);
	}
}

void node_free(node** head)
{
	*head = node_top(*head);
	for (; *head; node_pop(head))
		;
}
