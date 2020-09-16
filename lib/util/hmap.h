#ifndef HMAP_H
#define HMAP_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define __USE_GNU
#include <search.h>

#include "util.h"

#define MAX_KEY_LEN 128


struct hmap_t {
    struct hsearch_data* tab;
    queue_t* items;
};

typedef struct hmap_t hmap_t;

hmap_t* hmap_new(size_t limit);
void* hmap_set(hmap_t* m, char* key, void* val);
void* hmap_get(hmap_t* m, const char* key);
void* hmap_remove(hmap_t* m, const char* key);
void hmap_free(hmap_t* m);
int hmap_haskey(hmap_t* m, const char* key);
int hmap_is_empty(hmap_t* m);


#endif

