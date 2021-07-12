#ifndef AGGREGATE_H
#define AGGREGATE_H

#include "field.h"
#include "query.h"
#include "column.h"
#include "util/vec.h"
#include "util/stringy.h"

struct aggresult {
	union {
		long i;
		double f;
		string s;
	} data;
	unsigned qty;
};

struct group;
struct aggregate;
typedef int (*aggregate_fn)(struct aggregate*,
                            struct group*,
                            struct aggresult*,
                            struct recgroup* rec);

struct aggregate {
	aggregate_fn call__;
	struct vec* args;
	struct vec results;
	struct column* linked_column;
	enum aggregate_function agg_type;
	enum field_type data_type;
};
typedef struct aggregate aggregate;

struct aggregate* aggregate_construct(struct aggregate*,
                                      enum aggregate_function);
void aggregate_destroy(struct aggregate*);

const char* aggregate_get_name(struct aggregate*);
void aggregate_add_column(struct aggregate*, struct column*);
int aggregate_resolve(struct aggregate*, struct column*);

int fql_count(struct aggregate*, struct group*, struct aggresult*, struct recgroup*);

int fql_min_i(struct aggregate*, struct group*, struct aggresult*, struct recgroup*);
int fql_min_f(struct aggregate*, struct group*, struct aggresult*, struct recgroup*);
int fql_min_s(struct aggregate*, struct group*, struct aggresult*, struct recgroup*);

int fql_max_i(struct aggregate*, struct group*, struct aggresult*, struct recgroup*);
int fql_max_f(struct aggregate*, struct group*, struct aggresult*, struct recgroup*);
int fql_max_s(struct aggregate*, struct group*, struct aggresult*, struct recgroup*);

int fql_sum_i(struct aggregate*, struct group*, struct aggresult*, struct recgroup*);
int fql_sum_f(struct aggregate*, struct group*, struct aggresult*, struct recgroup*);

#endif /* AGGREGATE_H */
