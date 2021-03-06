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
string* string_from_string(const string*);
string* string_construct_from_string(string*, const string*);
string* string_from_char_ptr(const char*);
string* string_construct_from_char_ptr(string*, const char*);
string* string_from_stringview(const struct stringview*);
string* string_construct_from_stringview(string*, const struct stringview*);
string* string_take(char*);
string* string_construct_take(string*, char*);

/* equal to vec */
#define string_destroy(s_) vec_destroy(s_)
#define string_get(s_) vec_begin(s_)
#define string_empty(s_) vec_empty(s_)
#define string_append(src_, dest_) vec_extend(src_, dest_)

/* stringview interface */
void string_copy_from_stringview(string* s, const struct stringview*);
void string_append_stringview(string*, const struct stringview*);
struct stringview string_get_stringview(string*);

/* char interface */
void string_push_back(string*, char);
size_t string_strcat(string*, const char*);
size_t string_strncat(string*, const char*, size_t);
size_t string_strcpy(string*, const char*);
size_t string_strncpy(string*, const char*, size_t);
size_t string_sprintf(string* s, const char* fmt, ...);
const char* string_c_str(const string* s);

/* string iterface */
char* string_export(string*);
void string_clear(string*);
void string_resize(string*, size_t);
void string_copy(string* dest, const string* src);
void string_find_replace(string*, const char* from, const char* to);
const char*
string_find_replace_one(string*, const char* from, const char* to, size_t);

#endif  /* STRINGY_H */
