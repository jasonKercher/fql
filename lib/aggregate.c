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

aggregate* aggregate_construct(aggregate* agg, enum aggregate_function agg_type)
{
	*agg = (aggregate) {NULL,
	                    new_t_(vec, column*),
	                    {0},
	                    agg_type,
	                    FIELD_UNDEFINED};

	vec_construct(&agg->results, sizeof(struct aggresult));

	return agg;
}

void aggregate_destroy(aggregate* agg)
{
	delete_(vec, agg->args);
	if (agg->data_type == FIELD_STRING) {
		struct aggresult* it = vec_begin(&agg->results);
		for (; it != vec_end(&agg->results); ++it) {
			string_destroy(&it->data.s);
		}
	}
	vec_destroy(&agg->results);
}

const char* aggregate_get_name(aggregate* agg)
{
	return agg_str[agg->agg_type];
}

void aggregate_add_column(aggregate* agg, column* col)
{
	vec_push_back(agg->args, &col);
}

int aggregate_resolve(aggregate* agg)
{
	switch (agg->agg_type) {
	case AGG_COUNT:
		agg->call__ = &fql_count;
		agg->data_type = FIELD_INT;
		return FQL_GOOD;
	default:
		fprintf(stderr,
		        "Aggregate function `%s' not yet implemented\n",
		        agg_str[agg->agg_type]);
		return FQL_FAIL;
	}
}
