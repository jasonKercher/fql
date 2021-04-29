#include "hashmap.h"
#include "util.h"
#include "stringview.h"

/* Hashing based on FNV-1 */
const uint64_t _FNV1_INIT = 14695981039346656037UL;
const uint64_t _PRIME = 1099511628211UL;
const size_t _NONE = (size_t) -1;
const double _FULL_PERCENT = .9;	/* Percent capacity where we increase size */

size_t _next_power_of_2 (const size_t n)
{
	size_t value = 1;
	while  (value <  n) {
		value = value << 1;
	}
	return  value;
}

uint64_t _hash(const HashMap* m, const char* key, unsigned* n);
uint64_t _hash_nocase(const HashMap* m, const char* key, unsigned* n);
uint64_t _hash_rtrim(const HashMap* m, const char* key, unsigned* n);
uint64_t _hash_nocase_rtrim(const HashMap* m, const char* key, unsigned* n);
struct hm_entry* _get_entry(HashMap* m, const char* key, unsigned* key_len, uint64_t* hash);
void _increase_size(HashMap* m);
struct hm_entry* _composite_get_entry(CompositeMap* m, const Vec* key, unsigned* key_len, uint64_t* hash);


HashMap* hashmap_new(const unsigned elem_size, size_t limit, const unsigned props)
{
	HashMap* new_map = NULL;
	malloc_(new_map, sizeof(*new_map));
	return hashmap_construct(new_map, elem_size, limit, props);
}

HashMap* hashmap_construct(HashMap* m, const unsigned elem_size, size_t limit, const unsigned props)
{
	limit = _next_power_of_2(limit);

	*m = (HashMap) {
		 { 0 }          /* values */
		,NULL           /* _entries */
		,NULL           /* _keys */
		,NULL           /* _key_temp */
		,NULL           /* get_hash__ */
		,limit          /* _limit */
		,NULL           /* _keybuf */
		,0              /* _keybuf_head */
		,limit          /* _keybuf_len */
		,elem_size      /* elem_size */
		,props          /* props */
	};

	switch (m->props) {
	case HASHMAP_PROP_NOCASE:
		m->get_hash__ = _hash_nocase;
		break;
	case HASHMAP_PROP_RTRIM:
		m->get_hash__ = _hash_rtrim;
		break;
	case HASHMAP_PROP_NOCASE | HASHMAP_PROP_RTRIM:
		m->get_hash__ = _hash_nocase_rtrim;
		break;
	default:
		m->get_hash__ = _hash;
	}

	vec_construct(&m->values, elem_size);
	vec_reserve(&m->values, limit / 2);

	malloc_(m->_entries, sizeof(struct hm_entry) * limit);
	memset(m->_entries, -1, sizeof(struct hm_entry) * limit);

	malloc_(m->_keybuf, limit);  /* Expect this to grow */

	return m;
}

void hashmap_free(HashMap* m)
{
	hashmap_destroy(m);
	free_(m);
}

void hashmap_destroy(HashMap* m)
{
	vec_destroy(&m->values);
	free_(m->_entries);
	free_(m->_keybuf);
}

void hashmap_nset(HashMap* m, const char* key, void* data, unsigned n)
{
	uint64_t hash = 0;
	struct hm_entry* entry = _get_entry(m, key, &n, &hash);

	if (entry->val_idx == _NONE) {  /* New value */
		entry->key_idx = m->_keybuf_head;
		entry->key_len = n;
		entry->val_idx = m->values.size;
		entry->hash = hash;
		m->_keybuf_head += n;
		vec_push_back(&m->values, data);
		if (m->values.size > _FULL_PERCENT * m->_limit) {
			_increase_size(m);
		}
	} else {
		vec_set(&m->values, entry->val_idx, data);
	}
}

void* hashmap_nget(HashMap* m, const char* key, unsigned n)
{
	uint64_t hash = 0;
	struct hm_entry* entry = _get_entry(m, key, &n, &hash);

	if (entry->val_idx == _NONE) {  /* New value */
		return NULL;
	}

	return vec_at(&m->values, entry->val_idx);
}

/* elem size is Vec elements now */
MultiMap* multimap_new(const unsigned elem_size, size_t limit, const unsigned props)
{
	MultiMap* new_map = NULL;
	malloc_(new_map, sizeof(*new_map));
	return multimap_construct(new_map, elem_size, limit, props);
}

MultiMap* multimap_construct(MultiMap* m, const unsigned elem_size, size_t limit, const unsigned props)
{
	hashmap_construct_(m, Vec, limit, props);
	m->elem_size = elem_size;  /* This parameter is the elem_size of
				    * of the vectors created for each entry
				    */
	return m;
}

