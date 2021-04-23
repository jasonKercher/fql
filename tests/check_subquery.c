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

START_TEST(test_subquery_read)
{
	struct fql_field* fields = NULL;
	int plan_count = 0;
	int field_count = 0;
	int rows = 0;

	//c1519b0d	c8	87082
	plan_count = fql_make_plans(fql, "select * from (select bar from t1 where foo = 'c1519b0d')");
	ck_assert_int_eq(plan_count, 1);

	field_count = fql_field_count(fql);
	ck_assert_int_eq(field_count, 1);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 1);
	ck_assert_int_eq(fields[0].type, FQL_STRING);
	ck_assert_str_eq(fields[0].data.s, "c8");

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 0);
	ck_assert_int_eq(fql_field_count(fql), 0);

	plan_count = fql_make_plans(fql, "select shnt, bar from (select left(foo, 1) shnt, * from t1)");
	ck_assert_int_eq(plan_count, 1);

	field_count = fql_field_count(fql);
	ck_assert_int_eq(field_count, 2);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 1);
	ck_assert_int_eq(fields[0].type, FQL_STRING);
	ck_assert_int_eq(fields[1].type, FQL_STRING);

	ck_assert_str_eq(fields[0].data.s, "4");
	ck_assert_str_eq(fields[1].data.s, "70");

	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "b");
	ck_assert_str_eq(fields[1].data.s, "cb");

	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "4");
	ck_assert_str_eq(fields[1].data.s, "5f");

	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "8");
	ck_assert_str_eq(fields[1].data.s, "f8");

	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "2");
	ck_assert_str_eq(fields[1].data.s, "6c");

	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "3");
	ck_assert_str_eq(fields[1].data.s, "b0");

	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "1");
	ck_assert_str_eq(fields[1].data.s, "1e");

	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "c");
	ck_assert_str_eq(fields[1].data.s, "c8");

	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "6");
	ck_assert_str_eq(fields[1].data.s, "ae");

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 0);
	ck_assert_int_eq(fql_field_count(fql), 0);
}
END_TEST


START_TEST(test_subquery_nested)
{
	struct fql_field* fields = NULL;
	int plan_count = 0;
	int field_count = 0;
	int rows = 0;

	//44b72d44	70	30279
	//3138b3f8	b0	154715
	plan_count = fql_make_plans(fql, "select shnt from ("
					 "  select baz shnt from ("
				         "    select * from t1 where right(bar, 1) = 0"
					 "  )"
					 ")  ");
	ck_assert_int_eq(plan_count, 1);

	field_count = fql_field_count(fql);
	ck_assert_int_eq(field_count, 1);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 1);
	ck_assert_int_eq(fields[0].type, FQL_STRING);

	ck_assert_str_eq(fields[0].data.s, "30279");
	ck_assert_str_eq(fields[0].data.s, "154715");

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
	tcase_add_test(tc_subquery, test_subquery_read);
	tcase_add_test(tc_subquery, test_subquery_nested);

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


