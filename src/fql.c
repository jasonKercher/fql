#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <getopt.h>
#include <string.h>
#include <stdio.h>

#include <readline/readline.h>

#include "fql.h"

#define HELP_ARG        0x80
#define PARSE_ONLY_ARG  0x81
#define SCHEMA_PATH_ARG 0x82
#define SCHEMA_ARG      0x83
#define STABLE_ARG      0x84
#define STRICT_ARG      0x85
#define IN_STD_ARG      0x86
#define OUT_STD_ARG     0x87

const char* _help =
        "\nUsage fql -[bCdhHOpv] [-sS delim] [--strict] [file.sql]\n\n"

        "This is a text processing program using SQL (Transact-SQL specifically).\n\n"

        "If no file argument is provided, queries are read from stdin. Text input\n"
        "is assumed to be UTF-8 encoded. Files are referenced as tables in the\n"
        "query via a priority list. For example:\n"
        "select * from t1\n"
        "We try to resolve t1 in the following order:\n\n"

        "    RULE                   EXAMPLE MATCHES\n"
        "    exact match            t1\n"
        "    exact ignoring case    T1,t1\n"
        "    ignore extension       t1.txt,t1.csv\n"
        "    ignore extension/case  t1.txt,T1.csv\n\n"

        "To match t1.txt exactly (or match any filename with dots in them), use \n"
        "'select * from [t1.txt]'. Without the [], the parser is going to assume\n"
        "t1 is the schema for a table txt. Do the same for a filename with\n"
        "white space as the parser will treat this as '[TABLE] [ALIAS]'.\n\n"

        "Optional arguments:\n"
        " -b, --char-as-byte     scalar functions assume single-byte encoding\n"
        " -C, --cartesian        use cartesian join algorithm (less memory use)\n"
        " -d, --dry-run          validate and build a plan, but do not execute\n"
        " -h, --no-header        for default schema, do not print a header\n"
        " -H, --add-header       for no-header delimited, add a header\n"
        " -L, --summarize        allow SELECTion outside of groups.\n"
        " -o, --overwrite        creation of tables can overwrite existing files\n"
        " -O, --override         allow processing of unsupported language features\n"
        " -p, --print            print the processing plan\n"
        " -P, --pipe-factor arg  set multiplier for process pipe size\n"
        " -R, --rec-term arg     set a record terminator for delimited output\n"
        " -s, --in-delim arg     for delimited, specify an input seperator\n"
        " -S, --out-delim arg    for delimited, speficy seperator for SELECT\n"
        //" -t, --thread           Utilize pthreads to run processes in parallel\n"
        " -v, --verbose          print additional information to stderr\n"
        " --in-std arg           for delimited, set input standard. see below...\n"
        " --out-std arg          for delimited, set output standard. see below...\n"
        " --parse-only           just run the parser and output the steps\n"
        " --stable               preserve input order\n"
        " --schema arg           set a schema as default\n"
        " --schema-path arg      takes precedence over FQL_SCHEMA_PATH\n"
        " --strict               see strict rules below...\n\n"

        "Strict mode will only allow exact matches to files to be used as tables.\n"
        "It will also throw errors if you try to select foo from a file with 2:\n"
        "foo,bar,foo\n"
        "a,b,c\n\n"

        "Valid values for --in-std and --out-std, include ALL, RFC4180, WEAK, NONE\n"
        "NONE - Assume no text qualification.\n"
        "WEAK - Allow embedded quotes without duplication. No extra white space.\n"
        "RFC4180 (default) - The most flexible \"standard\" for delimited files.\n"
        "ALL - Quote every field. This option has no effect on input.\n\n"

        "More info: https://www.ietf.org/rfc/rfc4180.txt\n";

void _parse_args(struct fql_handle* handle, int c);
void _print_field(struct fql_field* field);

static int use_api = 0;
static char* query_str = NULL;

