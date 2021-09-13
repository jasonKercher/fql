#ifndef FQL_HANDLE_H
#define FQL_HANDLE_H

#ifdef __cplusplus
extern "C" {
#else
#include <stdbool.h>
#endif

#include "fqlimits.h"

struct query;

struct fql_handle {
	struct query** active_iter;
	struct vec* query_vec;
	struct vec* api_vec;
	struct hashmap* variable_map;
	struct hashmap* schema_map;
	struct vec* schema_paths;
	char* query_str;
	bool _out_delim_set;
	struct {
		struct vec* schema_path; /* actually string but extern "C" so... */
		struct vec* schema;      /* Same thing... this is also a string */
		char in_delim[DELIM_LEN_MAX + 1];
		char out_delim[DELIM_LEN_MAX + 1];
		char rec_terminator[DELIM_LEN_MAX + 1];
		int pipe_factor;
		int in_std;
		int out_std;
		int strictness;
		bool dry_run;
		bool force_cartesian;
		bool overwrite;
		bool override_warnings;
		bool print_header;
		bool add_header;
		bool print_plan;
		bool threading;
		bool verbose;
		bool parse_only;
		bool char_as_byte;
		bool loose_groups;
		bool stable;
		bool allow_stdin;
	} props;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* FQL_HANDLE_H */
