#ifndef SCHEMA_H
#define SCHEMA_H

#include "fqlhandle.h"
#include "fqlimits.h"
#include "util/queue.h"
#include "util/hmap.h"
#include "util/vec.h"

struct schema {
        struct vec* columns;
        struct hmap* col_map;
        char name[TABLE_NAME_MAX];
        char delimiter[DELIM_LEN_MAX];
};
typedef struct schema Schema;

enum join_side {
        SIDE_UNDEF,
        SIDE_LEFT,
        SIDE_RIGHT,
        SIDE_MIXED,
};

struct column;
struct query;

struct schema* schema_new();
struct schema* schema_construct(struct schema*);
void schema_free(void*);

int schema_assign_columns_limited(Vec* columns, Vec* sources, int);
int schema_assign_columns(Vec* columns, Vec* sources);
int schema_resolve_query(struct query*);
int schema_resolve(struct fql_handle*);

void schema_add_column(struct schema*, struct column*);
void schema_apply_column_alias(struct schema* schema, const char* alias);

#endif /* SCHEMA_H */
