#ifndef SEARCH_H
#define SEARCH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "expression.h"

enum comparison {
         COMP_NOT_SET = -1
        ,COMP_EQ
        ,COMP_NE
        ,COMP_GT
        ,COMP_GE
        ,COMP_LT
        ,COMP_LE
        ,COMP_LIKE
        ,COMP_NOT_LIKE
        ,COMP_NULL
        ,COMP_NOT_NULL
};

struct search {
        struct column* expr[2];
        struct search* out[2];
        int data_type;
        enum comparison comp_type;
};

struct search* search_new();
void search_free(struct search*);

void search_add_column(struct search*, struct expression*, const char* table_name);
void search_set_comparison(struct search* search, const char* op);

#ifdef __cplusplus
}
#endif

#endif /* SEARCH_H */
