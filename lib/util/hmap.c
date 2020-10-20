#include "hmap.h"
#include <limits.h>
#include <stdint.h>

struct hmap_item {
        void* data;
        char _key[HMAP_KEY_MAX]; /* no touchy touchy ;) */
        struct queue* _node;     /* no touchy touchy ;) */
};
typedef struct hmap_item h_item;

struct hmap* hmap_new(size_t limit, unsigned props)
{
        struct hmap* new_map = NULL;
        malloc_(new_map, sizeof(*new_map));

        struct hsearch_data* new_tab = NULL;
        malloc_(new_tab, sizeof(*new_tab));

        memset(new_tab, 0, sizeof(*new_tab));

        new_map->tab = new_tab;
        new_map->items = NULL;
        new_map->props = props;

        if (!hcreate_r(limit, new_tab)) {
                perror("hcreate_r");
                return NULL;
        }

        return new_map;
}

struct hmap_item* hmap_item_new(struct hmap* m, const char* key, void* data)
{
        struct hmap_item* item = NULL;
        malloc_(item, sizeof(*item));
        *item = (struct hmap_item) {
                 data
                ,""
                ,NULL
        };

        int size = strlen(key) + 1;
        size = size > HMAP_KEY_MAX ? HMAP_KEY_MAX : size;
        strncpy_(item->_key, key, size);

        if ((m->props & HMAP_NOCASE)) {
                string_to_lower(item->_key);
        }

        return item;
}

int hmap_insert(struct hmap* m, const char* key, void* data)
{
        struct hmap_item* item = hmap_item_new(m, key, data);

        ENTRY new_entry;
        ENTRY* ret = NULL;

        new_entry.key = item->_key;
        new_entry.data = item;

        if (!hsearch_r(new_entry, ENTER, &ret, m->tab)) {
                perror("hsearch_r");
                return 0;
        }

        item->_node = queue_enqueue(&m->items, item);

        return 1;
}

ENTRY* hmap_get_entry(struct hmap* m, const char* key)
{
        ENTRY search_entry;
        ENTRY* ret = NULL;

        char key_cpy[HMAP_KEY_MAX] = "";
        strncpy_(key_cpy, key, HMAP_KEY_MAX);

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

void* hmap_get(struct hmap* m, const char* key)
{
       ENTRY* ent = hmap_get_entry(m, key);
       if (!ent)
               return NULL;

       return ent->data;
}

void* hmap_set(struct hmap* m, char* key, void* data)
{
        ENTRY* ent = hmap_get_entry(m, key);
        if (ent && ent->data != HMAP_NONE) {
                void* old_data = ent->data;
                ent->data = data;
                return old_data;
        }

        if (ent) {
                struct hmap_item* item = ent->data;
                item->data = data;
        } else {
                hmap_insert(m, key, data);
        }

        return NULL;
}

void* hmap_get_a(struct hmap* m, void* key)
{
        char addr[sizeof(key) + 3];
        sprintf(addr, "%p", key);

        return hmap_get(m, addr);
}

void* hmap_set_a(struct hmap* m, void* key, void* data)
{
        char addr[sizeof(key) + 3];
        sprintf(addr, "%p", key);

        return hmap_set(m, addr, data);
}

_Bool hmap_haskey(struct hmap* m, const char* key)
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
 * Simulate delete by setting to HMAP_NONE (-1L). hsearch
 * does not have the ability to delete, however,
 * hmap_get will return the same way. Return the
 * data in case user would like to free it.
 */
void* hmap_remove(struct hmap* m, const char* key)
{
        ENTRY* ent = hmap_get_entry(m, key);
        if (!ent)
                return NULL;

        struct hmap_item* item = ent->data;
        void* data = item->data;

        queue_remove(&m->items, item->_node);
        free_(item);

        ent->data = HMAP_NONE;

        return data;
}

void hmap_free(struct hmap* m)
{
        struct hmap_item* item = queue_dequeue(&m->items);
        for (; item; item = queue_dequeue(&m->items)) {
                free_(item);
        }

        hdestroy_r(m->tab);
        free_(m->tab);
        free_(m);
}
