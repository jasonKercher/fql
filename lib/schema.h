#ifndef SCHEMA_H
#define SCHEMA_H

#include "util/stack.h"
#include "util/queue.h"
#include "util/hmap.h"
#include "fqlimits.h"

#include "expression.h"

struct schema {
        struct stack* columns;
        struct hmap* col_map;
        char name[TABLE_NAME_MAX];
};

struct schema* schema_new();
void schema_free(void*);

void schema_resolve(struct queue* query_list);

void schema_add_column(struct schema* schema,
                      struct expression* expr,
                      const char* table_name);
void schema_apply_column_alias(struct schema* schema, const char* alias);

#endif /* SCHEMA_H */
