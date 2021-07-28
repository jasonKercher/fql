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
 *
 */

void _assert_select_null_const(const char* query_string)
{
	int plan_count = fql_make_plans(fql, query_string);
	ck_assert_int_eq(plan_count, 1);

	struct fql_field* fields = NULL;
	int field_count = 0;
	int rows = 0;

	field_count = fql_field_count(fql);
	ck_assert_int_eq(field_count, 1);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 1);
	ck_assert_int_eq(fields[0].is_null, true);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 0);
	ck_assert_int_eq(fql_field_count(fql), 0);
}

START_TEST(test_null_const)
{
	_assert_select_null_const("select NULL");
	_assert_select_null_const("select 1 + NULL");
	_assert_select_null_const("select NULL + 1");
	_assert_select_null_const(
	        "select left('00000000' + cast(5 * (1+NULL) as varchar), 8)");
	/* To test for user, must think like user */
	_assert_select_null_const("select isnull(1 + NULL, NULL)");
}
END_TEST

START_TEST(test_null_in_logic)
{
	assert_select_int("select 1 where NULL - 4 is NULL", 1);
	assert_nothing("select 1 where NULL - 4 is not NULL");

	assert_select_int("select 1 where 6 - 4 is not NULL", 1);
	assert_nothing("select 1 where 6 - 4 is NULL");
}
END_TEST

Suite* fql_null_suite(void)
{
	Suite* s;
	s = suite_create("null");

	TCase* tc_null = tcase_create("null");
	tcase_add_checked_fixture(tc_null, fql_setup, fql_teardown);

	tcase_add_test(tc_null, test_null_const);
	tcase_add_test(tc_null, test_null_in_logic);

	suite_add_tcase(s, tc_null);

	return s;
}

int main(void)
{
	int number_failed;
	Suite* null_suite = fql_null_suite();
	SRunner* null_runner = srunner_create(null_suite);
	srunner_set_fork_status(null_runner, CK_NOFORK);

	srunner_run_all(null_runner, CK_VERBOSE);
	number_failed = srunner_ntests_failed(null_runner);
	srunner_free(null_runner);

	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}


