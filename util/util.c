#include "util.h"

#include <stdbool.h>
#include <dirent.h>

#include "queue.h"

int str2longbase(long* ret, const char* s, int base)
{
        char* endPtr = NULL;
        errno = 0;
        *ret = strtol(s, &endPtr, base);
        if ((errno == ERANGE && (*ret == LONG_MIN || *ret == LONG_MAX))
                        || (errno == EINVAL)
                        || (errno != 0 && *ret == 0)
                        || (errno == 0 && s && *endPtr != 0)) {
                perror(s);
                return 1;
        }

        return 0;
}

int str2long10(long* ret, const char* s)
{
        return str2longbase(ret, s, 10);
}

int str2long(long* ret, const char* s)
{
        if (s[0] == '0' && s[1] == 'x') {
                return str2longbase(ret, s, 16);
        }
        return str2longbase(ret, s, 10);
}

int str2double(double* ret, const char* s)
{
        char* endPtr = NULL;
        errno = 0;
        *ret = strtod(s, &endPtr);

        if (errno || (errno == 0 && s && *endPtr != 0)) {
                perror(s);
                return 1;
        }

        return 0;
}

int charncount(const char* s, char c, unsigned n)
{
        int count = 0;
        int i = 0;
        for(; s[i] != '\0' && i < n; ++i)
                if (s[i] == c)
                        ++count;

        return count;
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
                        return true;

        return false;
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

/* https://stackoverflow.com/questions/2736753/how-to-remove-extension-from-file-name
 * converted to leave user in control of allocation
 * Assumption: dest space >= filename space
 */
void getnoext(char* dest, const char* filename)
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

Queue* dir_list_files(const char* dir)
{
        /* Read file names into queue */
        Queue* files = NULL;
        DIR *dr;
        struct dirent *en;

        dr = opendir(dir); //open all or present directory
        if (!dr) {
                perror(dir);
                exit(EXIT_FAILURE);
        }

        while ((en = readdir(dr)) != NULL) {
                queue_enqueue(&files, strdup(en->d_name));
        }
        closedir(dr);

        return files;
}

