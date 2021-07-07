#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <getopt.h>
#include <string.h>
#include <stdio.h>

#include <readline/readline.h>

#include "fql.h"

const char* _help =
        "\nUsage fql -[bCdhHOpv] [-sS delim] [--strict] [file.sql]\n\n"

        "This is a text processing program using SQL (Transact-SQL specifically).\n\n"

        "If no file argument is provided, queries are read from stdin. Files are\n"
        "referenced as tables in the query via a priority list. For example, given\n"
        "the following query: 'select * from t1', we match files on these rules:\n\n"

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
        " -b, --char-as-byte   scalar functions assume single-byte encoding\n"
        " -C, --cartesian      use cartesian join algorithm (less memory use)\n"
        " -d, --dry-run        validate and build a plan, but do not execute\n"
        " -h, --no-header      for default schema, do not print a header\n"
        //" -H, --add-header     for no-header delimited, add a header\n"
        " -L, --loose-groups   allow selection of outside of grouping.\n"
        //" -o, --overwrite      creation of tables can overwrite existing files\n"
        " -O, --override       allow processing of unsupported language features\n"
        " -p, --print          print the processing plan\n"
        " -R, --rec-term arg   set a record terminator for delimited output\n"
        " -s, --in-delim arg   for delimited, specify an input seperator\n"
        " -S, --out-delim arg  for delimited, speficy seperator for SELECT\n"
        //" -t, --thread         Utilize pthreads to run processes in parallel\n"
        " -v, --verbose        print additional information to stderr\n"
        " --stable             preserve input order\n"
        " --schema arg         set a schema as default\n"
        " --schema-path arg    takes precedence over FQL_SCHEMA_PATH\n"
        " --strict             see strict rules below...\n\n"

        "Strict mode will only allow exact matches to files to be used as tables.\n"
        "It will also throw errors if you try to select foo from a file with 2:\n"
        "foo,bar,foo\n"
        "a,b,c\n";

void _parse_args(struct fql_handle* handle, char c);
void _print_field(struct fql_field* field);

static int use_api = 0;

void clean_up(int x) { }

int main(int argc, char** argv)
{
	///* Set up signal handling */
	//struct sigaction act;
	///* sigset_t and sa_flags only set to shut up valgrind */
	//sigset_t vg_shutup = { {0} };
	//act.sa_mask = vg_shutup;
	//act.sa_flags = 0;
	//act.sa_handler = clean_up;
	//sigaction(SIGINT, &act, NULL);
	//sigaction(SIGQUIT, &act, NULL);
	//sigaction(SIGTERM, &act, NULL);
	//sigaction(SIGHUP, &act, NULL);

	int c = 0;

	struct fql_handle* handle = fql_new();
	if (handle == NULL) {
		exit(EXIT_FAILURE);
	}

	static struct option long_options[] = {
	        /* long option, (no) arg, 0, short option */
	        //{"verbose", no_argument, 0, 'v'},
	        {"api", no_argument, 0, 'A'},
	        {"char-as-byte", no_argument, 0, 'b'},
	        {"cartesian", no_argument, 0, 'C'},
	        {"dry-run", no_argument, 0, 'd'},
	        {"no-header", no_argument, 0, 'h'},
	        {"add-header", no_argument, 0, 'H'},
	        {"loose-groups", no_argument, 0, 'L'},
	        {"overwrite", no_argument, 0, 'o'},
	        {"override", no_argument, 0, 'O'},
	        {"print", no_argument, 0, 'p'},
	        {"help", no_argument, 0, 'Q'},
	        {"in-delim", required_argument, 0, 's'},
	        {"out-delim", required_argument, 0, 'S'},
	        {"threading", no_argument, 0, 't'},
	        {"verbose", no_argument, 0, 'v'},
	        {"crlf", no_argument, 0, 'W'},
	        {"schema-path", required_argument, 0, 'x'},
	        {"schema", required_argument, 0, 'X'},
	        {"stable", no_argument, 0, 'z'},
	        {"strict", no_argument, 0, 'Z'},
	        {0, 0, 0, 0}};
	/* getopt_long stores the option index here. */
	int option_index = 0;

	while ((c = getopt_long(argc,
	                        argv,
	                        "AbCdhHLOpQs:S:tvWx:X:zZ",
	                        long_options,
	                        &option_index))
	       != -1)
		_parse_args(handle, c);

	size_t max_size = 1024;
	size_t running_size = 0;
	FILE* query_file = stdin;
	char* query = NULL;
	char* line = NULL;

	/* Query is the contents of the file */
	if (optind != argc) {
		query_file = fopen(argv[optind], "r");
		if (query_file == NULL) {
			return EXIT_FAILURE;
		}
	}
	if (optind != argc || !isatty(STDIN_FILENO)) {
		/* Found this neat trick on stack overflow for reading entire
		 * contents of file into a buffer...
		 */
		ssize_t bytes = getdelim(&query, &max_size, '\0', query_file);
		if (bytes == -1) {
			return EXIT_FAILURE;
		}
	} else {
		/* Get input from readline if we are interactive */
		query = malloc(max_size);
		if (query == NULL) {
			perror("malloc");
			return EXIT_FAILURE;
		}
		query[0] = '\0';

		/* Send readline output to /dev/tty */
		FILE* devtty = fopen("/dev/tty", "w");
		rl_outstream = devtty;
		do {
			line = readline("");
			if (!line) {
				break;
			}
			size_t n = snprintf(NULL, 0, "%s\n%s", query, line);
			running_size += n;
			if (running_size >= max_size) {
				max_size *= 2;
				query = realloc(query, max_size);
				if (!query) {
					perror("realloc");
					return EXIT_FAILURE;
				}
			}
			strcat(query, "\n");
			strcat(query, line);
		} while (1);
		//} else {
		//	query = malloc(max_size);
		//	if (query == NULL) {
		//		perror("malloc");
		//		return EXIT_FAILURE;
		//	}
		//	query[0] = '\0';

		//	size_t n = 0;
		//	while (getline(&line, &n, stdin) != -1) {
		//		running_size += n;
		//		if (running_size >= max_size) {
		//			max_size *= 2;
		//			query = realloc(query, max_size);
		//			if (!query) {
		//				perror("realloc");
		//				return EXIT_FAILURE;
		//			}
		//		}
		//		strcat(query, line);
		//	}
		//}
	}

	if (!use_api) {
		if (fql_exec(handle, query) == FQL_FAIL) {
			goto err_exit;
		}
		goto success_exit;
	}

	/* Using API will only execute first query.
	 * This is more of a debugging tool and
	 * likely way slower.
	 */
	int plan_count = fql_make_plans(handle, query);
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
	free(query);
	fql_free(handle);
	return EXIT_SUCCESS;

err_exit:
	if (line != NULL) {
		free(line);
	}
	free(query);
	fql_free(handle);
	return EXIT_FAILURE;
}

void _parse_args(struct fql_handle* handle, char c)
{
	switch (c) {
	case 'A':
		use_api = 1;
		break;
	case 'b':
		fql_set_char_as_byte(handle, 1);
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
		fql_set_add_header(handle, 0);
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
	case 'Q': /* --help */
		puts(_help);
		exit(EXIT_SUCCESS);
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
	case 'x': /* --schema-path */
		fql_set_schema_path(handle, optarg);
		break;
	case 'X': /* --schema */
		fql_set_schema(handle, optarg);
		break;
	case 'z': /* --stable */
		fql_set_stable(handle, 1);
		break;
	case 'Z': /* --strict */
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
