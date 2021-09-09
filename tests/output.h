#ifndef OUTPUT_H
#define OUTPUT_H

#include "check_common.h"

void output_const(struct fql_handle*, const char* resultdir);
void output_read(struct fql_handle*, const char* resultdir);
void output_logic(struct fql_handle*, const char* resultdir);
void output_scalar(struct fql_handle*, const char* resultdir);
void output_switch(struct fql_handle*, const char* resultdir);

#endif  /* OUTPUT_H */
