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

struct stringview*
stringview_construct(struct stringview* restrict, char* restrict, unsigned);
void stringview_set(struct stringview* restrict, const char* restrict);
void stringview_nset(struct stringview* restrict sv, const void* restrict s, unsigned n);
void stringview_set_string(struct stringview* restrict sv, string* restrict s);

int stringview_compare(const struct stringview* restrict,
                       const struct stringview* restrict);
int stringview_compare_nocase(const struct stringview* restrict,
                              const struct stringview* restrict);
int stringview_compare_rtrim(const struct stringview*, const struct stringview*);
int stringview_compare_nocase_rtrim(const struct stringview*, const struct stringview*);

#endif /* STRINGVIEW_H */
