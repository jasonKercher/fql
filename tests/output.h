#ifndef OUTPUT_H
#define OUTPUT_H

#include "check_common.h"

void output_const(struct fqlhandle*, const char* resultdir);
void output_read(struct fqlhandle*, const char* resultdir);
void output_logic(struct fqlhandle*, const char* resultdir);
void output_scalar(struct fqlhandle*, const char* resultdir);
void output_switch(struct fqlhandle*, const char* resultdir);

#endif  /* OUTPUT_H */
