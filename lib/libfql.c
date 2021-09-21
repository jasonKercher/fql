#include "aggregate.h"
#include "fql.h"
#include "hashmap.h"
#include "misc.h"
#include "scope.h"
#include "query.h"
#include "table.h"
#include "order.h"
#include "schema.h"
#include "reader.h"
#include "fqlsig.h"
#include "process.h"
#include "fqlplan.h"
#include "variable.h"
#include "fqlselect.h"
#include "antlr/antlr.h"
#include "util/vec.h"
#include "util/util.h"
#include "util/node.h"

#define PIPE_FACTOR_DEFAULT 16

/**
 * No need to expose this function. A user will
 * not know the size of fqlhandle.
 */
fqlhandle* fql_construct(fqlhandle* fql);

fqlhandle* fql_new()
{
	fqlhandle* new_handle = malloc_(sizeof(*new_handle));
	return fql_construct(new_handle);
}

fqlhandle* fql_construct(fqlhandle* fql)
{
	*fql = (fqlhandle) {
	        NULL,                          /* _scope */
	        new_(scope),                   /* glopal_scope */
	        NULL,                          /* schema_map */
	        new_t_(vec, query*),           /* query_vec */
	        new_t_(vec, struct fql_field), /* api_vec */
	        new_t_(vec, variable),         /* variables */
	        NULL,                          /* schema_paths */
	        NULL,                          /* query_str */
	        0,                             /* query_idx */
	        false,                         /* _out_delim_set */
	        {
	                new_(string),        /* schema_path */
	                new_(string),        /* schema */
	                "",                  /* in_delim */
	                "",                  /* out_delim */
	                "",                  /* rec_terminator */
	                PIPE_FACTOR_DEFAULT, /* pipe_factor */
	                QUOTE_RFC4180,       /* in_std */
	                QUOTE_RFC4180,       /* out_std */
	                0,                   /* strictness */
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
	                false,               /* loose_groups */
	                false,               /* stable */
	                false,               /* allow_stdin */
	        }                            /* props */
	};

	fql->_scope = fql->global_scope;

	fql->schema_map = new_t_(hashmap, schema*, 16, HASHMAP_PROP_DEFAULT);

	fqlsig_init_sig();

	return fql;
}

void fql_free(fqlhandle* fql)
{
	fql_reset(fql);

	if (fql->schema_paths != NULL) {
		string** it = vec_begin(fql->schema_paths);
		for (; it != vec_end(fql->schema_paths); ++it) {
			delete_(string, *it);
		}
		delete_(vec, fql->schema_paths);
	}
	delete_(scope, fql->global_scope);
	delete_(hashmap, fql->schema_map);
	delete_(vec, fql->variables);
	delete_(string, fql->props.schema_path);
	delete_(string, fql->props.schema);
	delete_(vec, fql->query_vec);
	delete_if_exists_(vec, fql->api_vec);
	free_if_exists_(fql->query_str);
	free_(fql);
}

int libfql_get_sql_type(const char* type_str)
{
	if (istring_eq(type_str, "int") || istring_eq(type_str, "bigint")
	    || istring_eq(type_str, "tinyint") || istring_eq(type_str, "smallint")) {
		return SQL_INT;
	} else if (istring_eq(type_str, "bit")) {
		return SQL_BIT;
	} else if (istring_eq(type_str, "float") || istring_eq(type_str, "real")) {
		return SQL_FLOAT;
	} else if (istring_eq(type_str, "varchar") || istring_eq(type_str, "nvarchar")) {
		return SQL_VARCHAR;
	} else if (istring_eq(type_str, "text") || istring_eq(type_str, "ntext")) {
		return SQL_TEXT;
	} else if (istring_eq(type_str, "char") || istring_eq(type_str, "nchar")) {
		return SQL_CHAR;
	}
	fprintf(stderr, "Type not implemented: `%s'\n", type_str);
	return SQL_UNDEFINED;
}

int libfql_set_variable_type(fqlhandle* fql, const char* type_str)
{
	variable* var = vec_back(fql->variables);
	var->type = libfql_get_sql_type(type_str);
	if (var->type == SQL_UNDEFINED) {
		return FQL_FAIL;
	}
	return FQL_GOOD;
}

int libfql_declare_variable(fqlhandle* fql, const char* varname)
{
	int* var_idx = hashmap_get(&fql->_scope->variable_map, varname);
	if (var_idx != NULL) {
		fprintf(stderr, "variable `%s' already declared\n", varname);
		return FQL_FAIL;
	}

	int new_var_idx = fql->variables->size;
	hashmap_set(&fql->_scope->variable_map, varname, &new_var_idx);

	vec_add_one(fql->variables);

	return new_var_idx;
}

