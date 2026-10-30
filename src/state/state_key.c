/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "host.h"

const value_t *state_find_key(const stating_t *s, const key_rule_t *rule)
{
    const char *const names[] = { rule->name };
    const module_key_t k = { { s->of->who.identity, s->of->who.backend,
        "all" }, { names, 1 } };
    const shown_t how = { "  ", config_states_module, &k };
    found_t f = config_find(&s->h->config, config_states_module, &k);

    if (!f.at.value && !f.other.value)
        f.at = (said_t){ state_find_own(s, rule), UINT32_MAX, 0 };
    if (!config_say_found(&s->h->config, rule->name, &f, s->prefix))
        return (nullptr);
    if (s->show)
        config_say_value(&s->h->config, rule->name, &f, &how);
    return (f.at.value);
}

bool state_take_key(const stating_t *s, const key_rule_t *rule,
    bridge_module_config_t *cfg)
{
    const value_t *v = nullptr;

    if (rule->check == CHECK_AUTHOR)
        return (true);
    if (!rule->width)
        return (state_take_list(s, rule, cfg));
    v = state_find_key(s, rule);
    if (v)
        config_put(cfg, rule, v);
    return (v != nullptr);
}
