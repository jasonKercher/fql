#include "prop.h"
#include "util/util.h"

void prop_init()
{
        props_t g_props = {
                 ","    /* in_delim */
                ,","    /* out_delim */
                ,FALSE  /* verbose */
                ,FALSE  /* dry_run */
                ,FALSE  /* override_warnings */
        };
}
