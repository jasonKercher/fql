#include "prop.h"
#include "util/util.h"

void prop_init()
{
        Props g_props = {
                 ","    /* in_delim */
                ,","    /* out_delim */
                ,false  /* verbose */
                ,false  /* dry_run */
                ,false  /* override_warnings */
        };
}
