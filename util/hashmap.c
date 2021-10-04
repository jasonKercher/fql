#include "hashmap.h"
#include "util.h"
#include "stringview.h"

/* hashing based on FNV-1 */
const uint64_t _FNV1_INIT = 14695981039346656037UL;
const uint64_t _PRIME = 1099511628211UL;
const size_t _NONE = (size_t)-1;
const double _FULL_PERCENT = .9; /* percent capacity where we increase size */

size_t _next_power_of_2(const size_t n)
{
	size_t value = 1;
	while (value < n) {
		value = value << 1;
	}
	return value;
}

uint64_t _hash(const hashmap* restrict, const char* restrict key, unsigned* restrict n);
uint64_t
_hash_nocase(const hashmap* restrict, const char* restrict key, unsigned* restrict n);
uint64_t
_hash_rtrim(const hashmap* restrict, const char* restrict key, unsigned* restrict n);
uint64_t _hash_nocase_rtrim(const hashmap* restrict,
                            const char* restrict key,
                            unsigned* restrict n);

void _increase_size(hashmap* restrict self);
struct hm_entry* _get_entry(hashmap* restrict, const char* restrict k, unsigned* restrict n, uint64_t* restrict h);
struct hm_entry* _composite_get_entry(compositemap* restrict self,
                                      const vec* restrict key,
                                      unsigned* restrict key_len,
                                      uint64_t* restrict hash);

set* set_construct(set* restrict self, size_t limit, const unsigned props)
{
	limit = _next_power_of_2(limit);

	*self = (hashmap) {
	        ._limit = limit,
	        ._keybuf_len = limit,
	        .props = props,
	};

	switch (self->props) {
	case HASHMAP_PROP_NOCASE:
		self->get_hash__ = _hash_nocase;
		break;
	case HASHMAP_PROP_RTRIM:
		self->get_hash__ = _hash_rtrim;
		break;
	case HASHMAP_PROP_NOCASE | HASHMAP_PROP_RTRIM:
		self->get_hash__ = _hash_nocase_rtrim;
		break;
	default:
		self->get_hash__ = _hash;
	}

	self->_entries = malloc_(sizeof(struct hm_entry) * limit);
	memset(self->_entries, -1, sizeof(struct hm_entry) * limit);

	self->_keybuf = malloc_(limit); /* expect this to grow */

	return self;
}

void set_destroy(set* restrict self)
{
	free_(self->_entries);
	free_(self->_keybuf);
}

void set_clear(set* restrict self)
{
	self->_keybuf_head = 0;
	memset(self->_entries, -1, sizeof(struct hm_entry) * self->_limit);
}

void set_nadd(set* restrict self, const char* restrict key, unsigned n)
{
	uint64_t hash = 0;
	struct hm_entry* entry = _get_entry(self, key, &n, &hash);

	if (entry->val_idx != _NONE) {
		return;
	}

	/* new value */
	entry->val_idx = 0;
	entry->key_idx = self->_keybuf_head;
	entry->key_len = n;
	entry->hash = hash;
	self->_keybuf_head += n;
	uintptr_t* size = (uintptr_t*)&self->values;
	++(*size);
	if (*size > _FULL_PERCENT * self->_limit) {
		_increase_size(self);
	}
}

bool set_nhas(set* restrict self, const char* restrict key, unsigned n)
{
	uint64_t hash = 0;
	struct hm_entry* entry = _get_entry(self, key, &n, &hash);
	return (entry->val_idx != _NONE);
}

hashmap* hashmap_construct(hashmap* restrict self,
                           const unsigned elem_size,
                           size_t limit,
                           const unsigned props)
{
	set_construct(self, limit, props);
	self->elem_size = elem_size;

	self->values = new_(vec, elem_size);
	vec_reserve(self->values, limit / 2);

	return self;
}

void hashmap_destroy(hashmap* restrict self)
{
	set_destroy(self);
	delete_(vec, self->values);
}

