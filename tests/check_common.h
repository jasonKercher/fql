#ifndef CHECK_COMMON_H
#define CHECK_COMMON_H

#define EPSILON .001

#include <stdlib.h>
#include <check.h>
#include "../lib/include/fql.h"

extern struct fqlhandle* fql;

void fql_setup();
void fql_teardown();

void assert_select_string(const char*, const char* EXPECTED_VALUE);
void assert_select_int(const char*, long EXPECTED_VALUE);
void assert_select_float(const char*, double EXPECTED_VALUE);
void assert_nothing(const char*);
#endif  /* CHECK_COMMON_H */
