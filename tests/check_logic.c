#include "check.h"
#include "check_common.h"

// #include <limits.h>

/* t2.tsv
foo,bar,baz
3ff8b279,84,30279
4fd746c3,fe,38922
32061b07,9a,30119
ff5487af,f4,75882
77903b4d,14,121263
90d8d854,23,154715
9a18b797,ed,100092
585339b5,54,87082
f99b9313,22,229701
*/

void _assert_select1(int plan_count)
{
	struct fql_field* fields = NULL;
	int field_count = 0;
	int rows = 0;

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

void _assert_nothing(int plan_count)
{
	struct fql_field* fields = NULL;
	int field_count = 0;
	int rows = 0;

	ck_assert_int_eq(plan_count, 1);

	field_count = fql_field_count(fql);
	ck_assert_int_eq(field_count, 1);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 0);
	ck_assert_int_eq(fql_field_count(fql), 0);
}

START_TEST(test_logic_const)
{
	// fql_logic_eq_i
	_assert_select1(fql_make_plans(fql, "select 1 where '01' = 1"));
	_assert_nothing(fql_make_plans(fql, "select 1 where -1 = 1"));

	// fql_logic_eq_f
	_assert_select1(fql_make_plans(fql, "select 1 where 50.5 = '050.5'"));
	_assert_nothing(fql_make_plans(fql, "select 1 where 50.4 = 50.5"));

	// fql_logic_eq_s
	_assert_select1(fql_make_plans(fql, "select 1 where 'hi' = 'Hi   '"));
	_assert_nothing(fql_make_plans(fql, "select 1 where 'hi' = ' hi'"));

	// fql_logic_ne_i
	_assert_select1(fql_make_plans(fql, "select 1 where 1 != 0"));
	_assert_nothing(fql_make_plans(
	        fql,
	        "select 1 where 9223372036854775807 <> 9223372036854775807"));

	// fql_logic_ne_f
	_assert_select1(fql_make_plans(fql, "select 1 where -.0001 != -.00011"));
	_assert_nothing(fql_make_plans(fql, "select 1 where 33.333 <> 33.333"));

	// fql_logic_ne_s
	_assert_select1(fql_make_plans(fql, "select 1 where 'abcde f' != 'abcde'"));
	_assert_nothing(fql_make_plans(fql, "select 1 where '' <> ''"));

	// fql_logic_gt_i
	_assert_select1(fql_make_plans(fql, "select 1 where 5 > -6"));
	_assert_nothing(fql_make_plans(
	        fql,
	        "select 1 where -9223372036854775808 > 9223372036854775807"));

	// fql_logic_gt_f
	_assert_select1(fql_make_plans(fql, "select 1 where 1.1 > 1"));
	_assert_nothing(fql_make_plans(fql, "select 1 where -1.1 > 1"));

	// fql_logic_gt_s
	_assert_select1(fql_make_plans(fql, "select 1 where 'Bad' > 'abc'"));
	_assert_nothing(
	        fql_make_plans(fql,
	                       "select 1 where 'A' > '{'")); /* THIS BREAKS FROM T-SQL */

	// fql_logic_ge_i
	_assert_select1(fql_make_plans(fql, "select 1 where 5 >= 5"));
	_assert_select1(fql_make_plans(fql, "select 1 where 5 >= '4'"));
	_assert_nothing(fql_make_plans(fql, "select 1 where -1 >= 1"));

	// fql_logic_ge_f
	_assert_select1(fql_make_plans(fql, "select 1 where -22.1 >= -31"));
	_assert_select1(fql_make_plans(fql, "select 1 where 22.1 >= 22.1"));
	_assert_nothing(fql_make_plans(fql, "select 1 where -22.10001 >= -22.1"));

	// fql_logic_ge_s
	_assert_select1(fql_make_plans(fql, "select 1 where 'abc' >= '[c]   '"));
	_assert_select1(fql_make_plans(fql, "select 1 where 'abc' >= 'abc   '"));
	_assert_nothing(fql_make_plans(fql, "select 1 where 'abc' >= 'abc d'"));

	// fql_logic_lt_i
	_assert_select1(fql_make_plans(fql, "select 1 where -1 < 1"));
	_assert_nothing(fql_make_plans(fql, "select 1 where '1' < -1"));

	// fql_logic_lt_f
	_assert_select1(fql_make_plans(fql, "select 1 where 5.992 < 5.993"));
	_assert_nothing(fql_make_plans(fql, "select 1 where -11 < -12.1"));

	// fql_logic_lt_s
	_assert_select1(fql_make_plans(fql, "select 1 where 'shnt' < 'TNHS'"));
	_assert_nothing(fql_make_plans(fql, "select 1 where 'shnt' < 'SHNT'"));

	// fql_logic_le_i
	_assert_select1(fql_make_plans(fql, "select 1 where 1 <= 1"));
	_assert_select1(fql_make_plans(fql, "select 1 where -12341 <= 1"));
	_assert_nothing(fql_make_plans(fql, "select 1 where 0 <= -1"));

	// fql_logic_le_f
	_assert_select1(fql_make_plans(fql, "select 1 where -888.1234 <= 888"));
	_assert_select1(fql_make_plans(fql, "select 1 where 888.0 <= 888"));
	_assert_nothing(fql_make_plans(fql, "select 1 where 888.1234 <= 888"));

	// fql_logic_le_s
	_assert_select1(
	        fql_make_plans(fql, "select 1 where 'hello world!' <= 'hello world!'"));
	_assert_select1(
	        fql_make_plans(fql, "select 1 where 'hello world' <= 'hello world!'"));
	_assert_nothing(
	        fql_make_plans(fql, "select 1 where 'hello_world!' <= 'hello world!'"));
}
END_TEST

