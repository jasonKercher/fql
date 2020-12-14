#ifndef STRINGVIEW_H
#define STRINGVIEW_H

/**
 * StringView is meant to be a read-only
 * string type. The contents should never be
 * direcly be modified. Also, there is no
 * guarantee that the data is NULL terminated.
 */

struct stringview {
        char* data;
        unsigned len;
};
typedef struct stringview StringView;

struct stringview* stringview_new(char*, unsigned);
#define stringview_new_(s_) { stringview_new(s_, strlen(s_)); }
struct stringview* stringview_init(struct stringview*, char*, unsigned);
void stringview_free(struct stringview*);

#endif  /* STRINGVIEW_H */