#include "stack.h"
#include "util.h"

stack* stack_push(stack** head, void* data)
{
	stack* newnode = malloc_(sizeof(*newnode));
	*newnode = (stack) {
		 data
		,NULL
		,*head
	};

	if (*head)
		(*head)->prev = newnode;
	*head = newnode;

	return newnode;
}

void stack_delete(stack** head, stack* node)
{
	if (!node)
		return;

	void* data = stack_remove(head, node);
	free_(data);
}

void* stack_remove(stack** head, stack* node)
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

void* stack_pop(stack** head)
{
	return stack_remove(head, *head);
}

stack* stack_top(stack* node)
{
	if (!node)
		return NULL;

	stack* head = node;
	while (head && head->prev)
		head = head->prev;

	return head;
}

stack* stack_bottom(stack* node)
{
	if (!node)
		return NULL;

	stack* last = node;
	while (last && last->next)
		last = last->next;

	return last;
}

int stack_count(stack* head)
{
	if (!head)
		return 0;

	int count = 1;
	stack* last = head;
	while ((last = last->next))
		++count;

	return count;
}

void stack_free_func(stack** head, generic_data_fn free_func)
{
	for (; *head; stack_pop(head))
		free_func((*head)->data);
}

void stack_free_data(stack** head)
{
	for (; *head; stack_delete(head, *head));
}

void stack_free(stack** head)
{
	for (; *head; stack_pop(head));
}
