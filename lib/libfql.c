#include "fql.h"
#include "util/util.h"
#include "util/vec.h"
#include "util/stack.h"
#include "util/queue.h"
#include "antlr/antlr.h"
#include "fqlplan.h"
#include "select.h"
#include "table.h"
#include "query.h"
#include "schema.h"

struct fql_handle* fql_new()
{
        struct fql_handle* new_handle;
        malloc_(new_handle, sizeof(*new_handle));

        return fql_construct(new_handle);
}

struct fql_handle* fql_construct(struct fql_handle* fql)
{
        *fql = (struct fql_handle) {
                 NULL                       /* queries */
                ,vec_new_(struct fql_plan)  /* plan_vec */
                ,vec_new_(struct fql_field) /* api_vec */
                ,NULL                       /* query_str */
                ,{
                         ""    /* in_delim */
                        ,""    /* out_delim */
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
        vec_free(fql->plan_vec);
        vec_free(fql->api_vec);
        free_(fql->query_str);
        free_(fql);
}

int _api_connect(struct fql_handle* fql, Plan* plan)
{
        //if (!vec_empty(fql->api_vec) || vec_empty(fql->plan_vec)) {
        //        return;
        //}

        Process* true_proc = plan->op_true->data;
        if (true_proc->action__ != fql_select) {
                fputs("Can only step through SELECT queries\n", stderr);
                return FQL_FAIL;
        }

        select_connect_api(fql->query_list->data, fql->api_vec);

        Dnode** it = vec_begin(plan->processes->nodes);
        for (; it != vec_end(plan->processes->nodes); ++it) {
                Process* proc = (*it)->data;
                if (proc->action__ == fql_read) {
                        Reader* reader = proc->proc_data;
                        reader->max_col_idx = INT_MAX;
                }
        }

        return FQL_GOOD;
}

int fql_field_count(struct fql_handle* fql)
{
        if (vec_empty(fql->api_vec) && !vec_empty(fql->plan_vec)) {
                _api_connect(fql, vec_begin(fql->plan_vec));
        }
        return fql->api_vec->size;
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
int fql_exec_plans(struct fql_handle* fql, int plan_count)
{
        int i = 0;
        for (; i < plan_count; ++i) {
                Plan* plan = vec_at(fql->plan_vec, i);
                if (plan->has_stepped) {
                        fputs("Cannot execute plan that has been"
                              " stepped through\n", stderr);
                        return FQL_FAIL;
                }
                if (process_exec_plan(plan) == FQL_FAIL) {
                        return FQL_FAIL;

                }
        }
        return FQL_GOOD;
}

int fql_exec_all_plans(struct fql_handle* fql)
{
        return fql_exec_plans(fql->plan_vec->data, fql->plan_vec->size);
}

int fql_exec(struct fql_handle* fql, const char* query_str)
{
        int plan_count = fql_make_plans(fql, query_str);
        int ret = 0;
        if (!fql->props.dry_run) {
                ret = fql_exec_plans(fql, plan_count);
        }

        queue_free_func(&fql->query_list, &query_free);
        vec_free(fql->plan_vec);

        return ret;
}

int fql_make_plans(struct fql_handle* fql, const char* query_str)
{
        if (fql->query_str) {
                free_(fql->query_str);
        }
        fql->query_str = strdup(query_str);

        analyze_query(fql);

        if (schema_resolve(fql)) {
                return FQL_FAIL;
        }

        build_plans(fql->plan_vec, fql->query_list);

        if (fql->props.print_plan) {
                print_plans(fql->plan_vec);
        }

        return fql->plan_vec->size;
}

int fql_step(struct fql_handle* fql, struct fql_field** fields)
{
        Plan* plan = vec_begin(fql->plan_vec);
        if (!plan->has_stepped) {
                if (vec_empty(fql->api_vec)) {
                        _api_connect(fql, plan);
                }
                plan->has_stepped = true;
        }

        int ret = process_step(plan);
        if (ret == 0) {
                plan_destroy(plan);
                vec_remove(fql->plan_vec, 0);

                Query* query = queue_dequeue(&fql->query_list);
                query_free(query);

                vec_resize(fql->api_vec, 0);
        }

        *fields = vec_begin(fql->api_vec);

        return ret;
}

