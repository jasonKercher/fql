#ifndef PARSEONLY_H
#define PARSEONLY_H

#include "gen/TSqlParserBaseListener.h"

class ParseOnly : public TSqlParserBaseListener {
private:
	std::vector<std::string> _rule_names;
public:
	ParseOnly(const std::vector<std::string>&);
	void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override;
	void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override;
};


#endif /* PARSEONLY_H */
