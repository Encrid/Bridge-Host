/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "host.h"

static const char *const BEFORE[] = { "order", "before" };
static const char *const UNLOAD[] = { "order", "unload_before" };

const keyed_t ORDER_KEYS[] = { { BEFORE, 2 }, { UNLOAD, 2 } };

static bool is_pair(const char *a, const char *b, const char *const *two)
{
    return ((!strcmp(a, two[0]) && !strcmp(b, two[1]))
        || (!strcmp(a, two[1]) && !strcmp(b, two[0])));
}

bool config_states_pair(const cfile_t *f, const void *asked, said_t *said)
{
    const pair_asked_t *a = asked;
    said_t list = {};
    const value_t *pair = nullptr;

    if (!config_states_key(f, a->key, &list))
        return (false);
    for (uint32_t i = 0; i < list.value->count; i++) {
        pair = &list.value->items[i];
        if (is_pair(pair->items[0].text, pair->items[1].text, a->two)) {
            *said = (said_t){ pair, list.file, list.line };
            return (true);
        }
    }
    return (false);
}

static bool add_order(host_t *h, enum order_kind kind, const value_t *pair)
{
    const char *two[] = { pair->items[0].text, pair->items[1].text };
    const pair_asked_t asked = { &ORDER_KEYS[kind], two };
    pairs_t *p = &h->orders[kind];
    found_t f = {};
    bridge_order_t *more = nullptr;

    for (uint32_t i = 0; i < p->count; i++)
        if (is_pair(p->pairs[i].before, p->pairs[i].after, two))
            return (true);
    f = config_find(&h->config, config_states_pair, &asked);
    more = realloc(p->pairs, sizeof(*more) * (p->count + 1));
    if (!more)
        return (false);
    p->pairs = more;
    if (!config_say_found(&h->config, &ORDER_KEYS[kind], &f, ""))
        return (false);
    p->pairs[p->count++] = (bridge_order_t){
        f.at.value->items[0].text, f.at.value->items[1].text };
    return (true);
}

static bool take_kind(host_t *h, enum order_kind kind)
{
    said_t said = {};
    bool taken = true;

    for (uint32_t f = 0; f < h->config.count; f++) {
        if (!config_states_key(&h->config.files[f], &ORDER_KEYS[kind], &said))
            continue;
        for (uint32_t i = 0; i < said.value->count; i++)
            taken = add_order(h, kind, &said.value->items[i]) && taken;
    }
    return (taken);
}

bool config_order(host_t *h)
{
    bool taken = take_kind(h, ORDER_SYSTEMS);

    taken = take_kind(h, ORDER_UNLOADS) && taken;
    return (taken && config_cycle(h));
}
