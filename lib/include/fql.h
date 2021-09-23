#ifndef FQL_H
#define FQL_H

#ifdef __cplusplus
extern "C" {
#else
#include <stdbool.h>
#endif

#define FQL_FAIL -5
#define FQL_NULL -1
#define FQL_GOOD 0

enum fql_type {
	FQL_UNDEFINED = -1,
	FQL_INT,
	FQL_FLOAT,
	FQL_STRING,
};

enum fql_operation {
	FQL_NONE,
	FQL_IF,
	FQL_SET,
	FQL_SELECT,
	FQL_DELETE,
	FQL_UPDATE,
};


struct fql_field {
	void* _in;
        enum fql_type type;
        union {
                const char* s;
                long i;
                double f;
        } data;
	bool is_null;
};

struct fqlhandle;

/**
 * constructor to make a new fql handle
 */
struct fqlhandle* fql_new();

/**
 * destructor for fql handle
 */
void fql_free(struct fqlhandle*);

/**
 * retain instance of fql, but free any existing
 * queries and states.
 */
int fql_reset(struct fqlhandle*);


/** Properties **/

/**
 * set verbose to receive extra output via stderr
 */
void fql_set_verbose(struct fqlhandle*, int);

/**
 * Do nothing other than run the parser.
 */
void fql_set_parse_only(struct fqlhandle*, int);

/**
 * set dry run to avoid execution. Useful if you
 * just want to look at the generated plan.
 */
void fql_set_dry_run(struct fqlhandle*, int);

/**
 * Like most database engines, the following will
 * throw an error if table T1 already exists:
 * ```sql
 * select *
 * into T1
 * from T2
 * ```
 * fql also observes this behavior, however,
 * it can be turned off by setting overwrite.
 */
void fql_set_overwrite(struct fqlhandle*, int);

/**
 * The grammar for this language is for the
 * complete t-sql language, however, I've only
 * implemented a small subset of the language.
 * If you try to use a part of the language that
 * I did not implement, a warning will prevent
 * execution. By setting override_warnings, one
 * can continue execution anyway.
 */
void fql_set_override_warnings(struct fqlhandle*, int);

/**
 * set print header to whether or not a header is
 * printed (default schema only).
 */
void fql_set_print_header(struct fqlhandle*, int);

/**
 * TODO: currently no-op
 * For a schema that does not have a header, one
 * can be added.
 */
void fql_set_add_header(struct fqlhandle*, int);

/**
 * Set print plan to print the execution plan to
 * stderr prior to execution.
 */
void fql_set_print_plan(struct fqlhandle*, int);

/**
 * Set an input delimiter if it is something weird
 * (default schema only). The default schema will
 * attempt to resolve a delimiter from < , | ; TAB >
 */
void fql_set_in_delim(struct fqlhandle*, const char*);

/**
 * Set an output delimiter to override whatever the
 * default schema chooses.
 */
void fql_set_out_delim(struct fqlhandle*, const char*);

/**
 * Set an output record terminator for delimited
 * output.  Default is line feed (\n).
 */
void fql_set_record_terminator(struct fqlhandle*, const char*);

/**
 * Turn on threading for individual queries
 * Currently, this works about 99% of the time.
 * When it works 100% of the time, it will become
 * the default.
 */
void fql_set_threading(struct fqlhandle*, int);

/**
 * By default, we are expecting UTF-8 input. If
 * you want to treat each byte as a character, set
 * char_as_byte.
 */
void fql_set_char_as_byte(struct fqlhandle*, int);

/**
 * More for testing purposes, we can instruct fql to
 * use a brute force cartesian join over something
 * more efficient like a hash join.
 */
void fql_set_force_cartesian(struct fqlhandle*, int);

/**
 * set a multiplier for the size of the pipes used to
 * pass data to different processes. Default: 4
 */
void fql_set_pipe_factor(struct fqlhandle*, int pipe_factor);

/**
 * This setting takes precedence over the
 * FQL_SCHEMA_PATHenvironment variable and
 * "$HOME/.config/fql/"
 */
void fql_set_schema_path(struct fqlhandle* fql, const char* schema_path);

/**
 * Use this option to set a default schema
 */
void fql_set_schema(struct fqlhandle* fql, const char* schema);

/**
 * set strict to make the validation pickier
 * (and safer) at the expense of verbosity
 */
void fql_set_strict_mode(struct fqlhandle*, int);

/**
 * Allow SELECTing of non-grouped data while using
 * GROUP BY. This will disable aggregates. For
 * example, the following query becomes legal:
 * will become legal:
 *
 * ```sql
 * select foo, bar
 * from t1
 * group by foo
 * ```
 *
 * Here bar will just be the *first* bar for
 * each unique foo.
 */
void fql_set_loose_groups(struct fqlhandle* fql, int);

/**
 * Preserve the input order of the records.
 *
 * TODO: Currently no-op because input order
 *       cannot be broken (on paper anyway).
 */
void fql_set_stable(struct fqlhandle* fql, int);

/**
 * Use Windows style CR + LF as output line ending instead
 * of Unix style LF only.
 */
void fql_set_crlf_output(struct fqlhandle* fql, int);

/**
 * Set input/output standards for text qualifying
 */
int fql_set_in_std(struct fqlhandle* fql, const char*);
int fql_set_out_std(struct fqlhandle* fql, const char*);

/**
 * Determine if we are allowed to use the internal
 * table "__STDIN." The user should be in control of
 * stdin so by default, __STDIN, is not available.
 */
void fql_set_allow_stdin(struct fqlhandle* fql, int);

/**
 * Import a variable assignment expression, and declare
 * the variable. This is the safest method of importing
 * variables while avoiding SQL injection. Examples:
 *
 *   INPUT             EQUIVALENT SQL
 *   varname = value   declare @varname varchar = 'value'
 *   varname           declare @varname varchar
 *
 * Variables declared with this method are allowed to
 * be re-declared within the SQL code if you wanted for
 * the purpose of changing type. For example:
 *
 *     struct fqlhandle* fql = fql_new();
 *     fql_import_variable(fql, "mynum=5");
 *     fql_exec(fql, "declare @mynum int");
 *
 * This would be the functional equivalent of:
 *
 *     declare @mynum int = 5
 */
int fql_import_variable(struct fqlhandle* fql, const char*);

/** executing **/

/**
 * Simplest form of interaction. Send a handle and a
 * query in text form, and it will execute it.
 * Returns:
 *   The FQL_GOOD or FQL_FAIL
 */
int fql_exec(struct fqlhandle*, const char*);

/**
 * Any query not run through fql_exec, starts here.
 * Returns:
 *   The number of plans made or FQL_FAIL
 */
int fql_make_plans(struct fqlhandle*, const char*);

/**
 * Retrieve a single record result
 * Returns:
 *   0        : no record
 *   1        : one record
 *   FQL_FAIL : error
 */
int fql_step(struct fqlhandle* fql, struct fql_field**);

/**
 * Runs all plans that have been made
 * Returns any number or FQL_FAIL on error
 */
int fql_exec_all_plans(struct fqlhandle* fql);

/**
 * Same as fql_exec_all_plans, except also send a
 * limit for the number of queries you would like executed.
 */
int fql_exec_plans(struct fqlhandle* fql, int plan_count);

/**
 * If a plan has been built, you can use this to retrieve
 * the number of fields coming your way.
 * If no plan has been built, this will return 0.
 */
int fql_field_count(struct fqlhandle*);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* FQL_H */
