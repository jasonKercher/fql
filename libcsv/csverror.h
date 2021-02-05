#ifndef CSV_ERROR_H
#define CSV_ERROR_H

#include "csv.h"
#include "charnode.h"

#define FAIL_IF(condition, errmsg)  {                   \
        if (condition) {                                \
                char* se = NULL;                        \
                size_t errsize = strlen(errmsg) + 1;    \
                if (errno) {                            \
                        se = strerror(errno);           \
                        errsize += strlen(se) + 2;      \
                }                                       \
                char* newmsg = NULL;                    \
                malloc_(newmsg, errsize);               \
                strcpy(newmsg, errmsg);                 \
                if (se) {                               \
                        strcat(newmsg, ": ");           \
                        strcat(newmsg, se);             \
                }                                       \
                err_push(newmsg);                       \
                return CSV_FAIL;                        \
        }                                               \
}

void err_remove(struct charnode* node);
struct charnode* err_push(const char*);
void err_printall();

#endif /* CSV_ERROR_H */
