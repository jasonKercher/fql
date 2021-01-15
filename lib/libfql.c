#include "fql.h"
#include "util/util.h"
#include "util/stack.h"
#include "util/queue.h"
#include "antlr/antlr.h"

#include "plan.h"

#include "table.h"
#include "query.h"
#include "schema.h"

struct fql_handle* fql_new()
{
        struct fql_handle* new_handle;
        malloc_(new_handle, sizeof(*new_handle));

        return fql_init(new_handle);
}

struct fql_handle* fql_init(struct fql_handle* fql)
{
        *fql = (struct fql_handle) {
                 NULL   /* query list */
                ,NULL   /* query_str */
                ,{
                         ","    /* in_delim */
                        ,","    /* out_delim */
                        ,false  /* verbose */
                        ,false  /* dry_run */
                        ,false  /* override_warnings */
                        ,false  /* print_plan */
                }
        };
        return fql;
}

void fql_free(struct fql_handle* fql)
{
        free_(fql);
}

/**
 * Property mutators
 */

void fql_set_verbose(struct fql_handle* fql, int verbose)
{
        fql->props.verbose = verbose;
}

void fql_set_dry_run(struct fql_handle* fql, int dry_run)
{
        fql->props.dry_run = dry_run;
}

void fql_set_override_warnings(struct fql_handle* fql, int override)
{
        fql->props.override_warnings = override;
}

void fql_set_print_plan(struct fql_handle* fql, int print_plan)
{
        fql->props.print_plan = print_plan;
}

void fql_set_in_delim(struct fql_handle* fql, const char* delim)
{
        strncpy_(fql->props.in_delim, delim, 32);
}

void fql_set_out_delim(struct fql_handle* fql, const char* delim)
{
        strncpy_(fql->props.out_delim, delim, 32);
}


/**
 * Methods
 */

int fql_exec(struct fql_handle* fql, const char* query_str)
{
        fql->query_str = strdup(query_str);

        analyze_query(fql);

        if (schema_resolve(fql->query_list)) {
                return FQL_FAIL;
        }

        Queue* plans = NULL;
        build_plans(&plans, fql->query_list);

        if (fql->props.print_plan) {
                print_plans(plans);
        }

        int ret = 0;
        if (!fql->props.dry_run) {
                ret = process_exec_plans(plans);
        }

        queue_free_func(&fql->query_list, &query_free);
        queue_free_func(&plans, &plan_free);

        return ret;
}

int fql_open(struct fql_handle* fql, const char* query_str)
{
        analyze_query(fql);

        int count = queue_count(fql->query_list);
        if (count != 1) {
                fprintf(stderr, "Expected 1 query. Found %d.\n", count);
                return FQL_FAIL;
        }

        Query* query = fql->query_list->data;
        queue_free(&fql->query_list);

        if (schema_resolve_query(query)) {
                return FQL_FAIL;
        }

        Plan* plan = plan_build(query);

        int ret = 0;
        //if (!fql->props.dry_run) {
        //        ret = process_exec_plan(plan);
        //}

        return ret;
}
