#include "queue.h"
#include "util.h"

queue* queue_enqueue(queue** head, void* data)
{
	queue* newnode = malloc_(sizeof(*newnode));
	*newnode = (queue) {
		 data
		,NULL
		,NULL
	};

	queue* back = queue_back(*head);

	if (back) {
		newnode->prev = back;
		back->next = newnode;
	}

	if (!*head)
		*head = newnode;

	return newnode;
}

void queue_delete(queue** head, queue* node)
{
	if (!node)
		return;

	void* data = queue_remove(head, node);
	free_(data);
}

void* queue_remove(queue** head, queue* node)
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

void* queue_dequeue(queue** head)
{
	return queue_remove(head, *head);
}

queue* queue_front(queue* node)
{
	if (!node)
		return NULL;

	queue* head = node;
	while (head && head->prev)
		head = head->prev;

	return head;
}

queue* queue_back(queue* node)
{
	if (!node)
		return NULL;

	queue* back = node;
	while (back && back->next)
		back = back->next;

	return back;
}

int queue_count(queue* head)
{
	if (!head)
		return 0;

	int count = 1;
	queue* back = head;
	while ((back = back->next))
		++count;

	return count;
}

void queue_free_func(queue** head, generic_data_fn free_func)
{
	for (; *head; queue_dequeue(head))
		free_func((*head)->data);
}

void queue_free_data(queue** head)
{
	for (; *head; queue_delete(head, *head));
}

void queue_free(queue** head)
{
	for (; *head; queue_dequeue(head));
}
