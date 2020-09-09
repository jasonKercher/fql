#include "antlr.h"
#include "upperstream.h"
#include "listenerinterface.h"
#include "errorlistener.h"

#include "../plan.h"

int analyze_query(plan_t* plan, const char* query_str)
{

        UpperStream input(query_str);

        TSqlLexer lexer(&input);

        antlr4::CommonTokenStream tokens(&lexer);
        TSqlParser parser(&tokens);

        /* Attach custom error listener for lexer */
        lexer.removeErrorListeners();
        LexerErrorListener myLexerErrorListener(&parser);
        lexer.addErrorListener(&myLexerErrorListener);

        /* Attach custom error listener for parser  */
        parser.removeErrorListeners();
        ParserErrorListener myParserErrorListener(&parser);
        parser.addErrorListener(&myParserErrorListener);

        //if(props.verbose)
        //      std::cerr << "Done Parsing...\n";

        antlr4::tree::ParseTree *tree = parser.tsql_file();

        if(myLexerErrorListener.getError() || myParserErrorListener.getError()) {
                std::cerr << "Lexer/Listener error\n";
                return 1;
        }

        std::vector<std::string> rule_names = parser.getRuleNames();

        ListenerInterface analyzer(plan, rule_names);

        antlr4::tree::ParseTreeWalker::DEFAULT.walk(&analyzer, tree);

        //If (props.dry_run)
        //      exit(0);

        //Auto plans = analyzer.get_plans();

        //TreeListener listener(props.inDelim, props.out_delim, plans);

        //Antlr4::tree::ParseTreeWalker::DEFAULT.walk(&listener, tree);

        return 0;

}
