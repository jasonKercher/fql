#include "check.h"
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

	/* More than 1 expression in constant subquery */
	plan_count = fql_make_plans(fql, "select (select 1, 2)");
	ck_assert_int_eq(plan_count, FQL_FAIL);

	/* Invalid function parameters */
	plan_count = fql_make_plans(fql, "select left('shnt', -1)");
	ck_assert_int_eq(plan_count, FQL_FAIL);

	/* Missing alias on subquery */
	plan_count = fql_make_plans(fql, "select * from (select 7)");
	ck_assert_int_eq(plan_count, FQL_FAIL);
}
END_TEST

START_TEST(test_failure_columns)
{
	struct fql_field* fields = NULL;
	int plan_count = 0;

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

	/* In select list, but not in grouping */
	plan_count = fql_make_plans(fql, "select foo from t1 group by bar");
	ck_assert_int_eq(plan_count, FQL_FAIL);

	/* In order by list, but not in grouping */
	plan_count = fql_make_plans(fql,
	                            "select bar   "
	                            "from t1      "
	                            "group by bar "
	                            "order by foo ");
	ck_assert_int_eq(plan_count, FQL_FAIL);
}
END_TEST

START_TEST(test_failure_fql_specific)
{
	struct fql_field* fields = NULL;
	int plan_count = 0;

	/* Function not yet implemented */
	plan_count = fql_make_plans(fql, "select patindex('%shnt%','xxxx')");
	ck_assert_int_eq(plan_count, FQL_FAIL);

	fql_set_allow_stdin(fql, true);

	/* Try to utilize __stdin twice */
	plan_count = fql_make_plans(fql,
	                            "select *              "
	                            "from __stdin x1       "
	                            "join __stdin x2       "
	                            "    on x1.foo = x2.foo");
	ck_assert_int_eq(plan_count, FQL_FAIL);
}
END_TEST

START_TEST(test_failure_multioperation)
{
	struct fql_field* fields = NULL;
	int plan_count = 0;

	/* INTO column missing from fresh file */
	fql_set_overwrite(fql, true);
	plan_count = fql_make_plans(fql,
	                            "select foo, bar     "
	                            "into [nobaz.temp]   "
	                            "from t1             "
	                            "                    "
	                            "select baz          "
	                            "from [nobaz.temp]   ");
	ck_assert_int_eq(plan_count, FQL_FAIL);

	/* This file already exists AND contains the column
	 * [baz]. We should be able to realize this query
	 * is going to fail up front because we can see
	 * readonlyt1 is going to be overwritten, and the
	 * new schema does not contain a column named [baz].
	 */
	plan_count = fql_make_plans(fql,
	                            "select foo, bar       "
	                            "into [readonlyt1.tsv] "
	                            "from t1               "
	                            "                      "
	                            "select baz            "
	                            "from [readonlyt1.tsv] ");
	ck_assert_int_eq(plan_count, FQL_FAIL);
}
END_TEST

START_TEST(test_failure_strict_mode)
{
	int plan_count = 0;
	fql_set_strict_mode(fql, 1);

	/* T4 header
	foo	bar	foo
	*/
	plan_count = fql_make_plans(fql, "select foo from [t4.tsv]");
	ck_assert_int_eq(plan_count, FQL_FAIL);

	/* Only exact match on file name */
	plan_count = fql_make_plans(fql, "select * from t1");
	ck_assert_int_eq(plan_count, FQL_FAIL);
}
END_TEST

START_TEST(test_failure_const_runtime)
{
	struct fql_field* fields = NULL;
	int plan_count = 0;

	/* non-numeric data where not expected */
	plan_count = fql_make_plans(fql, "select 's' + 1");
	ck_assert_int_eq(plan_count, FQL_FAIL);

	/* divide by 0 */
	plan_count = fql_make_plans(fql, "select 101 / 0");
	ck_assert_int_eq(plan_count, FQL_FAIL);

	/* function args */
	plan_count = fql_make_plans(fql, "select left('1234')");
	ck_assert_int_eq(plan_count, FQL_FAIL);

	/* Unfortunately, fql fails to keep up with SQL Server here.
	 * Many of the following queries will avoid overflows on SQL
	 * Server by utilizing arbitrary precision types.
	 */

	/* addition overflow */
	plan_count = fql_make_plans(fql, "select -9223372036854775806 + -124");
	ck_assert_int_eq(plan_count, FQL_FAIL);

	/* subtraction overflow */
	plan_count = fql_make_plans(fql, "select -9223372036854775806 - 124");
	ck_assert_int_eq(plan_count, FQL_FAIL);

	/* multiplictaion overflow */
	plan_count = fql_make_plans(fql, "select -922337203685477580 * 124");
	ck_assert_int_eq(plan_count, FQL_FAIL);

	/* Unary overflow */
	plan_count = fql_make_plans(fql, "select -(-9223372036854775808)");
	ck_assert_int_eq(plan_count, FQL_FAIL);
}
END_TEST

START_TEST(test_failure_runtime)
{
	struct fql_field* fields = NULL;
	int plan_count = 0;
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

	/* Invalid UTF-8 */
	plan_count = fql_make_plans(fql, "select left(foo, 1) from invalid_utf8");
	ck_assert_int_eq(plan_count, 1);
	do {
		rows = fql_step(fql, &fields);
	} while (rows == 1);
	ck_assert_int_eq(rows, FQL_FAIL);

	/* subquery expression returning more than one result */
	plan_count = fql_make_plans(fql, "select (select foo from t1)");
	ck_assert_int_eq(plan_count, 1);
	do {
		rows = fql_step(fql, &fields);
	} while (rows == 1);
	ck_assert_int_eq(rows, FQL_FAIL);

	/* subquery expression returning more than one result with source */
	plan_count = fql_make_plans(fql, "select (select foo from t1) from t2");
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
	tcase_add_test(tc_failure, test_failure_fql_specific);
	tcase_add_test(tc_failure, test_failure_multioperation);
	tcase_add_test(tc_failure, test_failure_strict_mode);
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
