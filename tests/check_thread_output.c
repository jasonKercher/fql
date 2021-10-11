#include "output.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

void thread_output_setup(void)
{
	fql_setup();
	fql_set_threading(fql, true);
	fql_set_overwrite(fql, true);
	//fql_set_verbose(fql, FQL_DEBUG);

	/* default for threading is 64. By setting
	 * to a lower number, we increase the chance
	 * of a race condition. 2 is min...
	 */
	fql_set_pipe_factor(fql, 2);
}

START_TEST(test_output_const)
{
	output_const(fql);
}
END_TEST

START_TEST(test_output_read)
{
	output_read(fql);
}
END_TEST

START_TEST(test_output_logic)
{
	output_logic(fql);
}
END_TEST

START_TEST(test_output_scalar)
{
	output_scalar(fql);
}
END_TEST

START_TEST(test_output_switch)
{
	output_switch(fql);
}
END_TEST

START_TEST(test_output_null)
{
	output_null(fql);
}
END_TEST

START_TEST(test_output_join)
{
	output_join(fql);
}
END_TEST

START_TEST(test_output_subquery)
{
	output_subquery(fql);
}
END_TEST

START_TEST(test_output_group)
{
	output_group(fql);
}
END_TEST

START_TEST(test_output_having)
{
	output_having(fql);
}
END_TEST

START_TEST(test_output_order)
{
	output_order(fql);
}
END_TEST

START_TEST(test_output_top)
{
	output_top(fql);
}
END_TEST

START_TEST(test_output_union)
{
	output_union(fql);
}
END_TEST

Suite* fql_thread_output_suite(void)
{
	Suite* s;
	s = suite_create("thread");

	TCase* tc_thread_output = tcase_create("thread");
	tcase_add_checked_fixture(tc_thread_output, thread_output_setup, fql_teardown);

	tcase_add_test(tc_thread_output, test_output_const);
	tcase_add_test(tc_thread_output, test_output_read);
	tcase_add_test(tc_thread_output, test_output_logic);
	tcase_add_test(tc_thread_output, test_output_scalar);
	tcase_add_test(tc_thread_output, test_output_switch);
	tcase_add_test(tc_thread_output, test_output_null);
	tcase_add_test(tc_thread_output, test_output_join);
	tcase_add_test(tc_thread_output, test_output_subquery);
	tcase_add_test(tc_thread_output, test_output_group);
	tcase_add_test(tc_thread_output, test_output_having);
	tcase_add_test(tc_thread_output, test_output_order);
	tcase_add_test(tc_thread_output, test_output_top);
	tcase_add_test(tc_thread_output, test_output_union);

	suite_add_tcase(s, tc_thread_output);

	return s;
}

int main(void)
{

	mkdir("thread_results", 0775);

	int number_failed;
	Suite* thread_output_suite = fql_thread_output_suite();
	SRunner* thread_output_runner = srunner_create(thread_output_suite);
	srunner_set_fork_status(thread_output_runner, CK_NOFORK);

	srunner_run_all(thread_output_runner, CK_VERBOSE);
	number_failed = srunner_ntests_failed(thread_output_runner);
	srunner_free(thread_output_runner);

	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

