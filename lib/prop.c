#include "prop.h"
#include "util/util.h"

void prop_init()
{
        struct props g_props = {
                 ","    /* in_delim */
                ,","    /* out_delim */
                ,FALSE  /* verbose */
                ,FALSE  /* dry_run */
                ,FALSE  /* override_warnings */
        };
}
