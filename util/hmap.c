#include "hmap.h"
#include <limits.h>
#include <search.h>
#include <stdint.h>

Hmap* hmap_new(size_t limit, unsigned props)
{
        Hmap* new_map = NULL;
        malloc_(new_map, sizeof(*new_map));

        return hmap_construct(new_map, limit, props);
}

Hmap* hmap_construct(Hmap* m, size_t limit, unsigned props)
{
        struct hsearch_data* new_tab = NULL;
        malloc_(new_tab, sizeof(*new_tab));

        memset(new_tab, 0, sizeof(*new_tab));

        m->tab = new_tab;
        m->props = props;
        m->_bufsize = HMAP_KEY_MAX * limit;

        malloc_(m->_buffer, m->_bufsize);
        m->_bufhead = m->_buffer;

        if (!hcreate_r(limit, new_tab)) {
                perror("hcreate_r");
                return NULL;
        }

        return m;
}

void hmap_destroy(Hmap* m)
{
        hdestroy_r(m->tab);
        free_(m->tab);
        free_(m->_buffer);
}

void hmap_free(Hmap* m)
{
        hmap_destroy(m);
        free_(m);
}

void _rtrim_strncpy(char* dest, const char* src, int char_limit)
{
        char* last_not_space = dest;

        int i = 0;
        for (;src[i] && i < char_limit - 1; ++i) {
                dest[i] = src[i];
                if (dest[i] != ' ') {
                        last_not_space = &dest[i];
                }
        }

        ++last_not_space;
        *last_not_space = '\0';
}

ENTRY* _get_entry(Hmap* m, const char* key, int char_limit)
{
        ENTRY search_entry;
        ENTRY* ret = NULL;

        char key_cpy[HMAP_KEY_MAX] = "";

        if ((m->props & HMAP_RTRIM)) {
                _rtrim_strncpy(key_cpy, key, char_limit);
        } else {
                strncpy_(key_cpy, key, char_limit);
        }

        if ((m->props & HMAP_NOCASE)) {
                string_to_lower(key_cpy);
        }

        search_entry.data = NULL;
        search_entry.key = key_cpy;

        int ret_val = hsearch_r(search_entry, FIND, &ret, m->tab);

        if (ret_val)
                return ret;

        return NULL;

}

ENTRY* hmap_nget_entry(Hmap* m, const char* key, int char_limit)
{
        if (++char_limit > HMAP_KEY_MAX) {
                char_limit = HMAP_KEY_MAX;
        }

        return _get_entry(m, key, char_limit);
}

ENTRY* hmap_get_entry(Hmap* m, const char* key)
{
        return _get_entry(m, key, HMAP_KEY_MAX);
}

void* hmap_nget(Hmap* m, const char* key, int char_limit)
{
       ENTRY* ent = hmap_nget_entry(m, key, char_limit);
       if (!ent)
               return NULL;

       return ent->data;
}

void* hmap_get(Hmap* m, const char* key)
{
       ENTRY* ent = hmap_get_entry(m, key);
       if (!ent)
               return NULL;

       return ent->data;
}

int _insert(Hmap* m, const char* key, void* data, int char_limit)
{

        /**
         * We cannot realloc here. realloc could invalidate
         * all the pointers in the hsearch_data table.
         *
         * This is issue would be resolved by implementing
         * a custom hashmap instead of wrapping hsearch_r.
         */
        if ((m->_bufhead - m->_buffer) + char_limit > m->_bufsize) {
                fputs("hmap buffer overflow\n", stderr);
                exit(EXIT_FAILURE);
        }

        if ((m->props & HMAP_RTRIM)) {
                _rtrim_strncpy(m->_bufhead, key, char_limit);
        } else {
                strncpy_(m->_bufhead, key, char_limit);
        }

        ENTRY new_entry;
        ENTRY* ret = NULL;

        if ((m->props & HMAP_NOCASE)) {
                string_to_lower(m->_bufhead);
        }

        new_entry.key = m->_bufhead;
        new_entry.data = data;

        m->_bufhead += char_limit + 2;

        if (!hsearch_r(new_entry, ENTER, &ret, m->tab)) {
                perror("hsearch_r");
                return 0;
        }

        return 1;
}

int hmap_ninsert(Hmap* m, const char* key, void* data, int char_limit)
{
        if (++char_limit > HMAP_KEY_MAX) {
                char_limit = HMAP_KEY_MAX;
        }

        return _insert(m, key, data, char_limit);
}

int hmap_insert(Hmap* m, const char* key, void* data)
{
        int size = strlen(key);
        size = size > HMAP_KEY_MAX ? HMAP_KEY_MAX : size + 1;
        return _insert(m, key, data, size);
}

void* hmap_nset(Hmap* m, const char* key, void* data, int char_limit)
{
        ENTRY* ent = hmap_nget_entry(m, key, char_limit);
        if (ent && ent->data != HMAP_NONE) {
                void* old_data = ent->data;
                ent->data = data;
                return old_data;
        }

        if (ent) {
                ent->data = data;
        } else {
                hmap_ninsert(m, key, data, char_limit);
        }

        return NULL;
}

void* hmap_set(Hmap* m, const char* key, void* data)
{
        ENTRY* ent = hmap_get_entry(m, key);
        if (ent && ent->data != HMAP_NONE) {
                void* old_data = ent->data;
                ent->data = data;
                return old_data;
        }

        if (ent) {
                ent->data = data;
        } else {
                hmap_insert(m, key, data);
        }

        return NULL;
}

void* hmap_get_a(Hmap* m, void* key)
{
        char addr[sizeof(key) + 3];
        sprintf(addr, "%p", key);

        return hmap_get(m, addr);
}

void* hmap_set_a(Hmap* m, void* key, void* data)
{
        char addr[sizeof(key) + 3];
        sprintf(addr, "%p", key);

        return hmap_set(m, addr, data);
}

_Bool hmap_haskey(Hmap* m, const char* key)
{
        ENTRY search_entry;
        ENTRY* ret = NULL;

        char key_cpy[HMAP_KEY_MAX] = "";
        strncpy_(key_cpy, key, HMAP_KEY_MAX);

        string_to_lower(key_cpy);

        search_entry.key = key_cpy;

        int ret_val = hsearch_r(search_entry, FIND, &ret, m->tab);

        if (ret_val /*&& ret->data != HMAP_NONE*/)
                return true;

        return false;
}

/**
 * Simulate delete by setting to HMAP_NONE (-1L).
 * hsearch_r does not have the ability to delete,
 * however, hmap_get will return the same way.
 * Return the data in case user would like to free it.
 */
void* hmap_remove(Hmap* m, const char* key)
{
        ENTRY* ent = hmap_get_entry(m, key);
        if (!ent)
                return NULL;

        void* data = ent->data;
        ent->data = HMAP_NONE;
        return data;
}

