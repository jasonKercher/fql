#ifndef FQL_H
#define FQL_H

#ifdef __cplusplus
extern "C" {
#endif

#define FQL_FAIL -5
#define FQL_GOOD 0

struct fql_handle;

struct fql_handle* fql_new();
struct fql_handle* fql_init(struct fql_handle*);
void fql_free(struct fql_handle*);

/* Props */

void fql_set_verbose(struct fql_handle*, int);
void fql_set_dry_run(struct fql_handle*, int);
void fql_set_override_warnings(struct fql_handle*, int);
void fql_set_print_plan(struct fql_handle*, int);
void fql_set_in_delim(struct fql_handle*, const char*);
void fql_set_out_delim(struct fql_handle*, const char*);

int fql_exec(struct fql_handle*, const char*);
int fql_open(struct fql_handle*, const char*);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* FQL_H */
