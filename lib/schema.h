#ifndef SCHEMA_H
#define SCHEMA_H

#include "util/stack.h"
#include "util/queue.h"
#include "util/hmap.h"
#include "fqlimits.h"

struct schema {
        struct stack* columns;
        struct hmap* col_map;
        char name[TABLE_NAME_MAX];
};

struct schema* schema_new();
void schema_free(void*);
void schema_resolve(struct queue* query_list);

#endif /* SCHEMA_H */
