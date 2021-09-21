#include "stringy.h"

#include <string.h>
#include "stringview.h"
#include "util.h"

#define _null_terminate_(s_) *((char*)vec_end(s_)) = '\0'

string* string_construct(string* restrict s)
{
	s = vec_construct(s, 1);
	_null_terminate_(s);
	return s;
}

string* string_from_string(const string* restrict src)
{
	string* new_string = new_(string);
	string_copy(new_string, src);
	return new_string;
}

string* string_construct_from_string(string* restrict dest, const string* restrict src)
{
	string_construct(dest);
	string_copy(dest, src);
	return dest;
}

string* string_from_stringview(const struct stringview* restrict sv)
{
	string* restrict new_string = new_(string);
	string_copy_from_stringview(new_string, sv);
	return new_string;
}

string* string_construct_from_stringview(string* restrict s,
                                         const struct stringview* restrict sv)
{
	string_construct(s);
	string_copy_from_stringview(s, sv);
	return s;
}

stringview string_get_stringview(const string* restrict s)
{
	return (stringview) {s->data, s->size};
}

string* string_from_char_ptr(const char* restrict src)
{
	string* restrict new_string = new_t_(vec, char);
	string_strcpy(new_string, src);
	return new_string;
}

string* string_construct_from_char_ptr(string* restrict s, const char* restrict cp)
{
	string_construct(s);
	string_strcpy(s, cp);
	return s;
}

string* string_take(char* restrict src)
{
	string* restrict new_string = malloc_(sizeof(*new_string));
	return string_construct_take(new_string, src);
}

string* string_construct_take(string* restrict s, char* restrict src)
{
	size_t len = strlen(src);

	/* I'm making an assumption about _alloc here.
	 * I don't know how much space is allocated,
	 * but it is >= len + 1
	 */
	*s = (string) {
	        src,     /* data */
	        len,     /* size */
	        len + 1, /* _alloc */
	        1,       /* _elem_s */
	};

	return s;
}

void string_copy_from_stringview(string* restrict s, const struct stringview* restrict sv)
{
	string_strncpy(s, sv->data, sv->len);
}

void string_append_stringview(string* restrict dest, const struct stringview* restrict sv)
{
	size_t index = dest->size;
	vec_resize(dest, dest->size + sv->len);
	void* end = vec_at(dest, index);
	memcpy(end, sv->data, sv->len);
	_null_terminate_(dest);
}

void string_push_back(string* restrict s, char c)
{
	char* back = vec_add_one(s);
	*back = c;
	back[1] = '\0';
}

size_t string_strcat(string* restrict dest, const char* restrict src)
{
	size_t len = strlen(src);
	size_t endidx = dest->size;
	vec_resize(dest, dest->size + len);
	void* end = vec_at(dest, endidx);
	memcpy(end, src, len + 1);
	return len;
}

size_t string_strncat(string* restrict dest, const char* restrict src, size_t n)
{
	size_t len = strnlen(src, n);

	size_t old_size = dest->size;
	string_resize(dest, dest->size + len);
	char* end = vec_at(dest, old_size);
	memcpy(end, src, len);
	_null_terminate_(dest);

	return dest->size - old_size;
}

size_t string_strcpy(string* restrict dest, const char* restrict src)
{
	size_t len = strlen(src);
	string_resize(dest, len);
	memcpy(dest->data, src, len + 1);
	return len;
}

size_t string_strncpy(string* restrict dest, const char* restrict src, size_t limit)
{
	string_resize(dest, limit);
	size_t i = 0;
	for (; src[i] != '\0' && i < limit; ++i) {
		((char*)dest->data)[i] = src[i];
	}
	string_resize(dest, i);
	_null_terminate_(dest);
	return i - 1;
}

size_t string_sprintf(string* restrict s, const char* restrict fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	va_list args2;
	va_copy(args2, args);
	size_t len = vsnprintf(NULL, 0, fmt, args);
	va_end(args);
	vec_resize(s, len);
	vsnprintf(s->data, len + 1, fmt, args2);
	va_end(args2);
	return len;
}

const char* string_c_str(const string* restrict s)
{
	return (const char*)s->data;
}

