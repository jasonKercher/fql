#include "check_common.h"
//#include "util/hashmap.h"
#include "util/hashmap.h"

typedef HashMap HashMap;

HashMap* m = NULL;

int one = 1;
int two = 2;
int ten = 10;
int twelve = 12;
int test = 100;
int Test = 101;
int test_ = 102;

void sets()
{
	hashmap_set(m, "one", &ten);
	hashmap_set(m, "two", &two);
	hashmap_set(m, "one", &one);
	hashmap_set(m, "ten", &ten);
	hashmap_set(m, "twelve", &twelve);
	hashmap_set(m, "test", &test);
	hashmap_set(m, "Test", &Test);
	hashmap_set(m, "test ", &test_);
}

START_TEST(test_hashmap_basic)
{
	m = hashmap_new_(int, test, 0);
	ck_assert_ptr_nonnull(m);

	sets();

	int val = 0;
	val = *(int*) hashmap_get(m, "one");
	ck_assert_uint_eq(val, one);
	val = *(int*) hashmap_get(m, "two");
	ck_assert_uint_eq(val, two);
	val = *(int*) hashmap_get(m, "ten");
	ck_assert_uint_eq(val, ten);
	val = *(int*) hashmap_get(m, "twelve");
	ck_assert_uint_eq(val, twelve);
	val = *(int*) hashmap_get(m, "test");
	ck_assert_uint_eq(val, test);
	val = *(int*) hashmap_get(m, "Test");
	ck_assert_uint_eq(val, Test);
	val = *(int*) hashmap_get(m, "test ");
	ck_assert_uint_eq(val, test_);
	void* no_match = hashmap_get(m, "no");
	ck_assert_ptr_null(no_match);

	hashmap_free(m);
}
END_TEST

START_TEST(test_hashmap_nocase)
{
	m = hashmap_new_(int, test, HASHMAP_PROP_NOCASE);
	ck_assert_ptr_nonnull(m);

	sets();

	int val = 0;
	val = *(int*) hashmap_get(m, "one");
	ck_assert_uint_eq(val, one);
	val = *(int*) hashmap_get(m, "two");
	ck_assert_uint_eq(val, two);
	val = *(int*) hashmap_get(m, "ten");
	ck_assert_uint_eq(val, ten);
	val = *(int*) hashmap_get(m, "twelve");
	ck_assert_uint_eq(val, twelve);
	val = *(int*) hashmap_get(m, "test");
	ck_assert_uint_eq(val, Test);
	val = *(int*) hashmap_get(m, "Test");
	ck_assert_uint_eq(val, Test);
	val = *(int*) hashmap_get(m, "test ");
	ck_assert_uint_eq(val, test_);
	void* no_match = hashmap_get(m, "no");
	ck_assert_ptr_null(no_match);

	hashmap_free(m);
}
END_TEST

START_TEST(test_hashmap_rtrim)
{
	m = hashmap_new_(int, test, HASHMAP_PROP_RTRIM);
	ck_assert_ptr_nonnull(m);

	sets();

	int val = 0;
	val = *(int*) hashmap_get(m, "one");
	ck_assert_uint_eq(val, one);
	val = *(int*) hashmap_get(m, "two");
	ck_assert_uint_eq(val, two);
	val = *(int*) hashmap_get(m, "ten");
	ck_assert_uint_eq(val, ten);
	val = *(int*) hashmap_get(m, "twelve");
	ck_assert_uint_eq(val, twelve);
	val = *(int*) hashmap_get(m, "test");
	ck_assert_uint_eq(val, test_);
	val = *(int*) hashmap_get(m, "Test");
	ck_assert_uint_eq(val, Test);
	val = *(int*) hashmap_get(m, "test ");
	ck_assert_uint_eq(val, test_);
	void* no_match = hashmap_get(m, "no");
	ck_assert_ptr_null(no_match);

	hashmap_free(m);
}
END_TEST

START_TEST(test_hashmap_nocase_rtrim)
{
	m = hashmap_new_(int, test, HASHMAP_PROP_NOCASE | HASHMAP_PROP_RTRIM);
	ck_assert_ptr_nonnull(m);

	sets();

	int val = 0;
	val = *(int*) hashmap_get(m, "one");
	ck_assert_uint_eq(val, one);
	val = *(int*) hashmap_get(m, "two");
	ck_assert_uint_eq(val, two);
	val = *(int*) hashmap_get(m, "ten");
	ck_assert_uint_eq(val, ten);
	val = *(int*) hashmap_get(m, "twelve");
	ck_assert_uint_eq(val, twelve);
	val = *(int*) hashmap_get(m, "test");
	ck_assert_uint_eq(val, test_);
	val = *(int*) hashmap_get(m, "Test");
	ck_assert_uint_eq(val, test_);
	val = *(int*) hashmap_get(m, "test ");
	ck_assert_uint_eq(val, test_);
	void* no_match = hashmap_get(m, "no");
	ck_assert_ptr_null(no_match);

	hashmap_free(m);
}
END_TEST

