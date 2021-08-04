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

START_TEST(test_operations_delete)
{
	/* delete all */
	fql_set_overwrite(fql, 1);
	ck_assert_int_eq(fql_exec(fql, "select * into [d1.tsv] from t1"), FQL_GOOD);
	ck_assert_int_eq(fql_exec(fql, "delete d1"), FQL_GOOD);
	assert_select_int("select count(*) from d1", 0);
	ck_assert_int_eq(fql_exec(fql, "select * into [d1.tsv] from t1"), FQL_GOOD);
	ck_assert_int_eq(fql_exec(fql, "delete d1 from d1 where 1=1"), FQL_GOOD);
	assert_select_int("select count(*) from d1", 0);
	ck_assert_int_eq(fql_exec(fql, "select * into [d1.tsv] from t1"), FQL_GOOD);
	ck_assert_int_eq(fql_exec(fql, "delete d1 from [d1.tsv] d1"), FQL_GOOD);
	assert_select_int("select count(*) from d1", 0);

	/* Delete with condition */
	ck_assert_int_eq(fql_exec(fql, "select * into [d1.tsv] from t1"), FQL_GOOD);
	ck_assert_int_eq(fql_exec(fql, "delete d1 where bar = 'b0'"), FQL_GOOD);
	assert_select_int("select count(*) from d1", 8);

	/* delete with join */
	ck_assert_int_eq(fql_exec(fql, "select * into [d1.tsv] from t1"), FQL_GOOD);
	ck_assert_int_eq(fql_exec(fql, "delete d1 from d1 join t2 on d1.foo = t2.foo"),
	                 FQL_GOOD);
	assert_select_int("select count(*) from d1", 8);
}
END_TEST

Suite* fql_operations_suite(void)
{
	Suite* s;
	s = suite_create("operations");

	TCase* tc_operations = tcase_create("operations");
	tcase_add_checked_fixture(tc_operations, fql_setup, fql_teardown);

	tcase_add_test(tc_operations, test_operations_delete);

	suite_add_tcase(s, tc_operations);

	return s;
}

int main(void)
{
	int number_failed;
	Suite* operations_suite = fql_operations_suite();
	SRunner* operations_runner = srunner_create(operations_suite);
	srunner_set_fork_status(operations_runner, CK_NOFORK);

	srunner_run_all(operations_runner, CK_VERBOSE);
	number_failed = srunner_ntests_failed(operations_runner);
	srunner_free(operations_runner);

	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}


