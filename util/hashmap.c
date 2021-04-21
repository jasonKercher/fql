#include "hashmap.h"
#include "util.h"

/* Hashing based on FNV-1 */
const uint64_t _OFFSET = 14695981039346656037UL;
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

uint64_t _hash(const HashMap* m, const char* key, int* n);
uint64_t _hash_nocase(const HashMap* m, const char* key, int* n);
uint64_t _hash_rtrim(const HashMap* m, const char* key, int* n);
uint64_t _hash_nocase_rtrim(const HashMap* m, const char* key, int* n);
struct hm_entry* _get_entry(HashMap* m, const char* key, int key_len, uint64_t* hash);
void _increase_size(HashMap* m);

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

void hashmap_nset(HashMap* m, const char* key, void* data, int n)
{
	uint64_t hash = 0;
	struct hm_entry* entry = _get_entry(m, key, n, &hash);

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

void* hashmap_nget(HashMap* m, const char* key, int n)
{
	uint64_t hash = 0;
	struct hm_entry* entry = _get_entry(m, key, n, &hash);

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

void multimap_nset(MultiMap* m, const char* key, void* data, int n)
{
	uint64_t hash = 0;
	struct hm_entry* entry = _get_entry(m, key, n, &hash);

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

uint64_t _hash(const HashMap* m, const char* key, int* n)
{
	uint64_t hash = _OFFSET;
	char* keyptr = &m->_keybuf[m->_keybuf_head];
	int i = 0;

	for (; i < *n; ++i, ++keyptr) {
		*keyptr = key[i];
		hash *= _PRIME;
		hash ^= (uint64_t) *keyptr;
	}

	return hash;
}

uint64_t _hash_nocase(const HashMap* m, const char* key, int* n)
{
	uint64_t hash = _OFFSET;
	char* keyptr = &m->_keybuf[m->_keybuf_head];
	int i = 0;

	for (; i < *n; ++i, ++keyptr) {
		*keyptr = tolower(key[i]);
		hash *= _PRIME;
		hash ^= (uint64_t) *keyptr;
	}

	return hash;
}

uint64_t _hash_rtrim(const HashMap* m, const char* key, int* n)
{
	int last_not_space_n = *n;
	uint64_t hash = _OFFSET;
	uint64_t last_not_space_hash = hash;
	char* keyptr = &m->_keybuf[m->_keybuf_head];
	int i = 0;

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

uint64_t _hash_nocase_rtrim(const HashMap* m, const char* key, int* n)
{
	int last_not_space_n = *n;
	uint64_t hash = _OFFSET;
	uint64_t last_not_space_hash = hash;
	char* keyptr = &m->_keybuf[m->_keybuf_head];
	int i = 0;

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

struct hm_entry* _get_entry(HashMap* m, const char* key, int key_len, uint64_t* hash)
{
	if (m->_keybuf_head + key_len > m->_keybuf_len) {
		m->_keybuf_len *= 2;
		realloc_(m->_keybuf, m->_keybuf_len);
	}

	*hash = m->get_hash__(m, key, &key_len);
	size_t idx = (size_t)(*hash & (m->_limit-1));

	/* use memcmp instead of strcmp in case non-char* key */
	struct hm_entry* entry = &m->_entries[idx];
	while (entry->val_idx != _NONE && (
		       entry->key_len != key_len
		    || memcmp(&m->_keybuf[entry->key_idx], &m->_keybuf[m->_keybuf_head], key_len) != 0)) {
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
		size_t idx = (size_t)(src_entries[i].hash & (m->_limit-1));

		/* Same logic as _get_entry */	
		struct hm_entry* dest_entry = &m->_entries[idx];
		while (dest_entry->val_idx != _NONE && (
			       dest_entry->key_len != src_entries[i].key_len 
			    || memcmp(&m->_keybuf[dest_entry->key_idx],
				      &m->_keybuf[m->_keybuf_head],
				      src_entries[i].key_len) != 0)) {
			idx = (idx + 1) % m->_limit;
			dest_entry = &m->_entries[idx];
		}
		*dest_entry = src_entries[i];
	}

	free(src_entries);
}
