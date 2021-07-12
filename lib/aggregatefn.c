#include "group.h"
#include "aggregate.h"
#include "misc.h"
#include "util/util.h"

int fql_count(aggregate* agg, group* group, struct aggresult* result, recgroup* rg)
{
	++result->data.i;
	return FQL_GOOD;
}

int fql_min_i(aggregate* agg, group* group, struct aggresult* result, recgroup* rg)
{
	column* col = *(column**)vec_begin(agg->args);
	long num = 0;
	try_(column_get_int(&num, col, rg));
	if (result->qty == 0 || num < result->data.i) {
		result->data.i = num;
	}
	return FQL_GOOD;
}

int fql_min_f(aggregate* agg, group* group, struct aggresult* result, recgroup* rg)
{
	column* col = *(column**)vec_begin(agg->args);
	double num = 0;
	try_(column_get_float(&num, col, rg));
	if (result->qty == 0 || num < result->data.f) {
		result->data.f = num;
	}
	return FQL_GOOD;
}

int fql_min_s(aggregate* agg, group* group, struct aggresult* result, recgroup* rg)
{
	column* col = *(column**)vec_begin(agg->args);
	stringview sv0 = {0};
	try_(column_get_stringview(&sv0, col, rg));

	stringview sv1 = string_get_stringview(&result->data.s);
	if (result->qty == 0
	    || stringview_compare_nocase_rtrim(&sv0, &sv1) < 0) {
		string_copy_from_stringview(&result->data.s, &sv0);
	}
	return FQL_GOOD;
}

int fql_max_i(aggregate* agg, group* group, struct aggresult* result, recgroup* rg)
{
	column* col = *(column**)vec_begin(agg->args);
	long num = 0;
	try_(column_get_int(&num, col, rg));
	if (result->qty == 0 || num > result->data.i) {
		result->data.i = num;
	}
	return FQL_GOOD;
}

int fql_max_f(aggregate* agg, group* group, struct aggresult* result, recgroup* rg)
{
	column* col = *(column**)vec_begin(agg->args);
	double num = 0;
	try_(column_get_float(&num, col, rg));
	if (result->qty == 0 || num > result->data.f) {
		result->data.f = num;
	}
	return FQL_GOOD;
}

int fql_max_s(aggregate* agg, group* group, struct aggresult* result, recgroup* rg)
{
	column* col = *(column**)vec_begin(agg->args);
	stringview sv0 = {0};
	try_(column_get_stringview(&sv0, col, rg));

	stringview sv1 = string_get_stringview(&result->data.s);
	if (result->qty == 0
	    || stringview_compare_nocase_rtrim(&sv0, &sv1) > 0) {
		string_copy_from_stringview(&result->data.s, &sv0);
	}
	return FQL_GOOD;
}

int fql_sum_i(aggregate* agg, group* group, struct aggresult* result, recgroup* rg)
{
	column* col = *(column**)vec_begin(agg->args);
	long num = 0;
	try_(column_get_int(&num, col, rg));

	result->data.i = overflow_safe_add_i(result->data.i, num);
	return FQL_GOOD;
}

int fql_sum_f(aggregate* agg, group* group, struct aggresult* result, recgroup* rg)
{
	column* col = *(column**)vec_begin(agg->args);
	double num = 0;
	try_(column_get_float(&num, col, rg));

	result->data.f += num;
	return FQL_GOOD;
}
