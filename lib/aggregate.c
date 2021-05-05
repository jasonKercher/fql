#include "aggregate.h"
#include <stdio.h>
#include "util/util.h"

static const char* agg_str[14] = {
	"UNDEFINED",
	"AVG",
	"CHECKSUM_AGG",
	"COUNT",
	"COUNT_BIG",
	"GROUPING",
	"GROUPING_ID",
	"MAX",
	"MIN",
	"STDEV",
	"STDEVP",
	"SUM",
	"VAR",
	"VARP",
};

Aggregate* aggregate_new(enum aggregate_function agg_type)
{
	Aggregate* new_agg = NULL;
	malloc_(new_agg, sizeof(*new_agg));

	if (aggregate_construct(new_agg, agg_type) == NULL) {
		free_(new_agg);
		return NULL;
	}
	return new_agg;
}

int _resolve(Aggregate* agg);

Aggregate* aggregate_construct(Aggregate* agg, enum aggregate_function agg_type)
{
	*agg = (Aggregate) {
		 { 0 }
		,NULL
		,agg_type
	};

	if (_resolve(agg)) {
		return NULL;
	}
	return agg;
}

void aggregate_free(Aggregate* agg)
{
	aggregate_destroy(agg);
	free_(agg);
}

void aggregate_destroy(Aggregate* agg)
{
	vec_destroy(&agg->results);
}

const char* aggregate_get_name(Aggregate* agg)
{
	return agg_str[agg->type];
}

int _resolve(Aggregate* agg)
{
	switch (agg->type) {
	default:
		fprintf(stderr, "Aggregate function `%s' not yet implemented\n", agg_str[agg->type]);
		return FQL_FAIL;
	}
}

