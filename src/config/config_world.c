/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "host.h"

static bool take_phases(host_t *h)
{
    static const char *const PHASES[] = { "phases" };
    const value_t *v = config_find_key(&h->config, PHASES, 1).value;

    if (!v)
        return (false);
    h->phases = malloc(sizeof(*h->phases) * (v->count + 1));
    for (uint32_t i = 0; h->phases && i < v->count; i++)
        h->phases[i] = v->items[i].text;
    h->phase_count = v->count;
    return (h->phases != nullptr);
}

static bool take_rules(host_t *h, const key_rule_t *rules, uint32_t count,
    void *into)
{
    const char *names[2] = {};
    uint32_t depth = 0;
    const value_t *v = nullptr;
    bool taken = true;

    for (uint32_t i = 0; i < count; i++) {
        if (!rules[i].width)
            continue;
        depth = config_get_names(&rules[i], names);
        v = config_find_key(&h->config, names, depth).value;
        if (v)
            config_put(into, &rules[i], v);
        taken = v && taken;
    }
    return (taken);
}

bool config_world(host_t *h)
{
    bool taken = take_phases(h);

    taken = take_rules(h, WORLD_KEYS, WORLD_KEY_COUNT, &h->d) && taken;
    taken = take_rules(h, BUDGET_KEYS, BUDGET_KEY_COUNT, &h->d.world.budget)
        && taken;
    return (config_order(h) && taken);
}
