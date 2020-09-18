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

struct props g_props;

#ifdef __cplusplus
}
#else 
extern struct props g_props;
#endif

void prop_init();

#endif /* PROP_H */
