#ifndef OUTPUT_H
#define OUTPUT_H

#include "check_common.h"

void output_const(struct fqlhandle*);
void output_read(struct fqlhandle*);
void output_logic(struct fqlhandle*);
void output_scalar(struct fqlhandle*);
void output_switch(struct fqlhandle*);
void output_null(struct fqlhandle*);
void output_join(struct fqlhandle*);
void output_subquery(struct fqlhandle*);
void output_group(struct fqlhandle*);
void output_having(struct fqlhandle*);
void output_order(struct fqlhandle*);
void output_top(struct fqlhandle*);
void output_union(struct fqlhandle*);
void output_difficult(struct fqlhandle*);
void output_operations(struct fqlhandle*);

#endif  /* OUTPUT_H */
