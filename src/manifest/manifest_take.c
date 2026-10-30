/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdlib.h>
#include <string.h>

#include "manifest_inside.h"

constexpr uint32_t AUTHOR_WORDS = 5;

static const char *const NAMES[AUTHOR_WORDS] = { "abi", "config", "settings",
    "pinned", "reloadable" };

static const uint8_t WORDS[AUTHOR_WORDS] = { MANIFEST_ABI, MANIFEST_CONFIG,
    MANIFEST_SETTINGS, MANIFEST_PINNED, MANIFEST_RELOADABLE };

static bool is_key(const char *key, size_t len, const char *name)
{
    return (len == strlen(name) && !memcmp(key, name, len));
}

static bool take_flag(bool *flag, const char *value)
{
    *flag = !strcmp(value, "true");
    return (*flag || !strcmp(value, "false"));
}

static bool take_value(manifest_t *m, uint8_t word, const char *value)
{
    char *end = nullptr;

    switch (word) {
        case MANIFEST_PINNED:
            return (take_flag(&m->pinned, value));
        case MANIFEST_RELOADABLE:
            return (take_flag(&m->reloadable, value));
        case MANIFEST_CONFIG:
            m->config = value;
            return (true);
        case MANIFEST_SETTINGS:
            m->settings = value;
            return (true);
        default:
            m->abi = strtoll(value, &end, 10);
            return (*value && !*end && m->abi >= 0);
    }
}

int manifest_take(manifest_t *m, const char *key, size_t len,
    const char *value)
{
    uint32_t at = 0;

    while (at < AUTHOR_WORDS && !is_key(key, len, NAMES[at]))
        at++;
    if (at == AUTHOR_WORDS)
        return (0);
    if (m->taken & WORDS[at])
        return (-2);
    m->taken |= WORDS[at];
    return (take_value(m, WORDS[at], value) ? 1 : -1);
}
