#ifndef STRINGY_H
#define STRINGY_H

#include <stdarg.h>
#include "vec.h"

/**
 * string is a vector of char with one exception:
 * in vec, there is always a trailing element
 * that is denoted as "end". that can contain
 * anything and should not be used. this is the
 * one difference here.  string will also have
 * this trailing element, but it will *always*
 * be a NULL terminator.
 */

struct stringview;

typedef vec string;

/* constructors */
string* string_construct(string* restrict);
string* string_from_string(const string* restrict);
string* string_construct_from_string(string* restrict, const string* restrict);
string* string_from_char_ptr(const char* restrict);
string* string_construct_from_char_ptr(string* restrict, const char* restrict);
string* string_from_stringview(const struct stringview* restrict);
string* string_construct_from_stringview(string* restrict,
                                         const struct stringview* restrict);
string* string_take(char* restrict);
string* string_construct_take(string* restrict, char* restrict);

/* equal to vec */
#define string_destroy(s_)         vec_destroy(s_)
#define string_get(s_)             vec_begin(s_)
#define string_empty(s_)           vec_empty(s_)
#define string_append(src_, dest_) vec_extend(src_, dest_)

/* stringview interface */
void string_copy_from_stringview(string* restrict s, const struct stringview* restrict);
void string_append_stringview(string* restrict, const struct stringview* restrict);
struct stringview string_get_stringview(const string* restrict);

/* char interface */
void string_push_back(string* restrict, char);
size_t string_strcat(string* restrict, const char* restrict);
size_t string_strncat(string* restrict, const char* restrict, size_t);
size_t string_strcpy(string* restrict, const char* restrict);
size_t string_strncpy(string* restrict, const char* restrict, size_t);
size_t string_sprintf(string* restrict s, const char* restrict fmt, ...);
const char* string_c_str(const string* restrict s);

/* string iterface */
char* string_export(string* restrict);
void string_clear(string* restrict);
void string_resize(string* restrict, size_t);
void string_copy(string* restrict dest, const string* restrict src);


/* Find and replace */

const char* string_find_replace_one(string* restrict,
                                    const char* restrict from,
                                    const char* restrict to,
                                    size_t);

void string_find_replace(string* restrict,
                         const char* restrict from,
                         const char* restrict to);

const char* string_find_replace_one_nocase(string* restrict,
                                           const char* restrict from,
                                           const char* restrict to,
                                           size_t);

void string_find_replace_nocase(string* restrict,
                                const char* restrict from,
                                const char* restrict to);

/* These are "one less allocation" options for possibly non-null terminated strings */
const char* string_find_replace_one_limited(string* restrict,
                                            const char* restrict from,
                                            const char* restrict to,
                                            size_t,
                                            unsigned);

void string_find_replace_limited(string* restrict,
                                 const char* restrict from,
                                 const char* restrict to,
                                 unsigned);

const char* string_find_replace_one_nocase_limited(string* restrict,
                                                   const char* restrict from,
                                                   const char* restrict to,
                                                   size_t,
                                                   unsigned);

void string_find_replace_nocase_limited(string*,
                                        const char* restrict from,
                                        const char* restrict to,
                                        unsigned);

#endif /* STRINGY_H */
