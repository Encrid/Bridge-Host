/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "host.h"

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
    size_t len = strlen(path);
    const char *between = config_folder_size(path) == len ? "" : "/";
    size_t size = len + strlen(name) + 2;
    char *full = calloc(size, 1);

    if (full)
        snprintf(full, size, "%s%s%s", path, between, name);
    return (full);
}

static void walk_folder(const walk_t *w, os_folder_t *in, const char *path,
    uint32_t levels)
{
    bool is_folder = false;
    char *full = nullptr;
    bool kept = false;
    os_folder_t *sub = nullptr;

    for (const char *name = os_folder_read_entry(in, &is_folder); name;
        name = os_folder_read_entry(in, &is_folder)) {
        full = join_path(path, name);
        kept = full && !is_ignored(w->h, full);
        sub = kept && is_folder && levels > 1 ? os_folder_open(full) : nullptr;
        if (sub)
            walk_folder(w, sub, full, levels - 1);
        if (kept && !is_folder)
            w->see(w->ud, full, in, name);
        os_folder_close(sub);
        free(full);
    }
}

static int walk_file(const walk_t *w, const char *path)
{
    size_t cut = config_folder_size(path);
    char *folder = calloc(cut + 1, 1);
    os_folder_t *in = nullptr;
    size_t len = 0;
    bool seen = false;

    if (folder)
        memcpy(folder, path, cut);
    if (folder)
        in = os_folder_open(cut ? folder : ".");
    seen = in && os_folder_read_file(in, path + cut, &len);
    if (seen)
        w->see(w->ud, path, in, path + cut);
    else
        say("cannot look in '%s'\n", path);
    os_folder_close(in);
    free(folder);
    return (seen ? 0 : -1);
}

int walk_content(const host_t *h, see_fn *see, void *ud)
{
    const walk_t w = { h, see, ud };
    os_folder_t *in = nullptr;
    bool folder = false;

    for (uint32_t i = 0; i < h->path_count; i++) {
        in = os_folder_open(h->paths[i]);
        folder = in != nullptr;
        if (folder)
            walk_folder(&w, in, h->paths[i], h->d.depth);
        os_folder_close(in);
        if (!folder && walk_file(&w, h->paths[i]) < 0)
            return (-1);
    }
    return (0);
}
