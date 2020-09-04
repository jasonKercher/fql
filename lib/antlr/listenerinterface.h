#ifndef LISTENER_H
#define LISTENER_H

#include "gen/TSqlParserBaseListener.h"

class ListenerInterface : public TSqlParserBaseListener {
public:
    void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override;
    void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override;
};


#endif
