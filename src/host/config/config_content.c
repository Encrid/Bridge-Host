/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "host.h"
#include "os.h"

static bool add_path(char ***paths, uint32_t *count, char *path)
{
    char *whole = path ? os_path_whole(path) : nullptr;
    char *other = nullptr;
    char **more = nullptr;
    bool same = false;

    for (uint32_t i = 0; whole && !same && i < *count; i++) {
        other = os_path_whole((*paths)[i]);
        same = other && !strcmp(other, whole);
        free(other);
    }
    free(whole);
    if (!same && path)
        more = host_realloc(*paths, sizeof(*more) * (*count + 1));
    if (more)
        *paths = more;
    if (more)
        (*paths)[(*count)++] = path;
    else
        free(path);
    return (same || more);
}

static bool collect(const host_t *h, const char *field, char ***paths,
    uint32_t *count)
{
    const char *const names[] = { "content", field };
    const keyed_t k = { names, 2 };
    said_t said = {};
    bool kept = true;

    *paths = nullptr;
    *count = 0;
    for (uint32_t f = 0; f < h->config.count; f++) {
        if (!config_states_key(&h->config.files[f], &k, &said))
            continue;
        for (uint32_t i = 0; i < said.value->count; i++)
            kept &= add_path(paths, count, config_path(&h->config, f,
                said.value->items[i].text));
    }
    return (kept);
}

bool config_content(host_t *h)
{
    bool kept = collect(h, "paths", &h->paths, &h->path_count);

    kept &= collect(h, "ignore", &h->ignore, &h->ignore_count);
    if (kept && !h->path_count)
        say("not stated: content.paths\n");
    h->files = host_calloc(h->d.files, sizeof(*h->files));
    h->found = host_calloc(h->d.files, sizeof(*h->found));
    h->watches = host_calloc(h->path_count, sizeof(*h->watches));
    return (kept && h->path_count > 0 && h->files && h->found
        && h->watches);
}

int config_load(const host_t *h, const char *identity, const char *prefix)
{
    const char *const own[] = { "content", "load", identity };
    const char *const all[] = { "content", "load", "all" };
    keyed_t k = { own, 3 };
    found_t f = config_find(&h->config, config_states_key, &k);

    if (!f.at.value) {
        k.names = all;
        f = config_find(&h->config, config_states_key, &k);
    }
    if (!config_say_found(&h->config, &k, &f, prefix))
        return (-1);
    return (f.at.value->flag ? 1 : 0);
}