void hashmap_clear(hashmap* self)
{
	vec_clear(self->values);
	self->_keybuf_head = 0;
	memset(self->_entries, -1, sizeof(struct hm_entry) * self->_limit);
}

void hashmap_nset(hashmap* restrict self,
                  const char* restrict key,
                  void* restrict data,
                  unsigned n)
{
	uint64_t hash = 0;
	struct hm_entry* entry = _get_entry(self, key, &n, &hash);

	if (entry->val_idx == _NONE) { /* new value */
		entry->key_idx = self->_keybuf_head;
		entry->key_len = n;
		entry->val_idx = self->values->size;
		entry->hash = hash;
		self->_keybuf_head += n;
		vec_push_back(self->values, data);
		if (self->values->size > _FULL_PERCENT * self->_limit) {
			_increase_size(self);
		}
	} else {
		vec_set_at(self->values, entry->val_idx, data, 1);
	}
}

void* hashmap_nget(hashmap* restrict self, const char* restrict key, unsigned n)
{
	uint64_t hash = 0;
	struct hm_entry* entry = _get_entry(self, key, &n, &hash);

	if (entry->val_idx == _NONE) { /* new value */
		return NULL;
	}

	return vec_at(self->values, entry->val_idx);
}

/* elem size is vec elements now */
multimap* multimap_construct(multimap* restrict self,
                             const unsigned elem_size,
                             size_t limit,
                             const unsigned props)
{
	hashmap_construct(self, sizeof(vec), limit, props);
	self->elem_size = elem_size; /* this parameter is the elem_size of
				   * of the vectors created for each entry
				   */
	return self;
}

void multimap_destroy(multimap* restrict self)
{
	vec* it = vec_begin(self->values);
	for (; it != vec_end(self->values); ++it) {
		vec_destroy(it);
	}
	hashmap_destroy(self);
}

void multimap_clear(multimap* restrict self)
{
	vec* it = vec_begin(self->values);
	for (; it != vec_end(self->values); ++it) {
		vec_destroy(it);
	}
	vec_clear(self->values);
	self->_keybuf_head = 0;
	memset(self->_entries, -1, sizeof(struct hm_entry) * self->_limit);
}

void multimap_nset(multimap* restrict self,
                   const char* restrict key,
                   void* restrict data,
                   unsigned n)
{
	uint64_t hash = 0;
	struct hm_entry* entry = _get_entry(self, key, &n, &hash);

	if (entry->val_idx == _NONE) { /* new value */
		entry->key_idx = self->_keybuf_head;
		entry->key_len = n;
		entry->val_idx = self->values->size;
		entry->hash = hash;
		self->_keybuf_head += n;
		vec new_vec;
		vec_construct(&new_vec, self->elem_size);
		vec_push_back(&new_vec, data);
		vec_push_back(self->values, &new_vec);
		if (self->values->size > _FULL_PERCENT * self->_limit) {
			_increase_size(self);
		}
	} else {
		vec* entryvec = vec_at(self->values, entry->val_idx);
		vec_push_back(entryvec, data);
	}
}

compositemap* compositemap_construct(compositemap* restrict self,
                                     const unsigned elem_size,
                                     size_t limit,
                                     const unsigned props)
{
	hashmap_construct(self, elem_size, limit, props);
	self->_keys = new_t_(vec, struct _keyloc);
	vec_reserve(self->_keys, self->_limit);
	return self;
}

void compositemap_free(compositemap* restrict self)
{
	compositemap_destroy(self);
	free_(self);
}

void compositemap_destroy(compositemap* restrict self)
{
	delete_(vec, self->_keys);
	delete_if_exists_(vec, self->_key_temp);
	hashmap_destroy(self);
}

void compositemap_clear(compositemap* restrict self)
{
	vec_clear(self->_keys);
	vec_clear(self->values);
	self->_keybuf_head = 0;
	memset(self->_entries, -1, sizeof(struct hm_entry) * self->_limit);
}

