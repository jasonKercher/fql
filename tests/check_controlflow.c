#include "check_common.h"

START_TEST(test_controlflow_if_nesting) { }
END_TEST

Suite* fql_controlflow_suite(void)
{
	Suite* s;
	s = suite_create("controlflow");

	TCase* tc_controlflow = tcase_create("controlflow");
	tcase_add_checked_fixture(tc_controlflow, fql_setup, fql_teardown);

	tcase_add_test(tc_controlflow, test_controlflow_if_nesting);

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


