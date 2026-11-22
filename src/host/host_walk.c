/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "host.h"

static os_folder_t *open_sub(const listed_t *at, const char *full, bool kept,
    uint32_t levels)
{
    return (kept && at->is_folder && levels > 1 ? os_folder_open(full)
        : nullptr);
}

static bool walk_folder(const walk_t *w, os_folder_t *in, const char *path,
    uint32_t levels)
{
    listing_t l = {};
    bool walked = list_folder(in, &l);
    bool kept = false;
    char *full = nullptr;
    os_folder_t *sub = nullptr;

    for (uint32_t i = 0; walked && i < l.count; i++) {
        full = join_path(path, l.items[i].name);
        walked = full != nullptr;
        kept = walked && !is_ignored(w->h, full);
        sub = open_sub(&l.items[i], full, kept, levels);
        if (sub)
            walked = walk_folder(w, sub, full, levels - 1);
        if (kept && !l.items[i].is_folder)
            w->see(w->ud, full, in, l.items[i].name);
        os_folder_close(sub);
        free(full);
    }
    free_listing(&l);
    return (walked);
}

bool walk_name(const char *path, see_fn *see, void *ud)
{
    size_t cut = config_folder_size(path);
    char *folder = host_calloc(cut + 1, 1);
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
    bool walked = false;

    if (!in)
        return (walk_file(&w, h->paths[at]));
    walked = walk_folder(&w, in, h->paths[at], h->d.depth);
    os_folder_close(in);
    if (walked)
        return (1);
    say("cannot look in '%s': no memory is left\n", h->paths[at]);
    return (-1);
}