/* NOTE: entry->key_len takes on a new meaning
 *       for a compositemap. in a normal hashmap,
 *       key_len is the length of the key. for a
 *       compositemap, key_len is loses that
 *       meaning since a key can consist of multiple
 *       individual keys. key_len represents the
 *       number of elements in the key.
 */
void compositemap_set(compositemap* restrict self,
                      const struct vec* restrict key,
                      void* restrict data)
{
	/* should only execute one time */
	if (self->_key_temp == NULL) {
		self->_key_temp = new_t_(vec, struct _keyloc);
	}
	vec_resize(self->_key_temp, key->size);

	uint64_t hash = 0;
	unsigned key_len = 0;
	struct hm_entry* entry = _composite_get_entry(self, key, &key_len, &hash);

	if (entry->val_idx == _NONE) { /* new value */
		entry->key_idx = self->_keys->size;
		entry->key_len = key->size;
		entry->val_idx = self->values->size;
		entry->hash = hash;
		self->_keybuf_head += key_len;
		vec_extend(self->_keys, self->_key_temp);
		vec_push_back(self->values, data);
		if (self->values->size > _FULL_PERCENT * self->_limit) {
			_increase_size(self);
		}
	} else {
		vec_set_at(self->values, entry->val_idx, data, 1);
	}
}

void* compositemap_get(compositemap* restrict self, const struct vec* restrict key)
{
	/* should only execute one time */
	if (self->_key_temp == NULL) {
		self->_key_temp = new_t_(vec, struct _keyloc);
	}
	vec_resize(self->_key_temp, key->size);

	uint64_t hash = 0;
	unsigned key_len = 0;
	struct hm_entry* entry = _composite_get_entry(self, key, &key_len, &hash);

	if (entry->val_idx == _NONE) {
		return NULL;
	}

	return vec_at(self->values, entry->val_idx);
}

bool _composite_eq(compositemap* restrict self, struct hm_entry* restrict ent)
{
	struct _keyloc* restrict kl0 = vec_at(self->_keys, ent->key_idx);
	struct _keyloc* restrict kl1 = vec_begin(self->_key_temp);
	unsigned i = 0;
	for (; i < ent->key_len; ++i) {
		if (kl0[i].len != kl1[i].len
		    || memcmp(&self->_keybuf[kl0[i].idx],
		              &self->_keybuf[kl1[i].idx],
		              kl0[i].len)) {
			return false;
		}
	}

	return true;
}

struct hm_entry* _composite_get_entry(compositemap* restrict self,
                                      const vec* restrict key,
                                      unsigned* restrict key_len,
                                      uint64_t* restrict hash)
{
	stringview* it = vec_begin(key);

	*hash = 1;
	*key_len = 0;
	unsigned i = 0;

	for (; it != vec_end(key); ++it) {
		while (self->_keybuf_head + it->len > self->_keybuf_len) {
			self->_keybuf_len *= 2;
			realloc_(self->_keybuf, self->_keybuf_len);
		}

		unsigned len = it->len;
		*hash *= (++i) * self->get_hash__(self, it->data, &len);
		*key_len += len;

		struct _keyloc* kl = vec_at(self->_key_temp, i - 1);
		kl->idx = self->_keybuf_head;
		kl->len = len;

		self->_keybuf_head += len;
	}

	size_t idx = (size_t)(*hash & (self->_limit - 1));
	struct hm_entry* entry = &self->_entries[idx];

	while (entry->val_idx != _NONE
	       && (entry->key_len != key->size || entry->hash != *hash
	           || !_composite_eq(self, entry))) {
		idx = (idx + 1) % self->_limit;
		entry = &self->_entries[idx];
	}

	self->_keybuf_head -= *key_len;

	return entry;
}

uint64_t
_hash(const hashmap* restrict self, const char* restrict key, unsigned* restrict n)
{
	uint64_t hash = _FNV1_INIT;
	char* keyptr = &self->_keybuf[self->_keybuf_head];
	unsigned i = 0;

	for (; i < *n; ++i, ++keyptr) {
		*keyptr = key[i];
		hash *= _PRIME;
		hash ^= (uint64_t)*keyptr;
	}

	return hash;
}

