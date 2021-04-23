#include "check_common.h"


START_TEST(test_subquery_const)
{
	struct fql_field* fields = NULL;
	int plan_count = 0;
	int field_count = 0;
	int rows = 0;

	/* Some lax rules here compared to SQL Server...
	 * No alias really necessary on 7 or subquery
	 */
	plan_count = fql_make_plans(fql, "select * from (select 7)");
	ck_assert_int_eq(plan_count, 1);

	field_count = fql_field_count(fql);
	ck_assert_int_eq(field_count, 1);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 1);
	ck_assert_int_eq(fields[0].type, FQL_INT);
	ck_assert_int_eq(fields[0].data.i, 7);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 0);
	ck_assert_int_eq(fql_field_count(fql), 0);


	plan_count = fql_make_plans(fql, "select '6' + t from (select 7 t)");
	ck_assert_int_eq(plan_count, 1);

	field_count = fql_field_count(fql);
	ck_assert_int_eq(field_count, 1);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 1);
	ck_assert_int_eq(fields[0].type, FQL_INT);
	ck_assert_int_eq(fields[0].data.i, 13);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 0);
	ck_assert_int_eq(fql_field_count(fql), 0);


	plan_count = fql_make_plans(fql, "select '6' + t from (select '7' t)");
	ck_assert_int_eq(plan_count, 1);

	field_count = fql_field_count(fql);
	ck_assert_int_eq(field_count, 1);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 1);
	ck_assert_int_eq(fields[0].type, FQL_STRING);
	ck_assert_str_eq(fields[0].data.s, "67");

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 0);
	ck_assert_int_eq(fql_field_count(fql), 0);
}
END_TEST

Suite* fql_subquery_suite(void)
{
	Suite* s;
	s = suite_create("subquery");

	TCase* tc_subquery = tcase_create("subquery");
	tcase_add_checked_fixture(tc_subquery, fql_setup, fql_teardown);

	tcase_add_test(tc_subquery, test_subquery_const);

	suite_add_tcase(s, tc_subquery);

	return s;
}

int main(void)
{
	int number_failed;
	Suite* subquery_suite = fql_subquery_suite();
	SRunner* subquery_runner = srunner_create(subquery_suite);
	srunner_set_fork_status (subquery_runner, CK_NOFORK);

	srunner_run_all(subquery_runner, CK_VERBOSE);
	number_failed = srunner_ntests_failed(subquery_runner);
	srunner_free(subquery_runner);

	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}


