#include "check_common.h"

/*
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

void _check_select_int(int expected_value)
{
	struct fql_field* fields = NULL;
	int field_count = 0;
	int rows = 0;

	field_count = fql_field_count(fql);
	ck_assert_int_eq(field_count, 1);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(fields[0].type, FQL_INT);
	ck_assert_int_eq(fields[0].is_null, false);
	ck_assert_int_eq(fields[0].data.i, expected_value);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 0);
}

void _check_select_str(const char* expected_value)
{
	struct fql_field* fields = NULL;
	int field_count = 0;
	int rows = 0;

	field_count = fql_field_count(fql);
	ck_assert_int_eq(field_count, 1);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(fields[0].type, FQL_STRING);
	ck_assert_int_eq(fields[0].is_null, false);
	ck_assert_str_eq(fields[0].data.s, expected_value);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 0);
}

void _check_flow(int expected_value)
{
	ck_assert_int_eq(fql_exec_until_select(fql), expected_value);
}

void _check_complete()
{
	ck_assert_int_eq(fql_field_count(fql), 0);
}

START_TEST(test_controlflow_if)
{
	int plan_count = 0;

	/** standard if **/
	plan_count = fql_make_plans(fql, "if 1=1 select -1 else select 0");
	ck_assert_int_eq(plan_count, 3);
	_check_flow(1);
	_check_select_int(-1);
	_check_complete();

	/* take ELSE path */
	plan_count = fql_make_plans(fql, "if 1=0 select -1 else select 0");
	ck_assert_int_eq(plan_count, 3);
	_check_flow(1);
	_check_select_int(0);
	_check_complete();


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
	_check_select_int(0);
	_check_select_int(1);
	_check_select_int(3);

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
	_check_select_int(2);
	_check_select_int(3);
	_check_complete();


	/** nesting **/
	plan_count = fql_make_plans(fql, "\
			if 1=1            \
			    if 2=2        \
			        select 2  \
			select 3         ");
	ck_assert_int_eq(plan_count, 4);
	_check_flow(2);
	_check_select_int(2);
	_check_select_int(3);
	_check_complete();

	/* check false */
	plan_count = fql_make_plans(fql, "\
			if 1=0            \
			    if 2=2        \
			        select 2  \
			select 3         ");
	ck_assert_int_eq(plan_count, 4);
	_check_flow(1);
	_check_select_int(3);
	_check_complete();

	/* check false on second if */
	plan_count = fql_make_plans(fql, "\
			if 1=1            \
			    if 2=0        \
			        select 2  \
			select 3         ");
	ck_assert_int_eq(plan_count, 4);
	_check_flow(2);
	_check_select_int(3);
	_check_complete();


	/** un-related ifs **/
	plan_count = fql_make_plans(fql, "\
			if 1=1            \
			    select 1      \
			if 2=2            \
			    select 2      \
			select 3         ");
	ck_assert_int_eq(plan_count, 5);

	_check_flow(1);
	_check_select_int(1);
	_check_flow(1);
	_check_select_int(2);
	_check_select_int(3);
	_check_complete();


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
	_check_select_int(1);
	_check_flow(1);
	_check_select_int(2);
	_check_select_int(4);
	_check_complete();

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
	_check_select_int(1);
	_check_flow(2);
	_check_select_int(3);
	_check_select_int(4);
	_check_complete();

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
	_check_select_int(1);
	_check_flow(2);
	_check_select_int(0);
	_check_select_int(4);
	_check_complete();

	/* miss all together */
	plan_count = fql_make_plans(fql, "\
			if 0=1            \
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
	_check_select_int(4);
	_check_complete();
}
END_TEST

