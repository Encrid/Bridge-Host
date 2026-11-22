/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "host.h"

const char *state_keep_prefix(made_t *m, const char *what, const char *path,
    const char *who)
{
    const stating_t keep = { .made = m };
    size_t size = strlen(what) + strlen(path) + strlen(who) + sizeof(": : : ");
    char *prefix = state_keep(&keep, host_calloc(size, 1));

    if (!prefix)
        return ("");
    snprintf(prefix, size, "%s: %s: %s: ", what, path, who);
    return (prefix);
}

void *state_keep(const stating_t *s, void *block)
{
    void *kept = manifest_keep(&s->made->lib, block);

    if (block && !kept)
        say("no memory left to keep a block\n");
    return (kept);
}

void state_free(made_t *m)
{
    manifest_free(&m->lib);
}
