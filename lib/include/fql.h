#ifndef FQL_H
#define FQL_H

#ifdef __cplusplus
extern "C" {
#endif

#define FQL_FAIL -5
#define FQL_GOOD 0

enum fql_type {
       FQL_UNDEFINED = -1,
       FQL_INT,
       FQL_FLOAT,
       FQL_STRING,
};

struct fql_field {
	void* _in;
        enum fql_type type;
        union {
                const char* s;
                long i;
                double f;
        } data;
};

struct fql_handle;

/** 
 * constructor to make a new fql handle
 */
struct fql_handle* fql_new();

/**
 * destructor for fql handle
 */
void fql_free(struct fql_handle*);

/**
 * Properties
 * All properties are boolean unless specified otherwise.
 */

/**
 * set verbose to receive extra output via stderr
 */
void fql_set_verbose(struct fql_handle*, int);

/**
 * set dry run (0,1) to avoid execution
 */
void fql_set_dry_run(struct fql_handle*, int);

/**
 * The grammar for this language is for the
 * complete t-sql language, however, I've only
 * implemented a small subset of the language.
 * If you try to use a part of the language that
 * I did not implement, a warning will prevent
 * execution. By setting override_warnings, one
 * can continue execution anyway.
 */
void fql_set_override_warnings(struct fql_handle*, int);

/**
 * set print header to whether or not a header is
 * printed (default schema only).
 */
void fql_set_print_header(struct fql_handle*, int);

/**
 * Set print plan to print the execution plan to
 * stderr prior to execution.
 */
void fql_set_print_plan(struct fql_handle*, int);

/**
 * Set an input delimiter if it is something weird
 * (default schema only). The default schema will
 * attempt to resolve a delimiter from < , | ; \t >
 */
void fql_set_in_delim(struct fql_handle*, const char*);

/** 
 * Set an output delimiter to override whatever the
 * default schema chooses.
 */
void fql_set_out_delim(struct fql_handle*, const char*);

/**
 * Turn on threading for individual queries
 * Currently, this works about 99% of the time.
 * When it works 100% of the time, it will become
 * the default.
 */
void fql_set_threading(struct fql_handle*, int);

/**
 * By default, we are expection UTF-8 input. If
 * you want to treat each byte as a character, set
 * char_as_byte.
 */
void fql_set_char_as_byte(struct fql_handle*, int);

/**
 * More for testing purposes, we can instruct fql to
 * use a brute force cartesian join over something 
 * more efficient like a hash join.
 */
void fql_set_force_cartesian(struct fql_handle*, int);

/**
 * set strict to make the validation pickier
 * (and safer) at the expense of verbosity
 */
void fql_set_strict_mode(struct fql_handle*, int);

/** executing **/

/**
 * Simplest form of interaction. Send a handle and a
 * query in text form, and it will execute it.
 */
int fql_exec(struct fql_handle*, const char*);

/**
 * Any query not run through fql_exec, starts here.
 * Returns:
 *   The number of plans made or FQL_FAIL
 */
int fql_make_plans(struct fql_handle*, const char*);

/**
 * Retrieve a single record result
 * Returns:
 *   0        : no record
 *   1        : one record
 *   FQL_FAIL : error
 */
int fql_step(struct fql_handle* fql, struct fql_field**);

/**
 * Runs all plans that have been made
 * Returns any number or FQL_FAIL on error
 */
int fql_exec_all_plans(struct fql_handle* fql);

/**
 * Same as fql_exec_all_plans, except also send a
 * limit for the number of queries you would like executed.
 */
int fql_exec_plans(struct fql_handle* fql, int plan_count);

/**
 * If a plan has been built, you can use this to retrieve
 * the number of fields coming your way.
 * If no plan has been built, this will return 0.
 */
int fql_field_count(struct fql_handle*);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* FQL_H */
