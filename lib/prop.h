#ifndef PROP_H
#define PROP_H

#ifdef __cplusplus
extern "C" {
#endif

struct props {
        char in_delim[32];
        char out_delim[32];
        int verbose;
        int dry_run;
        int override_warnings;
};

typedef struct props props_t;

props_t g_props;

#ifdef __cplusplus
}
#else 
extern props_t g_props;
#endif

void prop_init();

#endif
