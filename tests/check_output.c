#include "output.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

void output_setup(void)
{
	fql_setup();
	fql_set_overwrite(fql, true);
}

START_TEST(test_output_const)
{
	output_const(fql, "results");
}
END_TEST

START_TEST(test_output_read)
{
	output_read(fql, "results");
}
END_TEST

START_TEST(test_output_logic)
{
	output_logic(fql, "results");
}
END_TEST

START_TEST(test_output_scalar)
{
	output_scalar(fql, "results");
}
END_TEST

START_TEST(test_output_switch)
{
	output_switch(fql, "results");
}
END_TEST

Suite* fql_output_suite(void)
{
	Suite* s;
	s = suite_create("output");

	TCase* tc_output = tcase_create("output");
	tcase_add_checked_fixture(tc_output, output_setup, fql_teardown);

	tcase_add_test(tc_output, test_output_const);
	tcase_add_test(tc_output, test_output_read);
	tcase_add_test(tc_output, test_output_logic);
	tcase_add_test(tc_output, test_output_scalar);
	tcase_add_test(tc_output, test_output_switch);

	suite_add_tcase(s, tc_output);

	return s;
}

int main(void)
{

	mkdir("results", 0775);

	int number_failed;
	Suite* output_suite = fql_output_suite();
	SRunner* output_runner = srunner_create(output_suite);
	srunner_set_fork_status(output_runner, CK_NOFORK);

	srunner_run_all(output_runner, CK_VERBOSE);
	number_failed = srunner_ntests_failed(output_runner);
	srunner_free(output_runner);

	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}


