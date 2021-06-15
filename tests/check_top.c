#include "check_common.h"

/* t1.tsv
foo	bar	baz
44b72d44	70	30279
b8e63822	cb	38922
46a1bad3	5f	30119
8ab25d8f	f8	75882
282a4957	6c	121263
3138b3f8	b0	154715
18e25713	1e	100092
c1519b0d	c8	87082
6ed156a7	ae	229701
*/

START_TEST(test_top_const)
{
	struct fql_field* fields = NULL;
	int plan_count = 0;
	int field_count = 0;
	int rows = 0;

	plan_count = fql_make_plans(fql, "select top 0 1");
	ck_assert_int_eq(plan_count, 1);

	field_count = fql_field_count(fql);
	ck_assert_int_eq(field_count, 1);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 0);
	ck_assert_int_eq(fql_field_count(fql), 0);

	plan_count = fql_make_plans(fql, "select top 2 5");
	ck_assert_int_eq(plan_count, 1);

	field_count = fql_field_count(fql);
	ck_assert_int_eq(field_count, 1);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(fields[0].type, FQL_INT);
	ck_assert_int_eq(fields[0].data.i, 5);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 0);
	ck_assert_int_eq(fql_field_count(fql), 0);

	plan_count = fql_make_plans(fql, "select top 4 foo from t1");
	ck_assert_int_eq(plan_count, 1);

	field_count = fql_field_count(fql);
	ck_assert_int_eq(field_count, 1);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(fields[0].type, FQL_STRING);
	ck_assert_str_eq(fields[0].data.s, "44b72d44");

	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "b8e63822");
	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "46a1bad3");
	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "8ab25d8f");

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 0);
	ck_assert_int_eq(fql_field_count(fql), 0);
}
END_TEST

START_TEST(test_top_order)
{
	struct fql_field* fields = NULL;
	int plan_count = 0;
	int field_count = 0;
	int rows = 0;

	plan_count =
	        fql_make_plans(fql,
	                       "select top 2 foo from t1 order by bar desc");
	ck_assert_int_eq(plan_count, 1);

	field_count = fql_field_count(fql);
	ck_assert_int_eq(field_count, 1);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(fields[0].type, FQL_STRING);
	ck_assert_str_eq(fields[0].data.s, "8ab25d8f");

	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "b8e63822");

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 0);
	ck_assert_int_eq(fql_field_count(fql), 0);
}
END_TEST

START_TEST(test_top1_subquery)
{
	struct fql_field* fields = NULL;
	int plan_count = 0;
	int field_count = 0;
	int rows = 0;

	plan_count = fql_make_plans(fql, "select (select top 1 foo from t1)");
	ck_assert_int_eq(plan_count, 1);

	field_count = fql_field_count(fql);
	ck_assert_int_eq(field_count, 1);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(fields[0].type, FQL_STRING);
	ck_assert_str_eq(fields[0].data.s, "44b72d44");

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 0);
	ck_assert_int_eq(fql_field_count(fql), 0);
}
END_TEST

START_TEST(test_top_expression)
{
	struct fql_field* fields = NULL;
	int plan_count = 0;
	int field_count = 0;
	int rows = 0;

	plan_count =
	        fql_make_plans(fql, "select top ((5 * 2) / 4) baz from t1");
	ck_assert_int_eq(plan_count, 1);

	field_count = fql_field_count(fql);
	ck_assert_int_eq(field_count, 1);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 1);
	ck_assert_int_eq(fields[0].type, FQL_STRING);
	ck_assert_str_eq(fields[0].data.s, "30279");
	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "38922");

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 0);
	ck_assert_int_eq(fql_field_count(fql), 0);
}
END_TEST

Suite* fql_top_suite(void)
{
	Suite* s;
	s = suite_create("top");

	TCase* tc_top = tcase_create("group");
	tcase_add_checked_fixture(tc_top, fql_setup, fql_teardown);

	tcase_add_test(tc_top, test_top_const);
	tcase_add_test(tc_top, test_top_order);
	tcase_add_test(tc_top, test_top_expression);
	tcase_add_test(tc_top, test_top1_subquery);

	suite_add_tcase(s, tc_top);

	return s;
}

int main(void)
{
	int number_failed;
	Suite* top_suite = fql_top_suite();
	SRunner* top_runner = srunner_create(top_suite);
	srunner_set_fork_status(top_runner, CK_NOFORK);

	srunner_run_all(top_runner, CK_VERBOSE);
	number_failed = srunner_ntests_failed(top_runner);
	srunner_free(top_runner);

	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}


