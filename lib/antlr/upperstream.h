#include <antlr4-runtime.h>

class UpperStream : public antlr4::ANTLRInputStream
{
    //private:
        //ANTLRInputStream* _stream;

    public:
        //UpperStream(ANTLRInputStream*);
        UpperStream(const std::string& input) : ANTLRInputStream(input) {}

        UpperStream(std::istream &stream) : ANTLRInputStream(stream) {}

        size_t LA(ssize_t i) override;

        //void consume() override;





} ;

