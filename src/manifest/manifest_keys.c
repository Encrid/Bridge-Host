/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stddef.h>
#include <string.h>

#include "manifest_inside.h"

static const key_rule_t KEYS[] = {
    { "hot_reload", MANIFEST_CHECK_FLAG, 1,
        offsetof(bridge_module_config_t, hot_reload), 0 },
    { "on_error", MANIFEST_CHECK_ERROR, 1,
        offsetof(bridge_module_config_t, on_error), 0 },
    { "element_limit", MANIFEST_CHECK_LIMIT, 8,
        offsetof(bridge_module_config_t, element_limit), BRIDGE_NO_LIMIT },
    { "max_system_failures", MANIFEST_CHECK_LIMIT, 8,
        offsetof(bridge_module_config_t, max_system_failures),
        BRIDGE_NO_LIMIT },
    { "work_limit", MANIFEST_CHECK_LIMIT, 8,
        offsetof(bridge_module_config_t, work_limit), BRIDGE_NO_LIMIT },
    { "memory_limit", MANIFEST_CHECK_LIMIT, 8,
        offsetof(bridge_module_config_t, memory_limit), BRIDGE_NO_LIMIT },
    { "init_timeout", MANIFEST_CHECK_LIMIT, 8,
        offsetof(bridge_module_config_t, init_timeout), BRIDGE_NO_LIMIT },
    { "reload_timeout", MANIFEST_CHECK_LIMIT, 8,
        offsetof(bridge_module_config_t, reload_timeout), BRIDGE_NO_LIMIT },
    { "shutdown_timeout", MANIFEST_CHECK_LIMIT, 8,
        offsetof(bridge_module_config_t, shutdown_timeout), BRIDGE_NO_LIMIT },
    { "log_level", MANIFEST_CHECK_LEVEL, 1,
        offsetof(bridge_module_config_t, log_level), 0 },
    { "scratch", MANIFEST_CHECK_COUNT, 4,
        offsetof(bridge_module_config_t, scratch), 0 },
    { "log_ring", MANIFEST_CHECK_COUNT, 4,
        offsetof(bridge_module_config_t, log_ring), 0 },
    { "spans", MANIFEST_CHECK_COUNT, 4,
        offsetof(bridge_module_config_t, spans), 0 },
    { "spawns", MANIFEST_CHECK_LIMIT, 8,
        offsetof(bridge_module_config_t, spawns), BRIDGE_NONE },
    { "persistent", MANIFEST_CHECK_LIMIT, 8,
        offsetof(bridge_module_config_t, persistent), BRIDGE_NONE },
    { "changes", MANIFEST_CHECK_LIMIT, 8,
        offsetof(bridge_module_config_t, changes), BRIDGE_NONE },
    { "losses", MANIFEST_CHECK_NAMES_NONE, 0, 0, BRIDGE_NONE },
    { "read", MANIFEST_CHECK_NAMES, 0, 0, 0 },
    { "write", MANIFEST_CHECK_NAMES, 0, 0, 0 },
    { "abi", MANIFEST_CHECK_AUTHOR, 0, 0, 0 },
    { "config", MANIFEST_CHECK_AUTHOR, 0, 0, 0 },
    { "settings", MANIFEST_CHECK_AUTHOR, 0, 0, 0 },
    { "pinned", MANIFEST_CHECK_AUTHOR, 0, 0, 0 },
    { "reloadable", MANIFEST_CHECK_AUTHOR, 0, 0, 0 }
};

constexpr uint32_t KEY_COUNT = sizeof(KEYS) / sizeof(KEYS[0]);

const key_rule_t *manifest_get_key(uint32_t at)
{
    return (at < KEY_COUNT ? &KEYS[at] : nullptr);
}

const key_rule_t *manifest_find_key(const char *name)
{
    for (uint32_t i = 0; i < KEY_COUNT; i++)
        if (!strcmp(KEYS[i].name, name))
            return (&KEYS[i]);
    return (nullptr);
}

const char *manifest_check_key(const char *name, const manifest_value_t *v)
{
    const key_rule_t *rule = manifest_find_key(name);

    if (!rule)
        return ("no key of a module");
    return (manifest_check_value(v, manifest_get_rule(rule->check)));
}

void manifest_put(bridge_module_config_t *cfg, const key_rule_t *rule,
    const manifest_value_t *v)
{
    int64_t n = v->kind == MANIFEST_WHOLE ? v->whole : rule->none;
    uint8_t one = (uint8_t)n;
    uint32_t four = (uint32_t)n;

    if (rule->check == MANIFEST_CHECK_FLAG)
        one = v->flag ? BRIDGE_TRUE : BRIDGE_FALSE;
    if (rule->check == MANIFEST_CHECK_ERROR)
        one = v->text[0] == 'f' ? BRIDGE_FAIL : BRIDGE_SKIP;
    if (rule->width == 1)
        memcpy((char *)cfg + rule->at, &one, 1);
    if (rule->width == 4)
        memcpy((char *)cfg + rule->at, &four, 4);
    if (rule->width == 8)
        memcpy((char *)cfg + rule->at, &n, 8);
}
