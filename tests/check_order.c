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

START_TEST(test_order_const)
{
	struct fql_field* fields = NULL;
	int plan_count = 0;
	int field_count = 0;
	int rows = 0;

	plan_count = fql_make_plans(fql, "select 1 foo order by foo");
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


	plan_count = fql_make_plans(fql, "select 2 order by 1");
	ck_assert_int_eq(plan_count, 1);

	field_count = fql_field_count(fql);
	ck_assert_int_eq(field_count, 1);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 1);
	ck_assert_int_eq(fields[0].type, FQL_INT);
	ck_assert_int_eq(fields[0].data.i, 2);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 0);
	ck_assert_int_eq(fql_field_count(fql), 0);
}
END_TEST

START_TEST(test_order_distinct)
{
	struct fql_field* fields = NULL;
	int plan_count = 0;
	int field_count = 0;
	int rows = 0;

	plan_count = fql_make_plans(fql, "select distinct bar from t3 order by bar");
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
	ck_assert_str_eq(fields[0].data.s, "e0");
	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "fb");

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 0);
	ck_assert_int_eq(fql_field_count(fql), 0);

}
END_TEST

START_TEST(test_order_mutiple)
{
	struct fql_field* fields = NULL;
	int plan_count = 0;
	int field_count = 0;
	int rows = 0;

	plan_count = fql_make_plans(fql, "select foo from t3 order by bar, foo");
	ck_assert_int_eq(plan_count, 1);

	field_count = fql_field_count(fql);
	ck_assert_int_eq(field_count, 1);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 1);
	ck_assert_int_eq(fields[0].type, FQL_STRING);

	ck_assert_str_eq(fields[0].data.s, "0cda1b43");
	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "6Ed156a7");
	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "24b256e0");
	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "3c3cc25d");
	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "9f04f0a0");
	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "0d88f1f3");
	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "2764151e");
	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "44b72d44");
	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "8e4690dc");
	rows = fql_step(fql, &fields);

	ck_assert_int_eq(rows, 0);
	ck_assert_int_eq(fql_field_count(fql), 0);

	plan_count =
	        fql_make_plans(fql,
	                       "select foo from t3 order by bar desc, foo");
	ck_assert_int_eq(plan_count, 1);

	field_count = fql_field_count(fql);
	ck_assert_int_eq(field_count, 1);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 1);
	ck_assert_int_eq(fields[0].type, FQL_STRING);

	ck_assert_str_eq(fields[0].data.s, "2764151e");
	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "44b72d44");
	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "8e4690dc");
	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "0d88f1f3");
	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "24b256e0");
	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "3c3cc25d");
	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "9f04f0a0");
	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "0cda1b43");
	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "6Ed156a7");
	rows = fql_step(fql, &fields);

	ck_assert_int_eq(rows, 0);
	ck_assert_int_eq(fql_field_count(fql), 0);
}

START_TEST(test_order_numeric)
{
	struct fql_field* fields = NULL;
	int plan_count = 0;
	int field_count = 0;
	int rows = 0;

	plan_count = fql_make_plans(fql, "select baz + 2   "
			                 " from t3         "
			                 " order by baz + 0");
	ck_assert_int_eq(plan_count, 1);

	field_count = fql_field_count(fql);
	ck_assert_int_eq(field_count, 1);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 1);
	ck_assert_int_eq(fields[0].type, FQL_INT);

	ck_assert_int_eq(fields[0].data.i, 7793);
	rows = fql_step(fql, &fields);
	ck_assert_int_eq(fields[0].data.i, 19077);
	rows = fql_step(fql, &fields);
	ck_assert_int_eq(fields[0].data.i, 19952);
	rows = fql_step(fql, &fields);
	ck_assert_int_eq(fields[0].data.i, 38040);
	rows = fql_step(fql, &fields);
	ck_assert_int_eq(fields[0].data.i, 45492);
	rows = fql_step(fql, &fields);
	ck_assert_int_eq(fields[0].data.i, 83148);
	rows = fql_step(fql, &fields);
	ck_assert_int_eq(fields[0].data.i, 95936);
	rows = fql_step(fql, &fields);
	ck_assert_int_eq(fields[0].data.i, 143029);
	rows = fql_step(fql, &fields);
	ck_assert_int_eq(fields[0].data.i, 185939);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 0);
	ck_assert_int_eq(fql_field_count(fql), 0);
}
END_TEST

START_TEST(test_order_aggregate)
{
	struct fql_field* fields = NULL;
	int plan_count = 0;
	int field_count = 0;
	int rows = 0;

	plan_count = fql_make_plans(fql, "select count(*)   "
			                 " from t3          "
					 " group by bar     "
					 " order by count(*)");
	ck_assert_int_eq(plan_count, 1);

	field_count = fql_field_count(fql);
	ck_assert_int_eq(field_count, 1);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 1);
	ck_assert_int_eq(fields[0].type, FQL_INT);

	ck_assert_int_eq(fields[0].data.i, 1);
	rows = fql_step(fql, &fields);
	ck_assert_int_eq(fields[0].data.i, 2);
	rows = fql_step(fql, &fields);
	ck_assert_int_eq(fields[0].data.i, 3);
	rows = fql_step(fql, &fields);
	ck_assert_int_eq(fields[0].data.i, 3);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 0);
	ck_assert_int_eq(fql_field_count(fql), 0);

	plan_count = fql_make_plans(fql,
	                            "select count(*)   "
	                            " from t3          "
	                            " group by bar     "
	                            " order by 1");
	ck_assert_int_eq(plan_count, 1);

	field_count = fql_field_count(fql);
	ck_assert_int_eq(field_count, 1);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 1);
	ck_assert_int_eq(fields[0].type, FQL_INT);

	ck_assert_int_eq(fields[0].data.i, 1);
	rows = fql_step(fql, &fields);
	ck_assert_int_eq(fields[0].data.i, 2);
	rows = fql_step(fql, &fields);
	ck_assert_int_eq(fields[0].data.i, 3);
	rows = fql_step(fql, &fields);
	ck_assert_int_eq(fields[0].data.i, 3);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 0);
	ck_assert_int_eq(fql_field_count(fql), 0);
}
END_TEST

Suite* fql_order_suite(void)
{
	Suite* s;
	s = suite_create("order");

	TCase* tc_order = tcase_create("order");
	tcase_add_checked_fixture(tc_order, fql_setup, fql_teardown);

	tcase_add_test(tc_order, test_order_const);
	tcase_add_test(tc_order, test_order_distinct);
	tcase_add_test(tc_order, test_order_mutiple);
	tcase_add_test(tc_order, test_order_numeric);
	tcase_add_test(tc_order, test_order_aggregate);

	suite_add_tcase(s, tc_order);

	return s;
}

int main(void)
{
	int number_failed;
	Suite* order_suite = fql_order_suite();
	SRunner* order_runner = srunner_create(order_suite);
	srunner_set_fork_status (order_runner, CK_NOFORK);

	srunner_run_all(order_runner, CK_VERBOSE);
	number_failed = srunner_ntests_failed(order_runner);
	srunner_free(order_runner);

	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}


