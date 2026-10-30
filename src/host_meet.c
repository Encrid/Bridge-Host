/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "host.h"

static bool keep_old(const file_t *f, const handed_t *now, const char *why)
{
    if (why)
        say("not replaced: %s: %s%s%s\n", f->path, now->who.identity,
            now->who.identity[0] ? " " : "", why);
    else if (!f->of->who.reloadable)
        say("not replaced: %s: it does not say it can be\n",
            f->of->who.identity);
    return (why || !f->of->who.reloadable);
}

static enum replaced reload_found(host_t *h, const file_t *f,
    const handed_t *now)
{
    made_t made = {};
    const char *prefix = state_keep_prefix(&made, "not replaced", f->path,
        now->who.identity);
    const stating_t s = { h, now, prefix, false, &made };
    const value_t *hot = nullptr;
    bridge_artifact_t a;
    enum replaced got = REPLACE_REFUSED;

    if (now->who.abi == BRIDGE_ABI)
        hot = state_find_key(&s, config_get_rule(MODULE_KEYS,
            MODULE_KEY_COUNT, nullptr, "hot_reload"));
    if (hot && !hot->flag)
        got = REPLACE_NOT_HOT;
    else if (hot && make_artifact(&s, f, &a) && !bridge_reload(h->w, &a, 1))
        got = REPLACED;
    state_free(&made);
    return (got);
}

static void replace(host_t *h, file_t *f, handed_t *now, const char *why)
{
    enum replaced got = keep_old(f, now, why) ? REPLACE_KEPT
        : reload_found(h, f, now);

    if (got == REPLACE_REFUSED)
        say("not replaced: %s: %s was refused\n", f->path,
            now->who.identity);
    if (got == REPLACE_NOT_HOT)
        say("not replaced: %s: its hot_reload is false\n",
            now->who.identity);
    h->counts.failed += got == REPLACE_REFUSED;
    if (got != REPLACED) {
        free(now);
        return;
    }
    say("replaced: %s by %s, %s\n", f->of->who.identity,
        now->who.identity, f->path);
    free(f->of);
    f->of = now;
}

bool meet(host_t *h, file_t *f, const char *path, handed_t *now)
{
    const char *why = manifest_read((const unsigned char *)now->bytes,
        now->len, &now->who);
    bool kept = false;

    if (f->path && f->loaded) {
        replace(h, f, now, why);
        return (false);
    }
    kept = keep_path(f, path);
    free(f->of);
    f->of = now;
    if (why && !now->who.identity[0])
        say("not a module: %s: %s\n", path, why);
    else if (why)
        say("not loaded: %s: %s %s\n", path, now->who.identity, why);
    return (!why && kept);
}
