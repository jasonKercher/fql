#ifndef AGGREGATE_H
#define AGGREGATE_H

#include "query.h"
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
typedef int(*aggregate_fn)(struct aggregate*, struct group*, struct vec* rec);

struct aggregate {
	struct vec results;
	aggregate_fn call__;
	enum aggregate_function type;
};
typedef struct aggregate Aggregate;

struct aggregate* aggregate_new(enum aggregate_function);
struct aggregate* aggregate_construct(struct aggregate*, enum aggregate_function);
void aggregate_free(struct aggregate*);
void aggregate_destroy(struct aggregate*);

const char* aggregate_get_name(struct aggregate*);

int fql_count(struct aggregate*, struct group*, struct vec* rec, unsigned idx);

#endif  /* AGGREGATE_H */
