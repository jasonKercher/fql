#include "check_common.h"

#include <stdlib.h>

struct fql_handle* fql = NULL;

void fql_setup()
{
	fql = fql_new();
}

void fql_teardown()
{
	fql_free(fql);
}

void assert_select_string(const char* query_string, const char* EXPECTED_VALUE)
{
	int plan_count = fql_make_plans(fql, query_string);

	struct fql_field* fields = NULL;
	int field_count = 0;
	int rows = 0;

	ck_assert_int_eq(plan_count, 1);

	field_count = fql_field_count(fql);
	ck_assert_int_eq(field_count, 1);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 1);
	ck_assert_int_eq(fields[0].type, FQL_STRING);

	ck_assert_str_eq(fields[0].data.s, EXPECTED_VALUE);
	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 0);
	ck_assert_int_eq(fql_field_count(fql), 0);
}

void assert_select_int(const char* query_string, long EXPECTED_VALUE)
{
	int plan_count = fql_make_plans(fql, query_string);

	struct fql_field* fields = NULL;
	int field_count = 0;
	int rows = 0;

	ck_assert_int_eq(plan_count, 1);

	field_count = fql_field_count(fql);
	ck_assert_int_eq(field_count, 1);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 1);
	ck_assert_int_eq(fields[0].type, FQL_INT);

	ck_assert_int_eq(fields[0].data.i, EXPECTED_VALUE);
	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 0);
	ck_assert_int_eq(fql_field_count(fql), 0);
}

void assert_nothing(const char* query_string)
{
	int plan_count = fql_make_plans(fql, query_string);

	struct fql_field* fields = NULL;
	int field_count = 0;
	int rows = 0;

	ck_assert_int_eq(plan_count, 1);

	field_count = fql_field_count(fql);
	ck_assert_int_eq(field_count, 1);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 0);
	ck_assert_int_eq(fql_field_count(fql), 0);
}
