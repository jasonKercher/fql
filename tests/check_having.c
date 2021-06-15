#include "check_common.h"

/* t3.psv
foo|bar|baz
6Ed156a7|ae|7791
9f04f0a0|cd|19075
3c3cc25d|cd|83146
24b256e0|cd|19950
0d88f1f3|e0|95934
0cda1b43|ae|38038
8e4690dc|fb|185937
2764151e|fb|45490
44b72d44|fb|143027
*/

START_TEST(test_having_const)
{
	struct fql_field* fields = NULL;
	int plan_count = 0;
	int field_count = 0;
	int rows = 0;

	plan_count = fql_make_plans(fql, "select 1 group by 2 having 1=1");
	ck_assert_int_eq(plan_count, 1);

	field_count = fql_field_count(fql);
	ck_assert_int_eq(field_count, 1);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 1);
	ck_assert_int_eq(fields[0].type, FQL_INT);
	ck_assert_int_eq(fields[0].data.i, 1);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 0);
	ck_assert_int_eq(fql_field_count(fql), 0);

	plan_count = fql_make_plans(fql, "select 1 group by 3 having 1=0");
	ck_assert_int_eq(plan_count, 1);

	field_count = fql_field_count(fql);
	ck_assert_int_eq(field_count, 1);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 0);
	ck_assert_int_eq(fql_field_count(fql), 0);

	plan_count =
	        fql_make_plans(fql, "select count(*) group by 1 having 1=0");
	ck_assert_int_eq(plan_count, 1);

	field_count = fql_field_count(fql);
	ck_assert_int_eq(field_count, 1);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 0);
	ck_assert_int_eq(fql_field_count(fql), 0);
}
END_TEST

START_TEST(test_having_aggregate)
{
	struct fql_field* fields = NULL;
	int plan_count = 0;
	int field_count = 0;
	int rows = 0;

	plan_count = fql_make_plans(
	        fql,
	        "select bar from t3 group by bar having count(*) > 1");
	ck_assert_int_eq(plan_count, 1);

	field_count = fql_field_count(fql);
	ck_assert_int_eq(field_count, 1);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 1);
	ck_assert_int_eq(fields[0].type, FQL_STRING);

	ck_assert_str_eq(fields[0].data.s, "ae");
	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "cd");
	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "fb");

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 0);
	ck_assert_int_eq(fql_field_count(fql), 0);

	/* Identical to distinct */
	plan_count = fql_make_plans(
	        fql,
	        "select bar from t3 group by bar having max(foo) > '9'");
	ck_assert_int_eq(plan_count, 1);

	field_count = fql_field_count(fql);
	ck_assert_int_eq(field_count, 1);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 1);
	ck_assert_int_eq(fields[0].type, FQL_STRING);
	ck_assert_str_eq(fields[0].data.s, "cd");

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 0);
	ck_assert_int_eq(fql_field_count(fql), 0);
}
END_TEST

Suite* fql_having_suite(void)
{
	Suite* s;
	s = suite_create("having");

	TCase* tc_having = tcase_create("group");
	tcase_add_checked_fixture(tc_having, fql_setup, fql_teardown);

	tcase_add_test(tc_having, test_having_const);
	tcase_add_test(tc_having, test_having_aggregate);

	suite_add_tcase(s, tc_having);

	return s;
}

int main(void)
{
	int number_failed;
	Suite* having_suite = fql_having_suite();
	SRunner* having_runner = srunner_create(having_suite);
	srunner_set_fork_status(having_runner, CK_NOFORK);

	srunner_run_all(having_runner, CK_VERBOSE);
	number_failed = srunner_ntests_failed(having_runner);
	srunner_free(having_runner);

	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}


