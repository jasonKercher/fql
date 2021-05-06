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

	return aggregate_construct(new_agg, agg_type);
}

Aggregate* aggregate_construct(Aggregate* agg, enum aggregate_function agg_type)
{
	*agg = (Aggregate) {
		 NULL
		,vec_new_(Column*)
		,{ 0 }
		,agg_type
		,FIELD_UNDEFINED
	};

	vec_construct_(&agg->results, struct aggresult);

	return agg;
}

void aggregate_free(Aggregate* agg)
{
	aggregate_destroy(agg);
	free_(agg);
}

void aggregate_destroy(Aggregate* agg) 
{ 
	vec_free(agg->args);
	if (agg->data_type == FIELD_STRING) {
		struct aggresult* it = vec_begin(&agg->results);
		for(; it != vec_end(&agg->results); ++it) {
			string_destroy(&it->data.s);
		}
	}
	vec_destroy(&agg->results);
}

const char* aggregate_get_name(Aggregate* agg)
{
	return agg_str[agg->agg_type];
}

void aggregate_add_column(Aggregate* agg, Column* col)
{
	vec_push_back(agg->args, &col);
}

int aggregate_resolve(Aggregate* agg)
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

