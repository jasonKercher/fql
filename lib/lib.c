#include "fql.h"
#include "util/util.h"
#include "util/stack.h"
#include "util/queue.h"
#include "antlr/antlr.h"

#include "plan.h"
#include "prop.h"

#include "expression.h"
#include "table.h"
#include "query.h"
#include "schema.h"

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
        queue_t* query_list = NULL;
        analyze_query(&query_list, query_str);

        schema_resolve(query_list);

        queue_t* plans = NULL;
        build_plans(&plans, query_list);

        queue_free_func(&query_list, &query_free);
        queue_free_func(&plans, &plan_free);

        return 0;
}


