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

void assert_select_1(int plan_count)
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

void assert_nothing(int plan_count)
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
        assert_select_1(fql_make_plans(fql, "select 1 where '01' = 1"));
        assert_nothing( fql_make_plans(fql, "select 1 where -1 = 1"));

        // fql_logic_eq_f
        assert_select_1(fql_make_plans(fql, "select 1 where 50.5 = '050.5'"));
        assert_nothing( fql_make_plans(fql, "select 1 where 50.4 = 50.5"));

        // fql_logic_eq_s
        assert_select_1(fql_make_plans(fql, "select 1 where 'hi' = 'Hi   '"));
        assert_nothing( fql_make_plans(fql, "select 1 where 'hi' = ' hi'"));

        // fql_logic_ne_i
        assert_select_1(fql_make_plans(fql, "select 1 where 1 != 0"));
        assert_nothing( fql_make_plans(fql, "select 1 where 9223372036854775807 <> 9223372036854775807"));

        // fql_logic_ne_f
        assert_select_1(fql_make_plans(fql, "select 1 where -.0001 != -.00011"));
        assert_nothing( fql_make_plans(fql, "select 1 where 33.333 <> 33.333"));

        // fql_logic_ne_s
        assert_select_1(fql_make_plans(fql, "select 1 where 'abcde f' != 'abcde'"));
        assert_nothing( fql_make_plans(fql, "select 1 where '' <> ''"));

        // fql_logic_gt_i
        assert_select_1(fql_make_plans(fql, "select 1 where 5 > -6"));
        assert_nothing( fql_make_plans(fql, "select 1 where -9223372036854775808 > 9223372036854775807"));

        // fql_logic_gt_f
        assert_select_1(fql_make_plans(fql, "select 1 where 1.1 > 1"));
        assert_nothing( fql_make_plans(fql, "select 1 where -1.1 > 1"));

        // fql_logic_gt_s
        assert_select_1(fql_make_plans(fql, "select 1 where 'Bad' > 'abc'"));
        assert_nothing( fql_make_plans(fql, "select 1 where 'A' > '{'")); /* THIS BREAKS FROM T-SQL */

        // fql_logic_ge_i
        assert_select_1(fql_make_plans(fql, "select 1 where 5 >= 5"));
        assert_select_1(fql_make_plans(fql, "select 1 where 5 >= '4'"));
        assert_nothing( fql_make_plans(fql, "select 1 where -1 >= 1"));

        // fql_logic_ge_f
        assert_select_1(fql_make_plans(fql, "select 1 where -22.1 >= -31"));
        assert_select_1(fql_make_plans(fql, "select 1 where 22.1 >= 22.1"));
        assert_nothing( fql_make_plans(fql, "select 1 where -22.10001 >= -22.1"));

        // fql_logic_ge_s
        assert_select_1(fql_make_plans(fql, "select 1 where 'abc' >= '[c]   '"));
        assert_select_1(fql_make_plans(fql, "select 1 where 'abc' >= 'abc   '"));
        assert_nothing( fql_make_plans(fql, "select 1 where 'abc' >= 'abc d'"));

        // fql_logic_lt_i
        assert_select_1(fql_make_plans(fql, "select 1 where -1 < 1"));
        assert_nothing( fql_make_plans(fql, "select 1 where '1' < -1"));

        // fql_logic_lt_f
        assert_select_1(fql_make_plans(fql, "select 1 where 5.992 < 5.993"));
        assert_nothing( fql_make_plans(fql, "select 1 where -11 < -12.1"));

        // fql_logic_lt_s
        assert_select_1(fql_make_plans(fql, "select 1 where 'shnt' < 'TNHS'"));
        assert_nothing( fql_make_plans(fql, "select 1 where 'shnt' < 'SHNT'"));

        // fql_logic_le_i
        assert_select_1(fql_make_plans(fql, "select 1 where 1 <= 1"));
        assert_select_1(fql_make_plans(fql, "select 1 where -12341 <= 1"));
        assert_nothing( fql_make_plans(fql, "select 1 where 0 <= -1"));

        // fql_logic_le_f
        assert_select_1(fql_make_plans(fql, "select 1 where -888.1234 <= 888"));
        assert_select_1(fql_make_plans(fql, "select 1 where 888.0 <= 888"));
        assert_nothing( fql_make_plans(fql, "select 1 where 888.1234 <= 888"));

        // fql_logic_le_s
        assert_select_1(fql_make_plans(fql, "select 1 where 'hello world!' <= 'hello world!'"));
        assert_select_1(fql_make_plans(fql, "select 1 where 'hello world' <= 'hello world!'"));
        assert_nothing( fql_make_plans(fql, "select 1 where 'hello_world!' <= 'hello world!'"));
}
END_TEST


Suite* fql_read_suite(void)
{
        Suite* s;
        s = suite_create("read");

        TCase* tc_read = tcase_create("read");
        tcase_add_checked_fixture(tc_read, fql_setup, fql_teardown);

        tcase_add_test(tc_read, test_logic_const);

        suite_add_tcase(s, tc_read);

        return s;
}

int main(void)
{
        int number_failed;
        Suite* read_suite = fql_read_suite();
        SRunner* read_runner = srunner_create(read_suite);
        srunner_set_fork_status (read_runner, CK_NOFORK);

        srunner_run_all(read_runner, CK_VERBOSE);
        number_failed = srunner_ntests_failed(read_runner);
        srunner_free(read_runner);

        return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}


