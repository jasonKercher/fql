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
	fql_set_overwrite(fql, 1);

	/* delete all */
	ck_assert_int_eq(fql_exec(fql,
	                          "select *       "
	                          "into [d1.tsv]  "
	                          "from t1        "
	                          "               "
	                          "delete d1      "),
	                 FQL_GOOD);
	assert_select_int("select count(*) from d1", 0);

	/* delete with useless from */
	ck_assert_int_eq(fql_exec(fql,
	                          "select *       "
	                          "into [d1.tsv]  "
	                          "from t1        "
	                          "               "
	                          "delete d1      "
	                          "from d1        "
	                          "where 1=1      "),
	                 FQL_GOOD);
	assert_select_int("select count(*) from d1", 0);

	/* Delete via alias */
	ck_assert_int_eq(fql_exec(fql,
	                          "select *         "
	                          "into [d1.tsv]    "
	                          "from t1          "
	                          "                 "
	                          "delete d1        "
	                          "from [d1.tsv] d1 "),
	                 FQL_GOOD);
	assert_select_int("select count(*) from d1", 0);

	/* Delete despite alias */
	ck_assert_int_eq(fql_exec(fql,
	                          "select *         "
	                          "into [d1.tsv]    "
	                          "from t1          "
	                          "                 "
	                          "delete [d1.tsv]  "
	                          "from [d1.tsv] d1 "),
	                 FQL_GOOD);
	assert_select_int("select count(*) from d1", 0);

	/* delete with condition */
	ck_assert_int_eq(fql_exec(fql,
	                          "select *         "
	                          "into [d1.tsv]    "
	                          "from t1          "
	                          "                 "
	                          "delete d1        "
	                          "where bar = 'b0' "),
	                 FQL_GOOD);
	assert_select_int("select count(*) from d1", 8);

	/* delete with join */
	ck_assert_int_eq(fql_exec(fql,
	                          "select *             "
	                          "into [d1.tsv]        "
	                          "from t1              "
	                          "                     "
	                          "delete d1            "
	                          "from d1              "
	                          "join t2              "
	                          "  on d1.foo = t2.foo "),
	                 FQL_GOOD);
	assert_select_int("select count(*) from d1", 8);
}
END_TEST

START_TEST(test_operations_update)
{
	fql_set_overwrite(fql, 1);

	/* update all */
	ck_assert_int_eq(fql_exec(fql,
	                          "select *       "
	                          "into [d1.tsv]  "
	                          "from t1        "
	                          "               "
	                          "update d1      "
	                          "set bar = 'xx' "),
	                 FQL_GOOD);
	assert_select_int("select count(*) from d1 where bar != 'xx'", 0);

	/* update useless from */
	ck_assert_int_eq(fql_exec(fql,
	                          "select *       "
	                          "into [d1.tsv]  "
	                          "from t1        "
	                          "               "
	                          "update d1      "
	                          "set bar = 'xx' "
	                          "from d1        "
	                          "where 1=1      "),
	                 FQL_GOOD);
	assert_select_int("select count(*) from d1 where bar != 'xx'", 0);

	/* update via alias */
	ck_assert_int_eq(fql_exec(fql,
	                          "select *         "
	                          "into [d1.tsv]    "
	                          "from t1          "
	                          "                 "
	                          "update d1        "
	                          "set bar = 'xx'   "
	                          "from [d1.tsv] d1 "),
	                 FQL_GOOD);
	assert_select_int("select count(*) from d1 where bar != 'xx'", 0);

	/* update THROUGH aliased source */
	ck_assert_int_eq(fql_exec(fql,
	                          "select *         "
	                          "into [d1.tsv]    "
	                          "from t1          "
	                          "                 "
	                          "update [d1.tsv]  "
	                          "set bar = 'xx'   "
	                          "from [d1.tsv] d1 "),
	                 FQL_GOOD);
	assert_select_int("select count(*) from d1 where bar != 'xx'", 0);

	/* update with condition */
	ck_assert_int_eq(fql_exec(fql,
	                          "select *         "
	                          "into [d1.tsv]    "
	                          "from t1          "
	                          "                 "
	                          "update d1        "
	                          "set bar = 'xx'   "
	                          "where bar = 'b0' "),
	                 FQL_GOOD);
	assert_select_int("select count(*) from d1 where bar != 'xx'", 8);

	/* update with join */
	ck_assert_int_eq(fql_exec(fql,
	                          "select *             "
	                          "into [d1.tsv]        "
	                          "from t1              "
	                          "                     "
	                          "update d1            "
	                          "set bar = 'xx'       "
	                          "from d1              "
	                          "join t2              "
	                          "  on d1.foo = t2.foo "),
	                 FQL_GOOD);
	assert_select_int("select count(*) from d1 where bar != 'xx'", 8);
}
END_TEST

START_TEST(test_operations_schema_change)
{
	struct fql_field* fields = NULL;
	int plan_count = 0;
	int field_count = 0;
	int rows = 0;

	fql_set_overwrite(fql, 1);

	/* Testing that we do not error finding extra. Also, since we are
	 * planning the queries together, the second query, will see extra
	 * as an int. So we do not need to cast it for sum to work.
	 */
	plan_count = fql_make_plans(fql,
	                            "select *, 5 extra "
	                            "into [d1.tsv]     "
	                            "from t1           "
	                            "                  "
	                            "select sum(extra) "
	                            "from d1           ");
	ck_assert_int_eq(plan_count, 2);

	/* Run the first query (SELECT INTO) */
	ck_assert_int_ne(fql_exec_plans(fql, 1), FQL_FAIL);

	field_count = fql_field_count(fql);
	ck_assert_int_eq(field_count, 1);

	/* Now we want to step through the second query to
	 * retrieve the value returned by SUM.
	 */
	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 1);
	ck_assert_int_eq(fields[0].type, FQL_INT);
	ck_assert_int_eq(fields[0].data.i, 45);

	rows = fql_step(fql, &fields);
	ck_assert_int_eq(rows, 0);
}
END_TEST


Suite* fql_operations_suite(void)
{
	Suite* s;
	s = suite_create("operations");

	TCase* tc_operations = tcase_create("operations");
	tcase_add_checked_fixture(tc_operations, fql_setup, fql_teardown);

	tcase_add_test(tc_operations, test_operations_delete);
	tcase_add_test(tc_operations, test_operations_update);
	tcase_add_test(tc_operations, test_operations_schema_change);

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