START_TEST(test_controlflow_variables)
{
	int plan_count = 0;

	/* Initialize to NULL, set to not null */
	plan_count = fql_make_plans(fql, "\
			declare @var int            \
			select 1 where @var is NULL \
			set @var = 10               \
			select @var                ");
	ck_assert_int_eq(plan_count, 4);

	_check_flow(1);
	_check_select_int(1);
	_check_flow(1);
	_check_select_int(10);
	_check_complete();

	/* Basic variable declare + initialize */
	plan_count = fql_make_plans(fql, "\
			declare @var int = -7 \
			select @var          ");
	ck_assert_int_eq(plan_count, 2);

	_check_flow(1);
	_check_select_int(-7);
	_check_complete();

	/* typing */
	plan_count = fql_make_plans(fql, "\
			declare @var varchar = '1'     \
			set @var += '1'                \
			select @var                   ");
	ck_assert_int_eq(plan_count, 3);
	_check_flow(2);
	_check_select_str("11");
	_check_complete();

	/* initialize via subquery */
	plan_count = fql_make_plans(fql, "\
			declare @var varchar = (     \
				select top 1 foo     \
				from t1              \
				order by foo         \
			)                            \
			select @var                 ");
	ck_assert_int_eq(plan_count, 2);
	_check_flow(1);
	_check_select_str("18e25713");
	_check_complete();

	/* Exact same thing but with aggregate */
	plan_count = fql_make_plans(fql, "\
			declare @var varchar = (     \
				select min(foo)      \
				from t1              \
			)                            \
			select @var                 ");
	ck_assert_int_eq(plan_count, 2);
	_check_flow(1);
	_check_select_str("18e25713");
	_check_complete();

	/* Use imported variable */
	fql_import_variable(fql, "var=5");
	plan_count = fql_make_plans(fql, "select @var");
	ck_assert_int_eq(plan_count, 1);
	_check_select_str("5");
	_check_complete();

	/* re-declare imported variable */
	fql_import_variable(fql, "var=5");
	plan_count = fql_make_plans(fql, "declare @var int select @var");
	ck_assert_int_eq(plan_count, 2);
	_check_flow(1);
	_check_select_int(5);
	_check_complete();
}
END_TEST

START_TEST(test_controlflow_while)
{
	int plan_count = 0;
	plan_count = fql_make_plans(fql, "\
			declare @i int = 0 \
			while @i < 10      \
			    set @i += 1    \
			select @i         ");
	ck_assert_int_eq(plan_count, 4);

	_check_flow(22);
	_check_select_int(10);
	_check_complete();

	plan_count = fql_make_plans(fql, "\
			declare @foo varchar = (                 \
			    select top 1 foo                     \
			    from t1                              \
			    order by foo                         \
			)                                        \
			while @foo is not NULL                   \
			BEGIN                                    \
			    select bar from t1 where foo = @foo  \
			    set @foo = (                         \
				select top 1 foo                 \
				from t1                          \
				where foo > @foo                 \
				order by foo                     \
			    )                                    \
			END                                     ");
	ck_assert_int_eq(plan_count, 4);

	_check_flow(2);
	_check_select_str("1e");
	_check_flow(2);
	_check_select_str("6c");
	_check_flow(2);
	_check_select_str("b0");
	_check_flow(2);
	_check_select_str("70");
	_check_flow(2);
	_check_select_str("5f");
	_check_flow(2);
	_check_select_str("ae");
	_check_flow(2);
	_check_select_str("f8");
	_check_flow(2);
	_check_select_str("cb");
	_check_flow(2);
	_check_select_str("c8");
	_check_flow(2);
	_check_complete();
}
END_TEST

Suite* fql_controlflow_suite(void)
{
	Suite* s;
	s = suite_create("controlflow");

	TCase* tc_controlflow = tcase_create("controlflow");
	tcase_add_checked_fixture(tc_controlflow, fql_setup, fql_teardown);

	tcase_add_test(tc_controlflow, test_controlflow_if);
	tcase_add_test(tc_controlflow, test_controlflow_variables);
	tcase_add_test(tc_controlflow, test_controlflow_while);

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


