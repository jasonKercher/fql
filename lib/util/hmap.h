#ifndef HMAP_H
#define HMAP_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Case insensitive hash map
 * Too lazy to make my own, so just wrapped hsearch_r
 */
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <search.h>

#include "util.h"

#define MAX_KEY_LEN 128


struct hmap {
    struct hsearch_data* tab;
    struct queue* items;
};

struct hmap* hmap_new(size_t limit);
void* hmap_set(struct hmap* m, char* key, void* val);
void* hmap_get(struct hmap* m, const char* key);
void* hmap_remove(struct hmap* m, const char* key);
void hmap_free(struct hmap* m);
int hmap_haskey(struct hmap* m, const char* key);
int hmap_is_empty(struct hmap* m);

#ifdef __cplusplus
}
#endif

#endif

