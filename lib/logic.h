#ifndef LOGIC_H
#define LOGIC_H

#include "expression.h"

enum comparison {
         COMP_NOT_SET = -1
        ,COMP_FALSE
        ,COMP_TRUE
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

struct logic {
        struct column* col[2];
        struct logic* out[2];
        int data_type;
        enum comparison comp_type;
};

struct logic* logic_new();
void logic_free(struct logic*);

void logic_get_description(struct logic* logic, char* msg);

void logic_add_column(struct logic*, struct expression*, const char* table_name);
void logic_set_comparison(struct logic* logic, const char* op);

struct logic_tree {
        struct logic* begin;
        struct logic* end_true;
        struct logic* end_false;
};
struct logic_tree* logic_tree_new();
void logic_free_tree(struct logic_tree*);

#endif /* LOGIC_H */
