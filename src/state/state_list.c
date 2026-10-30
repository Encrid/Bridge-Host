/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "host.h"

static bool permit_in(const config_t *c, uint32_t file,
    const module_key_t *k, names_t *into)
{
    const cfile_t *f = &c->files[file];
    said_t said = {};
    bool stated = false;

    if (config_states_module(f, k, &said)) {
        for (uint32_t i = 0; i < said.value->count; i++)
            names_add(into, said.value->items[i].text);
        return (true);
    }
    for (uint32_t i = 0; i < f->named_count; i++)
        stated = permit_in(c, f->named[i], k, into) || stated;
    return (stated);
}

static bool find_names(const stating_t *s, const char *name, names_t *got)
{
    const char *const names[] = { name };
    const module_key_t k = { { s->of->who.identity, s->of->who.backend,
        "all" }, { names, 1 } };
    const config_t *c = &s->h->config;
    said_t said = {};

    if (c->local >= 0 && config_states_module(&c->files[c->local], &k,
        &said))
        return (permit_in(c, (uint32_t)c->local, &k, got));
    return (permit_in(c, 0, &k, got));
}

static void say_names(const stating_t *s, const char *key,
    const names_t *got)
{
    if (!s->show)
        return;
    say_part("  %s = [", key);
    for (uint32_t i = 0; i < got->count; i++)
        say_part("%s\"%s\"", i ? ", " : "", got->names[i]);
    say("]\n");
}

static void put_names(bridge_module_config_t *cfg, const char *key,
    const names_t *got, bool none)
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

bool state_take_list(const stating_t *s, const module_rule_t *rule,
    bridge_module_config_t *cfg)
{
    names_t got = {};
    bool stated = rule->check == CHECK_NAMES && find_names(s, rule->name,
        &got);
    const value_t *v = stated ? nullptr : state_find_key(s, rule);

    if (stated)
        say_names(s, rule->name, &got);
    if (!stated && !v)
        return (false);
    for (uint32_t i = 0; v && v->kind == VALUE_LIST && i < v->count; i++)
        names_add(&got, v->items[i].text);
    got.names = state_keep(s, got.names);
    got.count = got.names ? got.count : 0;
    put_names(cfg, rule->name, &got, v && v->kind != VALUE_LIST);
    return (true);
}
