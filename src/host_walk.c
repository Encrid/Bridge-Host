/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <dirent.h>

#include "host.h"
#include "os.h"

static bool is_ignored(const host_t *h, const char *path)
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

static char *join_path(const char *path, const char *name)
{
    size_t size = strlen(path) + strlen(name) + 2;
    char *full = nullptr;

    if (!strcmp(name, ".") || !strcmp(name, ".."))
        return (nullptr);
    full = calloc(size, 1);
    if (full)
        snprintf(full, size, "%s/%s", path, name);
    return (full);
}

static void walk_folder(const walk_t *w, const char *path, uint32_t levels)
{
    DIR *d = opendir(path);
    DIR *sub = nullptr;
    char *full = nullptr;
    bool kept = false;

    for (struct dirent *e = d ? readdir(d) : nullptr; e; e = readdir(d)) {
        full = join_path(path, e->d_name);
        kept = full && !is_ignored(w->h, full);
        sub = kept ? opendir(full) : nullptr;
        if (sub)
            closedir(sub);
        if (sub && levels > 1)
            walk_folder(w, full, levels - 1);
        if (kept && !sub)
            w->see(w->ud, full);
        free(full);
    }
    if (d)
        closedir(d);
}

static int walk_path(const walk_t *w, const char *path)
{
    DIR *d = opendir(path);
    FILE *file = d ? nullptr : fopen(path, "rb");

    if (d)
        closedir(d);
    if (!d && !file) {
        say("cannot look in '%s'\n", path);
        return (-1);
    }
    if (file) {
        fclose(file);
        w->see(w->ud, path);
        return (0);
    }
    walk_folder(w, path, w->h->d.depth);
    return (0);
}

int walk_content(const host_t *h, see_fn *see, void *ud)
{
    const walk_t w = { h, see, ud };

    for (uint32_t i = 0; i < h->path_count; i++)
        if (walk_path(&w, h->paths[i]) < 0)
            return (-1);
    return (0);
}
