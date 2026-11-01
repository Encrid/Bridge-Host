/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "host.h"

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

bool walk_name(const char *path, see_fn *see, void *ud)
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
        see(ud, path, in, path + cut);
    os_folder_close(in);
    free(folder);
    return (seen);
}

static int walk_file(const walk_t *w, const char *path)
{
    if (walk_name(path, w->see, w->ud))
        return (0);
    say("cannot look in '%s'\n", path);
    return (-1);
}

int walk_path(const host_t *h, uint32_t at, see_fn *see, void *ud)
{
    const walk_t w = { h, see, ud };
    os_folder_t *in = os_folder_open(h->paths[at]);

    if (!in)
        return (walk_file(&w, h->paths[at]));
    walk_folder(&w, in, h->paths[at], h->d.depth);
    os_folder_close(in);
    return (1);
}
