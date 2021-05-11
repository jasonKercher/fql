#include "order.h"
#include "column.h"

order* order_construct(order* self)
{
	vec_construct_(&self->columns, column*);
	return self;
}

void order_destroy(order* self)
{
	vec_destroy(&self->columns);
}
