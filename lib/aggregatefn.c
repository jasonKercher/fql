#include "group.h"
#include "aggregate.h"

int fql_count(Aggregate* agg, Group* group, struct aggresult* result, Vec* recs)
{
	++result->qty;
	++result->data.i;
	return FQL_GOOD;
}
