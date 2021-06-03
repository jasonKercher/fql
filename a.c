/**
 * This is a C version of this stackoverflow answer:
 * https://stackoverflow.com/a/34901612/635822
 */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <string.h>

#include "util/stringy.h"
#include "util/stringview.h"
#include "util/vec.h"
#include "util/util.h"

/* return END if none fount */
const char* _replace_one(string* s,
                         const char* oldstr,
                         const char* newstr,
                         unsigned begin_idx)
{
	unsigned oldlen = strlen(oldstr);
	unsigned newlen = strlen(newstr);

	char* begin = vec_at(s, begin_idx);
	char* pos = memmem(begin, s->size, oldstr, oldlen);

	if (pos == NULL) {
		return vec_end(s);
	}

	int idx = pos - begin;

	unsigned i = 0;
	for (; i < oldlen && i < newlen; ++i) {
		begin[idx++] = newstr[i];
	}

	if (i == newlen) {
		return begin + idx;
	}

	if (i < oldlen) {
		vec_erase_at(s, i, oldlen - i);
	} else { /* j < newstr.len */
		vec_insert_at(s, i, &newstr[i], newlen - i);
	}

	return begin + idx;
}

void _replace_all(string* s, const char* oldstr, const char* newstr)
{
	unsigned i = 0;
	for (; i < s->size; ++i) {
		const char* next = _replace_one(s, oldstr, newstr, i);
		i += next - (const char*)vec_begin(s) - 1;
	}
	return;
}

void extract_character_ranges(vec* ranges, string* likebuf)
{
	int range_idx = 0;
	const char* begin = vec_begin(likebuf);
	const char* start = begin;
	char* end = NULL;

	char key[10];

	while ((start = memchr(start, '[', likebuf->size - (start - begin)))
	       && (end = memchr(start, ']', likebuf->size - (start - begin)))) {
		string bracketed = {0};
		string_construct(&bracketed);

		sprintf(key, "[[%d]]", range_idx);
		string_strncpy(&bracketed, start, end - start);
		_replace_one(likebuf,
		             string_c_str(&bracketed),
		             key,
		             start - begin);

		/* in case of realloc */
		unsigned begin_idx = start - begin;
		begin = vec_begin(likebuf);
		start = begin + begin_idx;
		start += strlen(key);

		_replace_all(&bracketed, "[", "\\[");
		_replace_all(&bracketed, "]", "\\]");

		char* bracket_cpy = strdup(string_c_str(&bracketed));
		string_sprintf(&bracketed, "[%s]", bracket_cpy);
		free_(bracket_cpy);

		vec_push_back(ranges, &bracketed);

		++range_idx;
	}

	int open = 0;
	begin = vec_begin(likebuf);
	start = begin;
	const char* search = start;

	do {
		start = memchr(search, '[', likebuf->size - (search - begin));
		end = memchr(search, ']', likebuf->size - (search - begin));

		if (start == NULL && end == NULL) {
			break;
		}

		if (start < end || end == NULL) {
			open++;
			search = start + 1;
		} else {
			if (open <= 0) {
				end[1] = '\\';
				char end_bracket = ']';
				vec_insert_at(likebuf,
				              vec_get_idx_(likebuf, end + 1),
				              &end_bracket,
				              1);
				search = end + 2;
			} else {
				open--;
				search = end + 1;
			}
		}
	} while (search < (const char*)vec_end(likebuf));
}

void like_to_regex(string* regex_buffer,
                   string* like_buffer,
                   vec* ranges,
                   const stringview sqllike)
{
	string_copy_from_stringview(like_buffer, &sqllike);
	_replace_all(like_buffer, ".", "\\.");
	_replace_all(like_buffer, "^", "\\^");
	_replace_all(like_buffer, "$", "\\$");
	_replace_all(like_buffer, "+", "\\+");
	_replace_all(like_buffer, "?", "\\?");
	_replace_all(like_buffer, "(", "\\(");
	_replace_all(like_buffer, ")", "\\)");
	_replace_all(like_buffer, "{", "\\{");
	_replace_all(like_buffer, "}", "\\}");
	_replace_all(like_buffer, "\\", "\\\\");
	_replace_all(like_buffer, "|", "\\|");
	_replace_all(like_buffer, ".", "\\.");
	_replace_all(like_buffer, "*", "\\*");
	extract_character_ranges(ranges, like_buffer);
	_replace_all(like_buffer, "%", ".*");
	_replace_all(like_buffer, "_", ".");

	char key[10];
	unsigned i = 0;
	for (; i < ranges->size; ++i) {
		sprintf(key, "[[%d]]", i);
		_replace_all(like_buffer, key, string_c_str(vec_at(ranges, i)));
	}
	string_sprintf(regex_buffer, "^%s$", string_c_str(like_buffer));
}

int main(int argc, char** argv)
{
	if (argc <= 1) {
		fputs("Not enough args\n", stderr);
		exit(EXIT_FAILURE);
	}

	string* likebuf = new_(string);
	string* rebuf = new_(string);
	vec* ranges = new_t_(vec, string);

	like_to_regex(rebuf,
	              likebuf,
	              ranges,
	              (stringview) {argv[1], strlen(argv[1])});

	puts(string_c_str(rebuf));
}
