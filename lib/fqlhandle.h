#ifndef FQL_HANDLE_H
#define FQL_HANDLE_H

#ifdef __cplusplus
extern "C" {
#endif

struct fql_handle {
        struct queue* query_list;
        char* query_str;
        struct {
                char in_delim[32];
                char out_delim[32];
                int verbose;
                int dry_run;
                int override_warnings;
                int print_plan;
        } props;
};

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* FQL_HANDLE_H */