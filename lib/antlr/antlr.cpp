#include "antlr.h"
#include "upperstream.h"
#include "listenerinterface.h"
#include "errorlistener.h"

#include "plan.h"

int analyze_query(struct queue** query_list, const char* query_str)
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

        antlr4::tree::ParseTree *tree = parser.tsql_file();

        if(myLexerErrorListener.getError() || 
           myParserErrorListener.getError()) {
                std::cerr << "Lexer/Listener error\n";
                return 1;
        }

        std::vector<std::string> rule_names = parser.getRuleNames();

        ListenerInterface analyzer(query_list, rule_names);

        antlr4::tree::ParseTreeWalker::DEFAULT.walk(&analyzer, tree);

        return 0;

}
