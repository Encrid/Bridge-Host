/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdlib.h>
#include <string.h>

#include "manifest_inside.h"

void manifest_add_name(name_list_t *into, const char *name)
{
    const char **more = nullptr;

    for (uint32_t i = 0; i < into->count; i++)
        if (!strcmp(into->names[i], name))
            return;
    more = realloc(into->names, sizeof(*more) * (into->count + 1));
    if (!more)
        return;
    into->names = more;
    into->names[into->count++] = name;
}

static void put_names(bridge_module_config_t *cfg, const char *key,
    const name_list_t *got, bool none)
{
    if (key[0] == 'r') {
        cfg->read = got->names;
        cfg->read_count = got->count;
    } else if (key[0] == 'w') {
        cfg->write = got->names;
        cfg->write_count = got->count;
    } else {
        cfg->losses = got->names;
        cfg->loss_count = none ? BRIDGE_NONE : got->count;
    }
}

bool manifest_take_list(const manifest_module_t *m, const key_rule_t *rule,
    bridge_module_config_t *cfg)
{
    const key_path_t path = { &rule->name, 1 };
    manifest_found_t found = {};
    const manifest_value_t *v = manifest_find_value(m, &path, rule->check,
        &found);
    name_list_t got = {};

    if (!v)
        return (false);
    for (uint32_t i = 0; v->kind == MANIFEST_LIST && i < v->count; i++)
        manifest_add_name(&got, v->items[i].text);
    got.names = manifest_keep(m, got.names);
    got.count = got.names ? got.count : 0;
    put_names(cfg, rule->name, &got, v->kind != MANIFEST_LIST);
    return (true);
}
