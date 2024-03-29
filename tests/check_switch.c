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


START_TEST(test_switch_const)
{
	assert_select_int("select case 0     "
	                  "    when 1 then 2 "
	                  "    when 0 then 3 "
	                  "    end           ",
	                  3);
	assert_select_int("select case         "
	                  "    when 0=1 then 2 "
	                  "    when 0=0 then 3 "
	                  "    end             ",
	                  3);
	assert_select_int("select case           "
	                  "    when 1 = 2 then 0 "
	                  "    when 2 = 3 then 1 "
	                  "    else 8 end        ",
	                  8);
}
END_TEST

START_TEST(test_switch_in_logic)
{
	assert_nothing("select 1 where 2 = case when 1=1 then 1 else 0 end");
	assert_select_int("select 1 where 2 = case when 1=1 then 2 else -1 end", 1);
}
END_TEST

/* Case statements use the same precedence as logic and implicit casts
 * to determine type. These rules are applied across all possible
 * resulting expressions.
 */
START_TEST(test_switch_type_resolution)
{
	assert_select_int("select case when 'x' = 'y' then 1 else '2' end", 2);
	assert_select_int("select case when 'x' = 'x' then '2' else 1 end", 2);

	assert_select_string("select case when 1=1 then '2' else '3' end", "2");
	assert_select_int("select case 'a'          "
	                  "    when 'b' then '5'    "
	                  "    when 'x' then 2      "
	                  "    when 'a' then '1' end",
	                  1);
}
END_TEST

Suite* fql_switch_suite(void)
{
	Suite* s;
	s = suite_create("switch");

	TCase* tc_switch = tcase_create("switch");
	tcase_add_checked_fixture(tc_switch, fql_setup, fql_teardown);

	tcase_add_test(tc_switch, test_switch_const);
	tcase_add_test(tc_switch, test_switch_in_logic);
	tcase_add_test(tc_switch, test_switch_type_resolution);

	suite_add_tcase(s, tc_switch);

	return s;
}

int main(void)
{
	int number_failed;
	Suite* switch_suite = fql_switch_suite();
	SRunner* switch_runner = srunner_create(switch_suite);
	srunner_set_fork_status(switch_runner, CK_NOFORK);

	srunner_run_all(switch_runner, CK_VERBOSE);
	number_failed = srunner_ntests_failed(switch_runner);
	srunner_free(switch_runner);

	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}


