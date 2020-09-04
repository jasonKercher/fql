/* Verbose output here. */
void ListenerInterface::enterEveryRule(antlr4::ParserRuleContext * ctx)
{
    //if(_verbose)
    //    std::cerr << "ENTER: " << _ruleNames[ctx->getRuleIndex()] << " : " << ctx->getText() << std::endl;

    //if(_errorTokens.size() > 0)
    //{
    //    std::cerr << "The following keyword parsed correctly, but is currently not supported:\n";
    //    std::cerr << _errorTokens[0];
    //    _errorTokens.pop_back();

    //    if(_overrideWarnings)
    //        std::cerr << "\nCAUTION: Overriding the above warnings!  Results may be incorrect!\n";
    //    else
    //        clean_exit("\nResults will not be printed due to the above warnings.  Use the -o option to Override at your own risk.\n");
    //}
}

void ListenerInterface::exitEveryRule(antlr4::ParserRuleContext * ctx)
{
    //if(_verbose)
    //    std::cerr << "EXIT:  " << _ruleNames[ctx->getRuleIndex()] << " : " << ctx->getText() << std::endl;
}


