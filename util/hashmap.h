#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdlib.h>
#include <stdint.h>
#include "vec.h"

#define HASHMAP_PROP_DEFAULT 0x00
#define HASHMAP_PROP_NOCASE  0x01
#define HASHMAP_PROP_RTRIM   0x02

struct hm_entry {
        void* value;
        size_t keyidx;
        unsigned keylen;
};

struct hashmap {
        struct hm_entry* _entries;
        uint64_t _limit;
        char* _keybuf;
        size_t _keybufhead;
        size_t _keybuflen;
        unsigned props;
};
typedef struct hashmap HashMap;

struct hashmap* hashmap_new(size_t limit, unsigned props);
struct hashmap* hashmap_construct(struct hashmap*, size_t limit, unsigned props);
void hashmap_free(struct hashmap*);
void hashmap_destroy(struct hashmap*);
void hashmap_nset(struct hashmap*, const char* key, void*, int);
void hashmap_set(struct hashmap*, const char* key, void*);
void* hashmap_nget(struct hashmap*, const char* key, int);
void* hashmap_get(struct hashmap*, const char* key);

#endif
