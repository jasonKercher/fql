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
		,FIELD_UNDEFINED
	};

	vec_construct_(&agg->results, struct aggresult);

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

void aggregate_destroy(Aggregate* agg) { }

const char* aggregate_get_name(Aggregate* agg)
{
	return agg_str[agg->agg_type];
}

int _resolve(Aggregate* agg)
{
	switch (agg->agg_type) {
	case AGG_COUNT:
		agg->call__ = &fql_count;
		agg->data_type = FIELD_INT;
		return FQL_GOOD;
	default:
		fprintf(stderr, "Aggregate function `%s' not yet implemented\n", agg_str[agg->agg_type]);
		return FQL_FAIL;
	}
}

