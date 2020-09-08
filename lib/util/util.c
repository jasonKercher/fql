#include "util.h"

long stringtolong10(const char* s)
{
        return stringtolong(s, 10);
}

long stringtolong(const char* s, int base)
{
        char* endPtr = NULL;
        errno = 0;
        long val = strtol(s, &endPtr, base);
        //if (s == endPtr)
        if ((errno == ERANGE && (val == LONG_MIN || val == LONG_MAX))
                        || (errno == EINVAL)
                        || (errno != 0 && val == 0)
                        || (errno == 0 && s && *endPtr != 0))
                perror("strtol");
        else
                return val;

        exit(EXIT_FAILURE);
}

int charcount(const char* s, char c)
{
        int count = 0;
        int i = 0;
        for(; s[i] != '\0'; ++i)
                if (s[i] == c)
                        ++count;

        return count;
}

int strhaschar(const char* s, char c)
{
        int i = 0;
        for (; s[i] != '\0'; ++i)
                if (s[i] == c)
                        return TRUE;

        return FALSE;
}

void removecharat(char* s, int i)
{
        for (; s[i] != '\0'; ++i)
                s[i] = s[i+1];
        s[i] = '\0';
}

char* randstr(char* s, const int len)
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

/* https://stackoverflow.com/questions/2736753/how-to-remove-extension-from-file-name */
char* getnoext(const char* filename)
{
        char* retstr = NULL;
        char* lastdot = NULL;
        char* lastsep = NULL;

        if (!filename)
                return NULL;
        if (!(retstr = malloc(strlen(filename) + 1)))
                return NULL;

        strncpy(retstr, filename, PATH_MAX);
        lastdot = strrchr(retstr, '.');
        lastsep = ('/' == 0) ? NULL : strrchr(retstr, '/');

        if (lastdot != NULL) {
                if (lastsep != NULL) {
                        if (lastsep < lastdot)
                                *lastdot = '\0';
                } else {
                        *lastdot = '\0';
                }
        }

        return retstr;
}


char* getext(char* filename)
{
        char* base = basename(filename);
        const char *dot = strrchr(base, '.');
        if(dot && dot != base) {
                char* ext = NULL;
                malloc_(ext, strlen(dot) + 1);
                strcpy(ext, dot);
                return ext;
        }
        return "";
}

int string_eq(const char* s1, const char* s2)
{
        return !strcmp(s1, s2);
}

int istring_eq(const char* s1, const char* s2)
{
        return !strcasecmp(s1, s2);
}

void string_to_lower(char* s)
{
        for ( ; *s; ++s)
                *s = tolower(*s);
}

char * strnstr(const char *s, const char *find, size_t slen)
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
