#include "misc.h"
#include "csverror.h"
#include "util.h"

/**
 * Error Handlers
 */

static struct charnode* _err_str_head = NULL;

void err_remove(struct charnode* node)
{
       cn_remove(&_err_str_head, node);
}

struct charnode* err_push(const char* err)
{
        return cn_push(&_err_str_head, err);
}

void err_printall()
{
        while(_err_str_head) {
                const char* data = cn_pop(&_err_str_head);
                fprintf(stderr, "%s\n", data);
                free_(data);
        }
}


