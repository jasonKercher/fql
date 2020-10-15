#ifndef HMAP_H
#define HMAP_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Too lazy to make my own hash map,
 * so just wrapped hsearch_r
 */
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <search.h>

#include "util.h"

#define NONE (void*) ULONG_MAX

#define MAX_KEY_LEN 128

#define HMAP_NOCASE 0x01

struct hmap {
    struct hsearch_data* tab;
    unsigned props;
};

struct hmap* hmap_new(size_t limit, unsigned);
void hmap_free(struct hmap* m);
void* hmap_set(struct hmap* m, char* key, void* data);
void* hmap_get(struct hmap* m, const char* key);
void* hmap_set_a(struct hmap* m, void* key, void* data);
void* hmap_get_a(struct hmap* m, void* key);
void* hmap_remove(struct hmap* m, const char* key);
_Bool hmap_haskey(struct hmap* m, const char* key);
int hmap_insert(struct hmap* m, char* key, void* data);
ENTRY* hmap_get_entry(struct hmap* m, const char* key);

#ifdef __cplusplus
}
#endif

#endif

