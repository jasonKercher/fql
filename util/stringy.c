#include "stringy.h"

#include <string.h>
#include "stringview.h"
#include "util.h"

#define _null_terminate_(s_) *((char*)vec_end(s_)) = '\0'

string* string_construct(string* s)
{
	s = vec_construct(s, 1);
	_null_terminate_(s);
	return s;
}

string* string_from_string(string* src)
{
	string* new_string = new_(string);
	string_copy(new_string, src);
	return new_string;
}

string* string_construct_from_string(string* dest, string* src)
{
	string_construct(dest);
	string_copy(dest, src);
	return dest;
}

string* string_from_stringview(struct stringview* sv)
{
	string* new_string = new_(string);
	string_copy_from_stringview(new_string, sv);
	return new_string;
}

string* string_construct_from_stringview(string* s, struct stringview* sv)
{
	string_construct(s);
	string_copy_from_stringview(s, sv);
	return s;
}

string* string_from_char_ptr(const char* src)
{
	string* new_string = new_t_(vec, char);
	string_strcpy(new_string, src);
	return new_string;
}

string* string_construct_from_char_ptr(string* s, const char* cp)
{
	string_construct(s);
	string_strcpy(s, cp);
	return s;
}

string* string_take(char* src)
{
	string* new_string = malloc_(sizeof(*new_string));
	return string_construct_take(new_string, src);
}

string* string_construct_take(string* s, char* src)
{
	size_t len = strlen(src);

	/* I'm making an assumption about _alloc here.
	 * I don't know how much space is allocated,
	 * but it is >= len + 1
	 */
	*s = (string) {
		 src            /* data */
		,len            /* size */
		,len + 1        /* _alloc */
		,1              /* _elem_s */
	};

	return s;
}

void string_copy_from_stringview(string* s, struct stringview* sv)
{
	string_strncpy(s, sv->data, sv->len);
}

void string_append_stringview(string* dest, struct stringview* sv)
{
	size_t index = dest->size;
	vec_resize(dest, dest->size + sv->len);
	void* end = vec_at(dest, index);
	memcpy(end, sv->data, sv->len);
	_null_terminate_(dest);
}

void string_push_back(string* s, char c)
{
	char* back = vec_add_one(s);
	*back = c;
	back[1] = '\0';
}

size_t string_strcat(string* dest, const char* src)
{
	size_t len = strlen(src);
	size_t endidx = dest->size;
	vec_resize(dest, dest->size + len);
	void* end = vec_at(dest, endidx);
	memcpy(end, src, len + 1);
	return len;
}

size_t string_strncat(string* dest, const char* src, size_t n)
{
	size_t len = strnlen(src, n);

	size_t old_size = dest->size;
	string_resize(dest, dest->size + len);
	char* end = vec_at(dest, old_size);
	memcpy(end, src, len);
	_null_terminate_(dest);

	return dest->size - old_size;
}

size_t string_strcpy(string* dest, const char* src)
{
	size_t len = strlen(src);
	string_resize(dest, len);
	memcpy(dest->data, src, len + 1);
	return len;
}

size_t string_strncpy(string* dest, const char* src, size_t limit)
{
	string_resize(dest, limit);
	size_t i = 0;
	for (; src[i] != '\0' && i < limit; ++i) {
		((char*)dest->data)[i] = src[i];
	}
	string_resize(dest, i);
	_null_terminate_(dest);
	return i-1;
}

size_t string_sprintf(string* s, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	va_list args2;
	va_copy(args2, args);
	size_t len = vsnprintf(NULL, 0, fmt, args);
	va_end(args);
	vec_resize(s, len);
	vsnprintf(s->data, len+1, fmt, args2);
	va_end(args2);
	return len;
}

const char* string_c_str(string* s)
{
	return (const char*) s->data;
}

void string_clear(string* s)
{
	vec_clear(s);
	_null_terminate_(s);
}

void string_copy(string* dest, string* src)
{
	vec_resize(dest, src->size);
	memcpy(dest->data, src->data, src->size);
	_null_terminate_(dest);
}

void string_resize(string* s, size_t n)
{
	vec_resize(s, n);
	_null_terminate_(s);
}
