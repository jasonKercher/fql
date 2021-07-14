#include "parseonly.h"

ParseOnly::ParseOnly(const std::vector<std::string>& rules)
{
	_rule_names = rules;
}


void ParseOnly::enterEveryRule(antlr4::ParserRuleContext * ctx)
{
	std::cerr << "ENTER: " << _rule_names[ctx->getRuleIndex()] << " : " << ctx->getText() << std::endl;
}

void ParseOnly::exitEveryRule(antlr4::ParserRuleContext * ctx)
{
	std::cerr << "EXIT:  " << _rule_names[ctx->getRuleIndex()] << " : " << ctx->getText() << std::endl;
}



