#ifndef FQL_HANDLE_H
#define FQL_HANDLE_H

#ifdef __cplusplus
extern "C" {
#endif

struct fql_handle {
        struct queue* query_list;
        struct vec* api_vec;
        char* query_str;
        struct {
                char in_delim[32];
                char out_delim[32];
                int dry_run;
                int force_cartesian;
                int override_warnings;
                int print_header;
                int print_plan;
                int threading;
                int verbose;
        } props;
};

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* FQL_HANDLE_H */
