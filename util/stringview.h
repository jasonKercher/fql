#ifndef STRINGVIEW_H
#define STRINGVIEW_H

#include "stringy.h"

/**
 * StringView is meant to be a read-only
 * string type. The contents should never be
 * direcly be modified. Also, there is no
 * guarantee that the data is NULL terminated.
 */

struct stringview {
	const char* data;
	unsigned len;
};
typedef struct stringview StringView;

struct stringview* stringview_new(char*, unsigned);
#define stringview_new_(s_) { stringview_new(s_, strlen(s_)); }
struct stringview* stringview_construct(struct stringview*, char*, unsigned);
struct stringview* stringview_construct_from_string(struct stringview*, String*);
void stringview_set(struct stringview*, char*);
void stringview_nset(struct stringview* sv, char* s, unsigned n);
void stringview_set_string(struct stringview* sv, String* s);
void stringview_free(struct stringview*);

#endif  /* STRINGVIEW_H */