char* string_export(string* restrict s)
{
	char* data = s->data;
	string_construct(s);
	return data;
}

void string_clear(string* restrict s)
{
	vec_clear(s);
	_null_terminate_(s);
}

void string_copy(string* restrict dest, const string* restrict src)
{
	vec_resize(dest, src->size);
	memcpy(dest->data, src->data, src->size);
	_null_terminate_(dest);
}

void string_resize(string* restrict s, size_t n)
{
	vec_resize(s, n);
	_null_terminate_(s);
}

const char* string_find_replace_one(string* restrict s,
                                    const char* restrict oldstr,
                                    const char* restrict newstr,
                                    size_t begin_idx)
{
	unsigned newlen = strlen(newstr);
	return string_find_replace_one_limited(s, oldstr, newstr, begin_idx, newlen);
}

const char* string_find_replace_one_limited(string* restrict s,
                                            const char* restrict oldstr,
                                            const char* restrict newstr,
                                            size_t begin_idx,
                                            unsigned newlen)
{
	unsigned oldlen = strlen(oldstr);
	char* begin = vec_at(s, begin_idx);
	char* pos = memmem(begin, s->size, oldstr, oldlen);

	if (pos == NULL) {
		return vec_end(s);
	}

	/* unnecessary optimization */
	if (newlen == oldlen) {
		memcpy(pos, newstr, newlen);
		return pos + newlen;
	}

	size_t pos_idx = vec_get_idx_(s, pos);

	vec_erase(s, pos, oldlen);
	vec_insert(s, pos, newstr, newlen);

	return vec_at(s, pos_idx + newlen);
}


void string_find_replace_limited(string* restrict s,
                                 const char* restrict oldstr,
                                 const char* restrict newstr,
                                 unsigned newlen)
{
	unsigned i = 0;
	for (; i < s->size; ++i) {
		const char* next =
		        string_find_replace_one_limited(s, oldstr, newstr, i, newlen);
		i += next - (const char*)vec_begin(s);
	}
}


void string_find_replace(string* restrict s,
                         const char* restrict oldstr,
                         const char* restrict newstr)
{
	string_find_replace_limited(s, oldstr, newstr, strlen(newstr));
}

/* separate implementation for nocase limited because it must allocate */
const char* string_find_replace_one_nocase(string* restrict s,
                                           const char* restrict oldstr,
                                           const char* restrict newstr,
                                           size_t begin_idx)
{
	unsigned newlen = strlen(newstr);
	return string_find_replace_one_nocase_limited(s,
	                                              oldstr,
	                                              newstr,
	                                              begin_idx,
	                                              newlen);
}

const char* string_find_replace_one_nocase_limited(string* restrict s,
                                                   const char* restrict oldstr,
                                                   const char* restrict newstr,
                                                   size_t begin_idx,
                                                   unsigned newlen)
{
	unsigned oldlen = strlen(oldstr);
	char* begin = vec_at(s, begin_idx);

	/* We will assume oldstr is null terminated.
	 * We expect begin to be... It's a string...
	 */
	char* pos = strcasestr(begin, oldstr);

	if (pos == NULL) {
		return vec_end(s);
	}

	size_t pos_idx = vec_get_idx_(s, pos);

	/* unnecessary optimization */
	if (newlen == oldlen) {
		memcpy(pos, newstr, newlen);
		return pos + newlen;
	}

	vec_erase(s, pos, oldlen);
	vec_insert(s, pos, newstr, newlen);

	return vec_at(s, pos_idx + newlen);
}


void string_find_replace_nocase_limited(string* restrict s,
                                        const char* restrict oldstr,
                                        const char* restrict newstr,
                                        unsigned newlen)
{
	unsigned i = 0;
	for (; i < s->size; ++i) {
		const char* next = string_find_replace_one_nocase_limited(s,
		                                                          oldstr,
		                                                          newstr,
		                                                          i,
		                                                          newlen);
		i += next - (const char*)vec_begin(s);
	}
}


void string_find_replace_nocase(string* restrict s,
                                const char* restrict oldstr,
                                const char* restrict newstr)
{
	string_find_replace_nocase_limited(s, oldstr, newstr, strlen(newstr));
}
