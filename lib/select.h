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

struct select {
        enum op oper_type;
        struct schema* schema;
        struct writer* writer;
};
typedef struct select Select;

struct select* select_new();
struct select* select_init(struct select*);
void select_free(struct select*);

void select_add_column(struct select*, struct column*);

void select_apply_process(struct select*, struct plan*);
void select_apply_column_alias(struct select*, const char* alias);

int select_record(struct select*, struct vec* rec);

#ifdef __cplusplus
}
#endif

#endif /* SELECT_H */
