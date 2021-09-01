#include "check_common.h"
#include "fql.h"
#include <stdio.h>

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

void stdin_setup()
{
	fql_setup();
	fql_set_allow_stdin(fql, true);
	freopen("t1.tsv", "r", stdin);
}

START_TEST(test_stdin_t1)
{
	struct fql_field* fields = NULL;
	int plan_count = 0;
	int field_count = 0;
	int rows = 0;

	/* standard select */
	plan_count = fql_make_plans(fql, "select foo, bar, baz from __STDIN");
	ck_assert_int_eq(plan_count, 1);

	field_count = fql_field_count(fql);
	ck_assert_int_eq(field_count, 3);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 1);
	ck_assert_int_eq(fields[0].type, FQL_STRING);
	ck_assert_int_eq(fields[1].type, FQL_STRING);
	ck_assert_int_eq(fields[2].type, FQL_STRING);

	ck_assert_str_eq(fields[0].data.s, "44b72d44");
	ck_assert_str_eq(fields[1].data.s, "70");
	ck_assert_str_eq(fields[2].data.s, "30279");

	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "b8e63822");
	ck_assert_str_eq(fields[1].data.s, "cb");
	ck_assert_str_eq(fields[2].data.s, "38922");

	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "46a1bad3");
	ck_assert_str_eq(fields[1].data.s, "5f");
	ck_assert_str_eq(fields[2].data.s, "30119");

	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "8ab25d8f");
	ck_assert_str_eq(fields[1].data.s, "f8");
	ck_assert_str_eq(fields[2].data.s, "75882");

	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "282a4957");
	ck_assert_str_eq(fields[1].data.s, "6c");
	ck_assert_str_eq(fields[2].data.s, "121263");

	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "3138b3f8");
	ck_assert_str_eq(fields[1].data.s, "b0");
	ck_assert_str_eq(fields[2].data.s, "154715");

	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "18e25713");
	ck_assert_str_eq(fields[1].data.s, "1e");
	ck_assert_str_eq(fields[2].data.s, "100092");

	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "c1519b0d");
	ck_assert_str_eq(fields[1].data.s, "c8");
	ck_assert_str_eq(fields[2].data.s, "87082");

	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "6ed156a7");
	ck_assert_str_eq(fields[1].data.s, "ae");
	ck_assert_str_eq(fields[2].data.s, "229701");

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 0);
	ck_assert_int_eq(fql_field_count(fql), 0);
}
END_TEST

START_TEST(test_stdin_non_cwd)
{
	struct fql_field* fields = NULL;
	int plan_count = 0;
	int field_count = 0;
	int rows = 0;

	/*  parent directory */
	plan_count = fql_make_plans(fql, "select foo from [../parent]");
	ck_assert_int_eq(plan_count, 1);

	field_count = fql_field_count(fql);
	ck_assert_int_eq(field_count, 1);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 1);
	ck_assert_int_eq(fields[0].type, FQL_STRING);
	ck_assert_str_eq(fields[0].data.s, "parent directory");

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 0);
	ck_assert_int_eq(fql_field_count(fql), 0);


	/* Sub directory */
	plan_count = fql_make_plans(fql, "select foo from [sub/sub]");
	ck_assert_int_eq(plan_count, 1);

	field_count = fql_field_count(fql);
	ck_assert_int_eq(field_count, 1);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 1);
	ck_assert_int_eq(fields[0].type, FQL_STRING);
	ck_assert_str_eq(fields[0].data.s, "sub directory");

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 0);
	ck_assert_int_eq(fql_field_count(fql), 0);
}
END_TEST

