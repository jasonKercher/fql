#ifndef CHECK_COMMON_H
#define CHECK_COMMON_H

#define EPSILON .001

#include <stdlib.h>
#include "../lib/include/fql.h"

extern struct fql_handle* fql;

void fql_setup();
void fql_teardown();

#endif  /* CHECK_COMMON_H */
