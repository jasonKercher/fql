#ifndef SELECT_H
#define SELECT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "operation.h"
#include "schema.h"

struct select {
        enum oper oper_type;
        struct schema* schema;
};

struct select* select_new();
void select_free(struct select*);

void select_add_column(struct select*
                      ,struct expression* expr
                      ,const char* table_name);

void select_apply_column_alias(struct select*, const char* alias);

#ifdef __cplusplus
}
#endif

#endif /* SELECT_H */
