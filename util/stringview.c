#include "stringview.h"

#include "util.h"

stringview* stringview_construct(stringview* restrict sv, char* restrict s, unsigned len)
{
	stringview_nset(sv, s, len);
	return sv;
}

void stringview_set(stringview* restrict sv, const char* restrict s)
{
	*sv = (stringview) {s, strlen(s)};
}

void stringview_nset(stringview* restrict sv, const void* restrict s, unsigned n)
{
	*sv = (stringview) {s, n};
}

void stringview_set_string(stringview* restrict sv, string* restrict s)
{
	*sv = (stringview) {s->data, s->size};
}

int stringview_compare_nocase(const stringview* restrict sv0,
                              const stringview* restrict sv1)
{
	int len = (sv0->len > sv1->len) ? sv1->len : sv0->len;
	int ret = num_compare_(sv0->len, sv1->len);
	int maybe_ret = strncasecmp(sv0->data, sv1->data, len);
	if (maybe_ret) {
		return maybe_ret;
	}
	return ret;
}

int stringview_compare(const stringview* restrict sv0, const stringview* restrict sv1)
{
	int len = (sv0->len > sv1->len) ? sv1->len : sv0->len;
	int ret = num_compare_(sv0->len, sv1->len);
	int maybe_ret = strncmp(sv0->data, sv1->data, len);
	if (maybe_ret) {
		return maybe_ret;
	}
	return ret;
}

int stringview_compare_rtrim(const stringview* sv0, const stringview* sv1)
{
	const stringview* short_sv = sv0;
	const stringview* long_sv = sv1;
	if (sv0->len > sv1->len) {
		short_sv = sv1;
		long_sv = sv0;
	}

	const unsigned char *s0 = (unsigned char*) sv0->data;
	const unsigned char *s1 = (unsigned char*) sv1->data;

	int ret = 0;
	unsigned i = 0;
	for (; ret == 0 && i < short_sv->len; ++i) {
		ret = s0[i] - s1[i];
	}

	/* All remaining bytes must be white space to stay truthy */
	const unsigned char *long_str = (unsigned char*) long_sv->data;

	for (; ret == 0 && i < long_sv->len; ++i) {
		ret = (isspace(long_str[i])) ? 0 : sv0->len - sv1->len;
	}
	return ret;
}

int stringview_compare_nocase_rtrim(const stringview* sv0, const stringview* sv1)
{
	const stringview* short_sv = sv0;
	const stringview* long_sv = sv1;
	if (sv0->len > sv1->len) {
		short_sv = sv1;
		long_sv = sv0;
	}

	const unsigned char* s0 = (unsigned char*)sv0->data;
	const unsigned char* s1 = (unsigned char*)sv1->data;

	int ret = 0;
	unsigned i = 0;
	for (; ret == 0 && i < short_sv->len; ++i) {
		ret = tolower (s0[i]) - tolower (s1[i]);
	}

	/* All remaining bytes must be white space to stay truthy */
	const unsigned char *long_str = (unsigned char*) long_sv->data;

	for (; ret == 0 && i < long_sv->len; ++i) {
		ret = (isspace(long_str[i])) ? 0 : sv0->len - sv1->len;
	}
	return ret;
}