int main(int argc, char** argv)
{
	int c = 0;

	struct fql_handle* handle = fql_new();
	if (handle == NULL) {
		fputs("fql failed to initialize\n", stderr);
		exit(EXIT_FAILURE);
	}

	static struct option long_options[] = {
	        /* long option, (no) arg, 0, short option */
	        {"api", no_argument, 0, 'A'},
	        {"char-as-byte", no_argument, 0, 'b'},
	        {"command", required_argument, 0, 'c'},
	        {"cartesian", no_argument, 0, 'C'},
	        {"dry-run", no_argument, 0, 'd'},
	        {"no-header", no_argument, 0, 'h'},
	        {"add-header", no_argument, 0, 'H'},
	        {"summarize", no_argument, 0, 'L'},
	        {"overwrite", no_argument, 0, 'o'},
	        {"override", no_argument, 0, 'O'},
	        {"print", no_argument, 0, 'p'},
	        {"pipe-factor", required_argument, 0, 'P'},
	        {"in-delim", required_argument, 0, 's'},
	        {"out-delim", required_argument, 0, 'S'},
	        {"threading", no_argument, 0, 't'},
	        {"verbose", no_argument, 0, 'v'},
	        {"crlf", no_argument, 0, 'W'},
	        {"help", no_argument, 0, HELP_ARG},
	        {"in-std", required_argument, 0, IN_STD_ARG},
	        {"out-std", required_argument, 0, OUT_STD_ARG},
	        {"parse-only", no_argument, 0, PARSE_ONLY_ARG},
	        {"schema-path", required_argument, 0, SCHEMA_PATH_ARG},
	        {"schema", required_argument, 0, SCHEMA_ARG},
	        {"stable", no_argument, 0, STABLE_ARG},
	        {"strict", no_argument, 0, STRICT_ARG},
	        {0, 0, 0, 0},
	};
	/* getopt_long stores the option index here. */
	int option_index = 0;

	char opt_string[64] = "Abc:CdhHLoOpP:s:S:tvW";
	char* opt_iter = opt_string + strlen(opt_string) - 1;
	*++opt_iter = HELP_ARG;
	*++opt_iter = IN_STD_ARG;
	*++opt_iter = ':';
	*++opt_iter = OUT_STD_ARG;
	*++opt_iter = ':';
	*++opt_iter = PARSE_ONLY_ARG;
	*++opt_iter = SCHEMA_PATH_ARG;
	*++opt_iter = ':';
	*++opt_iter = SCHEMA_ARG;
	*++opt_iter = ':';
	*++opt_iter = STABLE_ARG;
	*++opt_iter = STRICT_ARG;
	*++opt_iter = '\0';

	while ((c = getopt_long(argc, argv, opt_string, long_options, &option_index))
	       != -1)
		_parse_args(handle, c);

	size_t max_size = 1024;
	size_t running_size = 0;
	FILE* query_file = stdin;
	char* line = NULL;

	/* Query is the contents of the file */
	if (optind != argc) {
		query_file = fopen(argv[optind], "r");
		if (query_file == NULL) {
			perror(argv[optind]);
			return EXIT_FAILURE;
		}
		fql_set_allow_stdin(handle, 1);
	}

	if (query_str == NULL && (optind != argc || !isatty(STDIN_FILENO))) {
		/* Found this neat trick on stack overflow for reading entire
		 * contents of file into a buffer...
		 */
		ssize_t bytes = getdelim(&query_str, &max_size, '\0', query_file);
		if (bytes == -1) {
			return EXIT_FAILURE;
		}
	} else if (query_str == NULL) {
		/* Get input from readline if we are interactive */
		query_str = malloc(max_size);
		if (query_str == NULL) {
			perror("malloc");
			return EXIT_FAILURE;
		}
		query_str[0] = '\0';

		/* Send readline output to /dev/tty */
		FILE* devtty = fopen("/dev/tty", "w");
		rl_outstream = devtty;
		do {
			line = readline("");
			if (!line) {
				break;
			}
			size_t n = snprintf(NULL, 0, "%s\n%s", query_str, line);
			running_size += n;
			if (running_size >= max_size) {
				max_size *= 2;
				query_str = realloc(query_str, max_size);
				if (!query_str) {
					perror("realloc");
					return EXIT_FAILURE;
				}
			}
			strcat(query_str, "\n");
			strcat(query_str, line);
		} while (1);
	}

	if (!use_api) {
		if (fql_exec(handle, query_str) == FQL_FAIL) {
			goto err_exit;
		}
		goto success_exit;
	}

	/* Using API will only execute first query.
	 * This is more of a debugging tool and
	 * likely way slower.
	 */
	int plan_count = fql_make_plans(handle, query_str);
	if (plan_count == FQL_FAIL) {
		goto err_exit;
	}

	struct fql_field* fields = NULL;
	int field_count = fql_field_count(handle);

	for (int i = 0; i < plan_count; ++i) {
		int rows = 0;
		while ((rows = fql_step(handle, &fields)) == 1) {
			for (int j = 0; j < field_count; ++j) {
				if (j) {
					putc(',', stdout);
				}
				_print_field(&fields[j]);
			}
			putc('\n', stdout);
		}

		if (rows == FQL_FAIL) {
			goto err_exit;
		}
	}

success_exit:
	if (line != NULL) {
		free(line);
	}
	free(query_str);
	fql_free(handle);
	return EXIT_SUCCESS;

err_exit:
	if (line != NULL) {
		free(line);
	}
	free(query_str);
	fql_free(handle);
	return EXIT_FAILURE;
}

