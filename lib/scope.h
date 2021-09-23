#ifndef SCOPE_H
#define SCOPE_H

#include "util/stringy.h"
#include "util/hashmap.h"

struct scope {
	struct scope* parent_scope;
	struct hashmap variable_map;
	bool is_in_block;
};
typedef struct scope scope;

struct scope* scope_construct(struct scope*);
void scope_destroy(struct scope*);

int scope_get_var_index(struct scope*, const char*);

#endif /* SCOPE_H */
