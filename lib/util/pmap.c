#include "pmap.h"

Pmap* pmap_new(size_t limit, unsigned props)
{
        return hmap_new(limit, props);
}

Pmap* pmap_init(Pmap* pmap, size_t limit, unsigned props)
{
        return hmap_init(pmap, limit, props);
}

void pmap_free(Pmap* pmap)
{
        hmap_free(pmap);
}

Vec* pmap_get(Pmap* pmap, const char* key)
{
       return hmap_get(pmap, key);
}

Vec* pmap_set(Pmap* pmap, char* key, void* data)
{
        ENTRY* ent = hmap_get_entry(pmap, key);
        if (ent && ent->data != HMAP_NONE) {
                Vec* v = ent->data;
                vec_push_back(v, data);
                return v;
        }

        Vec* new_vec = vec_new_(void*);
        vec_push_back(new_vec, data);

        if (ent) {
                ent->data = new_vec;
        } else {
                hmap_insert(pmap, key, new_vec);
        }

        return NULL;
}