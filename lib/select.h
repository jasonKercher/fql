#ifndef SELECT_H
#define SELECT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "query.h"
#include "operation.h"
#include "writer.h"
#include "schema.h"
#include "column.h"

struct select;

typedef int(*select_record_fn)(struct select*, struct vec*);

struct select {
        enum op oper_type;
        struct vec* api;
        struct schema* schema;
        struct writer* writer;
        select_record_fn select_fn;
};
typedef struct select Select;

struct select* select_new();
struct select* select_init(struct select*);
void select_free(struct select*);

void select_add_column(struct select*, struct column*);
void select_connect_api(struct select*, struct vec*);
void select_use_non_api(struct select*);
void select_apply_process(struct select*, struct fql_plan*);
void select_apply_column_alias(struct select*, const char* alias);

int select_record(struct select*, struct vec* rec);
int select_record_api(struct select*, struct vec* rec);

#ifdef __cplusplus
}
#endif

#endif /* SELECT_H */
