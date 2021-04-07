#include "upperstream.h"

size_t UpperStream::LA(ssize_t i)
{
	size_t c = ANTLRInputStream::LA(i);
	if (c <= 0)
		return c;

	return toupper(c);
}
