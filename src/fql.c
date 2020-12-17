#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <getopt.h>
#include <string.h>
#include <stdio.h>

#include <readline/readline.h>

#include "fql.h"

static const char* help_string = "\n No Help here !\n\n";

void parseargs(char c)
{
        switch (c) {
        case 'D':
                fql_set_dry_run(1);
                break;
        case 'h':
                puts(help_string);
                exit(0);
        case 'O':
                fql_set_override_warnings(1);
                break;
        case 'p':
                fql_set_print_plan(1);
                break;
        case 's':
                fql_set_in_delim(optarg);
                break;
        case 'S':
                fql_set_out_delim(optarg);
                break;
        case 'v':
                fql_set_verbose(1);
                break;
        default:
                abort();
        }
}

void clean_up(int x)
{
}

int main (int argc, char **argv)
{
        /* Set up signal handling */
        struct sigaction act;
        /* sigset_t and sa_flags only set to shut up valgrind */
        sigset_t vg_shutup = { {0} };
        act.sa_mask = vg_shutup;
        act.sa_flags = 0;
        act.sa_handler = clean_up;
        sigaction(SIGINT, &act, NULL);
        sigaction(SIGQUIT, &act, NULL);
        sigaction(SIGTERM, &act, NULL);
        sigaction(SIGHUP, &act, NULL);

        int c = 0;

        fql_init();

        static struct option long_options[] =
        {
                /* long option, (no) arg, 0, short option */
                //{"verbose", no_argument, 0, 'v'},
                {"dry-run",no_argument, 0, 'D' },
                {"help", no_argument, 0, 'h'},
                {"override-warnings", no_argument, 0, 'O'},
                {"print-plan", no_argument, 0, 'p'},
                {"in-delimiter", required_argument, 0, 's'},
                {"out-delimiter", required_argument, 0, 'S'},
                {"verbose", no_argument, 0, 'v'},
                {0, 0, 0, 0}
        };
        /* getopt_long stores the option index here. */
        int option_index = 0;

        while ( (c = getopt_long (argc, argv, "hDOps:S:v",
                                        long_options, &option_index)) != -1)
                        parseargs(c);


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

        int ret = fql_exec(query);

        free(line);

        return ret;
}
