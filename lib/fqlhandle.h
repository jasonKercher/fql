#ifndef FQLHANDLE_H
#define FQLHANDLE_H

#ifdef __cplusplus
extern "C" {
#else
#include <stdbool.h>
#endif

#include "fqlimits.h"

struct query;

struct fqlhandle {
	struct vec* query_vec;
	struct vec* api_vec;
	struct vec* cfl_stack;
	struct hashmap* schema_map;
	struct vec* schema_paths;
	char* query_str;
	unsigned query_idx;
	bool _out_delim_set;
	struct {
		struct vec* schema_path; /* actually string but extern "C" so.. */
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
/* internal typedef.. library user still require "struct" */
typedef struct fqlhandle fqlhandle;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* FQLHANDLE_H */
