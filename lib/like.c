#include "logic.h"
#include <string.h>
#include <stdio.h>

like* like_construct(like* self)
{
	string_construct(&self->like_buffer);
	string_construct(&self->regex_buffer);
	vec_construct_(&self->ranges, string);
	self->regex = NULL;
	return self;
}

void like_destroy(like* self)
{
	string* it = vec_begin(&self->ranges);
	for (; it != vec_end(&self->ranges); ++it) {
		string_destroy(it);
	}
	string_destroy(&self->like_buffer);
	string_destroy(&self->regex_buffer);
	if (self->regex != NULL) {
		pcre2_match_data_free(self->match);
		pcre2_code_free(self->regex);
	}
}

/* This is just a C version of the a C++ solution:
 * https://stackoverflow.com/a/34901612/635822
 */
void _extract_character_ranges(vec* ranges, string* likebuf)
{
	int range_idx = 0;
	const char* begin = vec_begin(likebuf);
	const char* start = begin;
	char* end = NULL;

	char key[32];

	while ((start = memchr(start, '[', likebuf->size - (start - begin)))
	       && (end = memchr(start, ']', likebuf->size - (start - begin)))) {
		string bracketed = {0};
		string_construct(&bracketed);
		string_strncpy(&bracketed, start, end - start + 1);

		sprintf(key, "[[%d]]", range_idx);
		string_find_replace_one(likebuf,
		                        string_c_str(&bracketed),
		                        key,
		                        start - begin);

		/* in case of realloc */
		unsigned begin_idx = start - begin;
		begin = vec_begin(likebuf);
		start = begin + begin_idx;
		start += strlen(key);

		/* Take surrounding brackets out */
		vec_erase_at(&bracketed, 0, 1);
		char* b_end = vec_back(&bracketed);
		vec_erase_one(&bracketed, b_end);

		/* Escaple all brackets left */
		string_find_replace(&bracketed, "[", "\\[");
		string_find_replace(&bracketed, "]", "\\]");

		/* AND put them back in */
		char bracket = '[';
		vec_insert_at(&bracketed, 0, &bracket, 1);
		bracket = ']';
		vec_insert_at(&bracketed, bracketed.size, &bracket, 1);

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

		if ((start && start < end) || end == NULL) {
			open++;
			search = start + 1;
		} else {
			if (open <= 0) {
				end[0] = '\\';
				char end_bracket = ']';
				vec_insert(likebuf, end + 1, &end_bracket, &end_bracket);
				search = end + 3;
			} else {
				open--;
				search = end + 1;
			}
		}
	} while (search < (const char*)vec_end(likebuf));
}

int like_to_regex(like* self, const stringview sql_like)
{
	//fprintf(stderr, "LIKE: %.*s\n", sql_like.len, sql_like.data);

	string* likebuf = &self->like_buffer;
	string_copy_from_stringview(likebuf, &sql_like);
	string_find_replace(likebuf, "\\", "\\\\");
	string_find_replace(likebuf, ".", "\\.");
	string_find_replace(likebuf, "^", "\\^");
	string_find_replace(likebuf, "$", "\\$");
	string_find_replace(likebuf, "+", "\\+");
	string_find_replace(likebuf, "?", "\\?");
	string_find_replace(likebuf, "(", "\\(");
	string_find_replace(likebuf, ")", "\\)");
	string_find_replace(likebuf, "{", "\\{");
	string_find_replace(likebuf, "}", "\\}");
	string_find_replace(likebuf, "|", "\\|");
	string_find_replace(likebuf, "*", "\\*");
	_extract_character_ranges(&self->ranges, likebuf);
	string_find_replace(likebuf, "%", ".*");
	string_find_replace(likebuf, "_", ".");

	char key[32];
	unsigned i = 0;
	for (; i < self->ranges.size; ++i) {
		sprintf(key, "[[%d]]", i);
		string_find_replace(likebuf, key, string_c_str(vec_at(&self->ranges, i)));
	}

	string_sprintf(&self->regex_buffer, "^%s$", string_c_str(likebuf));

	int pcre_error = 0;
	PCRE2_SIZE pcre_error_offset = 0;
	self->regex = pcre2_compile((PCRE2_SPTR)string_c_str(&self->regex_buffer),
	                            self->regex_buffer.size,
	                            PCRE2_CASELESS,
	                            &pcre_error,
	                            &pcre_error_offset,
	                            NULL);
	if (self->regex == NULL) {
		fprintf(stderr,
		        "Failed to generate regular expression from `%.*s'\n",
		        sql_like.len,
		        sql_like.data);
		return FQL_FAIL;
	}

	self->match = pcre2_match_data_create_from_pattern(self->regex, NULL);
	//fprintf(stderr, "REGEX: %s\n", string_c_str(&self->regex_buffer));

	return FQL_GOOD;
}
