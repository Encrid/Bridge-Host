/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "host.h"

static bool is_named(const char *identity, const char *name)
{
    size_t len = strlen(name);

    return (!strncmp(identity, name, len) && identity[len] == '@');
}

static bool is_loaded(const host_t *h, const char *name)
{
    for (uint32_t i = 0; i < h->d.files; i++)
        if (h->files[i].loaded && is_named(h->files[i].of->who.identity, name))
            return (true);
    return (false);
}

static bool waits(const host_t *h, const char *identity)
{
    const pairs_t *p = &h->orders[ORDER_UNLOADS];

    for (uint32_t i = 0; i < p->count; i++)
        if (is_named(identity, p->pairs[i].after)
            && is_loaded(h, p->pairs[i].before))
            return (true);
    return (false);
}

static file_t *pick_next(host_t *h)
{
    file_t *next = nullptr;
    file_t *f = nullptr;

    for (uint32_t i = 0; i < h->d.files; i++) {
        f = &h->files[i];
        if (f->loaded && !waits(h, f->of->who.identity) && (!next
            || strcmp(f->of->who.identity, next->of->who.identity) < 0))
            next = f;
    }
    return (next);
}

void host_unload_all(host_t *h)
{
    file_t *f = pick_next(h);
    [[maybe_unused]] int unloaded = 0;

    while (f) {
        unloaded = bridge_unload(h->w, f->of->who.identity);
        f->loaded = false;
        f = pick_next(h);
    }
}
