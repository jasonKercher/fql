#ifndef FQL_H
#define FQL_H

#define FQL_FAIL -5
#define FQL_GOOD 0

void fql_init();

/* Props */

void fql_set_verbose(int);
void fql_set_dry_run(int);
void fql_set_override_warnings(int);
void fql_set_in_delim(const char*);
void fql_set_out_delim(const char*);

int fql_exec(const char*);

#endif
