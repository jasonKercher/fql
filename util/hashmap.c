#include "hashmap.h"
#include "util.h"

/* Hashing based on FNV-1 */
const uint64_t _OFFSET = 14695981039346656037UL;
const uint64_t _PRIME = 1099511628211UL;

HashMap* hashmap_new(size_t limit, unsigned props)
{
        HashMap* new_map = NULL;
        malloc_(new_map, sizeof(*new_map));
        return new_map;
}

HashMap* hashmap_construct(HashMap* m, size_t limit, unsigned props)
{
        *m = (HashMap) {
                 NULL   /* _entries */
                ,limit  /* _size */
                ,NULL   /* _keybuf */
                ,0      /* _keybufhead */
                ,limit  /* _keybuflen */
                ,props  /* props */
        };

        malloc_(m->_entries, sizeof(struct hm_entry) * limit);
        memset(m->_entries, 0, sizeof(struct hm_entry) * limit);

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
        free_(m->_entries);
        free_(m->_keybuf);
}

uint64_t _nhash(HashMap* m, const char* key, int n)
{
        uint64_t hash = _OFFSET;
        char* keyptr = &m->_keybuf[m->_keybufhead];
        int i = 0;

        for (; i < n; ++i) {
                *keyptr = key[i];
                hash *= _PRIME;
                hash ^= (uint64_t) *keyptr;
        }

        return hash;
}

uint64_t _nhash_nocase(HashMap* m, const char* key, int n)
{
        uint64_t hash = _OFFSET;
        char* keyptr = &m->_keybuf[m->_keybufhead];
        int i = 0;

        for (; i < n; ++i) {
                *keyptr = tolower(key[i]);
                hash *= _PRIME;
                hash ^= (uint64_t) *keyptr;
        }

        return hash;
}

void hashmap_nset(HashMap* m, const char* key, void* data, int n)
{
        if (m->_keybufhead + n > m->_keybuflen) {
                m->_keybuflen *= 2;
                realloc_(m->_keybuf, n + m->_keybuflen);
        }

        uint64_t hash = 0;
        if (m->props & HASHMAP_PROP_NOCASE) {
                hash = _nhash_nocase(m, key, n);
        } else {
                hash = _nhash(m, key, n);
        }

        size_t idx = (size_t)(hash & (m->_limit-1));

        /* use memcmp instead of strcmp in case non-char* key */
        struct hm_entry* entry = &m->_entries[idx];
        while (entry->value != NULL && 
               entry->keylen == n &&
               memcmp(&m->_keybuf[entry->keyidx], &m->_keybuf[m->_keybufhead], n) == 0) {
                idx = (idx + 1) % m->_limit;
                entry = &m->_entries[idx];
        }

        if (entry->value == NULL) {  /* New value */
                entry->keyidx = m->_keybufhead;
                entry->keylen = n;
                m->_keybufhead += n;
        } 
        entry->value = data;
}

void hashmap_set(HashMap* m, const char* key, void* data)
{
}

void* hashmap_nget(HashMap* m, const char* key, int n)
{
        return NULL;
}
void* hashmap_get(HashMap* m, const char* key)
{
        return NULL;
}

