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

START_TEST(test_difficult_typing)
{

	struct fql_field* fields = NULL;
	int plan_count = 0;
	int field_count = 0;
	int rows = 0;

	/* Difficulty here is keeping all the types correct.
	 * Using '0x' + <hex string> + int should give us
	 * integer output.
	 */
	plan_count = fql_make_plans(fql, "select bar, ('0x' + bar + 0) * 1 from t2");
	ck_assert_int_eq(plan_count, 1);

	field_count = fql_field_count(fql);
	ck_assert_int_eq(field_count, 2);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 1);
	ck_assert_int_eq(fields[0].type, FQL_STRING);
	ck_assert_int_eq(fields[1].type, FQL_INT);

	ck_assert_str_eq(fields[0].data.s, "12");
	ck_assert_int_eq(fields[1].data.i, 18);
	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "a6");
	ck_assert_int_eq(fields[1].data.i, 166);
	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "00");
	ck_assert_int_eq(fields[1].data.i, 0);
	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "e8");
	ck_assert_int_eq(fields[1].data.i, 232);
	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "8c");
	ck_assert_int_eq(fields[1].data.i, 140);
	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "52");
	ck_assert_int_eq(fields[1].data.i, 82);
	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "ef");
	ck_assert_int_eq(fields[1].data.i, 239);
	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "0a");
	ck_assert_int_eq(fields[1].data.i, 10);
	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "18");
	ck_assert_int_eq(fields[1].data.i, 24);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 0);
	ck_assert_int_eq(fql_field_count(fql), 0);
}
END_TEST

Suite* fql_difficult_suite(void)
{
	Suite* s;
	s = suite_create("difficult");

	TCase* tc_difficult = tcase_create("difficult");
	tcase_add_checked_fixture(tc_difficult, fql_setup, fql_teardown);

	tcase_add_test(tc_difficult, test_difficult_typing);

	suite_add_tcase(s, tc_difficult);

	return s;
}

int main(void)
{
	int number_failed;
	Suite* difficult_suite = fql_difficult_suite();
	SRunner* difficult_runner = srunner_create(difficult_suite);
	srunner_set_fork_status (difficult_runner, CK_NOFORK);

	srunner_run_all(difficult_runner, CK_VERBOSE);
	number_failed = srunner_ntests_failed(difficult_runner);
	srunner_free(difficult_runner);

	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}


