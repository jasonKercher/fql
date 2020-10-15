#ifndef SCHEMA_H
#define SCHEMA_H

#include "expression.h"
#include "fqlimits.h"
#include "util/stack.h"
#include "util/queue.h"
#include "util/hmap.h"

struct schema {
        struct stack* columns;
        struct hmap* col_map;
        char name[TABLE_NAME_MAX];
};

struct column;

struct schema* schema_new();
void schema_free(void*);

void schema_resolve(struct queue* query_list);

void schema_add_column(struct schema*, struct column*);
void schema_apply_column_alias(struct schema* schema, const char* alias);

#endif /* SCHEMA_H */
