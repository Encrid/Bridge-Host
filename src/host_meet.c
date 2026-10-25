/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "host.h"

file_t *find_file(host_t *h, const char *path)
{
    file_t *free_one = nullptr;

    for (int i = 0; i < FILES; i++) {
        if (h->files[i].path && !strcmp(h->files[i].path, path))
            return (&h->files[i]);
        if (!h->files[i].path && !free_one)
            free_one = &h->files[i];
    }
    return (free_one);
}

static bool keep_old(const file_t *f, const handed_t *now, const char *why)
{
    if (why)
        printf("not replaced: %s: %s%s%s\n", f->path, now->who.identity,
            now->who.identity[0] ? " " : "", why);
    else if (!f->of->who.reloadable)
        printf("not replaced: %s: it does not say it can be\n",
            f->of->who.identity);
    return (why || !f->of->who.reloadable);
}

static bool is_reloaded(host_t *h, const file_t *f, const handed_t *now)
{
    char prefix[1200];
    made_t made = {};
    const stating_t s = { h, now, prefix, false, &made };
    bridge_artifact_t a;
    bool reloaded = false;

    snprintf(prefix, sizeof(prefix), "not replaced: %s: %s: ", f->path,
        now->who.identity);
    reloaded = now->who.abi == BRIDGE_ABI && make_artifact(&s, f, &a)
        && !bridge_reload(h->w, &a, 1);
    state_free(&made);
    return (reloaded);
}

static void replace(host_t *h, file_t *f, handed_t *now, const char *why)
{
    if (keep_old(f, now, why)) {
        handed_free(now);
        return;
    }
    if (!is_reloaded(h, f, now)) {
        printf("not replaced: %s: %s was refused\n", f->path,
            now->who.identity);
        h->counts.failed++;
        handed_free(now);
        return;
    }
    printf("replaced: %s by %s, %s\n", f->of->who.identity,
        now->who.identity, f->path);
    handed_free(f->of);
    f->of = now;
}

bool meet(host_t *h, file_t *f, const char *path, handed_t *now)
{
    const char *why = manifest_read((const unsigned char *)now->bytes,
        now->len, &now->who);

    if (f->path && f->loaded) {
        replace(h, f, now, why);
        return (false);
    }
    if (!f->path)
        f->path = malloc(strlen(path) + 1);
    if (f->path)
        strcpy(f->path, path);
    handed_free(f->of);
    f->of = now;
    if (why && !now->who.identity[0])
        printf("not a module: %s: %s\n", path, why);
    else if (why)
        printf("not loaded: %s: %s %s\n", path, now->who.identity, why);
    return (!why && f->path);
}
