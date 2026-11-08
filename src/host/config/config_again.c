/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "host.h"

static uint32_t get_phase(const host_t *h, const char *name)
{
    uint32_t at = 0;

    while (at < h->phase_count && strcmp(h->phases[at], name))
        at++;
    return (at);
}

static bool take_cap(host_t *h, const char *name)
{
    const char *key[2] = { "again", name };
    const keyed_t k = { key, 2 };
    found_t f = config_find(&h->config, config_states_key, &k);
    uint32_t at = get_phase(h, name);

    if (!config_say_found(&h->config, &k, &f, ""))
        return (false);
    if (at == h->phase_count) {
        say("refused: %s:%u: again.%s: not a name of phases\n",
            h->config.files[f.at.file].path, f.at.line, name);
        return (false);
    }
    h->again[at] = (uint32_t)f.at.value->whole;
    return (true);
}

bool config_again(host_t *h)
{
    static const char *const AGAIN[] = { "again" };
    const keyed_t k = { AGAIN, 1 };
    names_t names = config_names(&h->config, &k);
    found_t none = config_find(&h->config, config_states_key, &k);
    bool taken = true;

    if (!names.count && !none.at.value)
        say("not stated: again\n");
    taken = names.count || none.at.value;
    for (uint32_t i = 0; i < names.count; i++)
        taken = take_cap(h, names.names[i]) && taken;
    free(names.names);
    return (taken);
}
