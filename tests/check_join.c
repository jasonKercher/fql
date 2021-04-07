#include "check_common.h"

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


START_TEST(test_hashjoin_asterisk)
{
	struct fql_field* fields = NULL;
	int plan_count = 0;
	int field_count = 0;
	int rows = 0;

	/* select t1.*, t2.* */
	plan_count = fql_make_plans(fql, "select * from t1 join t2 on t1.foo = t2.foo");
	ck_assert_int_eq(plan_count, 1);

	field_count = fql_field_count(fql);
	ck_assert_int_eq(field_count, 6);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 1);
	ck_assert_int_eq(fields[0].type, FQL_STRING);
	ck_assert_int_eq(fields[1].type, FQL_STRING);
	ck_assert_int_eq(fields[2].type, FQL_STRING);
	ck_assert_int_eq(fields[3].type, FQL_STRING);
	ck_assert_int_eq(fields[4].type, FQL_STRING);
	ck_assert_int_eq(fields[5].type, FQL_STRING);

	ck_assert_str_eq(fields[0].data.s, "6ed156a7");
	ck_assert_str_eq(fields[1].data.s, "ae");
	ck_assert_str_eq(fields[2].data.s, "229701");
	ck_assert_str_eq(fields[3].data.s, "6Ed156A7");
	ck_assert_str_eq(fields[4].data.s, "e8");
	ck_assert_str_eq(fields[5].data.s, "75882");

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 0);
	ck_assert_int_eq(fql_field_count(fql), 0);

	/* select t2.*, t1.* */
	plan_count = fql_make_plans(fql, "select * from t2 join t1 on t1.foo = t2.foo");
	ck_assert_int_eq(plan_count, 1);

	field_count = fql_field_count(fql);
	ck_assert_int_eq(field_count, 6);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 1);
	ck_assert_int_eq(fields[0].type, FQL_STRING);
	ck_assert_int_eq(fields[1].type, FQL_STRING);
	ck_assert_int_eq(fields[2].type, FQL_STRING);
	ck_assert_int_eq(fields[3].type, FQL_STRING);
	ck_assert_int_eq(fields[4].type, FQL_STRING);
	ck_assert_int_eq(fields[5].type, FQL_STRING);

	ck_assert_str_eq(fields[0].data.s, "6Ed156A7");
	ck_assert_str_eq(fields[1].data.s, "e8");
	ck_assert_str_eq(fields[2].data.s, "75882");
	ck_assert_str_eq(fields[3].data.s, "6ed156a7");
	ck_assert_str_eq(fields[4].data.s, "ae");
	ck_assert_str_eq(fields[5].data.s, "229701");

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 0);
	ck_assert_int_eq(fql_field_count(fql), 0);
}
END_TEST

START_TEST(test_force_cartesian_join)
{
	struct fql_field* fields = NULL;
	int plan_count = 0;
	int field_count = 0;
	int rows = 0;

	/* For a standard equality join, we must force a
	 * cartesian join because it will want to use a
	 * hash join algorithm here.
	 */
	fql_set_force_cartesian(fql, 1);
	plan_count = fql_make_plans(fql, "select t1.Foo from t1 join t2 on t1.foo = t2.foo");
	ck_assert_int_eq(plan_count, 1);

	field_count = fql_field_count(fql);
	ck_assert_int_eq(field_count, 1);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 1);
	ck_assert_int_eq(fields[0].type, FQL_STRING);
	ck_assert_str_eq(fields[0].data.s, "6ed156a7");

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 0);
	ck_assert_int_eq(fql_field_count(fql), 0);
}
END_TEST

START_TEST(test_join_functions)
{
	struct fql_field* fields = NULL;
	int plan_count = 0;
	int field_count = 0;
	int rows = 0;

	/* bar	bar
	 * 70	a6
	 * ae	e8
	 */
	fql_set_force_cartesian(fql, 1);
	plan_count = fql_make_plans(fql, "select t1.bar, t2.bar from t1 join t2 on right(t1.foo, 2) = right(t2.foo, 2)");
	ck_assert_int_eq(plan_count, 1);

	field_count = fql_field_count(fql);
	ck_assert_int_eq(field_count, 2);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 1);
	ck_assert_int_eq(fields[0].type, FQL_STRING);
	ck_assert_int_eq(fields[1].type, FQL_STRING);
	ck_assert_str_eq(fields[0].data.s, "70");
	ck_assert_str_eq(fields[1].data.s, "a6");

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 1);
	ck_assert_str_eq(fields[0].data.s, "ae");
	ck_assert_str_eq(fields[1].data.s, "e8");

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 0);
	ck_assert_int_eq(fql_field_count(fql), 0);
}
END_TEST

Suite* fql_join_suite(void)
{
	Suite* s;
	s = suite_create("join");

	TCase* tc_join = tcase_create("join");
	tcase_add_checked_fixture(tc_join, fql_setup, fql_teardown);

	tcase_add_test(tc_join, test_hashjoin_asterisk);
	tcase_add_test(tc_join, test_force_cartesian_join);
	tcase_add_test(tc_join, test_join_functions);
	//tcase_add_test(tc_join, test_join_t1_asterisk);
	//tcase_add_test(tc_join, test_join_const);
	//tcase_add_test(tc_join, test_join_operators);

	suite_add_tcase(s, tc_join);

	return s;
}

int main(void)
{
	int number_failed;
	Suite* join_suite = fql_join_suite();
	SRunner* join_runner = srunner_create(join_suite);
	srunner_set_fork_status (join_runner, CK_NOFORK);

	srunner_run_all(join_runner, CK_VERBOSE);
	number_failed = srunner_ntests_failed(join_runner);
	srunner_free(join_runner);

	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}