START_TEST(test_stdin_t1_asterisk)
{
	struct fql_field* fields = NULL;
	int plan_count = 0;
	int field_count = 0;
	int rows = 0;

	/* standard select */
	plan_count = fql_make_plans(fql, "select * from __STDIN");
	ck_assert_int_eq(plan_count, 1);

	field_count = fql_field_count(fql);
	ck_assert_int_eq(field_count, 3);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 1);
	ck_assert_int_eq(fields[0].type, FQL_STRING);
	ck_assert_int_eq(fields[1].type, FQL_STRING);
	ck_assert_int_eq(fields[2].type, FQL_STRING);

	ck_assert_str_eq(fields[0].data.s, "44b72d44");
	ck_assert_str_eq(fields[1].data.s, "70");
	ck_assert_str_eq(fields[2].data.s, "30279");

	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "b8e63822");
	ck_assert_str_eq(fields[1].data.s, "cb");
	ck_assert_str_eq(fields[2].data.s, "38922");

	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "46a1bad3");
	ck_assert_str_eq(fields[1].data.s, "5f");
	ck_assert_str_eq(fields[2].data.s, "30119");

	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "8ab25d8f");
	ck_assert_str_eq(fields[1].data.s, "f8");
	ck_assert_str_eq(fields[2].data.s, "75882");

	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "282a4957");
	ck_assert_str_eq(fields[1].data.s, "6c");
	ck_assert_str_eq(fields[2].data.s, "121263");

	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "3138b3f8");
	ck_assert_str_eq(fields[1].data.s, "b0");
	ck_assert_str_eq(fields[2].data.s, "154715");

	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "18e25713");
	ck_assert_str_eq(fields[1].data.s, "1e");
	ck_assert_str_eq(fields[2].data.s, "100092");

	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "c1519b0d");
	ck_assert_str_eq(fields[1].data.s, "c8");
	ck_assert_str_eq(fields[2].data.s, "87082");

	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "6ed156a7");
	ck_assert_str_eq(fields[1].data.s, "ae");
	ck_assert_str_eq(fields[2].data.s, "229701");

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 0);
	ck_assert_int_eq(fql_field_count(fql), 0);
}
END_TEST


START_TEST(test_stdin_const)
{
	struct fql_field* fields = NULL;
	int plan_count = 0;
	int field_count = 0;
	int rows = 0;

	/* select constant with stdin */
	plan_count = fql_make_plans(fql, "select 5 from __STDIN");
	ck_assert_int_eq(plan_count, 1);

	field_count = fql_field_count(fql);
	ck_assert_int_eq(field_count, 1);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 1);
	ck_assert_int_eq(fields[0].type, FQL_INT);
	ck_assert_int_eq(fields[0].data.i, 5);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(fields[0].data.i, 5);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(fields[0].data.i, 5);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(fields[0].data.i, 5);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(fields[0].data.i, 5);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(fields[0].data.i, 5);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(fields[0].data.i, 5);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(fields[0].data.i, 5);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(fields[0].data.i, 5);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 0);
	ck_assert_int_eq(fql_field_count(fql), 0);
}
END_TEST

START_TEST(test_stdin_operators)
{
	struct fql_field* fields = NULL;
	int plan_count = 0;
	int field_count = 0;
	int rows = 0;

	/* string operators */
	plan_count = fql_make_plans(fql, "select foo + bar, baz / 10, baz from __STDIN");
	ck_assert_int_eq(plan_count, 1);

	field_count = fql_field_count(fql);
	ck_assert_int_eq(field_count, 3);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 1);
	ck_assert_int_eq(fields[0].type, FQL_STRING);
	ck_assert_int_eq(fields[1].type, FQL_INT);
	ck_assert_int_eq(fields[2].type, FQL_STRING);

	ck_assert_str_eq(fields[0].data.s, "44b72d4470");
	ck_assert_int_eq(fields[1].data.i, 3027);
	ck_assert_str_eq(fields[2].data.s, "30279");

	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "b8e63822cb");
	ck_assert_int_eq(fields[1].data.i, 3892);
	ck_assert_str_eq(fields[2].data.s, "38922");

	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "46a1bad35f");
	ck_assert_int_eq(fields[1].data.i, 3011);
	ck_assert_str_eq(fields[2].data.s, "30119");

	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "8ab25d8ff8");
	ck_assert_int_eq(fields[1].data.i, 7588);
	ck_assert_str_eq(fields[2].data.s, "75882");

	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "282a49576c");
	ck_assert_int_eq(fields[1].data.i, 12126);
	ck_assert_str_eq(fields[2].data.s, "121263");

	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "3138b3f8b0");
	ck_assert_int_eq(fields[1].data.i, 15471);
	ck_assert_str_eq(fields[2].data.s, "154715");

	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "18e257131e");
	ck_assert_int_eq(fields[1].data.i, 10009);
	ck_assert_str_eq(fields[2].data.s, "100092");

	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "c1519b0dc8");
	ck_assert_int_eq(fields[1].data.i, 8708);
	ck_assert_str_eq(fields[2].data.s, "87082");

	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "6ed156a7ae");
	ck_assert_int_eq(fields[1].data.i, 22970);
	ck_assert_str_eq(fields[2].data.s, "229701");

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 0);
	ck_assert_int_eq(fql_field_count(fql), 0);
}
END_TEST

