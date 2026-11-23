/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "host.h"

static void say_ignored(host_t *h, const char *path)
{
    if (BRIDGE_DEBUG <= h->d.written)
        say("ignored: %s\n", path);
    h->counts.failed += BRIDGE_DEBUG <= h->d.failing;
}

bool is_ignored(host_t *h, const char *path)
{
    char *whole = os_path_whole(path);
    char *ignored = nullptr;
    size_t len = 0;
    bool is = false;

    for (uint32_t i = 0; whole && !is && i < h->ignore_count; i++) {
        ignored = os_path_whole(h->ignore[i]);
        len = ignored ? strlen(ignored) : 0;
        is = ignored && !strncmp(whole, ignored, len)
            && (!whole[len] || whole[len] == '/' || whole[len] == '\\');
        free(ignored);
    }
    free(whole);
    if (is)
        say_ignored(h, path);
    return (is);
}

char *join_path(const char *path, const char *name)
{
    size_t len = strlen(path);
    const char *between = config_folder_size(path) == len ? "" : "/";
    size_t size = len + strlen(name) + 2;
    char *full = host_calloc(size, 1);

    if (full)
        snprintf(full, size, "%s%s%s", path, between, name);
    return (full);
}
