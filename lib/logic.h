#ifndef LOGIC_H
#define LOGIC_H

#include "process.h"
#include "field.h"
#include "util/dgraph.h"

/* COMP_FALSE and COMP_TRUE are passive processes
 * that are removed. They do not count.
 */
#define COMP_COUNT 10
enum comparison {
         COMP_NOT_SET = -3
        ,COMP_FALSE
        ,COMP_TRUE
        ,COMP_EQ        /* 0 */
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
typedef int(*logic_fn)(struct logic*, struct vec*);

struct logic {
        struct column* col[2];
        logic_fn logic__;
        enum field_type data_type;
        enum comparison comp_type;
};
typedef struct logic Logic;

struct logic* logic_new();
struct logic* logic_construct(struct logic*);
void logic_destroy(struct logic*);
void logic_free(struct logic*);

void logic_assign_process(struct logic*, struct process*);
void logic_add_column(struct logic*, struct column*);
void logic_set_comparison(struct logic* logic, const char* op);


enum logicgroup_type {
        LG_UNDEFINED = -1,
        LG_ROOT,
        LG_AND,
        LG_NOT,
};

struct logicgroup {
        enum logicgroup_type type;
        struct vec items;               /* LogicGroup* */
        struct vec* joinable;
        struct logic* join_logic;
        struct logic* condition;
};
typedef struct logicgroup LogicGroup;

struct logicgroup* logicgroup_new(enum logicgroup_type);
struct logicgroup* logicgroup_construct(struct logicgroup*, enum logicgroup_type);
void logicgroup_free(struct logicgroup*);

unsigned logicgroup_get_condition_count(struct logicgroup* lg);
int logic_can_be_false(struct logicgroup*, struct logic*);
int logicgroup_eval(struct logicgroup*, struct vec*, struct logic* skip);


/* logic functions */
int fql_logic_eq_i(struct logic*, struct vec*);
int fql_logic_eq_f(struct logic*, struct vec*);
int fql_logic_eq_s(struct logic*, struct vec*);
int fql_logic_ne_i(struct logic*, struct vec*);
int fql_logic_ne_f(struct logic*, struct vec*);
int fql_logic_ne_s(struct logic*, struct vec*);
int fql_logic_gt_i(struct logic*, struct vec*);
int fql_logic_gt_f(struct logic*, struct vec*);
int fql_logic_gt_s(struct logic*, struct vec*);
int fql_logic_ge_i(struct logic*, struct vec*);
int fql_logic_ge_f(struct logic*, struct vec*);
int fql_logic_ge_s(struct logic*, struct vec*);
int fql_logic_lt_i(struct logic*, struct vec*);
int fql_logic_lt_f(struct logic*, struct vec*);
int fql_logic_lt_s(struct logic*, struct vec*);
int fql_logic_le_i(struct logic*, struct vec*);
int fql_logic_le_f(struct logic*, struct vec*);
int fql_logic_le_s(struct logic*, struct vec*);
int fql_logic_like(struct logic*, struct vec*);
int fql_logic_not_like(struct logic*, struct vec*);
int fql_logic_is_null(struct logic*, struct vec*);
int fql_logic_not_null(struct logic*, struct vec*);


static logic_fn logic_matrix[COMP_COUNT][FIELD_TYPE_COUNT] = {
        {&fql_logic_eq_i    , &fql_logic_eq_f    , &fql_logic_eq_s},
        {&fql_logic_ne_i    , &fql_logic_ne_f    , &fql_logic_ne_s},
        {&fql_logic_gt_i    , &fql_logic_gt_f    , &fql_logic_gt_s},
        {&fql_logic_ge_i    , &fql_logic_ge_f    , &fql_logic_ge_s},
        {&fql_logic_lt_i    , &fql_logic_lt_f    , &fql_logic_lt_s},
        {&fql_logic_le_i    , &fql_logic_le_f    , &fql_logic_le_s},
        {NULL               , NULL               , &fql_logic_like},
        {NULL               , NULL               , &fql_logic_not_like},
        {&fql_logic_is_null , &fql_logic_is_null , &fql_logic_is_null},
        {&fql_logic_not_null, &fql_logic_not_null, &fql_logic_not_null},
};


#endif /* LOGIC_H */
