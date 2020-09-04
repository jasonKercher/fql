#include <antlr4-runtime.h>
#include "gen/TSqlParser.h"
#include "gen/TSqlLexer.h"

class LexerErrorListener : public antlr4::BaseErrorListener
{
    private:
        TSqlParser *_owner;
        bool _error = false;

    public:
        LexerErrorListener(TSqlParser *aOwner) : _owner(aOwner) {}
        virtual void syntaxError(antlr4::Recognizer *recognizer, antlr4::Token *offendingSymbol, size_t line, size_t charPositionInLine, const std::string &msg, std::exception_ptr e) override;
        bool getError() {return _error;}
};

class ParserErrorListener : public antlr4::BaseErrorListener
{
    private:
        TSqlParser *_owner;
        bool _error = false;

    public:
        ParserErrorListener(TSqlParser *aOwner) : _owner(aOwner) {}
        virtual void syntaxError(antlr4::Recognizer *recognizer, antlr4::Token *offendingSymbol, size_t line, size_t charPositionInLine, const std::string &msg, std::exception_ptr e) override;
        bool getError() {return _error;}
};
