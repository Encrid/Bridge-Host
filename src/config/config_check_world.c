/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "config.h"

static const char *check_rule(const key_rule_t *rules, uint32_t count,
    const entry_t *e, bool local)
{
    const char *table = e->depth == 2 ? e->key[0] : nullptr;
    const key_rule_t *rule = nullptr;

    if (e->depth <= 2)
        rule = config_get_rule(rules, count, table, e->key[e->depth - 1]);
    if (!rule)
        return ("no key of Bridge-Host's or Bridge's");
    if (local && rule->fixed)
        return ("fixed when the world is made: not in the local file");
    return (config_check_value(&e->value, rule->check));
}

const char *config_check_world(const entry_t *e, bool local)
{
    bool presets = e->depth == 1 && !strcmp(e->key[0], "presets");

    if (presets && local)
        return ("fixed when the world is made: not in the local file");
    if (presets)
        return (config_check_value(&e->value, CHECK_NAMES));
    if (!strcmp(e->key[0], "budget"))
        return (check_rule(BUDGET_KEYS, BUDGET_KEY_COUNT, e, local));
    return (check_rule(WORLD_KEYS, WORLD_KEY_COUNT, e, local));
}

const char *config_check_backend(const entry_t *e, bool local)
{
    if (e->depth != 3)
        return ("no key of Bridge-Host's or Bridge's");
    if (local)
        return ("fixed when the world is made: not in the local file");
    if (!strcmp(e->key[2], "library"))
        return (config_check_value(&e->value, CHECK_TEXT));
    return (config_check_value(&e->value, CHECK_VALUE));
}

const char *config_check_content(const entry_t *e)
{
    bool list = e->depth == 2 && (!strcmp(e->key[1], "paths")
        || !strcmp(e->key[1], "ignore"));

    if (list)
        return (config_check_value(&e->value, CHECK_NAMES));
    if (e->depth == 3 && !strcmp(e->key[1], "load"))
        return (config_check_value(&e->value, CHECK_FLAG));
    if (e->depth == 3 && !strcmp(e->key[1], "arrive"))
        return (config_check_value(&e->value, CHECK_ARRIVE));
    return (check_rule(WORLD_KEYS, WORLD_KEY_COUNT, e, false));
}
