#include "fql.h"
#include "util/util.h"
#include "util/stack.h"
#include "util/queue.h"
#include "antlr/antlr.h"

#include "plan.h"
#include "prop.h"

void fql_init()
{
        prop_init();
}

/**
 * Property mutators
 */

void fql_set_verbose(int verbose)
{
        g_props.verbose = verbose;
}

void fql_set_dry_run(int dry_run)
{
        g_props.dry_run = dry_run;
        if (dry_run) {
                g_props.verbose = dry_run;
        }
}

void fql_set_override_warnings(int override)
{
        g_props.override_warnings = override;
}

void fql_set_in_delim(const char* delim)
{
        strncpy_(g_props.in_delim, delim, 32);
}

void fql_set_out_delim(const char* delim)
{
        strncpy_(g_props.out_delim, delim, 32);
}


/**
 * Methods
 */

int fql_exec(const char* query_str)
{
        plan_t plan = { NULL };

        analyze_query(&plan, query_str);

        return 0;
}
