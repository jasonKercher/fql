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
		struct vec* schema_path; /* actually string but extern "C" so... */
		struct vec* schema;      /* Same thing... this is also a string */
		char in_delim[32];
		char out_delim[32];
		char rec_terminator[32];
		int pipe_factor;
		int dry_run;
		int force_cartesian;
		int overwrite;
		int override_warnings;
		int print_header;
		int add_header;
		int print_plan;
		int threading;
		int verbose;
		int parse_only;
		int char_as_byte;
		int strictness;
		int loose_groups;
		int stable;
	} props;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* FQL_HANDLE_H */
