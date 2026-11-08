/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "host.h"

static int find_key(const stating_t *s, const char *const *names,
    uint32_t depth, manifest_found_t *found)
{
    const module_key_t k = { { s->of->who.identity, s->of->who.backend,
        "all" }, { names, depth } };
    const shown_t how = { "  ", config_states_module, &k };
    found_t f = config_find(&s->h->config, config_states_module, &k);

    if (!f.at.value && !f.other.value)
        return (0);
    if (!config_say_found(&s->h->config, &k.key, &f, s->prefix))
        return (-1);
    if (s->show)
        config_say_value(&s->h->config, &k.key, &f, &how);
    *found = (manifest_found_t){ f.at.value, nullptr };
    return (1);
}

static int find_setting(const stating_t *s, const char *name,
    manifest_found_t *found)
{
    const char *const two[] = { s->of->who.identity, name };
    const char *const named[] = { "config", name };
    const keyed_t shown = { named, 2 };
    const shown_t how = { "  ", state_states_setting, two };
    found_t f = config_find(&s->h->config, state_states_setting, two);

    if (!f.at.value && !f.other.value)
        return (0);
    if (!config_say_found(&s->h->config, &shown, &f, s->prefix))
        return (-1);
    if (s->show)
        config_say_value(&s->h->config, &shown, &f, &how);
    *found = (manifest_found_t){ f.at.value, nullptr };
    if (state_is_path(&s->h->config, &f.at))
        found->text = state_keep(s, config_path(&s->h->config, f.at.file,
            f.at.value->text));
    return (1);
}

int state_find(void *ud, const char *const *names, uint32_t depth,
    manifest_found_t *found)
{
    const stating_t *s = ud;
    bool grant = depth == 1 && (!strcmp(names[0], "read")
        || !strcmp(names[0], "write"));

    if (depth == 2 && !strcmp(names[0], "config"))
        return (find_setting(s, names[1], found));
    if (grant && state_find_grant(s, names[0], found))
        return (1);
    return (find_key(s, names, depth, found));
}
