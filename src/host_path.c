/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "host.h"

bool is_ignored(const host_t *h, const char *path)
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
    return (is);
}

char *join_path(const char *path, const char *name)
{
    size_t len = strlen(path);
    const char *between = config_folder_size(path) == len ? "" : "/";
    size_t size = len + strlen(name) + 2;
    char *full = calloc(size, 1);

    if (full)
        snprintf(full, size, "%s%s%s", path, between, name);
    return (full);
}
