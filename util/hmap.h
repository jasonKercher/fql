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

#define HMAP_NONE (void*) ULONG_MAX

#define HMAP_KEY_MAX 128

#define HMAP_NOCASE 0x01
#define HMAP_RTRIM  0x02

struct hmap {
    struct hsearch_data* tab;
    char* _buffer;
    char* _bufhead;
    size_t _bufsize;
    unsigned props;
};
typedef struct hmap Hmap;

struct hmap* hmap_new(size_t limit, unsigned);
struct hmap* hmap_construct(struct hmap*, size_t limit, unsigned);
void hmap_destroy(struct hmap* m);
void hmap_free(struct hmap* m);
void* hmap_nset(struct hmap* m, const char* key, void* data, int);
void* hmap_set(struct hmap* m, const char* key, void* data);
void* hmap_nget(struct hmap* m, const char* key, int);
void* hmap_get(struct hmap* m, const char* key);
void* hmap_set_a(struct hmap* m, void* key, void* data);
void* hmap_get_a(struct hmap* m, void* key);
void* hmap_remove(struct hmap* m, const char* key);
_Bool hmap_haskey(struct hmap* m, const char* key);
int hmap_ninsert(struct hmap* m, const char* key, void* data, int);
int hmap_insert(struct hmap* m, const char* key, void* data);
ENTRY* hmap_nget_entry(struct hmap* m, const char* key, int char_limit);
ENTRY* hmap_get_entry(struct hmap* m, const char* key);

#ifdef __cplusplus
}
#endif

#endif

