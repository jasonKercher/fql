#ifndef SELECT_H
#define SELECT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "schema.h"

struct select {
        struct schema* schema;
};

struct select* select_new();
void select_free(struct select*);

void select_add_column(struct select*
                      ,struct expression* expr
                      ,char* table_name);

void select_apply_column_alias(struct select*, const char* alias);

#ifdef __cplusplus
}
#endif

#endif /* SELECT_H */
