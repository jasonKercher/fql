#ifndef STRINGY_H
#define STRINGY_H

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

/* constructors */
String* string_new();
String* string_construct(String*);
String* string_from_string(String*);
String* string_construct_from_string(String*, String*);
String* string_from_char_ptr(const char*);
String* string_construct_from_char_ptr(String*, const char*);
String* string_from_stringview(struct stringview*);
String* string_construct_from_stringview(String*, struct stringview*);
String* string_take(char*);
String* string_construct_take(String*, char*);

/* equal to vec */
#define string_destroy(s_) vec_destroy(s_)
#define string_free(s_) vec_free(s_)
#define string_get(s_) vec_begin(s_)
#define string_empty(s_) vec_empty(s_)
#define string_append(src_, dest_) vec_extend(src_, dest_)
#define string_resize(s_, size_) vec_resize(s_, size_)
#define string_clear(s_) vec_clear(s_)

/* stringview interface */
void string_copy_from_stringview(String* s, struct stringview*);
void string_append_stringview(String*, struct stringview*);

/* char interface */
void string_push_back(String*, char);
void string_strcat(String*, const char*);
void string_strcpy(String*, const char*);
void string_strncpy(String*, const char*, size_t);
void string_sprintf(String* s, const char* fmt, ...);

/* string iterface */
void string_copy(String* dest, String* src);





#endif  /* STRINGY_H */