void multimap_sets()
{
	multimap_set(m, "one", &ten);
	multimap_set(m, "two", &two);
	multimap_set(m, "one", &one);
	multimap_set(m, "ten", &ten);
	multimap_set(m, "twelve", &twelve);
	multimap_set(m, "test", &test);
	multimap_set(m, "Test", &Test);
	multimap_set(m, "test ", &test_);
}

START_TEST(test_multimap_basic)
{
	m = multimap_new_(int, test, 0);
	ck_assert_ptr_nonnull(m);

	multimap_sets();

	int val = 0;
	Vec* vec = NULL;

	vec = multimap_get(m, "one");
	ck_assert_uint_eq(vec->size, 2);
	val = *(int*) vec_at(vec, 0);
	ck_assert_uint_eq(val, ten);
	val = *(int*) vec_at(vec, 1);
	ck_assert_uint_eq(val, one);

	vec = multimap_get(m, "two");
	ck_assert_uint_eq(vec->size, 1);
	val = *(int*) vec_at(vec, 0);
	ck_assert_uint_eq(val, two);

	vec = multimap_get(m, "ten");
	ck_assert_uint_eq(vec->size, 1);
	val = *(int*) vec_at(vec, 0);
	ck_assert_uint_eq(val, ten);

	vec = multimap_get(m, "twelve");
	ck_assert_uint_eq(vec->size, 1);
	val = *(int*) vec_at(vec, 0);
	ck_assert_uint_eq(val, twelve);

	vec = multimap_get(m, "test");
	ck_assert_uint_eq(vec->size, 1);
	val = *(int*) vec_at(vec, 0);
	ck_assert_uint_eq(val, test);

	vec = multimap_get(m, "Test");
	ck_assert_uint_eq(vec->size, 1);
	val = *(int*) vec_at(vec, 0);
	ck_assert_uint_eq(val, Test);

	vec = multimap_get(m, "test ");
	ck_assert_uint_eq(vec->size, 1);
	val = *(int*) vec_at(vec, 0);
	ck_assert_uint_eq(val, test_);

	void* no_match = multimap_get(m, "no");
	ck_assert_ptr_null(no_match);

	multimap_free(m);
}
END_TEST

START_TEST(test_multimap_nocase)
{
	m = multimap_new_(int, test, HASHMAP_PROP_NOCASE);
	ck_assert_ptr_nonnull(m);

	multimap_sets();

	int val = 0;
	Vec* vec = NULL;

	vec = multimap_get(m, "one");
	ck_assert_uint_eq(vec->size, 2);
	val = *(int*) vec_at(vec, 0);
	ck_assert_uint_eq(val, ten);
	val = *(int*) vec_at(vec, 1);
	ck_assert_uint_eq(val, one);

	vec = multimap_get(m, "two");
	ck_assert_uint_eq(vec->size, 1);
	val = *(int*) vec_at(vec, 0);
	ck_assert_uint_eq(val, two);

	vec = multimap_get(m, "ten");
	ck_assert_uint_eq(vec->size, 1);
	val = *(int*) vec_at(vec, 0);
	ck_assert_uint_eq(val, ten);

	vec = multimap_get(m, "twelve");
	ck_assert_uint_eq(vec->size, 1);
	val = *(int*) vec_at(vec, 0);
	ck_assert_uint_eq(val, twelve);

	vec = multimap_get(m, "test");
	ck_assert_uint_eq(vec->size, 2);
	val = *(int*) vec_at(vec, 0);
	ck_assert_uint_eq(val, test);
	val = *(int*) vec_at(vec, 1);
	ck_assert_uint_eq(val, Test);

	vec = multimap_get(m, "Test");
	ck_assert_uint_eq(vec->size, 2);
	val = *(int*) vec_at(vec, 0);
	ck_assert_uint_eq(val, test);
	val = *(int*) vec_at(vec, 1);
	ck_assert_uint_eq(val, Test);

	vec = multimap_get(m, "test ");
	ck_assert_uint_eq(vec->size, 1);
	val = *(int*) vec_at(vec, 0);
	ck_assert_uint_eq(val, test_);

	void* no_match = multimap_get(m, "no");
	ck_assert_ptr_null(no_match);

	multimap_free(m);
}
END_TEST

