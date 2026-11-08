/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdlib.h>
#include <string.h>

#include "manifest_inside.h"

bridge_setting_t manifest_get_setting(const char *key,
    const manifest_value_t *v, const char *text)
{
    switch (v->kind) {
        case MANIFEST_WHOLE:
            return ((bridge_setting_t){ .key = key, .kind = BRIDGE_INT,
                .v.i = v->whole });
        case MANIFEST_REAL:
            return ((bridge_setting_t){ .key = key, .kind = BRIDGE_FLOAT,
                .v.f = v->real });
        case MANIFEST_FLAG:
            return ((bridge_setting_t){ .key = key, .kind = BRIDGE_BOOL,
                .v.b = v->flag });
        default:
            return ((bridge_setting_t){ .key = key, .kind = BRIDGE_STRING,
                .v.s = text });
    }
}

static bool take_setting(const manifest_module_t *m, const char *name,
    bridge_setting_t *set)
{
    const char *const names[] = { "config", name };
    const key_path_t path = { names, 2 };
    manifest_found_t found = {};
    const manifest_value_t *v = manifest_find_value(m, &path,
        MANIFEST_CHECK_SETTING, &found);

    if (!v)
        return (false);
    *set = manifest_get_setting(name, v, found.text ? found.text : v->text);
    return (true);
}

static name_list_t split_names(const manifest_module_t *m)
{
    const char *settings = m->who->settings;
    char *copy = manifest_keep(m, malloc(strlen(settings) + 1));
    char *name = nullptr;
    name_list_t got = {};

    if (copy)
        strcpy(copy, settings);
    name = copy ? strtok(copy, " ") : nullptr;
    while (name) {
        manifest_add_name(&got, name);
        name = strtok(nullptr, " ");
    }
    return (got);
}

bool manifest_take_settings(const manifest_module_t *m,
    bridge_module_config_t *cfg)
{
    name_list_t declared = split_names(m);
    bridge_setting_t *set = manifest_keep(m, calloc(declared.count + 1,
        sizeof(*set)));
    bool taken = !m->host->undeclared(m->host->ud, declared.names,
        declared.count) && set;

    if (declared.count && !strcmp(m->who->config, BRIDGE_NO_CONFIG)) {
        manifest_tell(m, MANIFEST_TOLD_NO_COMPONENT, &(key_path_t){}, nullptr);
        taken = false;
    }
    for (uint32_t i = 0; set && i < declared.count; i++)
        taken = take_setting(m, declared.names[i], &set[i]) && taken;
    cfg->settings = declared.count ? set : nullptr;
    cfg->setting_count = declared.count;
    free(declared.names);
    return (taken);
}
