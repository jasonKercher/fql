#include "misc.h"
#include "../util/util.h"

void increase_buffer(char** buf, size_t* buflen)
{
        *buflen += BUFFER_FACTOR;
        if (*buflen == BUFFER_FACTOR){
                malloc_(*buf, *buflen);
        } else {
                realloc_(*buf, *buflen);
        }
}

void increase_buffer_to(char** buf, size_t* buflen, size_t target)
{
        target = BUFFER_FACTOR * (target / BUFFER_FACTOR + 1);
        if (*buflen > target)
                return;

        *buflen = target;
        if (*buflen == BUFFER_FACTOR){
                malloc_(*buf, *buflen);
        } else {
                realloc_(*buf, *buflen);
        }
}
