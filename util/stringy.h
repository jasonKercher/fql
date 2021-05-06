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
string* string_construct(string*);
string* string_from_string(string*);
string* string_construct_from_string(string*, string*);
string* string_from_char_ptr(const char*);
string* string_construct_from_char_ptr(string*, const char*);
string* string_from_stringview(struct stringview*);
string* string_construct_from_stringview(string*, struct stringview*);
string* string_take(char*);
string* string_construct_take(string*, char*);

/* equal to vec */
#define string_destroy(s_) vec_destroy(s_)
#define string_get(s_) vec_begin(s_)
#define string_empty(s_) vec_empty(s_)
#define string_append(src_, dest_) vec_extend(src_, dest_)
#define string_resize(s_, size_) vec_resize(s_, size_)
#define string_clear(s_) vec_clear(s_)

/* stringview interface */
void string_copy_from_stringview(string* s, struct stringview*);
void string_append_stringview(string*, struct stringview*);

/* char interface */
void string_push_back(string*, char);
void string_strcat(string*, const char*);
void string_strncat(string*, const char*, size_t);
void string_strcpy(string*, const char*);
void string_strncpy(string*, const char*, size_t);
void string_sprintf(string* s, const char* fmt, ...);

/* string iterface */
void string_copy(string* dest, string* src);

#endif  /* STRINGY_H */
