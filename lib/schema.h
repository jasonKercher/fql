#ifndef SCHEMA_H
#define SCHEMA_H

#include "util/stack.h"
#include "util/queue.h"
#include "fqlimits.h"

struct schema_t {
        stack_t* columns;
        char name[TABLE_NAME_MAX];
};
typedef struct schema_t schema_t;

schema_t* schema_new();
void schema_free(void*);
void schema_resolve(queue_t* query_list);

#endif /* SCHEMA_H */
