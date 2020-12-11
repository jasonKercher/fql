#ifndef STRING_H
#define STRING_H

#include "vec.h"

#include <stdarg.h>

/**
 * String is a vector of char with
 *
 * In vec, there is always a trailing element
 * that is denoted as "end". That can contain
 * anything and should not be used. This is the
 * one difference here.  String will also have
 * this trailing element, but it will *always*
 * be a NULL terminator.
 */

typedef Vec String;

String* string_new();
String* string_init(String*);
String* string_from_char_ptr(const char*);
String* string_take(char*);
#define string_free(s_) { vec_free(s_); }
#define string_get(s_) { vec_begin(s_); }
#define string_empty(s_) { vec_empty(s_); }
void string_push_back(String*, char);
#define string_append(src_, dest_) { vec_extend(src_, dest_); }
void string_cat(String*, const char*);
void string_cpy(String*, const char*);
void string_sprintf(String* s, const char* fmt, ...);

#endif  /* STRING_H */
