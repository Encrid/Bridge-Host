/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "host.h"

static bool take_check(const host_t *h, const char *name, bool *flag)
{
    const char *const names[] = { "checks", name };
    const keyed_t k = { names, 2 };
    found_t f = config_find(&h->config, config_states_key, &k);
    char key[64];

    if (!f.at.value)
        return (true);
    config_say_key(names, 2, key, sizeof(key));
    if (!config_say_found(&h->config, key, &f, ""))
        return (false);
    *flag = f.at.value->flag;
    return (true);
}

bool take_checks(host_t *h)
{
    const char *const name[] = { "profile", "validate" };
    bool *const flag[] = { &h->profile, &h->validate };
    bool taken = true;

    for (uint32_t i = 0; i < 2; i++)
        taken = take_check(h, name[i], flag[i]) && taken;
    return (taken);
}

const char *config_check_checks(const entry_t *e, bool local)
{
    if (e->depth != 2 || (strcmp(e->key[1], "profile")
        && strcmp(e->key[1], "validate")))
        return ("no key of Bridge-Host's or Bridge's");
    if (local)
        return ("fixed when the world is made: not in the local file");
    return (config_check_value(&e->value, CHECK_FLAG));
}