START_TEST(test_stdin_unary)
{
	struct fql_field* fields = NULL;
	int plan_count = 0;
	int field_count = 0;
	int rows = 0;

	/* string operators */
	plan_count = fql_make_plans(fql, "select -(baz+0) from __STDIN");
	ck_assert_int_eq(plan_count, 1);

	field_count = fql_field_count(fql);
	ck_assert_int_eq(field_count, 1);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 1);
	ck_assert_int_eq(fields[0].type, FQL_INT);

	ck_assert_int_eq(fields[0].data.i, -30279);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(fields[0].data.i, -38922);
	rows = fql_step(fql, &fields);
	ck_assert_int_eq(fields[0].data.i, -30119);
	rows = fql_step(fql, &fields);
	ck_assert_int_eq(fields[0].data.i, -75882);
	rows = fql_step(fql, &fields);
	ck_assert_int_eq(fields[0].data.i, -121263);
	rows = fql_step(fql, &fields);
	ck_assert_int_eq(fields[0].data.i, -154715);
	rows = fql_step(fql, &fields);
	ck_assert_int_eq(fields[0].data.i, -100092);
	rows = fql_step(fql, &fields);
	ck_assert_int_eq(fields[0].data.i, -87082);
	rows = fql_step(fql, &fields);
	ck_assert_int_eq(fields[0].data.i, -229701);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 0);
	ck_assert_int_eq(fql_field_count(fql), 0);
}
END_TEST


START_TEST(test_stdin_functions)
{
	struct fql_field* fields = NULL;
	int plan_count = 0;
	int field_count = 0;
	int rows = 0;

	/* standard use combination of left and right
	 * to pull substring (middle 2) from foo
	 */
	plan_count =
	        fql_make_plans(fql, "select foo, left(right(foo, 5), 2) from __STDIN");
	ck_assert_int_eq(plan_count, 1);

	field_count = fql_field_count(fql);
	ck_assert_int_eq(field_count, 2);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 1);
	ck_assert_int_eq(fields[0].type, FQL_STRING);
	ck_assert_int_eq(fields[1].type, FQL_STRING);

	ck_assert_str_eq(fields[0].data.s, "44b72d44");
	ck_assert_str_eq(fields[1].data.s, "72");

	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "b8e63822");
	ck_assert_str_eq(fields[1].data.s, "63");

	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "46a1bad3");
	ck_assert_str_eq(fields[1].data.s, "1b");

	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "8ab25d8f");
	ck_assert_str_eq(fields[1].data.s, "25");

	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "282a4957");
	ck_assert_str_eq(fields[1].data.s, "a4");

	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "3138b3f8");
	ck_assert_str_eq(fields[1].data.s, "8b");

	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "18e25713");
	ck_assert_str_eq(fields[1].data.s, "25");

	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "c1519b0d");
	ck_assert_str_eq(fields[1].data.s, "19");

	rows = fql_step(fql, &fields);
	ck_assert_str_eq(fields[0].data.s, "6ed156a7");
	ck_assert_str_eq(fields[1].data.s, "15");

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 0);
	ck_assert_int_eq(fql_field_count(fql), 0);
}
END_TEST

Suite* fql_stdin_suite(void)
{
	Suite* s;
	s = suite_create("stdin");

	TCase* tc_stdin = tcase_create("stdin");
	tcase_add_checked_fixture(tc_stdin, stdin_setup, fql_teardown);

	tcase_add_test(tc_stdin, test_stdin_t1);
	tcase_add_test(tc_stdin, test_stdin_non_cwd);
	tcase_add_test(tc_stdin, test_stdin_t1_asterisk);
	tcase_add_test(tc_stdin, test_stdin_const);
	tcase_add_test(tc_stdin, test_stdin_operators);
	tcase_add_test(tc_stdin, test_stdin_unary);
	tcase_add_test(tc_stdin, test_stdin_functions);

	suite_add_tcase(s, tc_stdin);

	return s;
}

int main(void)
{
	int number_failed;
	Suite* stdin_suite = fql_stdin_suite();
	SRunner* stdin_runner = srunner_create(stdin_suite);
	srunner_set_fork_status(stdin_runner, CK_NOFORK);

	srunner_run_all(stdin_runner, CK_VERBOSE);
	number_failed = srunner_ntests_failed(stdin_runner);
	srunner_free(stdin_runner);

	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}


