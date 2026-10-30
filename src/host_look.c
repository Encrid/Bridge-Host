/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "host.h"

static int order_files(const void *left, const void *right)
{
    const file_t *l = *(file_t *const *)left;
    const file_t *r = *(file_t *const *)right;
    int order = strcmp(l->of->who.identity, r->of->who.identity);

    return (order ? order : strcmp(l->path, r->path));
}

static void read_file(seeing_t *s, file_t *f, const char *path)
{
    handed_t now = {};
    handed_t *kept = nullptr;
    uint64_t mark = 0;

    now.bytes = read_whole(path, &now.len);
    if (!now.bytes)
        return;
    mark = hash_bytes(now.bytes, now.len);
    f->seen = true;
    if (!f->path || f->mark != mark)
        kept = malloc(sizeof(*kept));
    if (!kept) {
        free(now.bytes);
        return;
    }
    *kept = now;
    f->mark = mark;
    if (meet(s->h, f, path, kept))
        s->lk->found[s->lk->count++] = f;
    s->lk->did++;
}

static void see_file(void *ud, const char *path)
{
    seeing_t *s = ud;
    file_t *f = find_file(s->h, path);

    s->lk->full = s->lk->full || !f;
    if (f)
        read_file(s, f, path);
}

static void drop_gone(host_t *h, look_t *lk)
{
    file_t *f = nullptr;

    for (uint32_t i = 0; i < h->d.files; i++) {
        f = &h->files[i];
        if (!f->path || f->seen)
            continue;
        if (f->loaded && !bridge_unload(h->w, f->of->who.identity))
            say("unloaded: %s: %s is gone\n", f->of->who.identity,
                f->path);
        lk->did += f->loaded;
        free(f->path);
        handed_free(f->of);
        *f = (file_t){};
    }
}

int look(host_t *h, bool first)
{
    look_t lk = { .found = h->found };
    seeing_t s = { h, &lk };

    for (uint32_t i = 0; i < h->d.files; i++)
        h->files[i].seen = false;
    if (walk_content(h, see_file, &s) < 0)
        return (-1);
    if (lk.full && !h->full) {
        say("too many files: %u are looked at\n", h->d.files);
        lk.did++;
    }
    h->full = lk.full;
    drop_gone(h, &lk);
    qsort(lk.found, (size_t)lk.count, sizeof(lk.found[0]), order_files);
    for (int i = 0; i < lk.count; i++)
        load_found(h, lk.found[i], first);
    return (lk.did);
}
