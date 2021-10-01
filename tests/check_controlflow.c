#include "check_common.h"

void _check_flow(int expected_value)
{
	ck_assert_int_eq(fql_exec_until_select(fql), expected_value);
}

void _check_select(int expected_value)
{
	struct fql_field* fields = NULL;
	int field_count = 0;
	int rows = 0;

	field_count = fql_field_count(fql);
	ck_assert_int_eq(field_count, 1);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(fields[0].data.i, expected_value);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 0);
}

START_TEST(test_controlflow_if)
{
	int plan_count = 0;

	/** standard if **/
	plan_count = fql_make_plans(fql, "if 1=1 select -1 else select 0");
	ck_assert_int_eq(plan_count, 3);
	_check_flow(1);
	_check_select(-1);
	ck_assert_int_eq(fql_field_count(fql), 0);

	/* take ELSE path */
	plan_count = fql_make_plans(fql, "if 1=0 select -1 else select 0");
	ck_assert_int_eq(plan_count, 3);
	_check_flow(1);
	_check_select(0);
	ck_assert_int_eq(fql_field_count(fql), 0);


	/** If statement with block **/
	plan_count = fql_make_plans(fql, "\
			if 1=1            \
			BEGIN             \
			    select 0      \
			    select 1      \
			END               \
			else              \
			    select 2      \
			select 3         ");
	ck_assert_int_eq(plan_count, 5);
	_check_flow(1);
	_check_select(0);
	_check_select(1);
	_check_select(3);
	ck_assert_int_eq(fql_field_count(fql), 0);

	/* check ELSE */
	plan_count = fql_make_plans(fql, "\
			if 1=0            \
			BEGIN             \
			    select 0      \
			    select 1      \
			END               \
			else              \
			    select 2      \
			select 3         ");
	ck_assert_int_eq(plan_count, 5);
	_check_flow(1);
	_check_select(2);
	_check_select(3);
	ck_assert_int_eq(fql_field_count(fql), 0);


	/** nesting **/
	plan_count = fql_make_plans(fql, "\
			if 1=1            \
			    if 2=2        \
			        select 2  \
			select 3         ");
	ck_assert_int_eq(plan_count, 4);
	_check_flow(2);
	_check_select(2);
	_check_select(3);
	ck_assert_int_eq(fql_field_count(fql), 0);

	/* check false */
	plan_count = fql_make_plans(fql, "\
			if 1=0            \
			    if 2=2        \
			        select 2  \
			select 3         ");
	ck_assert_int_eq(plan_count, 4);
	_check_flow(1);
	_check_select(3);
	ck_assert_int_eq(fql_field_count(fql), 0);

	/* check false on second if */
	plan_count = fql_make_plans(fql, "\
			if 1=1            \
			    if 2=0        \
			        select 2  \
			select 3         ");
	ck_assert_int_eq(plan_count, 4);
	_check_flow(2);
	_check_select(3);
	ck_assert_int_eq(fql_field_count(fql), 0);


	/** un-related ifs **/
	plan_count = fql_make_plans(fql, "\
			if 1=1            \
			    select 1      \
			if 2=2            \
			    select 2      \
			select 3         ");
	ck_assert_int_eq(plan_count, 5);

	_check_flow(1);
	_check_select(1);
	_check_flow(1);
	_check_select(2);
	_check_select(3);
	ck_assert_int_eq(fql_field_count(fql), 0);


	/** nested else if in block **/
	plan_count = fql_make_plans(fql, "\
			if 1=1            \
			BEGIN             \
			    select 1      \
			    if 2=2        \
			        select 2  \
			    else if 3=3   \
			        select 3  \
			    else          \
			        select 0  \
			END               \
			select 4         ");
	ck_assert_int_eq(plan_count, 8);

	_check_flow(1);
	_check_select(1);
	_check_flow(1);
	_check_select(2);
	_check_select(4);
	ck_assert_int_eq(fql_field_count(fql), 0);

	/* hit 2nd branch... */
	plan_count = fql_make_plans(fql, "\
			if 1=1            \
			BEGIN             \
			    select 1      \
			    if -2=2       \
			        select 2  \
			    else if 3=3   \
			        select 3  \
			    else          \
			        select 0  \
			END               \
			select 4         ");
	ck_assert_int_eq(plan_count, 8);

	_check_flow(1);
	_check_select(1);
	_check_flow(2);
	_check_select(3);
	_check_select(4);
	ck_assert_int_eq(fql_field_count(fql), 0);

	/* hit else block */
	plan_count = fql_make_plans(fql, "\
			if 1=1            \
			BEGIN             \
			    select 1      \
			    if -2=2       \
			        select 2  \
			    else if 3=0   \
			        select 3  \
			    else          \
			        select 0  \
			END               \
			select 4         ");
	ck_assert_int_eq(plan_count, 8);

	_check_flow(1);
	_check_select(1);
	_check_flow(2);
	_check_select(0);
	_check_select(4);
	ck_assert_int_eq(fql_field_count(fql), 0);
}
END_TEST

Suite* fql_controlflow_suite(void)
{
	Suite* s;
	s = suite_create("controlflow");

	TCase* tc_controlflow = tcase_create("controlflow");
	tcase_add_checked_fixture(tc_controlflow, fql_setup, fql_teardown);

	tcase_add_test(tc_controlflow, test_controlflow_if);

	suite_add_tcase(s, tc_controlflow);

	return s;
}

int main(void)
{
	int number_failed;
	Suite* controlflow_suite = fql_controlflow_suite();
	SRunner* controlflow_runner = srunner_create(controlflow_suite);
	srunner_set_fork_status(controlflow_runner, CK_NOFORK);

	srunner_run_all(controlflow_runner, CK_VERBOSE);
	number_failed = srunner_ntests_failed(controlflow_runner);
	srunner_free(controlflow_runner);

	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}


