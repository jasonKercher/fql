#ifndef OUTPUT_H
#define OUTPUT_H

#include "check_common.h"

void output_const(struct fqlhandle*);
void output_read(struct fqlhandle*);
void output_logic(struct fqlhandle*);
void output_scalar(struct fqlhandle*);
void output_switch(struct fqlhandle*);
void output_null(struct fqlhandle*);

#endif  /* OUTPUT_H */
