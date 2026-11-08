/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "bridge_abi.h"

#include "manifest_inside.h"

static enum bridge_flag get_flag(bool yes)
{
    return (yes ? BRIDGE_TRUE : BRIDGE_FALSE);
}

const manifest_value_t *manifest_find_value(const manifest_module_t *m,
    const key_path_t *path, enum manifest_check check,
    manifest_found_t *found)
{
    int got = m->host->find(m->host->ud, path->names, path->depth, found);

    if (got)
        return (got > 0 ? found->value : nullptr);
    *found = (manifest_found_t){ manifest_find_own(m, path, check), nullptr };
    manifest_tell(m, found->value ? MANIFEST_TOLD_VALUE
        : MANIFEST_TOLD_NOT_STATED, path, found->value);
    return (found->value);
}

static bool take_key(const manifest_module_t *m, const key_rule_t *rule,
    bridge_module_config_t *cfg)
{
    const key_path_t path = { &rule->name, 1 };
    manifest_found_t found = {};
    const manifest_value_t *v = nullptr;

    if (rule->check == MANIFEST_CHECK_AUTHOR)
        return (true);
    if (!rule->width)
        return (manifest_take_list(m, rule, cfg));
    v = manifest_find_value(m, &path, rule->check, &found);
    if (v)
        manifest_put(cfg, rule, v);
    return (v != nullptr);
}

bool manifest_state(const manifest_module_t *m, bridge_module_config_t *cfg)
{
    const manifest_t *who = m->who;
    bool taken = manifest_own_words(m);

    *cfg = (bridge_module_config_t){ .identity = who->identity,
        .abi = BRIDGE_ABI, .config = who->config,
        .pinned = get_flag(who->pinned),
        .reloadable = get_flag(who->reloadable) };
    for (uint32_t i = 0; manifest_get_key(i); i++)
        taken = take_key(m, manifest_get_key(i), cfg) && taken;
    taken = manifest_take_phases(m, cfg) && taken;
    return (manifest_take_settings(m, cfg) && taken);
}

const manifest_value_t *manifest_state_key(const manifest_module_t *m,
    const char *name)
{
    const key_path_t path = { &name, 1 };
    const key_rule_t *rule = manifest_find_key(name);
    manifest_found_t found = {};

    if (!rule)
        return (nullptr);
    return (manifest_find_value(m, &path, rule->check, &found));
}
