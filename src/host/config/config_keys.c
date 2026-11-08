/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stddef.h>
#include <string.h>

#include "bridge.h"
#include "config.h"

const key_rule_t WORLD_KEYS[] = {
    { "phases", CHECK_NAMES, 0, 0, 0, nullptr, true },
    { "threads", CHECK_COUNT, 4, offsetof(deployment_t, world.threads), 0,
        nullptr, true },
    { "stack", CHECK_SIZE, 8, offsetof(deployment_t, world.stack), 0,
        nullptr, true },
    { "observer_passes", CHECK_COUNT, 4,
        offsetof(deployment_t, world.observer_passes), 0, nullptr, true },
    { "table_growth", CHECK_COUNT, 4,
        offsetof(deployment_t, world.table_growth), 0, nullptr, true },
    { "table_hash_fill", CHECK_COUNT, 4,
        offsetof(deployment_t, world.table_hash_fill), 0, nullptr, true },
    { "table_retry", CHECK_COUNT, 4,
        offsetof(deployment_t, world.table_retry), 0, nullptr, true },
    { "rounds", CHECK_ROUNDS, 4, offsetof(deployment_t, rounds), 0, nullptr,
        true },
    { "live", CHECK_FLAG, 1, offsetof(deployment_t, live), 0, nullptr, true },
    { "profile", CHECK_FLAG, 1, offsetof(deployment_t, world.profile), 0,
        "checks", true },
    { "validate", CHECK_FLAG, 1, offsetof(deployment_t, world.validate), 0,
        "checks", true },
    { "usage", CHECK_FLAG, 1, offsetof(deployment_t, usage), 0, "checks",
        true },
    { "written", CHECK_RECORD, 1, offsetof(deployment_t, written), 0,
        "records", false },
    { "failing", CHECK_RECORD, 1, offsetof(deployment_t, failing), 0,
        "records", false },
    { "files", CHECK_COUNT, 4, offsetof(deployment_t, files), 0, "content",
        false },
    { "depth", CHECK_COUNT, 4, offsetof(deployment_t, depth), 0, "content",
        false }
};

const uint32_t WORLD_KEY_COUNT = sizeof(WORLD_KEYS) / sizeof(WORLD_KEYS[0]);

const key_rule_t BUDGET_KEYS[] = {
    { "entities", CHECK_COUNT, 8, offsetof(bridge_budget_t, entities), 0,
        "budget", true },
    { "tables", CHECK_COUNT, 4, offsetof(bridge_budget_t, tables), 0,
        "budget", true },
    { "types", CHECK_COUNT, 4, offsetof(bridge_budget_t, types), 0,
        "budget", true },
    { "systems", CHECK_COUNT, 4, offsetof(bridge_budget_t, systems), 0,
        "budget", true },
    { "terms", CHECK_COUNT, 4, offsetof(bridge_budget_t, terms), 0,
        "budget", true },
    { "modules", CHECK_COUNT, 4, offsetof(bridge_budget_t, modules), 0,
        "budget", true },
    { "persistent", CHECK_COUNT, 8, offsetof(bridge_budget_t, persistent), 0,
        "budget", true },
    { "columns", CHECK_COUNT, 8, offsetof(bridge_budget_t, columns), 0,
        "budget", true },
    { "sparse_sets", CHECK_LIMIT, 8, offsetof(bridge_budget_t, sparse_sets),
        BRIDGE_NONE, "budget", true },
    { "direct_indexed", CHECK_LIMIT, 8,
        offsetof(bridge_budget_t, direct_indexed), BRIDGE_NONE, "budget",
        true }
};

const uint32_t BUDGET_KEY_COUNT = sizeof(BUDGET_KEYS) / sizeof(BUDGET_KEYS[0]);

static bool is_table(const char *rule, const char *table)
{
    if (!rule || !table)
        return (rule == table);
    return (!strcmp(rule, table));
}

const key_rule_t *config_get_rule(const key_rule_t *rules, uint32_t count,
    const char *table, const char *name)
{
    for (uint32_t i = 0; i < count; i++)
        if (!strcmp(rules[i].name, name) && is_table(rules[i].table, table))
            return (&rules[i]);
    return (nullptr);
}

uint32_t config_get_names(const key_rule_t *rule, const char **names)
{
    names[0] = rule->table ? rule->table : rule->name;
    names[1] = rule->name;
    return (rule->table ? 2 : 1);
}

void config_put(void *into, const key_rule_t *rule, const value_t *v)
{
    int64_t n = v->kind == MANIFEST_WHOLE ? v->whole : rule->none;
    uint8_t one = (uint8_t)n;
    uint32_t four = (uint32_t)n;

    if (rule->check == CHECK_FLAG)
        one = v->flag ? BRIDGE_TRUE : BRIDGE_FALSE;
    if (rule->width == 1)
        memcpy((char *)into + rule->at, &one, 1);
    if (rule->width == 4)
        memcpy((char *)into + rule->at, &four, 4);
    if (rule->width == 8)
        memcpy((char *)into + rule->at, &n, 8);
}
