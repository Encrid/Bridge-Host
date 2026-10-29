/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdlib.h>
#include <string.h>

#include "manifest_inside.h"

static bool is_key(const char *key, size_t len, const char *name)
{
    return (len == strlen(name) && !memcmp(key, name, len));
}

static int take_number(manifest_t *m, const char *key, size_t len,
    const char *value)
{
    char *end = nullptr;
    int64_t n = strtoll(value, &end, 10);

    if (!is_key(key, len, "abi"))
        return (0);
    if (!*value || *end || n < 0)
        return (-1);
    m->abi = n;
    return (1);
}

static int take_word(manifest_t *m, const char *key, size_t len,
    const char *value)
{
    if (is_key(key, len, "config"))
        m->config = value;
    else if (is_key(key, len, "settings"))
        m->settings = value;
    else
        return (0);
    return (1);
}

static int take_flag(manifest_t *m, const char *key, size_t len,
    const char *value)
{
    bool pinned = is_key(key, len, "pinned");

    if (!pinned && !is_key(key, len, "reloadable"))
        return (0);
    if (strcmp(value, "true") && strcmp(value, "false"))
        return (-1);
    if (pinned)
        m->pinned = value[0] == 't';
    else
        m->reloadable = value[0] == 't';
    m->has_pinned = m->has_pinned || pinned;
    m->has_reloadable = m->has_reloadable || !pinned;
    return (1);
}

int manifest_take(manifest_t *m, const char *key, size_t len,
    const char *value)
{
    int taken = take_number(m, key, len, value);

    if (!taken)
        taken = take_word(m, key, len, value);
    if (!taken)
        taken = take_flag(m, key, len, value);
    return (taken);
}
