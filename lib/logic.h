#ifndef LOGIC_H
#define LOGIC_H

#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>

#include "process.h"
#include "field.h"
#include "util/dgraph.h"
#include "util/hashmap.h"

/* COMP_FALSE and COMP_TRUE are passive processes
 * that are removed. they do not count.
 */
#define COMP_COUNT 10
enum comparison {
	COMP_NOT_SET = -3,
	COMP_FALSE,
	COMP_TRUE,
	COMP_EQ, /* 0 */
	COMP_NE,
	COMP_GT,
	COMP_GE,
	COMP_LT,
	COMP_LE,
	COMP_IN,
	COMP_SUBIN,
	COMP_LIKE,
	COMP_NULL,
};

struct logic;
struct like;
struct inlist;
struct query;
typedef int (*logic_fn)(struct logic*, struct vec*);

struct logic {
	struct column* col[2];
	struct like* like_data;
	struct inlist* in_data;
	logic_fn logic__;
	enum field_type data_type;
	enum comparison comp_type;
};
typedef struct logic logic;

struct logic* logic_construct(struct logic*);
void logic_destroy(struct logic*);

int logic_assign_process(struct logic*, struct process*);
void logic_add_column(struct logic*, struct column*);
void logic_set_comparison(struct logic* logic, const char* op);

struct inlist {
	struct vec* columns;
	struct query* subquery;
	set* list_data;
};
typedef struct inlist inlist;

struct inlist* inlist_construct(struct inlist*);
void inlist_destroy(struct inlist*);
void inlist_add_column(struct inlist*, struct column*);
enum field_type inlist_determine_type(struct inlist*, struct column*);
void inlist_cat_description(struct inlist*, string* msg);

struct like {
	string like_buffer;
	string regex_buffer;
	struct vec ranges;
	pcre2_code* regex;
	pcre2_match_data* match;
};
typedef struct like like;

struct like* like_construct(struct like*);
void like_destroy(struct like*);
int like_to_regex(struct like*, const struct stringview);

enum logicgroup_type {
	LG_UNDEFINED = -1,
	LG_ROOT,
	LG_AND,
	LG_NOT,
};

struct logicgroup {
	enum logicgroup_type type;
	struct vec items;    /* logicgroup* */
	struct vec* columns; /* All columns in group */
	struct vec* joinable;
	struct logic* join_logic;
	struct logic* condition;
	_Bool negation;
};
typedef struct logicgroup logicgroup;

struct logicgroup* logicgroup_construct(struct logicgroup*,
                                        enum logicgroup_type);
void logicgroup_destroy(struct logicgroup*);

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
int fql_logic_in_i(struct logic*, struct vec*);
int fql_logic_in_f(struct logic*, struct vec*);
int fql_logic_in_s(struct logic*, struct vec*);
int fql_logic_subin_i(struct logic*, struct vec*);
int fql_logic_subin_f(struct logic*, struct vec*);
int fql_logic_subin_s(struct logic*, struct vec*);
int fql_logic_like(struct logic*, struct vec*);
int fql_logic_is_null(struct logic*, struct vec*);

#endif /* LOGIC_H */
