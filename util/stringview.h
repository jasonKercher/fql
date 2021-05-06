#ifndef STRINGVIEW_H
#define STRINGVIEW_H

#include "stringy.h"

/**
 * stringview is meant to be a read-only
 * string type. the contents should never be
 * direcly be modified. also, there is no
 * guarantee that the data is NULL terminated.
 */

struct stringview {
	const char* data;
	unsigned len;
};
typedef struct stringview stringview;

//struct stringview* stringview_new(char*, unsigned);
//#define stringview_new_(s_) { stringview_new(s_, strlen(s_)); }
#define stringview_construct(s_) { stringview_new(s_, strlen(s_)); }
struct stringview* stringview_construct_s(struct stringview*, char*, unsigned);
struct stringview* stringview_construct_from_string(struct stringview*, string*);
void stringview_set(struct stringview*, char*);
void stringview_nset(struct stringview* sv, char* s, unsigned n);
void stringview_set_string(struct stringview* sv, string* s);
void stringview_free(struct stringview*);

#endif  /* STRINGVIEW_H */
