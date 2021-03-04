#ifndef UTIL_H
#define UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <libgen.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>

#include "queue.h"

typedef void(*generic_data_fn)(void*);
typedef int(*int_generic_data_fn)(void*);


/**
 * malloc wrapper that does error checking
 */
#define malloc_(dest_, size_) {         \
        dest_ = malloc(size_);          \
        if (!dest_) {                   \
                perror("malloc");       \
                exit(EXIT_FAILURE);     \
        }                               \
}

/**
 * realloc wrapper that does error checking
 */
#define realloc_(dest_, size_) {                \
        void* new_dest_ = realloc(dest_, size_);\
        if (!new_dest_) {                       \
                perror("realloc");              \
                exit(EXIT_FAILURE);             \
        }                                       \
        dest_ = new_dest_;                      \
        new_dest_ = NULL;                       \
}

/**
 * Wrapper macro for non-posix function strdup
 * strdup allocates memory for us. If dest is already
 * allocated memory, free it first.
 */
#define strdup_(src_, dest_)  {                                         \
        if (dest_)                                                      \
                free(dest_);                                            \
        dest_ = strdup(src_);                                           \
        if (!dest_) {                                                   \
                fprintf(stderr, "strdup failed on string %s.\n", src_); \
                exit(EXIT_FAILURE);                                     \
        }                                                               \
}

/**
 * strncpy but guaranteed to end with '\0'
 */
#define strncpy_(dest_, src_, n_) {     \
        strncpy(dest_, src_, n_-1);     \
        dest_[n_-1] = '\0';             \
}

/**
 * strncat but guaranteed to end with '\0'
 * TODO: Would be more efficient to just
 *       write this one from scratch...
 */
#define strncat_(dest_, src_, n_) {     \
        int l0_ = strlen(dest_);        \
        strncat(dest_, src_, l0_+n_-1); \
        dest_[n_-1] = '\0';             \
}


/**
 * Free pointer if not NULL and set to NULL
 */
#define free_(ptr_) {               \
        if (ptr_) {                 \
                free((void*)ptr_);  \
                ptr_ = NULL;        \
        }                           \
}


/**
 * This function is a wrapper for the standard strtol
 * function that also handles all errors internally.
 *
 * Returns:
 *      - parsed long int
 */
int str2long(long*, const char* s);

int str2double(double*, const char* s);

/**
 * charcount simply counts the occurences of char c
 * in the string s.
 *
 * Returns:
 *      - Number of occurences.
 */
int charcount(const char* s, char c);
int charncount(const char* s, char c, unsigned n);

/**
 * strhaschar checks whether a char c exists within string s.
 *
 * Returns:
 *      - 1 if yes
 *      - 0 if no
 */
int strhaschar(const char* s, char c);

/**
 * removecharat shifts all characters left 1 at the
 * provided index i in order to essentially remove that
 * character from the string.
 */
void removecharat(char* s, int i);

/**
 * randstr generates a random string of length n
 * out of all alpha-numeric characters.
 *
 * Returns:
 *      - char* of random characters
 */
char* randstr(char* s, const int n);


/**
 * getnoext assigns a filename with no extension
 */
void getnoext(char* dest, const char* filename);

/**
 * getext returns the extension from a provided filename
 * The returned char* will be allocated on the heap
 * and must be free'd!
 */
char* getext(char* filename);

/**
 * string_eq returns true if two strings are equal
 */
int string_eq(const char* s1, const char* s2);

/**
 * istring_eq returns true if two strings are equal
 * while ignoring case.  It basically just inverts
 * the return of strcasecmp.
 */
int istring_eq(const char* s1, const char* s2);

/**
 * Lower case a char* in place
 * NOTE:
 * Null terminator is assumed. Don't be stupid.
 */
void string_to_lower(char* s);

/**
 * Copy BSD's strnstr
 */
char* strnstr(const char *s, const char *find, size_t slen);

struct queue* dir_list_files(const char* dir);

#ifdef __cplusplus
}
#endif

#endif
