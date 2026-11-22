/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "host.h"

static int order_listed(const void *a, const void *b)
{
    return (strcmp(((const listed_t *)a)->name, ((const listed_t *)b)->name));
}

static bool add_listed(listing_t *l, const char *name, bool is_folder)
{
    listed_t *more = realloc(l->items, sizeof(*more) * (l->count + 1));
    char *copy = more ? malloc(strlen(name) + 1) : nullptr;

    if (more)
        l->items = more;
    if (!copy)
        return (false);
    strcpy(copy, name);
    l->items[l->count++] = (listed_t){ copy, is_folder };
    return (true);
}

bool list_folder(os_folder_t *in, listing_t *l)
{
    bool is_folder = false;
    bool kept = true;
    const char *name = os_folder_read_entry(in, &is_folder);

    *l = (listing_t){};
    while (kept && name) {
        kept = add_listed(l, name, is_folder);
        name = os_folder_read_entry(in, &is_folder);
    }
    if (kept && l->count)
        qsort(l->items, l->count, sizeof(*l->items), order_listed);
    return (kept);
}

void free_listing(listing_t *l)
{
    for (uint32_t i = 0; i < l->count; i++)
        free(l->items[i].name);
    free(l->items);
    *l = (listing_t){};
}
