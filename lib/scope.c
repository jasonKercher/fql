#include "scope.h"

#include "expression.h"
#include "util/util.h"

scope* scope_construct(scope* self)
{
	*self = (scope) {
	        .parent_scope = NULL,
	        .is_in_block = false,
	};
	hashmap_construct_(&self->variable_map, int, 16, HASHMAP_PROP_NOCASE);
	return self;
}

void scope_destroy(scope* self)
{
	hashmap_destroy(&self->variable_map);
}

void scope_reset(scope* self)
{
	hashmap_clear(&self->variable_map);
}

int scope_get_var_index(scope* self, const char* varname)
{
	int* idx = NULL;
	scope* it = self;
	for (; it && idx == NULL; it = it->parent_scope) {
		idx = hashmap_get(&it->variable_map, varname);
	}

	if (idx == NULL) {
		fprintf(stderr, "could not locate variable `%s'\n", varname);
		return FQL_FAIL;
	}
	return *idx;
}
