#ifndef MISC_H
#define MISC_H

#include <stdlib.h>
#include <stdbool.h>

#define BUFFER_FACTOR 128

/** **/
void increase_buffer(char**, size_t*);
void increase_buffer_to(char**, size_t*, size_t);


#endif
