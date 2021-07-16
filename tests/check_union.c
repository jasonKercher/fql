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

START_TEST(test_union_const)
{
	struct fql_field* fields = NULL;
	int plan_count = 0;
	int field_count = 0;
	int rows = 0;


	plan_count = fql_make_plans(fql, "select 1 union all select 2");
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
	ck_assert_int_eq(rows, 0);
	ck_assert_int_eq(fql_field_count(fql), 0);


	plan_count = fql_make_plans(fql, "select foo from t1 union all select 1");
	ck_assert_int_eq(plan_count, 1);

	field_count = fql_field_count(fql);
	ck_assert_int_eq(field_count, 1);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 1);
	ck_assert_int_eq(fields[0].type, FQL_STRING);
	ck_assert_str_eq(fields[0].data.s, "44b72d44");

	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "b8e63822");
	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "46a1bad3");
	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "8ab25d8f");
	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "282a4957");
	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "3138b3f8");
	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "18e25713");
	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "c1519b0d");
	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "6ed156a7");
	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "1");

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 0);
	ck_assert_int_eq(fql_field_count(fql), 0);


	plan_count = fql_make_plans(fql, "select 1 union all select foo from t1");
	ck_assert_int_eq(plan_count, 1);

	field_count = fql_field_count(fql);
	ck_assert_int_eq(field_count, 1);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 1);
	ck_assert_int_eq(fields[0].type, FQL_STRING);
	ck_assert_int_eq(fields[0].data.i, 1);

	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "44b72d44");
	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "b8e63822");
	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "46a1bad3");
	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "8ab25d8f");
	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "282a4957");
	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "3138b3f8");
	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "18e25713");
	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "c1519b0d");
	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "6ed156a7");

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 0);
	ck_assert_int_eq(fql_field_count(fql), 0);
}
END_TEST

START_TEST(test_union_distinct)
{
	struct fql_field* fields = NULL;
	int plan_count = 0;
	int field_count = 0;
	int rows = 0;

	plan_count = fql_make_plans(fql, "select distinct bar from t3");
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

	/* Identical to distinct */
	plan_count = fql_make_plans(fql, "select bar from t3 group by bar");
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

START_TEST(test_union_complex_unions)
{
	struct fql_field* fields = NULL;
	int plan_count = 0;
	int field_count = 0;
	int rows = 0;

	plan_count = fql_make_plans(fql, "select len(bar)+'1' from t3 group by bar");
	ck_assert_int_eq(plan_count, 1);

	field_count = fql_field_count(fql);
	ck_assert_int_eq(field_count, 1);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 1);
	ck_assert_int_eq(fields[0].type, FQL_INT);
	ck_assert_int_eq(fields[0].data.i, 3);
	rows = fql_step(fql, &fields);
	ck_assert_int_eq(fields[0].data.i, 3);
	rows = fql_step(fql, &fields);
	ck_assert_int_eq(fields[0].data.i, 3);
	rows = fql_step(fql, &fields);
	ck_assert_int_eq(fields[0].data.i, 3);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 0);
	ck_assert_int_eq(fql_field_count(fql), 0);

	/* Turns out SQL Server isn't smart enough to put this
	 * one together. Of course, I don't know what sort of
	 * smooth brain would even write something so dumb...
	 */
	plan_count = fql_make_plans(fql, "select baz + 2"
			                 " from t3"
					 " group by baz + (4 * 3) / 6");
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
	ck_assert_int_eq(fields[0].data.i, 83148);
	rows = fql_step(fql, &fields);
	ck_assert_int_eq(fields[0].data.i, 19952);
	rows = fql_step(fql, &fields);
	ck_assert_int_eq(fields[0].data.i, 95936);
	rows = fql_step(fql, &fields);
	ck_assert_int_eq(fields[0].data.i, 38040);
	rows = fql_step(fql, &fields);
	ck_assert_int_eq(fields[0].data.i, 185939);
	rows = fql_step(fql, &fields);
	ck_assert_int_eq(fields[0].data.i, 45492);
	rows = fql_step(fql, &fields);
	ck_assert_int_eq(fields[0].data.i, 143029);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 0);
	ck_assert_int_eq(fql_field_count(fql), 0);
}
END_TEST

START_TEST(test_union_aggregate)
{
	struct fql_field* fields = NULL;
	int plan_count = 0;
	int field_count = 0;
	int rows = 0;

	plan_count = fql_make_plans(fql, "select count(*) from t3");
	ck_assert_int_eq(plan_count, 1);

	field_count = fql_field_count(fql);
	ck_assert_int_eq(field_count, 1);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 1);
	ck_assert_int_eq(fields[0].type, FQL_INT);
	ck_assert_int_eq(fields[0].data.i, 9);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 0);
	ck_assert_int_eq(fql_field_count(fql), 0);

	plan_count = fql_make_plans(fql, "select bar, count(*) from t3 group by bar");
	ck_assert_int_eq(plan_count, 1);

	field_count = fql_field_count(fql);
	ck_assert_int_eq(field_count, 2);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 1);
	ck_assert_int_eq(fields[0].type, FQL_STRING);
	ck_assert_int_eq(fields[1].type, FQL_INT);
	ck_assert_str_eq(fields[0].data.s, "ae");
	ck_assert_int_eq(fields[1].data.i, 2);
	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "cd");
	ck_assert_int_eq(fields[1].data.i, 3);
	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "e0");
	ck_assert_int_eq(fields[1].data.i, 1);
	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "fb");
	ck_assert_int_eq(fields[1].data.i, 3);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 0);
	ck_assert_int_eq(fql_field_count(fql), 0);
}
END_TEST

Suite* fql_union_suite(void)
{
	Suite* s;
	s = suite_create("union");

	TCase* tc_union = tcase_create("union");
	tcase_add_checked_fixture(tc_union, fql_setup, fql_teardown);

	tcase_add_test(tc_union, test_union_const);
	//tcase_add_test(tc_union, test_union_distinct);
	//tcase_add_test(tc_union, test_union_complex_unions);
	//tcase_add_test(tc_union, test_union_aggregate);

	suite_add_tcase(s, tc_union);

	return s;
}

int main(void)
{
	int number_failed;
	Suite* union_suite = fql_union_suite();
	SRunner* union_runner = srunner_create(union_suite);
	srunner_set_fork_status(union_runner, CK_NOFORK);

	srunner_run_all(union_runner, CK_VERBOSE);
	number_failed = srunner_ntests_failed(union_runner);
	srunner_free(union_runner);

	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}