void _parse_args(struct fql_handle* handle, int c)
{
	switch (c) {
	case 'A':
		use_api = 1;
		break;
	case 'b':
		fql_set_char_as_byte(handle, 1);
		break;
	case 'c':
		fql_set_allow_stdin(handle, 1);
		query_str = strdup(optarg);
		break;
	case 'C':
		fql_set_force_cartesian(handle, 1);
		break;
	case 'd':
		fql_set_dry_run(handle, 1);
		break;
	case 'h':
		fql_set_print_header(handle, 0);
		break;
	case 'H':
		fql_set_add_header(handle, 1);
		break;
	case 'L':
		fql_set_loose_groups(handle, 1);
		break;
	case 'o':
		fql_set_overwrite(handle, 1);
		break;
	case 'O':
		fql_set_override_warnings(handle, 1);
		break;
	case 'p':
		fql_set_print_plan(handle, 1);
		break;
	case 'P':
		fql_set_pipe_factor(handle, atoi(optarg));
		break;
	case 's':
		fql_set_in_delim(handle, optarg);
		break;
	case 'S':
		fql_set_out_delim(handle, optarg);
		break;
	case 't':
		fql_set_threading(handle, 1);
		break;
	case 'v':
		fql_set_verbose(handle, 1);
		break;
	case 'W':
		fql_set_crlf_output(handle, 1);
		break;
	case HELP_ARG: /* --help */
		puts(_help);
		exit(EXIT_SUCCESS);
	case IN_STD_ARG: /* --in-std */
		if (fql_set_in_std(handle, optarg) == FQL_FAIL) {
			exit(EXIT_FAILURE);
		}
		break;
	case OUT_STD_ARG: /* --out-std */
		if (fql_set_out_std(handle, optarg) == FQL_FAIL) {
			exit(EXIT_FAILURE);
		}
		break;
	case PARSE_ONLY_ARG: /* --parse-only */
		fql_set_parse_only(handle, 1);
		break;
	case SCHEMA_PATH_ARG: /* --schema-path */
		fql_set_schema_path(handle, optarg);
		break;
	case SCHEMA_ARG: /* --schema */
		fql_set_schema(handle, optarg);
		break;
	case STABLE_ARG: /* --stable */
		fql_set_stable(handle, 1);
		break;
	case STRICT_ARG: /* --strict */
		fql_set_strict_mode(handle, 1);
		break;
	default:
		abort();
	}
}

void _print_field(struct fql_field* field)
{
	switch (field->type) {
	case FQL_INT:
		printf("%ld", field->data.i);
		break;
	case FQL_FLOAT:
		printf("%f", field->data.f);
		break;
	case FQL_STRING:
		fputs(field->data.s, stdout);
		break;
	default:
		fputs("INVALID_TYPE", stdout);
	}
}
