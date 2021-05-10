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

START_TEST(test_group_const)
{
	struct fql_field* fields = NULL;
	int plan_count = 0;
	int field_count = 0;
	int rows = 0;

	/* Some lax rules here compared to SQL Server...
	 * No alias really necessary on 7 or group
	 */
	plan_count = fql_make_plans(fql, "select 1 group by 2");
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


	plan_count = fql_make_plans(fql, "select count(*) group by 3");
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


	plan_count = fql_make_plans(fql, "select count(*)");
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
}
END_TEST

Suite* fql_group_suite(void)
{
	Suite* s;
	s = suite_create("group");

	TCase* tc_group = tcase_create("group");
	tcase_add_checked_fixture(tc_group, fql_setup, fql_teardown);

	tcase_add_test(tc_group, test_group_const);
	//tcase_add_test(tc_group, test_group_read);
	//tcase_add_test(tc_group, test_group_nested);

	suite_add_tcase(s, tc_group);

	return s;
}

int main(void)
{
	int number_failed;
	Suite* group_suite = fql_group_suite();
	SRunner* group_runner = srunner_create(group_suite);
	srunner_set_fork_status (group_runner, CK_NOFORK);

	srunner_run_all(group_runner, CK_VERBOSE);
	number_failed = srunner_ntests_failed(group_runner);
	srunner_free(group_runner);

	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}


