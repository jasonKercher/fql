#include "upperstream.h"
/*
UpperStream::UpperStream(ANTLRInputStream* newStream)
{
    _stream = newStream;
}
*/
/*
void UpperStream::consume()
{
    _stream->consume();
}
*/
size_t UpperStream::LA(ssize_t i)
{
    size_t c = ANTLRInputStream::LA(i);
    if (c <= 0)
        return c;

    return toupper(c);
}