START_TEST(test_multimap_rtrim)
{
	m = multimap_new_(int, test, HASHMAP_PROP_RTRIM);
	ck_assert_ptr_nonnull(m);

	multimap_sets();

	int val = 0;
	Vec* vec = NULL;

	vec = multimap_get(m, "one");
	ck_assert_uint_eq(vec->size, 2);
	val = *(int*) vec_at(vec, 0);
	ck_assert_uint_eq(val, ten);
	val = *(int*) vec_at(vec, 1);
	ck_assert_uint_eq(val, one);

	vec = multimap_get(m, "two");
	ck_assert_uint_eq(vec->size, 1);
	val = *(int*) vec_at(vec, 0);
	ck_assert_uint_eq(val, two);

	vec = multimap_get(m, "ten");
	ck_assert_uint_eq(vec->size, 1);
	val = *(int*) vec_at(vec, 0);
	ck_assert_uint_eq(val, ten);

	vec = multimap_get(m, "twelve");
	ck_assert_uint_eq(vec->size, 1);
	val = *(int*) vec_at(vec, 0);
	ck_assert_uint_eq(val, twelve);

	vec = multimap_get(m, "test");
	ck_assert_uint_eq(vec->size, 2);
	val = *(int*) vec_at(vec, 0);
	ck_assert_uint_eq(val, test);
	val = *(int*) vec_at(vec, 1);
	ck_assert_uint_eq(val, test_);

	vec = multimap_get(m, "Test");
	ck_assert_uint_eq(vec->size, 1);
	val = *(int*) vec_at(vec, 0);
	ck_assert_uint_eq(val, Test);

	vec = multimap_get(m, "test ");
	ck_assert_uint_eq(vec->size, 2);
	val = *(int*) vec_at(vec, 0);
	ck_assert_uint_eq(val, test);
	val = *(int*) vec_at(vec, 1);
	ck_assert_uint_eq(val, test_);

	void* no_match = multimap_get(m, "no");
	ck_assert_ptr_null(no_match);

	multimap_free(m);
}
END_TEST

START_TEST(test_multimap_nocase_rtrim)
{
	m = multimap_new_(int, test, HASHMAP_PROP_NOCASE | HASHMAP_PROP_RTRIM);
	ck_assert_ptr_nonnull(m);

	multimap_sets();

	int val = 0;
	Vec* vec = NULL;

	vec = multimap_get(m, "one");
	ck_assert_uint_eq(vec->size, 2);
	val = *(int*) vec_at(vec, 0);
	ck_assert_uint_eq(val, ten);
	val = *(int*) vec_at(vec, 1);
	ck_assert_uint_eq(val, one);

	vec = multimap_get(m, "two");
	ck_assert_uint_eq(vec->size, 1);
	val = *(int*) vec_at(vec, 0);
	ck_assert_uint_eq(val, two);

	vec = multimap_get(m, "ten");
	ck_assert_uint_eq(vec->size, 1);
	val = *(int*) vec_at(vec, 0);
	ck_assert_uint_eq(val, ten);

	vec = multimap_get(m, "twelve");
	ck_assert_uint_eq(vec->size, 1);
	val = *(int*) vec_at(vec, 0);
	ck_assert_uint_eq(val, twelve);

	vec = multimap_get(m, "test");
	ck_assert_uint_eq(vec->size, 3);
	val = *(int*) vec_at(vec, 0);
	ck_assert_uint_eq(val, test);
	val = *(int*) vec_at(vec, 1);
	ck_assert_uint_eq(val, Test);
	val = *(int*) vec_at(vec, 2);
	ck_assert_uint_eq(val, test_);

	vec = multimap_get(m, "Test");
	ck_assert_uint_eq(vec->size, 3);
	val = *(int*) vec_at(vec, 0);
	ck_assert_uint_eq(val, test);
	val = *(int*) vec_at(vec, 1);
	ck_assert_uint_eq(val, Test);
	val = *(int*) vec_at(vec, 2);
	ck_assert_uint_eq(val, test_);

	vec = multimap_get(m, "test ");
	ck_assert_uint_eq(vec->size, 3);
	val = *(int*) vec_at(vec, 0);
	ck_assert_uint_eq(val, test);
	val = *(int*) vec_at(vec, 1);
	ck_assert_uint_eq(val, Test);
	val = *(int*) vec_at(vec, 2);
	ck_assert_uint_eq(val, test_);

	void* no_match = multimap_get(m, "no");
	ck_assert_ptr_null(no_match);

	multimap_free(m);
}
END_TEST

Suite* fql_hashmap_suite(void)
{
	Suite* s;
	s = suite_create("hashmap");

	TCase* tc_hashmap = tcase_create("hashmap");
	tcase_add_test(tc_hashmap, test_hashmap_basic);
	tcase_add_test(tc_hashmap, test_hashmap_nocase);
	tcase_add_test(tc_hashmap, test_hashmap_rtrim);
	tcase_add_test(tc_hashmap, test_hashmap_nocase_rtrim);
	suite_add_tcase(s, tc_hashmap);

	TCase* tc_multimap = tcase_create("multimap");
	tcase_add_test(tc_multimap, test_multimap_basic);
	tcase_add_test(tc_multimap, test_multimap_nocase);
	tcase_add_test(tc_multimap, test_multimap_rtrim);
	tcase_add_test(tc_multimap, test_multimap_nocase_rtrim);
	suite_add_tcase(s, tc_multimap);

	return s;
}

int main(void)
{
	int number_failed;
	Suite* hashmap_suite = fql_hashmap_suite();
	SRunner* hashmap_runner = srunner_create(hashmap_suite);
	srunner_set_fork_status (hashmap_runner, CK_NOFORK);

	srunner_run_all(hashmap_runner, CK_VERBOSE);
	number_failed = srunner_ntests_failed(hashmap_runner);
	srunner_free(hashmap_runner);

	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}


