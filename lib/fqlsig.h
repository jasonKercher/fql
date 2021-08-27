#ifndef CSV_SIGNAL_H
#define CSV_SIGNAL_H

#include <signal.h>
#include <errno.h>
#include "util/node.h"

/**
 * Initialize signal handlers
 */
void fqlsig_init_sig();

/**
 * This function simply removes any temporary files
 * in the event of an error or SIGxxx detected.
 */
void fqlsig_cleanexit();

/**
 * Add temp file
 */
struct node* fqlsig_tmp_push(void* tmp_file);

/**
 * Remove temp node only
 */
void fqlsig_tmp_remove_node(struct node* node);

/**
 * Remove temp file
 */
void fqlsig_tmp_remove_file(const char* tmp_file);

/**
 * Remove all temp files
 */
void fqlsig_tmp_removeall();

#endif /* CSV_SIGNAL_H */
