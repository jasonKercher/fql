#include <antlr4-runtime.h>

class UpperStream : public antlr4::ANTLRInputStream
{
public:
        UpperStream(std::string_view input) : ANTLRInputStream(input) {}
        UpperStream(std::istream &stream) : ANTLRInputStream(stream) {}
        size_t LA(ssize_t i) override;
} ;

