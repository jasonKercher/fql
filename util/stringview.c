#include "stringview.h"

#include "util.h"

StringView* stringview_new(char* s, unsigned len)
{
	StringView* new_stringview = NULL;
	malloc_(new_stringview, sizeof(*new_stringview));

	return stringview_construct(new_stringview, s, len);
}

StringView* stringview_construct(StringView* sv, char* s, unsigned len)
{
	stringview_nset(sv, s, len);
	return sv;
}

StringView* stringview_construct_from_string(StringView* sv, String* s)
{
	stringview_set_string(sv, s);
	return sv;
}

void stringview_set(StringView* sv, char* s)
{
	*sv = (StringView) {
		 s
		,strlen(s)
	};
}

void stringview_nset(StringView* sv, char* s, unsigned n)
{
	*sv = (StringView) {
		 s
		,n
	};
}

void stringview_set_string(StringView* sv, String* s)
{
	*sv = (StringView) {
		 s->data
		,s->size
	};
}

void stringview_free(StringView* sv)
{
	free_(sv);
}
