#include "fql.h"
#include "util/util.h"
#include "util/vec.h"
#include "util/stack.h"
#include "util/queue.h"
#include "antlr/antlr.h"
#include "fqlplan.h"
#include "fqlselect.h"
#include "table.h"
#include "query.h"
#include "schema.h"
#include "reader.h"

struct fql_handle* fql_new()
{
	struct fql_handle* new_handle = malloc_(sizeof(*new_handle));
	return fql_construct(new_handle);
}

struct fql_handle* fql_construct(struct fql_handle* fql)
{
	*fql = (struct fql_handle) {
		 NULL                       /* queries */
		,new_(vec, sizeof(struct fql_field)) /* api_vec */
		,NULL                       /* query_str */
		,{
			 ""     /* in_delim */
			,""     /* out_delim */
			,false  /* force_cartesian */
			,false  /* dry_run */
			,false  /* override_warnings */
			,true   /* print_header */
			,false  /* print_plan */
			,false  /* threading */
			,false  /* verbose */
			,false  /* char_as_byte */
		}  /* props */
	};
	return fql;
}

void fql_free(struct fql_handle* fql)
{
	delete_(vec, fql->api_vec);
	free_(fql->query_str);
	free_(fql);
}

int _api_connect(struct fql_handle* fql, query* query)
{
	process* true_proc = query->plan->op_true->data;
	if (true_proc->action__ != fql_select) {
		fputs("can only step through SELECT queries\n", stderr);
		return FQL_FAIL;
	}

	fqlselect_connect_api(fql->query_list->data, fql->api_vec);

	/* Since we are using the api, we want to make sure
	 * we parse all fields.
	 */
	vec* node_vec = query->plan->processes->nodes;
	dnode** it = vec_begin(node_vec);
	for (; it != vec_end(node_vec); ++it) {
		process* proc = (*it)->data;
		if (proc->action__ == fql_read) {
			table* table = proc->proc_data;
			reader* reader = table->reader;
			reader->max_col_idx = INT_MAX;
		}
	}

	return FQL_GOOD;
}

int fql_field_count(struct fql_handle* fql)
{
	if (fql->query_list && vec_empty(fql->api_vec)) {
		_api_connect(fql, fql->query_list->data);
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

void fql_set_print_header(struct fql_handle* fql, int print_header)
{
	fql->props.print_header = print_header;
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

void fql_set_threading(struct fql_handle* fql, int threading)
{
	fql->props.threading = threading;
}

void fql_set_char_as_byte(struct fql_handle* fql, int char_as_byte)
{
	fql->props.char_as_byte = char_as_byte;
}

void fql_set_force_cartesian(struct fql_handle* fql, int force_cartesian)
{
	fql->props.force_cartesian = force_cartesian;
}

/**
 * Methods
 */
int fql_exec_plans(struct fql_handle* fql, int plan_count)
{
	int i = 0;
	queue* node = fql->query_list;
	for (; node && i < plan_count; node = node->next, ++i) {
		query* query = node->data;
		plan* plan = query->plan;
		if (plan->has_stepped) {
			fputs("Cannot execute plan that has been"
			      " stepped through\n", stderr);
			return FQL_FAIL;
		}

		op_preop(fql);

		if (fql->props.threading) {
			try_ (process_exec_plan_thread(plan));
		} else {
			try_ (process_exec_plan(plan));
		}
	}
	return FQL_GOOD;
}

int fql_exec_all_plans(struct fql_handle* fql)
{
	return fql_exec_plans(fql, INT_MAX);
}

int fql_exec(struct fql_handle* fql, const char* query_str)
{
	int plan_count = try_ (fql_make_plans(fql, query_str));
	int ret = 0;
	if (!fql->props.dry_run) {
		ret = fql_exec_plans(fql, plan_count);
	}

	queue_free_func(&fql->query_list, &query_free);

	return ret;
}

int fql_make_plans(struct fql_handle* fql, const char* query_str)
{
	if (fql->query_str) {
		free_(fql->query_str);
	}
	fql->query_str = strdup(query_str);

	if (analyze_query(fql)) {
		goto make_plans_fail;
	}

	if (schema_resolve(fql)) {
		goto make_plans_fail;
	}

	if (build_plans(fql->query_list)) {
		goto make_plans_fail;
	}

	if (fql->props.print_plan) {
		print_plans(fql->query_list);
	}

	return queue_count(fql->query_list);

make_plans_fail:
	queue_free_func(&fql->query_list, query_free);
	return FQL_FAIL;
}

int fql_step(struct fql_handle* fql, struct fql_field** fields)
{
	query* query = fql->query_list->data;
	struct fql_plan* plan = query->plan;
	if (!plan->has_stepped) {
		if (vec_empty(fql->api_vec)) {
			_api_connect(fql, query);
		}
		plan->has_stepped = true;
	}

	int ret = process_step(plan);
	if (ret == 0 || ret == FQL_FAIL) {
		struct query* query = queue_dequeue(&fql->query_list);
		query_free(query);

		vec_resize(fql->api_vec, 0);
	}

	*fields = vec_begin(fql->api_vec);

	return ret;
}

