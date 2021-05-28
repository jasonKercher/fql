#include "antlr.h"
#include "upperstream.h"
#include "errorlistener.h"
#include "treewalker.h"
#include "listenerinterface.h"

int analyze_query(struct fql_handle* fql)
{
	/* SQL grammar assumes all keywords are upper case */
	UpperStream input(fql->query_str);

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

	antlr4::tree::ParseTree* tree = parser.tsql_file();

	if (myLexerErrorListener.getError()
	    || myParserErrorListener.getError()) {
		std::cerr << "Lexer/Listener error\n";
		return 1;
	}

	std::vector<std::string> rule_names = parser.getRuleNames();

	TreeWalker walker;

	ListenerInterface analyzer(fql, &walker, rule_names);

	walker.set_walking(true);
	walker.walk(&analyzer, tree);
	walker.set_walking(false);

	return analyzer.get_return_code();
}
