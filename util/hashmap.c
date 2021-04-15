#include "hashmap.h"
#include "util.h"

/* Hashing based on FNV-1 */
const uint64_t _OFFSET = 14695981039346656037UL;
const uint64_t _PRIME = 1099511628211UL;
const size_t _NONE = (size_t) -1;

size_t _next_power_of_2 (size_t n)
{
	size_t value = 1;
	while  (value <  n) {
		value = value << 1;
	}
	return  value;
}

uint64_t _hash(HashMap* m, const char* key, int* n);
uint64_t _hash_nocase(HashMap* m, const char* key, int* n);
uint64_t _hash_rtrim(HashMap* m, const char* key, int* n);
uint64_t _hash_nocase_rtrim(HashMap* m, const char* key, int* n);

HashMap* hashmap_new(unsigned elem_size, size_t limit, unsigned props)
{
	HashMap* new_map = NULL;
	malloc_(new_map, sizeof(*new_map));
	return hashmap_construct(new_map, elem_size, limit, props);
}

HashMap* hashmap_construct(HashMap* m, unsigned elem_size, size_t limit, unsigned props)
{
	limit = _next_power_of_2(limit);

	*m = (HashMap) {
		 { 0 }          /* values */
		,NULL           /* _entries */
		,NULL           /* get_hash__ */
		,limit          /* _limit */
		,NULL           /* _keybuf */
		,0              /* _keybufhead */
		,limit          /* _keybuflen */
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

uint64_t _hash(HashMap* m, const char* key, int* n)
{
	uint64_t hash = _OFFSET;
	char* keyptr = &m->_keybuf[m->_keybufhead];
	int i = 0;

	for (; i < *n; ++i, ++keyptr) {
		*keyptr = key[i];
		hash *= _PRIME;
		hash ^= (uint64_t) *keyptr;
	}

	return hash;
}

uint64_t _hash_nocase(HashMap* m, const char* key, int* n)
{
	uint64_t hash = _OFFSET;
	char* keyptr = &m->_keybuf[m->_keybufhead];
	int i = 0;

	for (; i < *n; ++i, ++keyptr) {
		*keyptr = tolower(key[i]);
		hash *= _PRIME;
		hash ^= (uint64_t) *keyptr;
	}

	return hash;
}

uint64_t _hash_rtrim(HashMap* m, const char* key, int* n)
{
	int last_not_space_n = *n;
	uint64_t hash = _OFFSET;
	uint64_t last_not_space_hash = hash;
	char* keyptr = &m->_keybuf[m->_keybufhead];
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

uint64_t _hash_nocase_rtrim(HashMap* m, const char* key, int* n)
{
	int last_not_space_n = *n;
	uint64_t hash = _OFFSET;
	uint64_t last_not_space_hash = hash;
	char* keyptr = &m->_keybuf[m->_keybufhead];
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

void hashmap_nset(HashMap* m, const char* key, void* data, int n)
{
	if (m->_keybufhead + n > m->_keybuflen) {
		m->_keybuflen *= 2;
		realloc_(m->_keybuf, m->_keybuflen);
	}

	uint64_t hash = m->get_hash__(m, key, &n);
	size_t idx = (size_t)(hash & (m->_limit-1));

	/* use memcmp instead of strcmp in case non-char* key */
	struct hm_entry* entry = &m->_entries[idx];
	while (entry->val_idx != _NONE && (
		       entry->key_len != n
		    || memcmp(&m->_keybuf[entry->key_idx], &m->_keybuf[m->_keybufhead], n) != 0)) {
		idx = (idx + 1) % m->_limit;
		entry = &m->_entries[idx];
	}

	if (entry->val_idx == _NONE) {  /* New value */
		entry->key_idx = m->_keybufhead;
		entry->key_len = n;
		entry->val_idx = m->values.size;
		m->_keybufhead += n;
		vec_push_back(&m->values, data);
	} else {
		vec_set(&m->values, entry->val_idx, data);
	}
}

void* hashmap_nget(HashMap* m, const char* key, int n)
{
	if (m->_keybufhead + n > m->_keybuflen) {
		m->_keybuflen *= 2;
		realloc_(m->_keybuf, m->_keybuflen);
	}

	uint64_t hash = m->get_hash__(m, key, &n);
	size_t idx = (size_t)(hash & (m->_limit-1));

	/* use memcmp instead of strcmp in case non-char* key */
	struct hm_entry* entry = &m->_entries[idx];
	while (entry->val_idx != _NONE && (
			entry->key_len != n
		     || memcmp(&m->_keybuf[entry->key_idx], &m->_keybuf[m->_keybufhead], n) != 0)) {
		idx = (idx + 1) % m->_limit;
		entry = &m->_entries[idx];
	}

	if (entry->val_idx == _NONE) {  /* New value */
		return NULL;
	}

	return vec_at(&m->values, entry->val_idx);
}

/* elem size is Vec elements now */
MultiMap* multimap_new(unsigned elem_size, size_t limit, unsigned props)
{
	MultiMap* new_map = NULL;
	malloc_(new_map, sizeof(*new_map));
	return multimap_construct(new_map, elem_size, limit, props);
}

MultiMap* multimap_construct(MultiMap* m, unsigned elem_size, size_t limit, unsigned props)
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
	if (m->_keybufhead + n > m->_keybuflen) {
		m->_keybuflen *= 2;
		realloc_(m->_keybuf, m->_keybuflen);
	}

	uint64_t hash = m->get_hash__(m, key, &n);
	size_t idx = (size_t)(hash & (m->_limit-1));

	/* use memcmp instead of strcmp in case non-char* key */
	struct hm_entry* entry = &m->_entries[idx];
	while (entry->val_idx != _NONE && (
		       entry->key_len != n
		    || memcmp(&m->_keybuf[entry->key_idx], &m->_keybuf[m->_keybufhead], n) != 0)) {
		idx = (idx + 1) % m->_limit;
		entry = &m->_entries[idx];
	}

	if (entry->val_idx == _NONE) {  /* New value */
		entry->key_idx = m->_keybufhead;
		entry->key_len = n;
		entry->val_idx = m->values.size;
		m->_keybufhead += n;
		Vec new_vec;
		vec_construct(&new_vec, m->elem_size);
		vec_push_back(&new_vec, data);
		vec_push_back(&m->values, &new_vec);
	} else {
		Vec* entryvec = vec_at(&m->values, entry->val_idx);
		vec_push_back(entryvec, data);
	}
}
