#include "fql.h"
#include "misc.h"
#include "query.h"
#include "table.h"
#include "order.h"
#include "schema.h"
#include "reader.h"
#include "process.h"
#include "fqlplan.h"
#include "fqlselect.h"
#include "util/util.h"
#include "util/vec.h"
#include "util/stack.h"
#include "util/queue.h"
#include "antlr/antlr.h"

#define PIPE_FACTOR_DEFAULT 16

/**
 * No need to expose this function. A user will
 * not know the size of struct fql_handle.
 */
struct fql_handle* fql_construct(struct fql_handle* fql);

struct fql_handle* fql_new()
{
	struct fql_handle* new_handle = malloc_(sizeof(*new_handle));
	return fql_construct(new_handle);
}

struct fql_handle* fql_construct(struct fql_handle* fql)
{
	*fql = (struct fql_handle) {
	        NULL,                          /* queries */
	        new_t_(vec, struct fql_field), /* api_vec */
	        NULL,                          /* query_str */
	        {
	                new_(string),        /* schema_path */
	                new_(string),        /* schema */
	                "",                  /* in_delim */
	                "",                  /* out_delim */
	                "",                  /* rec_terminator */
	                PIPE_FACTOR_DEFAULT, /* pipe_factor */
	                false,               /* dry_run */
	                false,               /* force_cartesian */
	                false,               /* overwrite */
	                false,               /* override_warnings */
	                true,                /* print_header */
	                false,               /* add_header */
	                false,               /* print_plan */
	                false,               /* threading */
	                false,               /* verbose */
	                false,               /* parse_only */
	                false,               /* char_as_byte */
	                false,               /* strictness */
	                false,               /* loose_groups */
	                false,               /* stable */
	        }                            /* props */
	};
	return fql;
}

void fql_free(struct fql_handle* fql)
{
	delete_(string, fql->props.schema_path);
	delete_(string, fql->props.schema);
	delete_if_exists_(vec, fql->api_vec);
	free_if_exists_(fql->query_str);
	free_if_exists_(fql);
}

int _api_connect(struct fql_handle* fql, query* query)
{
	process* true_proc = query->plan->op_true->data;
	if (true_proc->action__ != fql_select
	    && true_proc->action__ != fql_orderby) {
		fputs("can only step through SELECT queries\n", stderr);
		return FQL_FAIL;
	}

	try_(fqlselect_connect_api(query, fql->api_vec));
	if (query->orderby != NULL) {
		order_connect_api(query, fql->api_vec);
	}

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
			reader->max_idx = INT_MAX;
		}
	}

	return FQL_GOOD;
}

int fql_field_count(struct fql_handle* fql)
{
	if (fql->query_list && vec_empty(fql->api_vec)) {
		try_(_api_connect(fql, fql->query_list->data));
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

void fql_set_parse_only(struct fql_handle* fql, int parse_only)
{
	fql->props.parse_only = parse_only;
}

void fql_set_dry_run(struct fql_handle* fql, int dry_run)
{
	fql->props.dry_run = dry_run;
}

void fql_set_overwrite(struct fql_handle* fql, int overwrite)
{
	fql->props.overwrite = overwrite;
}

void fql_set_override_warnings(struct fql_handle* fql, int override)
{
	fql->props.override_warnings = override;
}

void fql_set_print_header(struct fql_handle* fql, int print_header)
{
	fql->props.print_header = print_header;
}

void fql_set_add_header(struct fql_handle* fql, int add_header)
{
	fql->props.add_header = add_header;
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

void fql_set_record_terminator(struct fql_handle* fql, const char* term)
{
	strncpy_(fql->props.rec_terminator, term, 32);
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

void fql_set_pipe_factor(struct fql_handle* fql, int pipe_factor)
{
	if (pipe_factor < 2) {
		fprintf(stderr,
		        "pipe factor (%d) invalid, pipe factor set to %d\n",
		        pipe_factor,
		        PIPE_FACTOR_DEFAULT);
		fql->props.pipe_factor = 2;
	} else {
		fql->props.pipe_factor = pipe_factor;
	}
}

void fql_set_strict_mode(struct fql_handle* fql, int strictness)
{
	fql->props.strictness = strictness;
}

void fql_set_schema_path(struct fql_handle* fql, const char* schema_path)
{
	string_strcpy(fql->props.schema_path, schema_path);
}

void fql_set_schema(struct fql_handle* fql, const char* schema)
{
	string_strcpy(fql->props.schema, schema);
}

void fql_set_loose_groups(struct fql_handle* fql, int loose_groups)
{
	fql->props.loose_groups = loose_groups;
}

void fql_set_stable(struct fql_handle* fql, int stable)
{
	fql->props.stable = stable;
}

void fql_set_crlf_output(struct fql_handle* fql, int crlf)
{
	if (crlf) {
		fql->props.rec_terminator[0] = '\r';
		fql->props.rec_terminator[1] = '\n';
		fql->props.rec_terminator[2] = '\0';
	} else {
		fql->props.rec_terminator[0] = '\n';
		fql->props.rec_terminator[1] = '\0';
	}
}

/**
 * Methods
 */
int fql_exec_plans(struct fql_handle* fql, int plan_count)
{
	int i = 0;
	int ret = 0;
	for (; fql->query_list && i < plan_count; ++i) {
		query* query = fql->query_list->data;
		plan* plan = query->plan;
		if (plan->has_stepped) {
			fputs("Cannot execute plan that has been"
			      " stepped through\n",
			      stderr);
			return FQL_FAIL;
		}

		op_preop(fql);

		if (fql->props.threading) {
			ret = process_exec_plan_thread(plan);
		} else {
			ret = process_exec_plan(plan);
		}

		query_free(query);
		queue_dequeue(&fql->query_list);
		if (ret == FQL_FAIL) {
			break;
		}
	}
	if (ret != FQL_GOOD) {
		queue_free_func(&fql->query_list, query_free);
	}
	return ret;
}

int fql_exec_all_plans(struct fql_handle* fql)
{
	return fql_exec_plans(fql, INT_MAX);
}

int fql_exec(struct fql_handle* fql, const char* query_str)
{
	int plan_count = try_(fql_make_plans(fql, query_str));
	int ret = 0;
	if (!fql->props.dry_run) {
		ret = fql_exec_plans(fql, plan_count);
	}

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

	if (build_plans(fql)) {
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

void _free_api_strings(vec* api)
{
	struct fql_field* it = vec_begin(api);
	for (; it != vec_end(api); ++it) {
		if (it->type == FQL_STRING) {
			delete_(string, it->_in);
		}
	}
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

		/* Since we are now stepping, we only want
		 * to iterate over one record at a time.
		 */
		dnode** it = vec_begin(plan->processes->nodes);
		for (; it != vec_end(plan->processes->nodes); ++it) {
			process* proc = (*it)->data;
			proc->max_recs_iter = 1;
		}
	}

	int ret = process_step(plan);
	if (ret != 0 && ret != FQL_FAIL) {
		*fields = vec_begin(fql->api_vec);
		return ret;
	}
	struct query* q = queue_dequeue(&fql->query_list);
	query_free(q);
	_free_api_strings(fql->api_vec);
	vec_clear(fql->api_vec);
	return ret;
}
