/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "host.h"

static bool permit_in(const config_t *c, uint32_t file,
    const module_key_t *k, names_t *into)
{
    const cfile_t *f = &c->files[file];
    said_t said = {};
    bool stated = false;

    if (config_states_module(f, k, &said)) {
        for (uint32_t i = 0; i < said.value->count; i++)
            names_add(into, said.value->items[i].text);
        return (true);
    }
    for (uint32_t i = 0; i < f->named_count; i++)
        stated = permit_in(c, f->named[i], k, into) || stated;
    return (stated);
}

static bool find_names(const stating_t *s, const char *name, names_t *got)
{
    const char *const names[] = { name };
    const module_key_t k = { { s->of->who.identity, s->of->who.backend,
        "all" }, { names, 1 } };
    const config_t *c = &s->h->config;
    said_t said = {};

    if (c->local >= 0 && config_states_module(&c->files[c->local], &k,
        &said))
        return (permit_in(c, (uint32_t)c->local, &k, got));
    return (permit_in(c, 0, &k, got));
}

static void say_names(const stating_t *s, const char *key,
    const names_t *got)
{
    if (!s->show)
        return;
    say_part("  %s = [", key);
    for (uint32_t i = 0; i < got->count; i++)
        say_part("%s\"%s\"", i ? ", " : "", got->names[i]);
    say("]\n");
}

static value_t *make_list(const stating_t *s, const names_t *got)
{
    value_t *v = state_keep(s, calloc(1, sizeof(*v)));
    value_t *items = state_keep(s, calloc(got->count + 1, sizeof(*items)));

    if (!v || !items)
        return (nullptr);
    for (uint32_t i = 0; i < got->count; i++)
        items[i] = (value_t){ .kind = MANIFEST_TEXT,
            .text = (char *)got->names[i] };
    *v = (value_t){ .kind = MANIFEST_LIST, .items = items,
        .count = got->count };
    return (v);
}

bool state_find_grant(const stating_t *s, const char *name,
    manifest_found_t *found)
{
    names_t got = {};
    bool stated = find_names(s, name, &got);

    if (stated) {
        say_names(s, name, &got);
        *found = (manifest_found_t){ make_list(s, &got), nullptr };
    }
    free(got.names);
    return (stated);
}
