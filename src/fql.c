#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <getopt.h>
#include <string.h>
#include <stdio.h>

#include <readline/readline.h>

#include "fql.h"

void _parse_args(struct fql_handle* handle, char c);
void _print_field(struct fql_field* field);

static const char* help_string = "\n No Help here !\n\n";

static int use_api = 0;

void clean_up(int x)
{
}

int main (int argc, char **argv)
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

	static struct option long_options[] =
	{
		/* long option, (no) arg, 0, short option */
		//{"verbose", no_argument, 0, 'v'},
		{"api", no_argument, 0, 'A'},
		{"char-as-byte", no_argument, 0, 'b'},
		{"force-cartesian", no_argument, 0, 'C'},
		{"dry-run", no_argument, 0, 'd'},
		{"no-header", no_argument, 0, 'h'},
		{"help", no_argument, 0, 'H'},
		{"override-warnings", no_argument, 0, 'O'},
		{"print-plan", no_argument, 0, 'p'},
		{"in-delimiter", required_argument, 0, 's'},
		{"out-delimiter", required_argument, 0, 'S'},
		{"threading", no_argument, 0, 't'},
		{"verbose", no_argument, 0, 'v'},
		{0, 0, 0, 0}
	};
	/* getopt_long stores the option index here. */
	int option_index = 0;

	while ( (c = getopt_long (argc, argv, "AbCdhHOps:S:tv",
					long_options, &option_index)) != -1)
			_parse_args(handle, c);


	char query[1024] = "";
	char* line = NULL;

	/* Get input from readline if we are interactive */
	if(isatty(STDIN_FILENO)) {
		/* Send readline output to /dev/tty */
		FILE* devtty = fopen("/dev/tty", "w");
		rl_outstream = devtty;
		do {
			line = readline("");
			if(line)
				sprintf(query, "%s\n%s", query, line);
			else
				break;
		} while(1);
	} else {
		size_t n = 0;
		while(getline(&line, &n, stdin) != -1)
			strcat(query, line);
	}

	free(line);

	int ret = 0;

	if (!use_api) {
		return fql_exec(handle, query);
	}


	/* Using API will only execute first query.
	 * This is more of a debugging tool and
	 * likely way slower.
	 */
	int plan_count = fql_make_plans(handle, query);
	if (plan_count == FQL_FAIL) {
		exit(EXIT_FAILURE);
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
			exit(EXIT_FAILURE);
		}
	}
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
	case 'H':
		puts(help_string);
		exit(0);
	case 'h':
		fql_set_print_header(handle, 0);
		break;
	case 'O':
		fql_set_override_warnings(handle, 1);
		break;
	case 'p':
		fql_set_print_plan(handle, 1);
		break;
	case 's':
		fql_set_in_delim(handle, optarg);
		break;
	case 'S':
		fql_set_out_delim(handle, optarg);
		break;
	case 'v':
		fql_set_verbose(handle, 1);
		break;
	case 't':
		fql_set_threading(handle, 1);
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
