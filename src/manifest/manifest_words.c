/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "manifest_inside.h"

constexpr uint32_t WORDS_SHOWN = 4;

static void tell_missing(const manifest_module_t *m, const char *name)
{
    const key_path_t path = { &name, 1 };

    manifest_tell(m, MANIFEST_TOLD_NO_WORD, &path, nullptr);
}

static void tell_words(const manifest_module_t *m)
{
    static const char *const NAMES[WORDS_SHOWN] = { "config", "settings",
        "pinned", "reloadable" };
    const manifest_value_t values[WORDS_SHOWN] = {
        { .kind = MANIFEST_TEXT, .text = (char *)m->who->config },
        { .kind = MANIFEST_TEXT, .text = (char *)m->who->settings },
        { .kind = MANIFEST_FLAG, .flag = m->who->pinned },
        { .kind = MANIFEST_FLAG, .flag = m->who->reloadable } };

    for (uint32_t i = 0; i < WORDS_SHOWN; i++)
        manifest_tell(m, MANIFEST_TOLD_VALUE, &(key_path_t){ &NAMES[i], 1 },
            &values[i]);
}

bool manifest_own_words(const manifest_module_t *m)
{
    bool config = m->who->config[0];
    bool pinned = m->who->taken & MANIFEST_PINNED;
    bool reloadable = m->who->taken & MANIFEST_RELOADABLE;

    if (!config)
        tell_missing(m, "config");
    if (!pinned)
        tell_missing(m, "pinned");
    if (!reloadable)
        tell_missing(m, "reloadable");
    tell_words(m);
    return (config && pinned && reloadable);
}
