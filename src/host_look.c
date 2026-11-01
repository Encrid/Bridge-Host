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

static void read_file(seeing_t *s, file_t *f, const char *path,
    const handed_t *now)
{
    uint64_t mark = hash_bytes(now->bytes, now->len);
    handed_t *kept = nullptr;

    f->unread = false;
    if (!f->path || f->mark != mark)
        kept = malloc(sizeof(*kept) + now->len + 1);
    if (!kept)
        return;
    *kept = (handed_t){ .bytes = memcpy(kept + 1, now->bytes, now->len + 1),
        .len = now->len };
    f->mark = mark;
    if (meet(s->h, f, path, kept))
        s->lk->found[s->lk->count++] = f;
    s->lk->did++;
}

void see_file(void *ud, const char *path, os_folder_t *in, const char *name)
{
    seeing_t *s = ud;
    file_t *f = find_file(s->h, path);
    handed_t now = {};

    s->lk->full = s->lk->full || !f;
    if (f)
        now.bytes = os_folder_read_file(in, name, &now.len);
    if (now.bytes) {
        read_file(s, f, path, &now);
    } else if (f) {
        if (!f->unread)
            say("cannot read '%s'\n", path);
        s->lk->did += !f->unread;
        f->unread = keep_path(f, path);
    }
    if (!f)
        return;
    f->at = s->at;
    f->seen = true;
}

int read_path(host_t *h, seeing_t *s)
{
    int read = 0;

    for (uint32_t i = 0; i < h->d.files; i++)
        if (h->files[i].at == s->at)
            h->files[i].seen = false;
    read = walk_path(h, s->at, see_file, s);
    if (read >= 0)
        drop_gone(h, s->lk);
    s->lk->whole++;
    return (read);
}

int look(host_t *h, enum look_kind kind)
{
    look_t lk = { .found = h->found };
    seeing_t s = { h, &lk, 0 };

    for (s.at = 0; s.at < h->path_count; s.at++)
        if (look_path(h, kind, &s) < 0)
            return (-1);
    retry_unread(h, &s);
    if (lk.full && !h->full) {
        say("too many files: %u are looked at\n", h->d.files);
        lk.did++;
    }
    h->full = lk.whole == h->path_count ? lk.full : h->full || lk.full;
    qsort(lk.found, (size_t)lk.count, sizeof(lk.found[0]), order_files);
    for (int i = 0; i < lk.count; i++)
        load_found(h, lk.found[i], kind == LOOK_FIRST);
    return (lk.did);
}
