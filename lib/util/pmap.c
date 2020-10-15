#include "pmap.h"

struct pmap* pmap_new(size_t limit, unsigned props)
{
        return hmap_new(limit, props);
}

void pmap_free(struct pmap* pmap)
{
        hmap_free(pmap);
}

struct vec* pmap_get(struct pmap* pmap, const char* key)
{
       return hmap_get(pmap, key);
}

struct vec* pmap_set(struct pmap* pmap, char* key, void* data)
{
        ENTRY* ent = hmap_get_entry(pmap, key);
        if (ent && ent->data != HMAP_NONE) {
                struct vec* v = ent->data;
                vec_push_back(v, data);
                return v;
        }

        struct vec* new_vec = vec_new_s(1);
        vec_push_back(new_vec, data);

        if (ent->data == HMAP_NONE) {
                ent->data = new_vec;
        } else {
                hmap_insert(pmap, key, new_vec); 
        }

        return NULL;
}
