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

struct stringview* stringview_construct(struct stringview*, char*, unsigned);
void stringview_set(struct stringview*, const char*);
void stringview_nset(struct stringview* sv, const char* s, unsigned n);
void stringview_set_string(struct stringview* sv, string* s);

int stringview_compare(const struct stringview* sv0,
                       const struct stringview* sv1);
int stringview_compare_nocase(const struct stringview* sv0,
                              const struct stringview* sv1);
int stringview_compare_rtrim(const struct stringview* sv0,
                             const struct stringview* sv1);
int stringview_compare_nocase_rtrim(const struct stringview* sv0,
                                    const struct stringview* sv1);

#endif /* STRINGVIEW_H */
