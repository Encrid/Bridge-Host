/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "host.h"

static bool take_setting(host_t *h, const char *const *key,
    bridge_setting_t *set)
{
    said_t said = config_find_key(&h->config, key, 3);

    if (said.value)
        *set = config_get_setting(key[2], said.value, said.value->text);
    return (said.value != nullptr);
}

bool config_backend_settings(host_t *h, uint32_t at)
{
    const char *key[3] = { "backend", h->held[at].name, nullptr };
    const keyed_t under = { key, 2 };
    names_t names = config_names(&h->config, &under);
    bridge_setting_t *set = calloc(names.count + 1, sizeof(*set));
    uint32_t count = 0;
    bool taken = set != nullptr;

    for (uint32_t i = 0; set && i < names.count; i++) {
        key[2] = names.names[i];
        if (strcmp(key[2], "library"))
            taken = take_setting(h, key, &set[count++]) && taken;
    }
    h->held[at].settings = set;
    h->backends[at].settings = set;
    h->backends[at].setting_count = count;
    free(names.names);
    return (taken);
}
