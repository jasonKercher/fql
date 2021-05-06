#ifndef AGGREGATE_H
#define AGGREGATE_H

#include "query.h"
#include "column.h"
#include "field.h"
#include "util/stringy.h"
#include "util/vec.h"

struct aggresult {
	union {
		long i;
		double f;
		String s;
	} data;
	unsigned qty;
};

struct group;
struct aggregate;
typedef int(*aggregate_fn)(struct aggregate*, struct group*, struct aggresult*, struct vec* rec);

struct aggregate {
	aggregate_fn call__;
	struct vec* args;
	struct vec results;
	enum aggregate_function agg_type;
	enum field_type data_type;
};
typedef struct aggregate Aggregate;

struct aggregate* aggregate_new(enum aggregate_function);
struct aggregate* aggregate_construct(struct aggregate*, enum aggregate_function);
void aggregate_free(struct aggregate*);
void aggregate_destroy(struct aggregate*);

const char* aggregate_get_name(struct aggregate*);
void aggregate_add_column(struct aggregate*, struct column*);
int aggregate_resolve(struct aggregate*);

int fql_count(struct aggregate*, struct group*, struct aggresult*, struct vec*);

int fql_min_i(struct aggregate*, struct group*, struct aggresult*, struct vec*);
int fql_min_f(struct aggregate*, struct group*, struct aggresult*, struct vec*);
int fql_min_s(struct aggregate*, struct group*, struct aggresult*, struct vec*);

int fql_max_i(struct aggregate*, struct group*, struct aggresult*, struct vec*);
int fql_max_f(struct aggregate*, struct group*, struct aggresult*, struct vec*);
int fql_max_s(struct aggregate*, struct group*, struct aggresult*, struct vec*);

int fql_sum_i(struct aggregate*, struct group*, struct aggresult*, struct vec*);
int fql_sum_f(struct aggregate*, struct group*, struct aggresult*, struct vec*);

#endif  /* AGGREGATE_H */
