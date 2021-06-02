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
#include "util/util.h"

void _replace_all(string* s, const stringview oldstr, const stringview newstr)
{
	if (oldstr.len > s->size) {
		return;
	}

	char* cstr = s->data;
	unsigned i = 0;
	for (; i < s->size; ++i) {
		char* pos = memmem(cstr, s->size, oldstr.data, oldstr.len);

		if (pos == NULL) {
			return;
		}

		i = pos - cstr;

		unsigned j = 0;
		for (; j < oldstr.len && j < newstr.len; ++j) {
			cstr[i++] = newstr.data[j];
		}

		if (j == newstr.len) {
			continue;
		}

		if (j < oldstr.len) {
			while (j++ < oldstr.len) {
				vec_remove(s, i);
			}
		} else { /* j < newstr.len */
			while (j < newstr.len) {
				vec_insert(s, i++, &newstr.data[j++]);
			}
		}
	}
	return;
}

void extract_character_ranges(vec* ranges, string* str)
{

	//std::map<std::string, std::string> ranges;

	int rangeID = 0;
	//std::string::size_type startPos = 0;
	//std::string::size_type endPos = 0;

	//while ((startPos = str.find("[", startPos)) != std::string::npos && (endPos = str.find("]", startPos + 1)) != std::string::npos)
	//{
	//    std::stringstream ss;
	//    ss << "[[" << rangeID << "]]";
	//    std::string chars = str.substr(startPos + 1, endPos - startPos - 1);
	//    str.replace(startPos, chars.size() + 2, ss.str());
	//    rangeID++;
	//    startPos += ss.str().size();

	//    replace_all(chars, "[", "\\[");
	//    replace_all(chars, "]", "\\]");
	//    ranges[ss.str()] = "[" + chars + "]";
	//}

	//int open = 0;
	//std::string::size_type searchPos = 0;
	//startPos = 0; endPos = 0;
	//do
	//{
	//    startPos = str.find("[", searchPos);
	//    endPos = str.find("]", searchPos);

	//    if (startPos == std::string::npos && endPos == std::string::npos)
	//        break;

	//    if (startPos < endPos || endPos == std::string::npos)
	//    {
	//        open++;
	//        searchPos = startPos + 1;
	//    }
	//    else
	//    {
	//        if (open <= 0)
	//        {
	//            str.replace(endPos, 1, "\\]");
	//            searchPos = endPos + 2;
	//        }
	//        else
	//        {
	//            open--;
	//            searchPos = endPos + 1;
	//        }
	//    }
	//} while (searchPos < str.size());
	//return ranges;
}

void like_to_regex(string* regex_buffer,
                   string* like_buffer,
                   vec* ranges,
                   const stringview* sqllike)
{
	string_copy_from_stringview(like_buffer, sqllike);
	_replace_all(like_buffer,
	             (stringview) {".", 1},
	             (stringview) {"\\.", 2});
	_replace_all(like_buffer,
	             (stringview) {"^", 1},
	             (stringview) {"\\^", 2});
	_replace_all(like_buffer,
	             (stringview) {"$", 1},
	             (stringview) {"\\$", 2});
	_replace_all(like_buffer,
	             (stringview) {"+", 1},
	             (stringview) {"\\+", 2});
	_replace_all(like_buffer,
	             (stringview) {"?", 1},
	             (stringview) {"\\?", 2});
	_replace_all(like_buffer,
	             (stringview) {"(", 1},
	             (stringview) {"\\(", 2});
	_replace_all(like_buffer,
	             (stringview) {")", 1},
	             (stringview) {"\\)", 2});
	_replace_all(like_buffer,
	             (stringview) {"{", 1},
	             (stringview) {"\\{", 2});
	_replace_all(like_buffer,
	             (stringview) {"}", 1},
	             (stringview) {"\\}", 2});
	_replace_all(like_buffer,
	             (stringview) {"\\", 1},
	             (stringview) {"\\\\", 2});
	_replace_all(like_buffer,
	             (stringview) {"|", 1},
	             (stringview) {"\\|", 2});
	_replace_all(like_buffer,
	             (stringview) {".", 1},
	             (stringview) {"\\.", 2});
	_replace_all(like_buffer,
	             (stringview) {"*", 1},
	             (stringview) {"\\*", 2});
	extract_character_ranges(like_buffer,
	                         ranges); //Escapes [ and ] where necessary
	_replace_all(like_buffer,
	             (stringview) {"%", 1},
	             (stringview) {".*", 1});
	_replace_all(like_buffer, (stringview) {"_", 1}, (stringview) {".", 1});
	//for (auto& range : ranges)
	//{
	//    replace_all(sqllike, range.first, range.second);
	//}
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
	vec* ranges = new_t_(vec, stringview);

	//like_to_regex(rebuf, likebuf, ranges, (stringview){argv[1],strlen(argv[1])});
}
