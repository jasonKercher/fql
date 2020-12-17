#ifndef PROP_H
#define PROP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

struct props {
        char in_delim[32];
        char out_delim[32];
        _Bool verbose;
        _Bool dry_run;
        _Bool override_warnings;
        _Bool print_plan;
};
typedef struct props Props;

#ifdef __cplusplus
struct props g_props;
}
#else
extern struct props g_props;
#endif

void prop_init();

#endif /* PROP_H */