START_TEST(test_logic_like)
{
	_assert_select1(fql_make_plans(fql, "select 1 where 'sh.nt' like '%.%'"));
	_assert_nothing(fql_make_plans(fql, "select 1 where 'sh.nt' not like '%.%'"));
}

START_TEST(test_logic_in)
{
	_assert_select1(fql_make_plans(fql, "select 1 where 's' in ('qcn', 's')"));
	_assert_select1(fql_make_plans(fql, "select 1 where 1 in (1, 78)"));
	_assert_nothing(fql_make_plans(fql, "select 1 where '' in (' n', '565')"));
	_assert_select1(fql_make_plans(fql, "select 1 where '' in (' n', '      ')"));
}

START_TEST(test_logic_function)
{
	_assert_select1(
	        fql_make_plans(fql, "select 1 where left('hello', 2) = 'he    '"));
	_assert_select1(
	        fql_make_plans(fql, "select 1 where 1000 + 233 < right('hi1234', 4)"));
	_assert_nothing(fql_make_plans(fql, "select 1 where 100 * -1 > 0 / 1000"));
}

START_TEST(test_logic_complex)
{
	_assert_select1(fql_make_plans(fql, "select 1 where 1=1 and 1=1"));
	_assert_select1(fql_make_plans(fql, "select 1 where 1=1 or 1=0"));
	_assert_nothing(fql_make_plans(fql, "select 1 where 1=1 and 1=0"));
	_assert_nothing(fql_make_plans(fql, "select 1 where 1=0 or 1=1 and 1=0"));
	_assert_select1(fql_make_plans(fql, "select 1 where 1=1 or 1=1 and 1=0"));
	_assert_nothing(fql_make_plans(fql, "select 1 where (1=1 or 1=1) and 1=0"));
	_assert_select1(fql_make_plans(fql, "select 1 where 1=0 and 1=1 or 1=1"));
	_assert_nothing(
	        fql_make_plans(fql, "select 1 where (1=1 and 1=0) or (1=1 and 1=0)"));
	_assert_select1(
	        fql_make_plans(fql, "select 1 where (1=1 and 1=0) or (1=1 and 1=1)"));
}

Suite* fql_logic_suite(void)
{
	Suite* s;
	s = suite_create("logic");

	TCase* tc_logic = tcase_create("logic");
	tcase_add_checked_fixture(tc_logic, fql_setup, fql_teardown);

	tcase_add_test(tc_logic, test_logic_const);
	tcase_add_test(tc_logic, test_logic_in);
	tcase_add_test(tc_logic, test_logic_like);
	tcase_add_test(tc_logic, test_logic_function);
	tcase_add_test(tc_logic, test_logic_complex);

	suite_add_tcase(s, tc_logic);

	return s;
}

int main(void)
{
	int number_failed;
	Suite* logic_suite = fql_logic_suite();
	SRunner* logic_runner = srunner_create(logic_suite);
	srunner_set_fork_status (logic_runner, CK_NOFORK);

	srunner_run_all(logic_runner, CK_VERBOSE);
	number_failed = srunner_ntests_failed(logic_runner);
	srunner_free(logic_runner);

	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}


