#include "csvsignal.h"
#include <string.h>
#include <stdio.h>
#include "charnode.h"
#include "util.h"
#include "misc.h"

/**
 * Signal Handlers
 * sigset_t and sa_flags only set to shut up valgrind
 */
static struct sigaction act;
static sigset_t vg_shutup = { {0} };
static int _signals_ready = false;
static struct charnode* _tmp_file_head = NULL;

void init_sig()
{
        if (_signals_ready)
                return;
        /** Attach signal handlers **/
        act.sa_mask = vg_shutup;
        act.sa_flags = 0;
        act.sa_handler = cleanexit;
        sigaction(SIGINT, &act, NULL);
        sigaction(SIGQUIT, &act, NULL);
        sigaction(SIGTERM, &act, NULL);
        sigaction(SIGHUP, &act, NULL);

        _signals_ready = true;
}

void cleanexit()
{
        tmp_removeall();
        exit(EXIT_FAILURE);
}

struct charnode* tmp_push(const char* tmp_file)
{
        return cn_push(&_tmp_file_head, strdup(tmp_file));
}

void tmp_remove_node(struct charnode* node)
{
        cn_remove(&_tmp_file_head, node);
}

void tmp_remove_file(const char* tmp_file)
{
        if (remove(tmp_file))
                perror(tmp_file);
}

void tmp_removeall()
{
        while (_tmp_file_head) {
                const char* data = cn_pop(&_tmp_file_head);
                tmp_remove_file(data);
                free_(data);
        }
}

