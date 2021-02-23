#include "pmap.h"

Pmap* pmap_new(size_t limit, unsigned props)
{
        return hmap_new(limit, props);
}

Pmap* pmap_construct(Pmap* pmap, size_t limit, unsigned props)
{
        return hmap_construct(pmap, limit, props);
}

void pmap_destroy(Pmap* pmap)
{
        hmap_destroy(pmap);
}

void pmap_free(Pmap* pmap)
{
        hmap_free(pmap);
}

Vec* pmap_nget(Pmap* pmap, const char* key, int char_limit)
{
       return hmap_nget(pmap, key, char_limit);
}

Vec* pmap_get(Pmap* pmap, const char* key)
{
       return hmap_get(pmap, key);
}

Vec* pmap_nset(Pmap* pmap, const char* key, void* data, int char_limit)
{
        ENTRY* ent = hmap_nget_entry(pmap, key, char_limit);
        if (ent && ent->data != HMAP_NONE) {
                Vec* v = ent->data;
                vec_push_back(v, &data);
                return v;
        }

        Vec* new_vec = vec_new_(void*);
        vec_push_back(new_vec, &data);

        if (ent) {
                ent->data = new_vec;
        } else {
                hmap_ninsert(pmap, key, new_vec, char_limit);
        }

        return NULL;
}

Vec* pmap_set(Pmap* pmap, const char* key, void* data)
{
        ENTRY* ent = hmap_get_entry(pmap, key);
        if (ent && ent->data != HMAP_NONE) {
                Vec* v = ent->data;
                vec_push_back(v, &data);
                return v;
        }

        Vec* new_vec = vec_new_(void*);
        vec_push_back(new_vec, &data);

        if (ent) {
                ent->data = new_vec;
        } else {
                hmap_insert(pmap, key, new_vec);
        }

        return NULL;
}
