#include "fqlsig.h"
#include <string.h>
#include <stdio.h>
#include "util/stringy.h"
#include "util/util.h"
#include "misc.h"


typedef void (*sig_fn)(int);

/* Signal Handlers
 * sigset_t and sa_flags only set to shut up valgrind
 */
static struct sigaction act;
static sigset_t vg_shutup = {{0}};
static int _signals_ready = false;
static node* _tmp_file_head = NULL;

static sig_fn libcsv_action__ = NULL;

void fqlsig_init_sig()
{
	if (_signals_ready)
		return;

	/* First thing we need to do is pull the old signal
	 * handler since setting a new one will overwrite.
	 */
	struct sigaction libcsv_act;
	sigaction(SIGINT, NULL, &libcsv_act);
	libcsv_action__ = libcsv_act.sa_handler;

	/* Attach signal handlers */
	act.sa_mask = vg_shutup;
	act.sa_flags = 0;
	act.sa_handler = fqlsig_cleanexit;
	sigaction(SIGINT, &act, NULL);
	sigaction(SIGQUIT, &act, NULL);
	sigaction(SIGTERM, &act, NULL);
	sigaction(SIGHUP, &act, NULL);

	_signals_ready = true;
}

void fqlsig_cleanexit(int signo)
{
	(void)signo;
	/* The -1 here tells the libcsv sig handler not
	 * to call exit(), so we can clean up here too.
	 */
	if (libcsv_action__ != NULL) {
		libcsv_action__(-1);
	}
	fqlsig_tmp_removeall();
	exit(EXIT_FAILURE);
}

node* fqlsig_tmp_push(const char* tmp_file)
{
	return node_enqueue(&_tmp_file_head, strdup(tmp_file));
}

void fqlsig_tmp_remove_node(node* node)
{
	char* filename = node_remove(&_tmp_file_head, node);
	free_(filename);
}

void fqlsig_tmp_remove_file(const char* tmp_file)
{
	if (remove(tmp_file)) {
		perror(tmp_file);
	}
}

void fqlsig_tmp_remove_node_and_file(node* node)
{
	char* tmp = node_remove(&_tmp_file_head, node);
	fqlsig_tmp_remove_file(tmp);
	free_(tmp);
}

void fqlsig_tmp_removeall()
{
	while (_tmp_file_head) {
		char* tmp = node_dequeue(&_tmp_file_head);
		fqlsig_tmp_remove_file(tmp);
		free_(tmp);
	}
}