uint64_t
_hash_nocase(const hashmap* restrict self, const char* restrict key, unsigned* restrict n)
{
	uint64_t hash = _FNV1_INIT;
	char* keyptr = &self->_keybuf[self->_keybuf_head];
	unsigned i = 0;

	for (; i < *n; ++i, ++keyptr) {
		*keyptr = tolower(key[i]);
		hash *= _PRIME;
		hash ^= (uint64_t)*keyptr;
	}

	return hash;
}

uint64_t
_hash_rtrim(const hashmap* restrict self, const char* restrict key, unsigned* restrict n)
{
	unsigned last_not_space_n = *n;
	uint64_t hash = _FNV1_INIT;
	uint64_t last_not_space_hash = hash;
	char* keyptr = &self->_keybuf[self->_keybuf_head];
	unsigned i = 0;

	for (; i < *n; ++i, ++keyptr) {
		*keyptr = key[i];
		hash *= _PRIME;
		hash ^= (uint64_t)*keyptr;
		if (*keyptr != ' ') {
			last_not_space_hash = hash;
			last_not_space_n = i + 1;
		}
	}

	*n = last_not_space_n;
	return last_not_space_hash;
}

uint64_t _hash_nocase_rtrim(const hashmap* restrict self,
                            const char* restrict key,
                            unsigned* restrict n)
{
	unsigned last_not_space_n = *n;
	uint64_t hash = _FNV1_INIT;
	uint64_t last_not_space_hash = hash;
	char* keyptr = &self->_keybuf[self->_keybuf_head];
	unsigned i = 0;

	for (; i < *n; ++i, ++keyptr) {
		*keyptr = tolower(key[i]);
		hash *= _PRIME;
		hash ^= (uint64_t)*keyptr;
		if (*keyptr != ' ') {
			last_not_space_hash = hash;
			last_not_space_n = i + 1;
		}
	}

	*n = last_not_space_n;
	return last_not_space_hash;
}

struct hm_entry* _get_entry(hashmap* restrict self,
                            const char* restrict key,
                            unsigned* restrict key_len,
                            uint64_t* restrict hash)
{
	while (self->_keybuf_head + *key_len > self->_keybuf_len) {
		self->_keybuf_len *= 2;
		realloc_(self->_keybuf, self->_keybuf_len);
	}

	*hash = self->get_hash__(self, key, key_len);
	size_t idx = (size_t)(*hash & (self->_limit - 1));

	/* use memcmp instead of strcmp in case non-char* key */
	struct hm_entry* entry = &self->_entries[idx];
	while (entry->val_idx != _NONE
	       && (entry->key_len != *key_len || entry->hash != *hash
	           || memcmp(&self->_keybuf[entry->key_idx],
	                     &self->_keybuf[self->_keybuf_head],
	                     *key_len)
	                      != 0)) {
		idx = (idx + 1) % self->_limit;
		entry = &self->_entries[idx];
	}

	return entry;
}

void _increase_size(hashmap* restrict self)
{
	size_t old_limit = self->_limit;
	struct hm_entry* src_entries = self->_entries;
	self->_limit = _next_power_of_2(++self->_limit);

	self->_entries = malloc_(self->_limit * sizeof(struct hm_entry));
	memset(self->_entries, -1, sizeof(struct hm_entry) * self->_limit);

	size_t i = 0;
	for (; i < old_limit; ++i) {
		if (src_entries[i].val_idx == _NONE) {
			continue;
		}

		size_t idx = (size_t)(src_entries[i].hash & (self->_limit - 1));

		struct hm_entry* dest_entry = &self->_entries[idx];

		/* no need to check equality here because
		 * we are assuming uniqueness
		 */
		while (dest_entry->val_idx != _NONE) {
			idx = (idx + 1) % self->_limit;
			dest_entry = &self->_entries[idx];
		}
		*dest_entry = src_entries[i];
	}

	free(src_entries);
}
