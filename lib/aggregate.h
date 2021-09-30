#ifndef AGGREGATE_H
#define AGGREGATE_H

#include "field.h"
#include "query.h"
#include "expression.h"
#include "util/vec.h"
#include "util/stringy.h"

union _aggdata {
	long i;
	double f;
	string s;
};

struct aggresult {
	union _aggdata data;
	union _aggdata auxdata;
	unsigned qty;
};

struct group;
struct aggregate;
typedef int (*aggregate_fn)(struct aggregate*,
                            struct group*,
                            struct aggresult*,
                            struct node* rec);

struct aggregate {
	aggregate_fn call__;
	struct vec* args;
	struct vec results;
	struct expression* linked_expression;
	enum aggregate_function agg_type;
	enum field_type data_type;
	enum mode return_mode;
};
typedef struct aggregate aggregate;

struct aggregate* aggregate_construct(struct aggregate*,
                                      enum aggregate_function);
void aggregate_destroy(struct aggregate*);

const char* aggregate_get_name(struct aggregate*);
void aggregate_add_expression(struct aggregate*, struct expression*);
int aggregate_resolve(struct aggregate*, struct expression*);

int fql_count(struct aggregate*, struct group*, struct aggresult*, struct node*);

int fql_min_i(struct aggregate*, struct group*, struct aggresult*, struct node*);
int fql_min_f(struct aggregate*, struct group*, struct aggresult*, struct node*);
int fql_min_s(struct aggregate*, struct group*, struct aggresult*, struct node*);

int fql_max_i(struct aggregate*, struct group*, struct aggresult*, struct node*);
int fql_max_f(struct aggregate*, struct group*, struct aggresult*, struct node*);
int fql_max_s(struct aggregate*, struct group*, struct aggresult*, struct node*);

int fql_sum_i(struct aggregate*, struct group*, struct aggresult*, struct node*);
int fql_sum_f(struct aggregate*, struct group*, struct aggresult*, struct node*);

int fql_avg_i(struct aggregate*, struct group*, struct aggresult*, struct node*);
int fql_avg_f(struct aggregate*, struct group*, struct aggresult*, struct node*);

#endif /* AGGREGATE_H */
