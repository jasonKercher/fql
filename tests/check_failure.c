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

/* t2.csv
foo,bar,baz
f4a2dd5d,12,30279
686ff044,a6,38922
c2f3d728,00,30119
6Ed156A7,e8,75882
0cda1b43,8c,121263
e370c956,52,154715
851d0237,ef,100092
9936b3c8,0a,87082
f95b84ce,18,229701
*/

START_TEST(test_failure_syntax)
{

	struct fql_field* fields = NULL;
	int plan_count = 0;
	int field_count = 0;
	int rows = 0;

	/* [column] [alias] [??] */
	plan_count = fql_make_plans(fql, "select shnt mlrv fgkc");
	ck_assert_int_eq(plan_count, FQL_FAIL);

	/* from keyword */
	plan_count = fql_make_plans(fql, "select 1 from");
	ck_assert_int_eq(plan_count, FQL_FAIL);

	/* ON? */
	plan_count = fql_make_plans(fql, "select foo from t1 join t2");
	ck_assert_int_eq(plan_count, FQL_FAIL);

	/* missing ')' */
	plan_count = fql_make_plans(fql, "select left('mlrv', 1");
	ck_assert_int_eq(plan_count, FQL_FAIL);
}
END_TEST

START_TEST(test_failure_columns)
{
	struct fql_field* fields = NULL;
	int plan_count = 0;
	int field_count = 0;
	int rows = 0;

	/* column doesn't exist */
	plan_count = fql_make_plans(fql, "select nope from t1");
	ck_assert_int_eq(plan_count, FQL_FAIL);

	/* ambiguous column */
	plan_count = fql_make_plans(fql, "select foo from t1 join t2 on t1.bar = t2.bar");
	ck_assert_int_eq(plan_count, FQL_FAIL);

	/* ambiguous in logic */
	plan_count = fql_make_plans(fql, "select t1.foo from t1 join t2 on t1.bar = bar");
	ck_assert_int_eq(plan_count, FQL_FAIL);

	/* overwrite table name by alias */
	plan_count = fql_make_plans(fql, "select t1.foo from t1 x");
	ck_assert_int_eq(plan_count, FQL_FAIL);
}
END_TEST

START_TEST(test_failure_const_runtime)
{
	struct fql_field* fields = NULL;
	int plan_count = 0;
	int field_count = 0;
	int rows = 0;

	/* non-numeric data where not expected */
	plan_count = fql_make_plans(fql, "select 's' + 1");
	ck_assert_int_eq(plan_count, FQL_FAIL);

	/* addition overflow */
	plan_count = fql_make_plans(fql, "select -9223372036854775806 + -124");
	ck_assert_int_eq(plan_count, FQL_FAIL);

	/* multiplictaion overflow */
	plan_count = fql_make_plans(fql, "select -922337203685477580 * 124");
	ck_assert_int_eq(plan_count, FQL_FAIL);

	/* divide by 0 */
	plan_count = fql_make_plans(fql, "select 101 / 0");
	ck_assert_int_eq(plan_count, FQL_FAIL);

	/* Unary overflow */
	plan_count = fql_make_plans(fql, "select -(-9223372036854775808)");
	ck_assert_int_eq(plan_count, FQL_FAIL);

	/* function args */
	plan_count = fql_make_plans(fql, "select left('1234')");
	ck_assert_int_eq(plan_count, FQL_FAIL);
}
END_TEST

START_TEST(test_failure_runtime)
{
	struct fql_field* fields = NULL;
	int plan_count = 0;
	int field_count = 0;
	int rows = 0;

	/* non-numeric data where not expected */
	plan_count = fql_make_plans(fql, "select bar + 1 from t1");
	ck_assert_int_eq(plan_count, 1);
	do {
		rows = fql_step(fql, &fields);
	} while (rows == 1);
	ck_assert_int_eq(rows, FQL_FAIL);

	/* arithmetic overflow */
	plan_count = fql_make_plans(fql, "select 123433333123412 * baz from t1");
	ck_assert_int_eq(plan_count, 1);
	do {
		rows = fql_step(fql, &fields);
	} while (rows == 1);
	ck_assert_int_eq(rows, FQL_FAIL);

	/* divide by 0 */
	plan_count = fql_make_plans(fql, "select 100.0 / ('0x' + bar + 0) from t2");
	ck_assert_int_eq(plan_count, 1);
	do {
		rows = fql_step(fql, &fields);
	} while (rows == 1);
	ck_assert_int_eq(rows, FQL_FAIL);
}
END_TEST

Suite* fql_failure_suite(void)
{
	Suite* s;
	s = suite_create("failure");

	TCase* tc_failure = tcase_create("failure");
	tcase_add_checked_fixture(tc_failure, fql_setup, fql_teardown);

	tcase_add_test(tc_failure, test_failure_syntax);
	tcase_add_test(tc_failure, test_failure_columns);
	tcase_add_test(tc_failure, test_failure_const_runtime);
	tcase_add_test(tc_failure, test_failure_runtime);

	suite_add_tcase(s, tc_failure);

	return s;
}

int main(void)
{
	int number_failed;
	Suite* failure_suite = fql_failure_suite();
	SRunner* failure_runner = srunner_create(failure_suite);
	srunner_set_fork_status (failure_runner, CK_NOFORK);

	srunner_run_all(failure_runner, CK_VERBOSE);
	number_failed = srunner_ntests_failed(failure_runner);
	srunner_free(failure_runner);

	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}