void multimap_free(MultiMap* m)
{
	multimap_destroy(m);
	free_(m);
}

void multimap_destroy(MultiMap* m)
{
	Vec* it = vec_begin(&m->values);
	for (; it != vec_end(&m->values); ++it) {
		vec_destroy(it);
	}
	hashmap_destroy(m);
}

void multimap_nset(MultiMap* m, const char* key, void* data, unsigned n)
{
	uint64_t hash = 0;
	struct hm_entry* entry = _get_entry(m, key, &n, &hash);

	if (entry->val_idx == _NONE) {  /* New value */
		entry->key_idx = m->_keybuf_head;
		entry->key_len = n;
		entry->val_idx = m->values.size;
		entry->hash = hash;
		m->_keybuf_head += n;
		Vec new_vec;
		vec_construct(&new_vec, m->elem_size);
		vec_push_back(&new_vec, data);
		vec_push_back(&m->values, &new_vec);
		if (m->values.size > _FULL_PERCENT * m->_limit) {
			_increase_size(m);
		}
	} else {
		Vec* entryvec = vec_at(&m->values, entry->val_idx);
		vec_push_back(entryvec, data);
	}
}

CompositeMap* compositemap_new(const unsigned elem_size, size_t limit, const unsigned props)
{
	CompositeMap* new_map = NULL;
	malloc_(new_map, sizeof(*new_map));

	return compositemap_construct(new_map, elem_size, limit, props);
}

CompositeMap* compositemap_construct(CompositeMap* m, const unsigned elem_size, size_t limit, const unsigned props)
{
	hashmap_construct(m, elem_size, limit, props);
	m->_keys = vec_new_(struct _keyloc);
	vec_reserve(m->_keys, m->_limit);
	return m;
}

void compositemap_free(CompositeMap* m)
{
	compositemap_destroy(m);
	free_(m);
}

void compositemap_destroy(CompositeMap* m)
{
	vec_free(m->_keys);
	hashmap_destroy(m);
}

void compositemap_set(CompositeMap* m, const struct vec* key, void* data)
{
	/* Should only execute one time */
	if (m->_key_temp == NULL) {
		m->_key_temp = vec_new_(struct _keyloc);
		vec_resize(m->_key_temp, key->size);
	}

	uint64_t hash = 0;
	unsigned key_len = 0;
	struct hm_entry* entry = _composite_get_entry(m, key, &key_len, &hash);

	if (entry->val_idx == _NONE) {  /* New value */
		entry->key_idx = m->_keys->size;
		entry->key_len = key_len;
		entry->val_idx = m->values.size;
		entry->hash = hash;
		m->_keybuf_head += key_len;
		vec_extend(m->_keys, m->_key_temp);
		vec_push_back(&m->values, data);
		if (m->values.size > _FULL_PERCENT * m->_limit) {
			_increase_size(m);
		}
	} else {
		vec_set(&m->values, entry->val_idx, data);
	}
}

void* compositemap_get(CompositeMap* m, const struct vec* key)
{
	/* Should only execute one time */
	if (m->_key_temp == NULL) {
		m->_key_temp = vec_new_(struct _keyloc);
		vec_resize(m->_key_temp, key->size);
	}

	uint64_t hash = 0;
	unsigned key_len = 0;
	struct hm_entry* entry = _composite_get_entry(m, key, &key_len, &hash);

	if (entry->val_idx == _NONE) {
		return NULL;
	}

	return vec_at(&m->values, entry->val_idx);
}

_Bool _composite_eq(CompositeMap* m, struct hm_entry* ent, unsigned n)
{
	struct _keyloc* kl0 = vec_at(m->_keys, ent->key_idx);
	struct _keyloc* kl1 = vec_begin(m->_key_temp);
	unsigned i = 0;
	for (; i < n; ++i)  {
		if (kl0[i].len != kl1[i].len
		 || memcmp(&m->_keybuf[kl0[i].idx], &m->_keybuf[kl1[i].idx], kl0[i].len)) {
			return false;
		}
	}

	return true;
}

struct hm_entry* _composite_get_entry(CompositeMap* m, const Vec* key, unsigned* key_len, uint64_t* hash)
{
	StringView* it = vec_begin(key);

	*hash = 1;
	*key_len = 0;
	unsigned i = 0;

	for (; it != vec_end(key); ++it) {
		if (m->_keybuf_head + it->len > m->_keybuf_len) {
			m->_keybuf_len *= 2;
			realloc_(m->_keybuf, m->_keybuf_len);
		}

		unsigned len = it->len;
		*hash *= (++i) * m->get_hash__(m, it->data, &len);
		*key_len += len;

		struct _keyloc* kl = vec_at(m->_key_temp, i-1);
		kl->idx = m->_keybuf_head;
		kl->len = len;

		m->_keybuf_head += len;
	}

