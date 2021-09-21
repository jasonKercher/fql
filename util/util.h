#ifndef UTIL_H
#define UTIL_H

#ifdef __cplusplus
extern "C" {
#define restrict
#endif

#include <libgen.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>

#include "node.h"

typedef void (*generic_data_fn)(void*);
typedef int (*int_generic_data_fn)(void*);

#define _quote_macro_(x) #x
#define quote_macro_(x)  _quote_macro_(x)

/**
 * malloc wrapper that does error checking
 */
#define malloc_(size_)                       \
	({                                   \
		void* dest_ = malloc(size_); \
		if (!dest_) {                \
			perror("malloc");    \
			exit(EXIT_FAILURE);  \
		}                            \
		dest_;                       \
	})

/**
 * realloc wrapper that does error checking
 */
#define realloc_(dest_, size_)                           \
	({                                               \
		void* new_dest_ = realloc(dest_, size_); \
		if (!new_dest_) {                        \
			perror("realloc");               \
			exit(EXIT_FAILURE);              \
		}                                        \
		dest_ = new_dest_;                       \
		new_dest_;                               \
	})

/**
 * strncpy but guaranteed to end with '\0'
 */
#define strncpy_(dest_, src_, n_)         \
	{                                 \
		dest_[0] = '\0';          \
		strncat(dest_, src_, n_); \
	}

/**
 * free pointer if not NULL and set to NULL
 */
#define free_(ptr_)                \
	{                          \
		free((void*)ptr_); \
		ptr_ = NULL;       \
	}
#define free_if_exists_(ptr_)              \
	{                                  \
		if (ptr_ != NULL) {        \
			free((void*)ptr_); \
			ptr_ = NULL;       \
		}                          \
	}

/* Allow blank __VA_ARGS__ */
#define optarg_(...) , ##__VA_ARGS__

/* allocate space and pass args to ${T_}_construct */
#define new_(T_, ...)                                        \
	({                                                   \
		T_* alloc_ = malloc_(sizeof(T_));            \
		T_##_construct(alloc_ optarg_(__VA_ARGS__)); \
		alloc_;                                      \
	})

/* same as new_ except second arg is a type that
 * we take the sizeof. For example:
 *
 *     vec* v = new_t_(vec, int);
 *
 * is the same as
 *
 *     vec* v = new_(vec, sizeof(int));
 */
#define new_t_(T_, data_T_, ...)                                              \
	({                                                                    \
		T_* alloc_ = malloc_(sizeof(T_));                             \
		T_##_construct(alloc_, sizeof(data_T_) optarg_(__VA_ARGS__)); \
		alloc_;                                                       \
	})

/* Will call ${T_}_destroy and free allocation */
#define delete_if_exists_(T_, p_, ...)                         \
	{                                                      \
		if (p_ != NULL) {                              \
			T_##_destroy(p_ optarg_(__VA_ARGS__)); \
			free_(p_);                             \
		}                                              \
	}
#define delete_(T_, p_, ...)                           \
	{                                              \
		T_##_destroy(p_ optarg_(__VA_ARGS__)); \
		free_(p_);                             \
	}

#define num_compare_(a, b) (((a) > (b)) - ((a) < (b)))

/**
 * this function is a wrapper for the standard strtol
 * function that also handles all errors internally.
 */
int str2long(long* restrict, const char* restrict s);
int str2double(double* restrict, const char* restrict s);

/**
 * charcount simply counts the occurences of char c
 * in the string s.
 */
int charcount(const char* restrict s, char c);
int charncount(const char* restrict s, char c, unsigned n);

/**
 * strhaschar checks whether a char c exists within string s.
 *
 * returns:
 *      - 1 if yes
 *      - 0 if no
 */
int strhaschar(const char* restrict s, char c);

/**
 * removecharat shifts all characters left 1 at the
 * provided index i in order to essentially remove that
 * character from the string.
 */
void removecharat(char* restrict s, int i);

/**
 * randstr generates a random string of length n
 * out of all alpha-numeric characters.
 *
 * returns:
 *      - char* of random characters
 */
char* randstr(char* restrict s, const int n);

/**
 * getnoext assigns a filename with no extension
 */
void getnoext(char* dest, const char* restrict filename);

/**
 * getext returns the extension from a provided filename
 * the returned char* will be allocated on the heap
 * and must be free'd!
 */
char* getext(char* filename);

/**
 * string_eq returns true if two strings are equal
 */
int string_eq(const char* s1, const char* restrict s2);

/**
 * istring_eq returns true if two strings are equal
 * while ignoring case.  it basically just inverts
 * the return of strcasecmp.
 */
int istring_eq(const char* restrict s1, const char* restrict s2);

/**
 * lower case a char* in place
 * NOTE:
 * null terminator is assumed. don't be stupid.
 */
void string_to_lower(char* restrict s);

/**
 * copy BSD's strnstr
 */
char* strnstr(const char* restrict s, const char* restrict find, size_t slen);

struct node* dir_list_files(const char* restrict dir);

#ifdef __cplusplus
}
#endif

#endif
