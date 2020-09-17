#include "hmap.h"
#include <limits.h>

#define NONE (void*) ULONG_MAX

void* _hmap_update(hmap_t* m, const char* key, void* data);
int _hmap_insert(hmap_t* m, char* key, void* data);
ENTRY* _hmap_get(hmap_t* m, const char* key);

hmap_t* hmap_new(size_t limit)
{
        hmap_t* new_map = NULL;
        malloc_(new_map, sizeof(*new_map));

        struct hsearch_data* new_tab = NULL;
        malloc_(new_tab, sizeof(*new_tab));

        memset(new_tab, 0, sizeof(*new_tab));

        new_map->tab = new_tab;
        new_map->items = NULL;

        if (!hcreate_r(limit, new_tab)) {
                perror("hcreate_r");
                return NULL;
        }

        return new_map;
}

int _hmap_insert(hmap_t* m, char* key, void* data)
{
        ENTRY new_entry;
        ENTRY* ret = NULL;

        char key_cpy[MAX_KEY_LEN] = "";
        strncpy_(key_cpy, key, MAX_KEY_LEN);

        string_to_lower(key_cpy);

        new_entry.key = key_cpy;
        new_entry.data = data;

        if (!hsearch_r(new_entry, ENTER, &ret, m->tab)) {
                perror("hsearch_r");
                return 0;
        }

        return 1;
}

void* _hmap_update(hmap_t* m, const char* key, void* data)
{
        ENTRY* ent = _hmap_get(m, key);
        if (!ent)
                return NULL;

        if (ent->data == NONE) {
                ent->data = data;
                return NULL;
        }

        void* old_data = ent->data;
        ent->data = data;

        return old_data;
}

ENTRY* _hmap_get(hmap_t* m, const char* key)
{
        ENTRY search_entry;
        ENTRY* ret = NULL;

        char key_cpy[MAX_KEY_LEN] = "";
        strncpy_(key_cpy, key, MAX_KEY_LEN);

        string_to_lower(key_cpy);

        search_entry.key = key_cpy;

        int ret_val = hsearch_r(search_entry, FIND, &ret, m->tab);

        if (ret_val)
                return ret;

        return NULL;
}

void* hmap_set(hmap_t* m, char* key, void* data)
{
        if (hmap_haskey(m, key))
                return _hmap_update(m, key, data);

        _hmap_insert(m, key, data);
        return NULL;
}

void* hmap_get(hmap_t* m, const char* key)
{
       ENTRY* ent = _hmap_get(m, key);
       if (!ent)
               return NULL;

       //hmap_t_item* item = ent->data;
       return ent->data;
}

int hmap_haskey(hmap_t* m, const char* key)
{
        ENTRY search_entry;
        ENTRY* ret = NULL;

        char key_cpy[MAX_KEY_LEN] = "";
        strncpy_(key_cpy, key, MAX_KEY_LEN);

        string_to_lower(key_cpy);

        search_entry.key = key_cpy;

        int ret_val = hsearch_r(search_entry, FIND, &ret, m->tab);

        if (ret_val /*&& ret->data != NONE*/)
                return TRUE;

        return FALSE;
}

/**
 * Simulate delete by setting to NULL. hsearch
 * does not have the ability to delete, however,
 * hmap_get will return the same way. Return the
 * data in case user would like to free it.
 */
void* hmap_remove(hmap_t* m, const char* key)
{
        ENTRY* ent = _hmap_get(m, key);
        //hmap_t_item* item = ent->data;
        if (!ent)
                return NULL;

        void* data = ent->data;
        ent->data = NONE;

        /* Remove the item */
        //queue_delete(&m->items, item->_node);

        return data;
}

void hmap_free(hmap_t* m)
{
        //queue_free_data(&m->items);
        hdestroy_r(m->tab);
        free_(m->tab);
        free_(m);
}

int hmap_is_empty(hmap_t* m)
{
        return (m->items == NULL);
}
