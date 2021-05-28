#ifndef FIELD_H
#define FIELD_H

#include "fql.h"
#include "util/stringy.h"
#include "util/stringview.h"

/** data type **/
#define FIELD_TYPE_COUNT 3
enum field_type {
	FIELD_UNDEFINED = -1,
	FIELD_INT,
	FIELD_FLOAT,
	FIELD_STRING,
};

struct function;
struct aggregate;

union field {
	struct function* fn;
	struct aggregate* agg;
	string* s;
	double f;
	long i;
};

enum field_type field_determine_type(enum field_type, enum field_type);
int field_to_int(long*, union field*, enum field_type*);
int field_to_float(double*, union field*, enum field_type*);
int field_to_stringview(struct stringview*, union field*, enum field_type*);

#endif /* FIELD_H */
