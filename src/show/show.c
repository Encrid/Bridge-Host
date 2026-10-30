/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "host.h"

static void say_files(const config_t *c)
{
    const cfile_t *f = nullptr;

    for (uint32_t i = 0; i < c->count; i++) {
        f = &c->files[i];
        say_part("file %s", f->path);
        if (f->by >= 0)
            say_part(", named by %s:%u", c->files[f->by].path, f->by_line);
        say("%s\n", (int32_t)i == c->local ? ", the local file" : "");
    }
}

static void say_key(const config_t *c, const char *const *names,
    uint32_t depth)
{
    const keyed_t k = { names, depth };
    const shown_t how = { "", config_states_key, &k };
    found_t f = config_find(c, config_states_key, &k);
    char key[256];

    config_say_value(c, config_say_key(names, depth, key, sizeof(key)), &f,
        &how);
}

static void say_under(const config_t *c, const char *top, const char *next)
{
    const char *key[3] = { top, next, nullptr };
    const keyed_t k = { key, 2 };
    names_t names = config_names(c, &k);

    for (uint32_t i = 0; i < names.count; i++) {
        key[2] = names.names[i];
        say_key(c, key, 3);
    }
    free(names.names);
}

static void say_world(const host_t *h)
{
    const char *names[2] = {};

    for (uint32_t i = 0; i < WORLD_KEY_COUNT; i++)
        say_key(&h->config, names, config_get_names(&WORLD_KEYS[i], names));
    for (uint32_t i = 0; i < BUDGET_KEY_COUNT; i++)
        say_key(&h->config, names, config_get_names(&BUDGET_KEYS[i], names));
    for (uint32_t i = 0; i < h->backend_count; i++)
        say_under(&h->config, "backend", h->held[i].name);
    say_under(&h->config, "content", "start");
    say_under(&h->config, "content", "arrive");
    say_under(&h->config, "content", "load");
    for (uint32_t i = 0; i < h->path_count; i++)
        say("content.paths: %s\n", h->paths[i]);
    for (uint32_t i = 0; i < h->ignore_count; i++)
        say("content.ignore: %s\n", h->ignore[i]);
    for (uint32_t i = 0; i < h->order_count; i++)
        say("order: %s before %s\n", h->order[i].before,
            h->order[i].after);
}

int show_config(host_t *h, bool taken)
{
    say_files(&h->config);
    if (!taken)
        return (1);
    say_world(h);
    if (walk_content(h, show_module, h) < 0)
        return (1);
    return (h->counts.failed ? 1 : 0);
}
