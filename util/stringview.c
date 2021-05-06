#include "stringview.h"

#include "util.h"

stringview* stringview_construct(stringview* sv, char* s, unsigned len)
{
	stringview_nset(sv, s, len);
	return sv;
}

stringview* stringview_construct_from_string(stringview* sv, string* s)
{
	stringview_set_string(sv, s);
	return sv;
}

void stringview_set(stringview* sv, char* s)
{
	*sv = (stringview) {
		 s
		,strlen(s)
	};
}

void stringview_nset(stringview* sv, char* s, unsigned n)
{
	*sv = (stringview) {
		 s
		,n
	};
}

void stringview_set_string(stringview* sv, string* s)
{
	*sv = (stringview) {
		 s->data
		,s->size
	};
}
