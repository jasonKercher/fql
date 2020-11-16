#ifndef LOGIC_H
#define LOGIC_H

#include "expression.h"
#include "util/dgraph.h"

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
        int data_type;
        enum comparison comp_type;
        struct dnode* proc_node;
};

struct logic* logic_new();
void logic_free(struct logic*);

void logic_get_description(struct logic* logic, char* msg);

void logic_add_column(struct logic*, struct column*);
void logic_set_comparison(struct logic* logic, const char* op);

struct logic_tree {
        struct dgraph* tree;
        struct dnode* end_true;
        struct dnode* end_false;
};
struct logic_tree* logic_tree_new();
void logic_tree_free(struct logic_tree*);

#endif /* LOGIC_H */
