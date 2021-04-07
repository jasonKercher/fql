#include "check_common.h"

#include <stdlib.h>

struct fql_handle* fql = NULL;

void fql_setup()
{
	fql = fql_new();
}

void fql_teardown()
{
	fql_free(fql);
}

