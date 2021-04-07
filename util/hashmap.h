#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdlib.h>
#include <stdint.h>
#include "vec.h"

#define HASHMAP_PROP_DEFAULT 0x00
#define HASHMAP_PROP_NOCASE  0x01
#define HASHMAP_PROP_RTRIM   0x02

struct hm_entry {
	size_t val_idx;
	size_t key_idx;
	unsigned key_len;
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
	unsigned elem_size;  /* only for multimap */
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
#define hashmap_set(m_, key_, data_) hashmap_nset(m_, key_, data_, strlen(key_))
void* hashmap_nget(struct hashmap*, const char* key, int);
#define hashmap_get(m_, key_) hashmap_nget(m_, key_, strlen(key_))

typedef struct hashmap MultiMap;

MultiMap* multimap_new(size_t elem_size, size_t limit, unsigned props);
#define multimap_new_(T_, limit_, props_) multimap_new(sizeof(T_), limit_, props_)
MultiMap* multimap_construct(MultiMap*, size_t elem_size, size_t limit, unsigned props);
#define multimap_construct_(h_, T_, limit_, props_) multimap_construct(h_, sizeof(T_), limit_, props_)
void multimap_free(MultiMap*);
void multimap_destroy(MultiMap*);
void multimap_nset(MultiMap*, const char* key, void*, int);
#define multimap_set(m_, key_, data_) multimap_nset(m_, key_, data_, strlen(key_))
#define multimap_nget(m_, key_, n_) hashmap_nget(m_, key_, n_)
#define multimap_get(m_, key_) hashmap_nget(m_, key_, strlen(key_))


#endif