	size_t idx = (size_t)(*hash & (m->_limit-1));
	struct hm_entry* entry = &m->_entries[idx];

	while (entry->val_idx != _NONE
	    && !_composite_eq(m, entry, key->size)) {
		idx = (idx + 1) % m->_limit;
		entry = &m->_entries[idx];
	}

	m->_keybuf_head -= *key_len;

	return entry;
}

uint64_t _hash(const HashMap* m, const char* key, unsigned* n)
{
	uint64_t hash = _FNV1_INIT;
	char* keyptr = &m->_keybuf[m->_keybuf_head];
	unsigned i = 0;

	for (; i < *n; ++i, ++keyptr) {
		*keyptr = key[i];
		hash *= _PRIME;
		hash ^= (uint64_t) *keyptr;
	}

	return hash;
}

uint64_t _hash_nocase(const HashMap* m, const char* key, unsigned* n)
{
	uint64_t hash = _FNV1_INIT;
	char* keyptr = &m->_keybuf[m->_keybuf_head];
	unsigned i = 0;

	for (; i < *n; ++i, ++keyptr) {
		*keyptr = tolower(key[i]);
		hash *= _PRIME;
		hash ^= (uint64_t) *keyptr;
	}

	return hash;
}

uint64_t _hash_rtrim(const HashMap* m, const char* key, unsigned* n)
{
	unsigned last_not_space_n = *n;
	uint64_t hash = _FNV1_INIT;
	uint64_t last_not_space_hash = hash;
	char* keyptr = &m->_keybuf[m->_keybuf_head];
	unsigned i = 0;

	for (; i < *n; ++i, ++keyptr) {
		*keyptr = key[i];
		hash *= _PRIME;
		hash ^= (uint64_t) *keyptr;
		if (*keyptr != ' ') {
			last_not_space_hash = hash;
			last_not_space_n = i+1;
		}
	}

	*n = last_not_space_n;
	return last_not_space_hash;
}

uint64_t _hash_nocase_rtrim(const HashMap* m, const char* key, unsigned* n)
{
	unsigned last_not_space_n = *n;
	uint64_t hash = _FNV1_INIT;
	uint64_t last_not_space_hash = hash;
	char* keyptr = &m->_keybuf[m->_keybuf_head];
	unsigned i = 0;

	for (; i < *n; ++i, ++keyptr) {
		*keyptr = tolower(key[i]);
		hash *= _PRIME;
		hash ^= (uint64_t) *keyptr;
		if (*keyptr != ' ') {
			last_not_space_hash = hash;
			last_not_space_n = i+1;
		}
	}

	*n = last_not_space_n;
	return last_not_space_hash;
}

struct hm_entry* _get_entry(HashMap* m, const char* key, unsigned* key_len, uint64_t* hash)
{
	if (m->_keybuf_head + *key_len > m->_keybuf_len) {
		m->_keybuf_len *= 2;
		realloc_(m->_keybuf, m->_keybuf_len);
	}

	*hash = m->get_hash__(m, key, key_len);
	size_t idx = (size_t)(*hash & (m->_limit-1));

	/* use memcmp instead of strcmp in case non-char* key */
	struct hm_entry* entry = &m->_entries[idx];
	while (entry->val_idx != _NONE && (
	        entry->key_len != *key_len
	     || memcmp(&m->_keybuf[entry->key_idx],
	               &m->_keybuf[m->_keybuf_head],
	               *key_len) != 0)) {
		idx = (idx + 1) % m->_limit;
		entry = &m->_entries[idx];
	}

	return entry;
}

void _increase_size(HashMap* m)
{
	size_t old_limit = m->_limit;
	struct hm_entry* src_entries = m->_entries;
	m->_limit = _next_power_of_2(++m->_limit);

	malloc_(m->_entries, m->_limit * sizeof(struct hm_entry));
	memset(m->_entries, -1, sizeof(struct hm_entry) * m->_limit);

	size_t i = 0;
	for (; i < old_limit; ++i) {
		if (src_entries[i].val_idx == _NONE) {
			continue;
		}

		size_t idx = (size_t)(src_entries[i].hash & (m->_limit-1));

		struct hm_entry* dest_entry = &m->_entries[idx];

		/* No need to check equality here because
		 * we are assuming uniqueness
		 */
		while (dest_entry->val_idx != _NONE) {
			idx = (idx + 1) % m->_limit;
			dest_entry = &m->_entries[idx];
		}
		*dest_entry = src_entries[i];
	}

	free(src_entries);
}






