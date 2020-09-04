#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <getopt.h>
#include <stdio.h>
//#include <antlr4-runtime.h>

//#include "analyzer.h"
//#include "listener.h"
//#include "antlr/TSqlLexer.h"
//#include "antlr/ErrorListener.h"
//#include "antlr/UpperStream.h"
//#include "antlr/TSqlParser.h"

#include <readline/readline.h>

#include "fql.h"
#include "util.h"

struct properties {
    char in_delim[32];
    char out_delim[32];
    int verbose;
    int dry_run;
    int override_warnings;
};

static const char* help_string = "\n No Help here !\n\n";

void parseargs(char c, struct properties* props)
{
    switch (c) {
    case 'h':
        puts(help_string);
        exit(0);
    case 'v':
        props->verbose = TRUE;
        break;
    case 'O':
        props->override_warnings = TRUE;
        break;
    case 'D':
        props->dry_run = TRUE;
        props->verbose = TRUE;
        break;
    case 's':
        strncpy_(props->in_delim, optarg, 32);
        break;
    case 'S':
        strncpy_(props->out_delim, optarg, 32);
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

    struct properties props = {
         ""
        ,""
        ,FALSE
        ,FALSE
        ,FALSE
    };

    int c = 0;

    static struct option long_options[] =
    {
        /* long option, (no) arg, 0, short option */
        //{"verbose", no_argument, 0, 'v'},
        {"help", no_argument, 0, 'h'},
        {"verbose", no_argument, 0, 'v'},
        {"override-warnings", no_argument, 0, 'O'},
        {"dry-run",no_argument, 0, 'D' },
        {"in-delimiter", required_argument, 0, 's'},
        {"out-delimiter", required_argument, 0, 'S'},
        {0, 0, 0, 0}
    };
    /* getopt_long stores the option index here. */
    int option_index = 0;

    while ( (c = getopt_long (argc, argv, "hvODs:S:",
                    long_options, &option_index)) != -1)
            parseargs(c, &props);


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
        } while(TRUE);
    } else {
        size_t n = 0;
        while(getline(&line, &n, stdin) != -1)
            strcat(query, line);
    }

    free(line);

    puts(query);

    //UpperStream input(fromStdin);

    //TSqlLexer lexer(&input);

    //antlr4::CommonTokenStream tokens(&lexer);
    //TSqlParser parser(&tokens);

    ///* Attach custom error listener for lexer */
    //lexer.removeErrorListeners();
    //LexerErrorListener myLexerErrorListener(&parser);
    //lexer.addErrorListener(&myLexerErrorListener);

    ///* Attach custom error listener for parser  */
    //parser.removeErrorListeners();
    //ParserErrorListener myParserErrorListener(&parser);
    //parser.addErrorListener(&myParserErrorListener);

    //if(props.verbose)
    //    std::cerr << "Done Parsing...\n";

    //antlr4::tree::ParseTree *tree = parser.tsql_file();

    //if(myLexerErrorListener.getError() || myParserErrorListener.getError())
    //    clean_exit("Lexer/Listener error.\n");

    //std::vector<std::string> ruleNames = parser.getRuleNames();

    //QueryAnalyzer analyzer(ruleNames, props.override_warnings, props.verbose);

    //antlr4::tree::ParseTreeWalker::DEFAULT.walk(&analyzer, tree);

    //if (props.dry_run)
    //    exit(0);

    //auto plans = analyzer.get_plans();

    //TreeListener listener(props.inDelim, props.out_delim, plans);

    //antlr4::tree::ParseTreeWalker::DEFAULT.walk(&listener, tree);
}
