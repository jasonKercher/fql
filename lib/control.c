#include "control.h"

#include "expression.h"
#include "util/util.h"

control* control_construct(control* self)
{
	hashmap_construct_(&self->variable_map, expression*, 16, HASHMAP_PROP_NOCASE);
	return self;
}

void control_destroy(control* self)
{
	hashmap_destroy(&self->variable_map);
}
