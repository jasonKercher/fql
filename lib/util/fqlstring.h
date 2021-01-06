#ifndef STRING_H
#define STRING_H

#include <stdarg.h>
#include "vec.h"

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

struct stringview;

typedef Vec String;

String* string_new();
String* string_init(String*);
String* string_from_char_ptr(const char*);
String* string_from_stringview(struct stringview*);
void string_copy_from_stringview(String* s, struct stringview*);
String* string_take(char*);
#define string_destroy(s_) { vec_destroy(s_); }
#define string_free(s_) { vec_free(s_); }
#define string_get(s_) { vec_begin(s_); }
#define string_empty(s_) { vec_empty(s_); }
#define string_append(src_, dest_) { vec_extend(src_, dest_); }
#define string_clear(s_) { vec_clear(s_); }
void string_copy_from_stringview(String* s, struct stringview*);
void string_append_stringview(String*, struct stringview*);
void string_push_back(String*, char);
void string_cat(String*, const char*);
void string_cpy(String*, const char*);
void string_sprintf(String* s, const char* fmt, ...);

#endif  /* STRING_H */
