#include "util.h"

#include <stdbool.h>
#include <dirent.h>

int str2longbase(long* restrict ret, const char* restrict s, int base)
{
	char* endptr = NULL;
	errno = 0;
	*ret = strtol(s, &endptr, base);
	if (errno) {
		perror(s);
	} else if (errno == 0 && s && *endptr != 0) {
		fprintf(stderr, "%s: could not convert to integer\n", s);
	} else {
		return 0;
	}

	return 1;
}

int str2long10(long* restrict ret, const char* restrict s)
{
	return str2longbase(ret, s, 10);
}

int str2long(long* restrict ret, const char* restrict s)
{
	if (s[0] == '0' && s[1] == 'x') {
		return str2longbase(ret, s, 16);
	}
	return str2longbase(ret, s, 10);
}

int str2double(double* restrict ret, const char* restrict s)
{
	char* endptr = NULL;
	errno = 0;
	*ret = strtod(s, &endptr);

	if (errno) {
		perror(s);
	} else if (errno == 0 && s && *endptr != 0) {
		fprintf(stderr, "%s: could not convert to float\n", s);
	} else {
		return 0;
	}

	return 1;
}

int charncount(const char* restrict s, char c, unsigned n)
{
	int count = 0;
	unsigned i = 0;
	for(; s[i] != '\0' && i < n; ++i)
		if (s[i] == c)
			++count;

	return count;
}

int charcount(const char* restrict s, char c)
{
	int count = 0;
	int i = 0;
	for(; s[i] != '\0'; ++i)
		if (s[i] == c)
			++count;

	return count;
}

int strhaschar(const char* restrict s, char c)
{
	int i = 0;
	for (; s[i] != '\0'; ++i)
		if (s[i] == c)
			return true;

	return false;
}

void removecharat(char* restrict s, int i)
{
	for (; s[i] != '\0'; ++i)
		s[i] = s[i+1];
	s[i] = '\0';
}

char* randstr(char* restrict s, const int len)
{
	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	int i = 0;

	for (; i < len; ++i)
		s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];

	s[i] = '\0';

	return s;
}

/* https://stackoverflow.com/questions/2736753/how-to-remove-extension-from-file-name
 * converted to leave user in control of allocation
 * assumption: dest space >= filename space
 */
void getnoext(char* dest, const char* restrict filename)
{
	char* lastdot = NULL;
	char* lastsep = NULL;

	if (!filename) {
		dest[0] = '\0';
		return;
	}

	strcpy(dest, filename);
	lastdot = strrchr(dest, '.');
	lastsep = ('/' == 0) ? NULL : strrchr(dest, '/');

	if (lastdot != NULL) {
		if (lastsep != NULL) {
			if (lastsep < lastdot)
				*lastdot = '\0';
		} else {
			*lastdot = '\0';
		}
	}
}


char* getext(char* filename)
{
	char* base = basename(filename);
	const char *dot = strrchr(base, '.');
	if(dot && dot != base) {
		char* ext = malloc_(strlen(dot) + 1);
		strcpy(ext, dot);
		return ext;
	}
	return "";
}

int string_eq(const char* restrict s1, const char* restrict s2)
{
	return !strcmp(s1, s2);
}

int istring_eq(const char* restrict s1, const char* restrict s2)
{
	return !strcasecmp(s1, s2);
}

void string_to_lower(char* restrict s)
{
	for ( ; *s; ++s)
		*s = tolower(*s);
}

char* strnstr(const char* restrict s, const char* restrict find, size_t slen)
{
	char c, sc;
	size_t len;

	if ((c = *find++) != '\0') {
		len = strlen(find);
		do {
			do {
				if (slen-- < 1 || (sc = *s++) == '\0')
					return (NULL);
			} while (sc != c);
			if (len > slen)
				return (NULL);
		} while (strncmp(s, find, len) != 0);
		s--;
	}
	return ((char *)s);
}

node* dir_list_files(const char* restrict dir)
{
	/* read file names into node */
	node* files = NULL;
	DIR *dr;
	struct dirent *en;

	dr = opendir(dir); //open all or present directory
	if (!dr) {
		perror(dir);
		exit(EXIT_FAILURE);
	}

	while ((en = readdir(dr)) != NULL) {
		node_enqueue(&files, strdup(en->d_name));
	}
	closedir(dr);

	return files;
}
