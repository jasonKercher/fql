#include "check.h"
#include "check_common.h"

/* Testing individual scalar functions. All behavior was
 * modeled after the behavior observed in SQL Server.
 *
 * For copy paste purposes, the Unicode chess set:
 * ♜♞♝♛♚♝♞♜♟ ♖♘♗♕♔♗♘♖♙
 * All of these are 3 bytes in UTF-8.
 * 
 * Here is captial beta for a 2-byte character:
 * ß
 */

START_TEST(test_scalar_len)
{
	/* simple string length */
	assert_select_int("select len('shnt')", 4);
	/* string length ignore trailing white space */
	assert_select_int("select len('shnt   ')", 4);
	/* UTF-8 recognition */
	assert_select_int("select len('♜♞ ')", 2);
	/* implicity convert to '100' */
	assert_select_int("select len(100)", 3);

	fql_set_char_as_byte(fql, true);

	/* count bytes ignoring trailing whitespace */
	assert_select_int("select len('♜♞ ')", 6);
}

START_TEST(test_scalar_datalength)
{
	/* string length */
	assert_select_int("select datalength('shnt')", 4);
	/* string length including trailing whitespace */
	assert_select_int("select datalength('shnt   ')", 7);
	/* byte count regardless of characters */
	assert_select_int("select datalength('♜♞ ')", 7);
	/* byte count of int type */
	assert_select_int("select datalength(100)", sizeof(long));
	/* byte count of float type */
	assert_select_int("select datalength(100.0)", sizeof(double));
}

START_TEST(test_scalar_left)
{
	/* basic behavior */
	assert_select_string("select left('shnt', 3)", "shn");
	/* read passed end */
	assert_select_string("select left('shnt  ', 12)", "shnt  ");
	/* UTF-8 recognition */
	assert_select_string("select left('♗♕♔♗♘', 3)", "♗♕♔");

	fql_set_char_as_byte(fql, true);

	/* Pull first X bytes */
	assert_select_string("select left('♗♕♔♗♘', 3)", "♗");
}

START_TEST(test_scalar_right)
{
	/* basic behavior including white space */
	assert_select_string("select right('shnt ', 3)", "nt ");
	/* read passed beginning */
	assert_select_string("select right('shnt  ', 12)", "shnt  ");
	/* UTF-8 recognition */
	assert_select_string("select right('♗♕♔♗♘', 3)", "♔♗♘");

	fql_set_char_as_byte(fql, true);

	/* Pull last X bytes */
	assert_select_string("select right('♗♕♔♗♘', 3)", "♘");
}

START_TEST(test_scalar_substring)
{
	/* basic behavior including white space */
	assert_select_string("select substring('shnt ', 3, 1)", "n");
	/* read passed beginning */
	assert_select_string("select substring('shnt  ', 12, 20)", "");
	/* read passed end */
	assert_select_string("select substring('shnt  ', 3, 20)", "nt  ");
	/* Negative location */
	assert_select_string("select substring('shnt', -1, 3)", "s");
	/* UTF-8 recognition */
	assert_select_string("select substring('♗♕♔♗♘', 3, 2)", "♔♗");

	fql_set_char_as_byte(fql, true);

	/* Pull Y bytes starting at byte X */
	assert_select_string("select substring('♗♕♔♗♘', 4, 3)", "♕");
}

START_TEST(test_scalar_replace)
{
	/* replace one match */
	assert_select_string("select replace('shnt', 'nt', 'xx')", "shxx");
	/* replace nothing */
	assert_select_string("select replace('shnt', 'xx', 'nt')", "shnt");
	/* replace multiple */
	assert_select_string("select replace('Haters gonna hate', 'hate', 'love')",
	                     "lovers gonna love");
	/* replace increase size */
	assert_select_string("select replace('Hello World', 'world', 'Entire Planet!')",
	                     "Hello Entire Planet!");
	/* replace decrease size */
	assert_select_string("select replace('Hello Entire Planet!', 'planet', 'Room')",
	                     "Hello Entire Room!");
}

START_TEST(test_scalar_cast)
{
	/* basic behavior */
	assert_select_int("select cast(1234 as bit)", 1);
	/* ONLY 0 returns 0 */
	assert_select_int("select cast('0' as bit)", 0);
	/* Everything else, returns 1 */
	assert_select_int("select cast(-1 as bit)", 1);

	/* no-op */
	assert_select_int("select cast(55 as int)", 55);
	/* basic int cast */
	assert_select_int("select cast('1234' as int)", 1234);
	/* slightly harder int cast */
	assert_select_int("select cast('-000001234' as int)", -1234);
	/* hex to int - NOTE: this is fql specific behavior */
	assert_select_int("select cast('0x7F' as int)", 127);
	/* truncate float */
	assert_select_int("select cast(5.0001 as int)", 5);

	/* no-op */
	assert_select_float("select cast(55.0 as float)", 55);
	/* basic float cast */
	assert_select_float("select cast('1234' as float)", 1234);
	/* slightly harder float cast */
	assert_select_float("select cast('-000001234' as float)", -1234);
	/* hex to float - NOTE: this is fql specific behavior */
	assert_select_float("select cast('6.12366' as float)", 6.12366);

	/* basic conversion */
	assert_select_string("select cast(55 as varchar)", "55");
	/* truncation */
	assert_select_string("select cast('shnt' as varchar(3))", "shn");

	/* basic conversion */
	assert_select_string("select cast(55 as char)", "55");
	/* truncation */
	assert_select_string("select cast('shnt' as char(3))", "shn");
	/* cast char passed end of string */
	assert_select_string("select cast('shnt' as char(10))", "shnt      ");
}

Suite* fql_scalar_suite(void)
{
	Suite* s;
	s = suite_create("scalar");

	TCase* tc_scalar = tcase_create("scalar");
	tcase_add_checked_fixture(tc_scalar, fql_setup, fql_teardown);

	tcase_add_test(tc_scalar, test_scalar_len);
	tcase_add_test(tc_scalar, test_scalar_datalength);
	tcase_add_test(tc_scalar, test_scalar_left);
	tcase_add_test(tc_scalar, test_scalar_right);
	tcase_add_test(tc_scalar, test_scalar_substring);
	tcase_add_test(tc_scalar, test_scalar_replace);
	tcase_add_test(tc_scalar, test_scalar_cast);

	suite_add_tcase(s, tc_scalar);

	return s;
}

int main(void)
{
	int number_failed;
	Suite* scalar_suite = fql_scalar_suite();
	SRunner* scalar_runner = srunner_create(scalar_suite);
	srunner_set_fork_status(scalar_runner, CK_NOFORK);

	srunner_run_all(scalar_runner, CK_VERBOSE);
	number_failed = srunner_ntests_failed(scalar_runner);
	srunner_free(scalar_runner);

	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}


