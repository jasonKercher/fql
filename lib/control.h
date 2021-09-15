#ifndef CONTROL_H
#define CONTROL_H

#include "util/hashmap.h"

struct control {
	struct hashmap variable_map;
	unsigned idx;
};
typedef struct control control;

struct control* control_construct(struct control*);
void control_destroy(struct control*);

#endif /* CONTROL_H */
