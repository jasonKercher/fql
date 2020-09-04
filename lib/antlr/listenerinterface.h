#ifndef LISTENER_H
#define LISTENER_H

#include "gen/TSqlParserBaseListener.h"

class ListenerInterface : public TSqlParserBaseListener {
private:
        std::vector<std::string> _rule_names;
public:
        ListenerInterface(const std::vector<std::string>&);
        void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override;
        void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override;
};


#endif
