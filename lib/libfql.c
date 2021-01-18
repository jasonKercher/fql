#include "fql.h"
#include "util/util.h"
#include "util/vec.h"
#include "util/stack.h"
#include "util/queue.h"
#include "antlr/antlr.h"

#include "fqlplan.h"

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
                 NULL                       /* queries */
                ,vec_new_(struct fql_plan)  /* plan_vec */
                ,NULL                       /* query_str */
                ,{
                         ","    /* in_delim */
                        ,","    /* out_delim */
                        ,false  /* verbose */
                        ,false  /* dry_run */
                        ,false  /* override_warnings */
                        ,false  /* print_plan */
                }  /* props */
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

int fql_open_plan(struct fql_handle* fql, Plan* plan)
{
        return FQL_GOOD;
}

int fql_step(struct fql_handle* fql)
{
        return FQL_GOOD;
}

int fql_exec_plans(struct fql_handle* fql, Plan* plans, int plan_count)
{
        int ret = process_exec_plans(plans, plan_count);
        return ret;
}

int fql_exec(struct fql_handle* fql, const char* query_str)
{
        Plan* plans = NULL;
        int plan_count = fql_make_plans(fql, &plans, query_str);
        int ret = 0;
        if (!fql->props.dry_run) {
                ret = process_exec_plans(plans, plan_count);
        }

        queue_free_func(&fql->query_list, &query_free);
        vec_free(fql->plan_vec);

        return ret;
}

int fql_make_plans(struct fql_handle* fql, struct fql_plan** plan, const char* query_str)
{
        fql->query_str = strdup(query_str);

        analyze_query(fql);

        if (schema_resolve(fql->query_list)) {
                return FQL_FAIL;
        }

        build_plans(fql->plan_vec, fql->query_list);

        if (fql->props.print_plan) {
                print_plans(fql->plan_vec);
        }

        *plan = vec_begin(fql->plan_vec);

        return fql->plan_vec->size;
}
