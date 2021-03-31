#include "check_common.h"
#include "util/hmap.h"

Hmap* m = NULL;

void sets()
{
        hmap_set(m, "one", (void*) 10);
        hmap_set(m, "two", (void*) 2);
        hmap_set(m, "one", (void*) 1);
        hmap_set(m, "ten", (void*) 10);
        hmap_set(m, "twelve", (void*) 12);
        hmap_set(m, "test", (void*) 100);
        hmap_set(m, "Test", (void*) 101);
        hmap_set(m, "test ", (void*) 102);
}

START_TEST(test_hmap_basic)
{
        m = hmap_new(100, 0);
        ck_assert_ptr_nonnull(m);

        sets();

        unsigned long val = 0;
        val = (unsigned long) hmap_get(m, "one");
        ck_assert_uint_eq(val, 1);
        val = (unsigned long) hmap_get(m, "two");
        ck_assert_uint_eq(val, 2);
        val = (unsigned long) hmap_get(m, "ten");
        ck_assert_uint_eq(val, 10);
        val = (unsigned long) hmap_get(m, "twelve");
        ck_assert_uint_eq(val, 12);
        val = (unsigned long) hmap_get(m, "test");
        ck_assert_uint_eq(val, 100);
        val = (unsigned long) hmap_get(m, "Test");
        ck_assert_uint_eq(val, 101);
        val = (unsigned long) hmap_get(m, "test ");
        ck_assert_uint_eq(val, 102);
        void* no_match = hmap_get(m, "no");
        ck_assert_ptr_null(no_match);

        hmap_free(m);
}
END_TEST

START_TEST(test_hmap_nocase)
{
        m = hmap_new(100, HMAP_NOCASE);
        ck_assert_ptr_nonnull(m);

        sets();

        unsigned long val = 0;
        val = (unsigned long) hmap_get(m, "one");
        ck_assert_uint_eq(val, 1);
        val = (unsigned long) hmap_get(m, "two");
        ck_assert_uint_eq(val, 2);
        val = (unsigned long) hmap_get(m, "ten");
        ck_assert_uint_eq(val, 10);
        val = (unsigned long) hmap_get(m, "twelve");
        ck_assert_uint_eq(val, 12);
        val = (unsigned long) hmap_get(m, "test");
        ck_assert_uint_eq(val, 101);
        val = (unsigned long) hmap_get(m, "Test");
        ck_assert_uint_eq(val, 101);
        val = (unsigned long) hmap_get(m, "test ");
        ck_assert_uint_eq(val, 102);
        void* no_match = hmap_get(m, "no");
        ck_assert_ptr_null(no_match);

        hmap_free(m);
}
END_TEST

START_TEST(test_hmap_rtrim)
{
        m = hmap_new(100, HMAP_RTRIM);
        ck_assert_ptr_nonnull(m);

        sets();

        unsigned long val = 0;
        val = (unsigned long) hmap_get(m, "one");
        ck_assert_uint_eq(val, 1);
        val = (unsigned long) hmap_get(m, "two");
        ck_assert_uint_eq(val, 2);
        val = (unsigned long) hmap_get(m, "ten");
        ck_assert_uint_eq(val, 10);
        val = (unsigned long) hmap_get(m, "twelve");
        ck_assert_uint_eq(val, 12);
        val = (unsigned long) hmap_get(m, "test");
        ck_assert_uint_eq(val, 102);
        val = (unsigned long) hmap_get(m, "Test");
        ck_assert_uint_eq(val, 101);
        val = (unsigned long) hmap_get(m, "test ");
        ck_assert_uint_eq(val, 102);
        void* no_match = hmap_get(m, "no");
        ck_assert_ptr_null(no_match);

        hmap_free(m);
}
END_TEST

START_TEST(test_hmap_nocase_rtrim)
{
        m = hmap_new(100, HMAP_NOCASE | HMAP_RTRIM);
        ck_assert_ptr_nonnull(m);

        sets();

        unsigned long val = 0;
        val = (unsigned long) hmap_get(m, "one");
        ck_assert_uint_eq(val, 1);
        val = (unsigned long) hmap_get(m, "two");
        ck_assert_uint_eq(val, 2);
        val = (unsigned long) hmap_get(m, "ten");
        ck_assert_uint_eq(val, 10);
        val = (unsigned long) hmap_get(m, "twelve");
        ck_assert_uint_eq(val, 12);
        val = (unsigned long) hmap_get(m, "test");
        ck_assert_uint_eq(val, 102);
        val = (unsigned long) hmap_get(m, "Test");
        ck_assert_uint_eq(val, 102);
        val = (unsigned long) hmap_get(m, "test ");
        ck_assert_uint_eq(val, 102);
        void* no_match = hmap_get(m, "no");
        ck_assert_ptr_null(no_match);

        hmap_free(m);
}
END_TEST


Suite* fql_hmap_suite(void)
{
        Suite* s;
        s = suite_create("hmap");

        TCase* tc_hmap = tcase_create("hmap");
        tcase_add_test(tc_hmap, test_hmap_basic);
        tcase_add_test(tc_hmap, test_hmap_nocase);
        tcase_add_test(tc_hmap, test_hmap_rtrim);
        tcase_add_test(tc_hmap, test_hmap_nocase_rtrim);
        suite_add_tcase(s, tc_hmap);

        return s;
}

int main(void)
{
        int number_failed;
        Suite* hmap_suite = fql_hmap_suite();
        SRunner* hmap_runner = srunner_create(hmap_suite);
        srunner_set_fork_status (hmap_runner, CK_NOFORK);

        srunner_run_all(hmap_runner, CK_VERBOSE);
        number_failed = srunner_ntests_failed(hmap_runner);
        srunner_free(hmap_runner);

        return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}


