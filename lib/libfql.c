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

#define PIPE_FACTOR_MIN            2
#define PIPE_FACTOR_MAX            1024
#define PIPE_FACTOR_DEFAULT        16
#define PIPE_FACTOR_THREAD_DEFAULT 64

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
	        .global_scope = new_(scope),
	        .query_vec = new_t_(vec, query*),
	        .api_vec = new_t_(vec, struct fql_field),
	        .variables = new_t_(vec, variable),
	        .branch_state = BRANCH_EXPECT_EXPR,
	        ._out_delim_set = false,
	        {
	                .schema_path = new_(string),
	                .schema = new_(string),
	                .verbosity = FQL_BASIC,
	                .pipe_factor = PIPE_FACTOR_DEFAULT,
	                .in_std = QUOTE_RFC4180,
	                .out_std = QUOTE_RFC4180,
	                .print_header = true,
	        }, /* props */
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

	delete_(vec, fql->variables);
	delete_(scope, fql->global_scope);
	delete_(hashmap, fql->schema_map);
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

	try_(variable_set_type(var, libfql_get_sql_type(type_str)));

	if (var->type == SQL_UNDEFINED) {
		return FQL_FAIL;
	}
	return FQL_GOOD;
}

int libfql_declare_variable(fqlhandle* fql, const char* varname)
{
	int* var_idx = hashmap_get(&fql->_scope->variable_map, varname);
	if (var_idx != NULL) {
		variable* var = vec_at(fql->variables, *var_idx);
		if (var->can_be_redeclared) {
			var->can_be_redeclared = false;
			return *var_idx;
		}
		fprintf(stderr, "variable `%s' already declared\n", varname);
		return FQL_FAIL;
	}

	int new_var_idx = fql->variables->size;
	hashmap_set(&fql->_scope->variable_map, varname, &new_var_idx);

	variable* var = vec_add_one(fql->variables);
	variable_construct(var);

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

	hashmap_clear(fql->schema_map);
	scope_reset(fql->global_scope);
	fql->_scope = fql->global_scope;

	variable* var_iter = vec_begin(fql->variables);
	for (; var_iter != vec_end(fql->variables); ++var_iter) {
		variable_destroy(var_iter);
	}
	vec_clear(fql->variables);

	return FQL_GOOD;
}

