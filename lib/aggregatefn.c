#include "group.h"
#include "aggregate.h"

int fql_count(aggregate* agg, group* group, struct aggresult* result, vec* recs)
{
	++result->qty;
	++result->data.i;
	return FQL_GOOD;
}
