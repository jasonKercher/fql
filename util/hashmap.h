#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdlib.h>
#include <stdint.h>
#include "vec.h"

#define HASHMAP_PROP_DEFAULT 0x00
#define HASHMAP_PROP_NOCASE  0x01
#define HASHMAP_PROP_RTRIM   0x02

struct hm_entry {
	uint64_t hash;     /* store the calculated hash for resize */
	size_t val_idx;    /* index for _entries */
	size_t key_idx;    /* index to start of key */
	unsigned key_len;
};

struct hashmap;
typedef uint64_t(*hash_fn)(const struct hashmap*, const char* key, unsigned* n);

/* for compositemapping _keybuf */
struct _keyloc {
	size_t idx;
	unsigned len;
};

struct hashmap {
	struct vec values;
	struct hm_entry* _entries;
	struct vec* _keys;     /* only for compositemap */
	struct vec* _key_temp; /* only for compositemap */
	hash_fn get_hash__;
	size_t _limit;
	char* _keybuf;
	size_t _keybuf_head;
	size_t _keybuf_len;
	unsigned elem_size;  /* only for multimap */
	unsigned props;
};
typedef struct hashmap hashmap;

struct hashmap* hashmap_construct(struct hashmap*, const unsigned elem_size, size_t limit, const unsigned props);
#define hashmap_construct_(h_, T_, limit_, props_) hashmap_construct(h_, sizeof(T_), limit_, props_)
void hashmap_destroy(struct hashmap*);
void hashmap_nset(struct hashmap*, const char* key, void*, unsigned);
#define hashmap_set(m_, key_, data_) hashmap_nset(m_, key_, data_, strlen(key_))
void* hashmap_nget(struct hashmap*, const char* key, unsigned);
#define hashmap_get(m_, key_) hashmap_nget(m_, key_, strlen(key_))

typedef struct hashmap multimap;

multimap* multimap_construct(multimap*, const unsigned elem_size, size_t limit, const unsigned props);
#define multimap_construct_(h_, T_, limit_, props_) multimap_construct(h_, sizeof(T_), limit_, props_)
void multimap_destroy(multimap*);
void multimap_nset(multimap*, const char* key, void*, unsigned);
#define multimap_set(m_, key_, data_) multimap_nset(m_, key_, data_, strlen(key_))
#define multimap_nget(m_, key_, n_) hashmap_nget(m_, key_, n_)
#define multimap_get(m_, key_) hashmap_nget(m_, key_, strlen(key_))

typedef struct hashmap compositemap;

compositemap* compositemap_construct(compositemap*, const unsigned elem_size, size_t limit, const unsigned props);
#define compositemap_construct_(h_, T_, limit_, props_) compositemap_construct(h_, sizeof(T_), limit_, props_)
void compositemap_destroy(compositemap*);
void compositemap_set(compositemap*, const struct vec* key, void*);
void* compositemap_get(compositemap*, const struct vec* key);

#endif  /* HASHMAP_H */