int _api_connect(fqlhandle* fql)
{
	query** query = vec_at(fql->query_vec, fql->query_idx);

	enum fql_operation operation = *(enum fql_operation*)(*query)->op;
	if (operation != FQL_SELECT || (*query)->into_table_name != NULL) {
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

/**
 * Property mutators
 */
void fql_set_verbose(fqlhandle* fql, enum fql_verbose verbosity)
{
	fql->props.verbosity = verbosity;
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
	if (std == NULL) {
		return FQL_FAIL;
	}
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
	if (std == NULL) {
		return FQL_FAIL;
	}
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
	if (delim == NULL) {
		return;
	}
	strncpy_(fql->props.in_delim, delim, DELIM_LEN_MAX);
}

void fql_set_out_delim(fqlhandle* fql, const char* delim)
{
	if (delim == NULL) {
		return;
	}
	strncpy_(fql->props.out_delim, delim, DELIM_LEN_MAX);
	fql->_out_delim_set = true;
}

void fql_set_record_terminator(fqlhandle* fql, const char* term)
{
	if (term == NULL) {
		return;
	}
	strncpy_(fql->props.rec_terminator, term, 32);
}

void fql_set_threading(fqlhandle* fql, int threading)
{
	if (threading && sysconf(_SC_NPROCESSORS_ONLN) < 2) {
		//fprintf(stderr,
		//        "Request to enable threading ignored due "
		//        "hardware configuration\n");
		return;
	}
	fql->props.threading = threading;
	if (fql->props.pipe_factor == PIPE_FACTOR_DEFAULT) {
		fql->props.pipe_factor = PIPE_FACTOR_THREAD_DEFAULT;
	}
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
	int applied_pipe_factor = pipe_factor;
	if (applied_pipe_factor < PIPE_FACTOR_MIN) {
		applied_pipe_factor = PIPE_FACTOR_MIN;
	} else if (applied_pipe_factor > PIPE_FACTOR_MAX) {
		applied_pipe_factor = PIPE_FACTOR_MAX;
	}

	fql->props.pipe_factor = pipe_factor;
	if (applied_pipe_factor != pipe_factor) {
		fprintf(stderr,
		        "Invalid pipe factor `%d'. Applied `%d'\n",
		        pipe_factor,
		        applied_pipe_factor);
	}
}

void fql_set_strict_mode(fqlhandle* fql, int strictness)
{
	fql->props.strictness = strictness;
}

void fql_set_schema_path(fqlhandle* fql, const char* schema_path)
{
	if (schema_path == NULL) {
		return;
	}
	string_strcpy(fql->props.schema_path, schema_path);
}

void fql_set_schema(fqlhandle* fql, const char* schema)
{
	if (schema == NULL) {
		return;
	}
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

int fql_import_variable(fqlhandle* fql, const char* assign_expression)
{
	if (assign_expression == NULL) {
		return FQL_FAIL;
	}

	/* skip leading space */
	for (; isspace(*assign_expression); ++assign_expression)
		;

	int len = strlen(assign_expression);
	char* expr_cpy = malloc_(len + 2); /* Null byte + Maybe adding @ */

	if (*assign_expression == '@') {
		strcpy(expr_cpy, assign_expression);
	} else {
		sprintf(expr_cpy, "@%s", assign_expression);
	}

	char* iter = expr_cpy + 1;
	if (*iter == '=' || *iter == '\0') {
		fputs("Cannot assign 0 length variable\n", stderr);
		free_(expr_cpy);
		return FQL_FAIL;
	}

	const char* varname = expr_cpy;

	/* read to = */
	for (; *iter && *iter != '='; ++iter)
		;

	const char* value = iter;
	if (*value) {
		++value;
	}

	*iter = '\0';

	/* back over any trailing space and make them NULL bytes */
	for (--iter; isspace(*iter); --iter) {
		*iter = '\0';
	}

	int idx = try_(libfql_declare_variable(fql, varname));

	variable* var = vec_at(fql->variables, idx);
	var->can_be_redeclared = true;

	if (*value) {
		variable_set_import_value(var, value);
	}

	free_(expr_cpy);
	return FQL_GOOD;
}

/**
 * Status
 */
int fql_field_count(fqlhandle* fql)
{
	if (!vec_empty(fql->query_vec) && vec_empty(fql->api_vec)) {
		try_(_api_connect(fql));
	}
	return fql->api_vec->size;
}

query* _get_active_query(fqlhandle* fql)
{
	if (vec_empty(fql->query_vec) || fql->query_idx >= fql->query_vec->size) {
		return NULL;
	}

	query** active_query = vec_at(fql->query_vec, fql->query_idx);
	return *active_query;
}

const char* fql_get_operation_text(struct fqlhandle* fql)
{
	query* active_query = _get_active_query(fql);
	if (active_query == NULL) {
		return "";
	}
	return string_c_str(active_query->text);
}


enum fql_operation fql_get_operation_type(fqlhandle* fql)
{
	query* active_query = _get_active_query(fql);
	if (active_query == NULL) {
		return FQL_NONE;
	}
	enum fql_operation* op = active_query->op;
	return *op;
}

void _exec_plans_print_footer(plan* plan, query* query)
{
	enum fql_operation* op = query->op;
	switch (*op) {
	case FQL_SELECT:
	case FQL_UPDATE:
	case FQL_DELETE:
		if (plan->rows_affected == 1) {
			fprintf(stderr, "\n1 row affected\n\n");
		} else {
			fprintf(stderr, "\n%lu rows affected\n\n", plan->rows_affected);
		}
	default:;
	}
}
/**
 * Execution
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

		if (fql->props.verbosity > FQL_BASIC) {
			fprintf(stderr, "EXEC: %s\n", string_c_str((*query)->text));
		}

		ret = query_prepare(*query);
		if (ret == FQL_FAIL) {
			break;
		}

		if (fql->props.threading) {
			ret = process_exec_plan_thread(plan, fql);
		} else {
			ret = process_exec_plan(plan);
		}

		fql->query_idx = (*query)->next_idx;

		if (ret == FQL_FAIL) {
			break;
		}

		if (fql->props.verbosity > FQL_QUIET) {
			_exec_plans_print_footer(plan, *query);
		}
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
	if (fql->props.verbosity > FQL_BASIC) {
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
	int ret = 0;
	if (!plan->has_stepped) {
		if (vec_empty(fql->api_vec)) {
			_api_connect(fql);
		}
		plan->has_stepped = true;

		ret = query_prepare(*query);
		if (ret == FQL_FAIL) {
			goto _step_fail_break;
		}

		/* Since we are now stepping, we only want
		 * to iterate over one record at a time.
		 */
		dnode** it = vec_begin(plan->processes->nodes);
		for (; it != vec_end(plan->processes->nodes); ++it) {
			process* proc = (*it)->data;
			proc->max_recs_iter = 1;
		}
	}

	ret = process_step(plan);
	if (ret != 0 && ret != FQL_FAIL) {
		*fields = vec_begin(fql->api_vec);
		return ret;
	}

_step_fail_break:

	plan->has_stepped = false;

	_free_api_strings(fql->api_vec);
	vec_clear(fql->api_vec);

	fql->query_idx = (*query)->next_idx;

	if (ret == FQL_FAIL || fql->query_idx >= fql->query_vec->size) {
		fql_reset(fql);
	}

	return ret;
}

long fql_exec_until_select(fqlhandle* fql)
{
	long exec_count = 0;
	query* current_query = _get_active_query(fql);
	while (current_query) {
		enum fql_operation* current_op = current_query->op;
		if (*current_op == FQL_SELECT && current_query->into_table_name == NULL) {
			break;
		}
		++exec_count;
		try_(fql_exec_plans(fql, 1));
		current_query = _get_active_query(fql);
	}
	return exec_count;
}
