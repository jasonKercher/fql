#include "errorlistener.h"

void LexerErrorListener::syntaxError(antlr4::Recognizer* recognizer,
                                     antlr4::Token* offendingSymbol,
                                     size_t line,
                                     size_t charPositionInLine,
                                     const std::string& msg,
                                     std::exception_ptr e)
{
	_error = true;
	if (offendingSymbol)
		std::cerr << "Lexer syntax error near `" << offendingSymbol->getText()
		          << "' Line: " << line << ". Position: " << charPositionInLine
		          << ".\n";
	else
		std::cerr << "Lexer syntax error at Line: " << line
		          << ". Position: " << charPositionInLine << ".\n";
}

void ParserErrorListener::syntaxError(antlr4::Recognizer* recognizer,
                                      antlr4::Token* offendingSymbol,
                                      size_t line,
                                      size_t charPositionInLine,
                                      const std::string& msg,
                                      std::exception_ptr e)
{
	_error = true;
	if (offendingSymbol)
		std::cerr << "Parser syntax error near `" << offendingSymbol->getText()
		          << "' Line: " << line << ". Position: " << charPositionInLine
		          << ".\n";
	else
		std::cerr << "Parser syntax error at Line: " << line
		          << ". Position: " << charPositionInLine << ".\n";
}
