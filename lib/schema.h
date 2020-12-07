#ifndef SCHEMA_H
#define SCHEMA_H

#include "expression.h"
#include "fqlimits.h"
#include "util/queue.h"
#include "util/hmap.h"
#include "util/vec.h"

struct schema {
        struct vec* columns;
        struct hmap* col_map;
        char name[TABLE_NAME_MAX];
};
typedef struct schema Schema;

struct column;

struct schema* schema_new();
struct schema* schema_init(struct schema*);
void schema_free(void*);

int schema_assign_columns_limited(Vec* columns, Vec* sources, int);
int schema_assign_columns(Vec* columns, Vec* sources);
int schema_resolve(struct queue* query_list);

void schema_add_column(struct schema*, struct column*);
void schema_apply_column_alias(struct schema* schema, const char* alias);

#endif /* SCHEMA_H */
