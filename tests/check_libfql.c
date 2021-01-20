/** HEY OH **/

#define EPSILON .001

#include <stdlib.h>
#include <check.h>
#include "../lib/include/fql.h"

struct fql_handle* fql = NULL;

void fql_setup()
{
        fql = fql_new();
}

void fql_teardown()
{
        fql_free(fql);
}

START_TEST(test_fql_const)
{
        ck_assert_ptr_nonnull(fql);

        struct fql_field* fields = NULL;
        int plan_count = 0;
        int field_count = 0;
        int rows = 0;


        /* constant int: select 1 */
        plan_count = fql_make_plans(fql, "select 1");
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


        /* constant float: select 1.1 */
        plan_count = fql_make_plans(fql, "select 1.1");
        ck_assert_int_eq(plan_count, 1);

        field_count = fql_field_count(fql);
        ck_assert_int_eq(field_count, 1);

        rows = fql_step(fql, &fields);
        ck_assert_int_eq(rows, 1);
        ck_assert_int_eq(fields[0].type, FQL_FLOAT);
        ck_assert_double_eq(fields[0].data.f, 1.1);

        rows = fql_step(fql, &fields);
        ck_assert_int_eq(rows, 0);
        ck_assert_int_eq(fql_field_count(fql), 0);


        /* constant string: select 'x' */
        plan_count = fql_make_plans(fql, "select 'x'");
        ck_assert_int_eq(plan_count, 1);

        field_count = fql_field_count(fql);
        ck_assert_int_eq(field_count, 1);

        rows = fql_step(fql, &fields);
        ck_assert_int_eq(rows, 1);
        ck_assert_int_eq(fields[0].type, FQL_STRING);
        ck_assert_str_eq(fields[0].data.s, "x");

        rows = fql_step(fql, &fields);
        ck_assert_int_eq(rows, 0);
        ck_assert_int_eq(fql_field_count(fql), 0);


        /* integer only maths */
        plan_count = fql_make_plans(fql, "select 3 / 2");
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


        /* const int to float promotion: select 1.1 + 1 */
        plan_count = fql_make_plans(fql, "select 1.1 + 1");
        ck_assert_int_eq(plan_count, 1);

        field_count = fql_field_count(fql);
        ck_assert_int_eq(field_count, 1);

        rows = fql_step(fql, &fields);
        ck_assert_int_eq(rows, 1);
        ck_assert_int_eq(fields[0].type, FQL_FLOAT);
        ck_assert_double_eq(fields[0].data.f, 2.1);

        rows = fql_step(fql, &fields);
        ck_assert_int_eq(rows, 0);
        ck_assert_int_eq(fql_field_count(fql), 0);


        /* some floating point maths: select 1 * 2.0 / (3 + 4.0) */
        plan_count = fql_make_plans(fql, "select 1 * 2.0 / (3 + 4.0)");
        ck_assert_int_eq(plan_count, 1);

        field_count = fql_field_count(fql);
        ck_assert_int_eq(field_count, 1);

        rows = fql_step(fql, &fields);
        ck_assert_int_eq(rows, 1);
        ck_assert_int_eq(fields[0].type, FQL_FLOAT);
        ck_assert_double_eq_tol(fields[0].data.f, 0.285714, EPSILON);

        rows = fql_step(fql, &fields);
        ck_assert_int_eq(rows, 0);
        ck_assert_int_eq(fql_field_count(fql), 0);


        /* some maths including unary operators: select 123 * -2.0 / (-3 + 4.1) */
        plan_count = fql_make_plans(fql, "select 123 * -2.0 / (-3 + 4.1)");
        ck_assert_int_eq(plan_count, 1);

        field_count = fql_field_count(fql);
        ck_assert_int_eq(field_count, 1);

        rows = fql_step(fql, &fields);
        ck_assert_int_eq(rows, 1);
        ck_assert_int_eq(fields[0].type, FQL_FLOAT);
        ck_assert_double_eq_tol(fields[0].data.f, -223.636263, EPSILON);

        rows = fql_step(fql, &fields);
        ck_assert_int_eq(rows, 0);
        ck_assert_int_eq(fql_field_count(fql), 0);


        /* string to integer maths */
        plan_count = fql_make_plans(fql, "select '13' / 5");
        ck_assert_int_eq(plan_count, 1);

        field_count = fql_field_count(fql);
        ck_assert_int_eq(field_count, 1);

        rows = fql_step(fql, &fields);
        ck_assert_int_eq(rows, 1);
        ck_assert_int_eq(fields[0].type, FQL_INT);
        ck_assert_int_eq(fields[0].data.i, 2);

        rows = fql_step(fql, &fields);
        ck_assert_int_eq(rows, 0);
        ck_assert_int_eq(fql_field_count(fql), 0);


        /* string to float maths */
        plan_count = fql_make_plans(fql, "select '1' / 3.0");
        ck_assert_int_eq(plan_count, 1);

        field_count = fql_field_count(fql);
        ck_assert_int_eq(field_count, 1);

        rows = fql_step(fql, &fields);
        ck_assert_int_eq(rows, 1);
        ck_assert_int_eq(fields[0].type, FQL_FLOAT);
        ck_assert_double_eq_tol(fields[0].data.f, 0.333333, EPSILON);

        rows = fql_step(fql, &fields);
        ck_assert_int_eq(rows, 0);
        ck_assert_int_eq(fql_field_count(fql), 0);


        /* string concatenation via operator */
        plan_count = fql_make_plans(fql, "select '4' + '5'");
        ck_assert_int_eq(plan_count, 1);

        field_count = fql_field_count(fql);
        ck_assert_int_eq(field_count, 1);

        rows = fql_step(fql, &fields);
        ck_assert_int_eq(rows, 1);
        ck_assert_int_eq(fields[0].type, FQL_STRING);
        ck_assert_str_eq(fields[0].data.s, "45");

        rows = fql_step(fql, &fields);
        ck_assert_int_eq(rows, 0);
        ck_assert_int_eq(fql_field_count(fql), 0);


        /* string concatenation followed by integer math */
        plan_count = fql_make_plans(fql, "select '1' + '9' + 5");
        ck_assert_int_eq(plan_count, 1);

        field_count = fql_field_count(fql);
        ck_assert_int_eq(field_count, 1);

        rows = fql_step(fql, &fields);
        ck_assert_int_eq(rows, 1);
        ck_assert_int_eq(fields[0].type, FQL_INT);
        ck_assert_int_eq(fields[0].data.i, 24);

        rows = fql_step(fql, &fields);
        ck_assert_int_eq(rows, 0);
        ck_assert_int_eq(fql_field_count(fql), 0);

}
END_TEST

Suite* fql_constants_suite(void)
{
        Suite* s;
        s = suite_create("constants");

        TCase* tc_constants = tcase_create("constants");
        tcase_add_checked_fixture(tc_constants, fql_setup, fql_teardown);

        tcase_add_test(tc_constants, test_fql_const);

        suite_add_tcase(s, tc_constants);

        return s;
}
int main(void)
{
        int number_failed;
        Suite* s = fql_constants_suite();
        SRunner* sr = srunner_create(s);
        srunner_set_fork_status (sr, CK_NOFORK);

        srunner_run_all(sr, CK_VERBOSE);
        number_failed = srunner_ntests_failed(sr);
        srunner_free(sr);

        return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

