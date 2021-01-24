#ifndef LOGIC_H
#define LOGIC_H

#include "process.h"
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

struct logic;
typedef _Bool(*logic_fn)(struct logic*, struct vec*);

struct logic {
        struct column* col[2];
        struct dnode* proc_node;
        logic_fn logic_fn;
        int data_type;
        enum comparison comp_type;
        
};
typedef struct logic Logic;

struct logic* logic_new();
struct logic* logic_construct(struct logic*);
void logic_free(struct logic*);

void logic_assign_process(struct logic*, struct process*);
void logic_add_column(struct logic*, struct column*);
void logic_set_comparison(struct logic* logic, const char* op);

struct logic_tree {
        struct dgraph* tree;
        struct dnode* end_true;
        struct dnode* end_false;
};
typedef struct logic_tree LogicTree;

struct logic_tree* logic_tree_new();
void logic_tree_free(struct logic_tree*);

#endif /* LOGIC_H */