int libfql_set_variable_limit(fqlhandle* fql, const char* limit_str)
{
	long limit = 0;
	if (str2long(&limit, limit_str)) {
		return FQL_FAIL;
	}
	if (limit <= 0) {
		fprintf(stderr, "invalid size argument(%ld)\n", limit);
		return FQL_FAIL;
	}
	variable* var = vec_back(fql->variables);
	var->limit = limit;
	return FQL_GOOD;
}

int fql_reset(fqlhandle* fql)
{
	query** it = vec_begin(fql->query_vec);
	for (; it != vec_end(fql->query_vec); ++it) {
		delete_(query, *it);
	}
	vec_clear(fql->query_vec);
	fql->query_idx = 0;

	fqlsig_tmp_removeall();

	/* lol I never implemented clear... */
	hashmap_destroy(fql->schema_map);
	hashmap_construct_(fql->schema_map, schema*, 16, HASHMAP_PROP_DEFAULT);

	return FQL_GOOD;
}

int _api_connect(fqlhandle* fql)
{
	query** query = vec_at(fql->query_vec, fql->query_idx);

	enum op operation = *(enum op*)(*query)->op;
	if (operation != OP_SELECT || (*query)->into_table_name != NULL) {
		fputs("can only step through SELECT queries\n", stderr);
		return FQL_FAIL;
	}

	try_(fqlselect_connect_api(*query, fql->api_vec));
	if ((*query)->orderby != NULL) {
		order_connect_api(*query, fql->api_vec);
	}

	/* Since we are using the api, we want to make sure
	 * we parse all fields.
	 */
	vec* node_vec = (*query)->plan->processes->nodes;
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

int fql_field_count(fqlhandle* fql)
{
	if (!vec_empty(fql->query_vec) && vec_empty(fql->api_vec)) {
		try_(_api_connect(fql));
	}
	return fql->api_vec->size;
}

/**
 * Property mutators
 */
void fql_set_verbose(fqlhandle* fql, int verbose)
{
	fql->props.verbose = verbose;
}

void fql_set_parse_only(fqlhandle* fql, int parse_only)
{
	fql->props.parse_only = parse_only;
}

void fql_set_dry_run(fqlhandle* fql, int dry_run)
{
	fql->props.dry_run = dry_run;
}

void fql_set_overwrite(fqlhandle* fql, int overwrite)
{
	fql->props.overwrite = overwrite;
}

void fql_set_override_warnings(fqlhandle* fql, int override)
{
	fql->props.override_warnings = override;
}

void fql_set_print_header(fqlhandle* fql, int print_header)
{
	fql->props.print_header = print_header;
}

void fql_set_add_header(fqlhandle* fql, int add_header)
{
	fql->props.add_header = add_header;
}

void fql_set_print_plan(fqlhandle* fql, int print_plan)
{
	fql->props.print_plan = print_plan;
}

int fql_set_in_std(fqlhandle* fql, const char* std)
{
	if (istring_eq(std, "ALL")) {
		fql->props.in_std = QUOTE_ALL;
	} else if (istring_eq(std, "WEAK")) {
		fql->props.in_std = QUOTE_WEAK;
	} else if (istring_eq(std, "NONE")) {
		fql->props.in_std = QUOTE_NONE;
	} else if (istring_eq(std, "RFC4180")) {
		fql->props.in_std = QUOTE_RFC4180;
	} else {
		fprintf(stderr,
		        "Invalid quote option: %s\n"
		        "Options: NONE, WEAK, RFC4180, ALL\n",
		        std);
		return FQL_FAIL;
	}
	return FQL_GOOD;
}

int fql_set_out_std(fqlhandle* fql, const char* std)
{
	if (istring_eq(std, "ALL")) {
		fql->props.out_std = QUOTE_ALL;
	} else if (istring_eq(std, "WEAK")) {
		fql->props.out_std = QUOTE_WEAK;
	} else if (istring_eq(std, "NONE")) {
		fql->props.out_std = QUOTE_NONE;
	} else if (istring_eq(std, "RFC4180")) {
		fql->props.out_std = QUOTE_RFC4180;
	} else {
		fprintf(stderr,
		        "Invalid quote option: %s\n"
		        "Options: NONE, WEAK, RFC4180, ALL\n",
		        std);
		return FQL_FAIL;
	}
	return FQL_GOOD;
}

void fql_set_in_delim(fqlhandle* fql, const char* delim)
{
	strncpy_(fql->props.in_delim, delim, DELIM_LEN_MAX);
}

void fql_set_out_delim(fqlhandle* fql, const char* delim)
{
	strncpy_(fql->props.out_delim, delim, DELIM_LEN_MAX);
	fql->_out_delim_set = true;
}

void fql_set_record_terminator(fqlhandle* fql, const char* term)
{
	strncpy_(fql->props.rec_terminator, term, 32);
}

void fql_set_threading(fqlhandle* fql, int threading)
{
	if (threading && sysconf(_SC_NPROCESSORS_ONLN) < 2) {
		fprintf(stderr,
		        "Request to enable threading ignored due "
		        "hardware configuration\n");
		return;
	}
	fql->props.threading = threading;
}

void fql_set_char_as_byte(fqlhandle* fql, int char_as_byte)
{
	fql->props.char_as_byte = char_as_byte;
}

void fql_set_force_cartesian(fqlhandle* fql, int force_cartesian)
{
	fql->props.force_cartesian = force_cartesian;
}

void fql_set_pipe_factor(fqlhandle* fql, int pipe_factor)
{
	if (pipe_factor < 2) {
		fprintf(stderr,
		        "pipe factor (%d) invalid, pipe factor set to %d\n",
		        pipe_factor,
		        PIPE_FACTOR_DEFAULT);
		fql->props.pipe_factor = PIPE_FACTOR_DEFAULT;
	} else {
		fql->props.pipe_factor = pipe_factor;
	}
}

void fql_set_strict_mode(fqlhandle* fql, int strictness)
{
	fql->props.strictness = strictness;
}

void fql_set_schema_path(fqlhandle* fql, const char* schema_path)
{
	string_strcpy(fql->props.schema_path, schema_path);
}

void fql_set_schema(fqlhandle* fql, const char* schema)
{
	string_strcpy(fql->props.schema, schema);
}

void fql_set_loose_groups(fqlhandle* fql, int loose_groups)
{
	fql->props.loose_groups = loose_groups;
}

void fql_set_stable(fqlhandle* fql, int stable)
{
	fql->props.stable = stable;
}

void fql_set_crlf_output(fqlhandle* fql, int crlf)
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

void fql_set_allow_stdin(fqlhandle* fql, int allow_stdin)
{
	fql->props.allow_stdin = allow_stdin;
}

/**
 * Methods
 */
int fql_exec_plans(fqlhandle* fql, int plan_count)
{
	int i = 0;
	int ret = 0;
	for (; fql->query_idx < fql->query_vec->size && i < plan_count; ++i) {
		query** query = vec_at(fql->query_vec, fql->query_idx);
		plan* plan = (*query)->plan;
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

		if (fql->props.verbose) {
			if (plan->rows_affected == 1) {
				fprintf(stderr, "1 row affected\n\n");
			} else {
				fprintf(stderr,
				        "%lu rows affected\n\n",
				        plan->rows_affected);
			}
		}

		if (ret == FQL_FAIL) {
			break;
		}

		fql->query_idx = (*query)->next_idx;
	}

	if (ret == FQL_FAIL || fql->query_idx >= fql->query_vec->size) {
		fql_reset(fql);
	}

	return ret;
}

int fql_exec_all_plans(fqlhandle* fql)
{
	return fql_exec_plans(fql, INT_MAX);
}

int fql_exec(fqlhandle* fql, const char* query_str)
{
	try_(fql_make_plans(fql, query_str));

	int ret = FQL_GOOD;
	if (!fql->props.dry_run) {
		ret = fql_exec_all_plans(fql);
	}

	fql_reset(fql);

	return ret;
}

int fql_make_plans(fqlhandle* fql, const char* query_str)
{
	if (fql->props.verbose) {
		fprintf(stderr, "\n[fql] Making Plan for:\n%s\n", query_str);
	}

	if (fql->query_str) {
		free_(fql->query_str);
	}
	fql->query_str = strdup(query_str);

	if (analyze_query(fql)) {
		fql_reset(fql);
		return FQL_FAIL;
	}

	if (schema_resolve(fql)) {
		fql_reset(fql);
		return FQL_FAIL;
	}

	if (plan_build(fql)) {
		fql_reset(fql);
		return FQL_FAIL;
	}

	if (fql->props.print_plan) {
		plan_print(fql);
	}

	return fql->query_vec->size;
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

int fql_step(fqlhandle* fql, struct fql_field** fields)
{
	query** query = vec_at(fql->query_vec, fql->query_idx);
	struct fqlplan* plan = (*query)->plan;
	if (!plan->has_stepped) {
		if (vec_empty(fql->api_vec)) {
			_api_connect(fql);
		}
		plan->has_stepped = true;

		op_preop(fql);

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
	_free_api_strings(fql->api_vec);
	vec_clear(fql->api_vec);

	fql->query_idx = (*query)->next_idx;

	if (ret == FQL_FAIL || fql->query_idx >= fql->query_vec->size) {
		fql_reset(fql);
	}

	return ret;
}
