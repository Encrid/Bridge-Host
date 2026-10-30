/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "host.h"

static void say_loaded(bridge_world_t *w)
{
    int count = bridge_list_modules(w, nullptr, 0);
    bridge_module_info_t *loaded = count > 0
        ? malloc(sizeof(*loaded) * (size_t)count) : nullptr;

    if (loaded)
        bridge_list_modules(w, loaded, count);
    say_part("still loaded:");
    for (int i = 0; loaded && i < count; i++)
        say_part("%s %s", i ? "," : "", loaded[i].identity);
    say("%s\n", count > 0 ? "" : " nothing");
    free(loaded);
}

static void say_profile(bridge_world_t *w)
{
    int count = bridge_profile(w, nullptr, 0);
    bridge_profile_row_t *rows = count > 0
        ? malloc(sizeof(*rows) * (size_t)count) : nullptr;

    if (rows)
        bridge_profile(w, rows, count);
    for (int i = 0; rows && i < count; i++)
        say("profile %s: %llu calls, %llu ns\n", rows[i].name,
            (unsigned long long)rows[i].calls,
            (unsigned long long)rows[i].nanos);
    free(rows);
}

static long long get_field(const bridge_budget_t *b, const key_rule_t *rule)
{
    uint32_t four = 0;
    int64_t eight = 0;

    if (rule->width == 4)
        memcpy(&four, (const char *)b + rule->at, 4);
    if (rule->width == 8)
        memcpy(&eight, (const char *)b + rule->at, 8);
    return (rule->width == 4 ? (long long)four : (long long)eight);
}

static void say_usage(bridge_world_t *w)
{
    bridge_budget_t used = {};
    bridge_budget_t most = {};
    int count = bridge_module_usage(w, nullptr, 0);
    bridge_module_usage_row_t *rows = count > 0
        ? malloc(sizeof(*rows) * (size_t)count) : nullptr;

    bridge_usage(w, &used, &most);
    for (uint32_t i = 0; i < BUDGET_KEY_COUNT; i++)
        say("usage %s: %lld now, %lld at most\n", BUDGET_KEYS[i].name,
            get_field(&used, &BUDGET_KEYS[i]),
            get_field(&most, &BUDGET_KEYS[i]));
    if (rows)
        bridge_module_usage(w, rows, count);
    for (int i = 0; rows && i < count; i++)
        say("usage %s: %llu persistent, %u systems, %u observers, "
            "%u entities\n", rows[i].identity,
            (unsigned long long)rows[i].persistent, rows[i].systems,
            rows[i].observers, rows[i].entities);
    free(rows);
}

void say_end(const host_t *h)
{
    say_loaded(h->w);
    if (h->d.world.profile == BRIDGE_TRUE)
        say_profile(h->w);
    if (h->d.usage == BRIDGE_TRUE)
        say_usage(h->w);
}
