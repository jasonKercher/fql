#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdlib.h>
#include <stdint.h>
#include "vec.h"

#define HASHMAP_PROP_DEFAULT 0x00
#define HASHMAP_PROP_NOCASE  0x01
#define HASHMAP_PROP_RTRIM   0x02

struct hm_entry {
        size_t validx;
        size_t keyidx;
        unsigned keylen;
};

struct hashmap;
typedef uint64_t(*hash_fn)(struct hashmap*, const char* key, int* n);

struct hashmap {
        struct vec values;
        struct hm_entry* _entries;
        hash_fn get_hash__;
        uint64_t _limit;
        char* _keybuf;
        size_t _keybufhead;
        size_t _keybuflen;
        unsigned props;
};
typedef struct hashmap HashMap;

struct hashmap* hashmap_new(size_t elem_size, size_t limit, unsigned props);
#define hashmap_new_(T_, limit_, props_) hashmap_new(sizeof(T_), limit_, props_)
struct hashmap* hashmap_construct(struct hashmap*, size_t elem_size, size_t limit, unsigned props);
#define hashmap_construct_(h_, T_, limit_, props_) hashmap_construct(h_, sizeof(T_), limit_, props_)
void hashmap_free(struct hashmap*);
void hashmap_destroy(struct hashmap*);

void hashmap_nset(struct hashmap*, const char* key, void*, int);
void hashmap_set(struct hashmap*, const char* key, void*);
void* hashmap_nget(struct hashmap*, const char* key, int);
void* hashmap_get(struct hashmap*, const char* key);

#endif
