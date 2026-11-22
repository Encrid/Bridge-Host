/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "say.h"

static void add_names(const cfile_t *f, const keyed_t *under,
    manifest_names_t *into)
{
    const entry_t *e = nullptr;

    for (uint32_t i = 0; i < f->toml.count; i++) {
        e = &f->toml.entries[i];
        if (e->value.kind && e->depth > under->depth
            && toml_has_prefix(e, under->names, under->depth))
            manifest_add_name(into, e->key[under->depth]);
    }
}

manifest_names_t config_names(const config_t *c, const keyed_t *under)
{
    manifest_names_t names = {};

    for (uint32_t f = 0; f < c->count; f++)
        add_names(&c->files[f], under, &names);
    if (names.lost)
        say("no memory left for a list of names\n");
    return (names);
}
