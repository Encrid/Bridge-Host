/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "host.h"

const char *config_check_unloads(const value_t *v)
{
    const char *why = config_check_value(v, CHECK_MODULE_PAIRS);
    const char *name = nullptr;

    for (uint32_t i = 0; !why && i < 2 * v->count; i++) {
        name = v->items[i / 2].items[i % 2].text;
        if (!strchr(name, ':') || strchr(name, '@'))
            why = "a module of a pair is no <namespace>:<name>";
    }
    for (uint32_t i = 0; !why && i < v->count; i++)
        if (!strcmp(v->items[i].items[0].text, v->items[i].items[1].text))
            why = "a pair names one module twice";
    return (why);
}

static bool is_end(const pairs_t *p, const bool *kept, uint32_t at)
{
    bool into = false;
    bool out = false;

    for (uint32_t i = 0; i < p->count; i++) {
        into = into || (kept[i]
            && !strcmp(p->pairs[i].after, p->pairs[at].before));
        out = out || (kept[i]
            && !strcmp(p->pairs[i].before, p->pairs[at].after));
    }
    return (!into || !out);
}

static uint32_t drop_once(const pairs_t *p, bool *kept)
{
    uint32_t dropped = 0;

    for (uint32_t i = 0; i < p->count; i++)
        if (kept[i] && is_end(p, kept, i)) {
            kept[i] = false;
            dropped++;
        }
    return (dropped);
}

static void say_cycle(const host_t *h, const bridge_order_t *pair)
{
    const char *two[] = { pair->before, pair->after };
    const pair_asked_t asked = { &ORDER_KEYS[ORDER_UNLOADS], two };
    found_t f = config_find(&h->config, config_states_pair, &asked);

    say("refused: %s:%u: order.unload_before: %s and %s make a cycle\n",
        h->config.files[f.at.file].path, f.at.line, pair->before,
        pair->after);
}

bool config_cycle(host_t *h)
{
    const pairs_t *p = &h->orders[ORDER_UNLOADS];
    bool *kept = malloc(sizeof(*kept) * (p->count + 1));
    uint32_t at = 0;

    if (!kept)
        return (false);
    for (uint32_t i = 0; i < p->count; i++)
        kept[i] = true;
    while (drop_once(p, kept) > 0)
        continue;
    while (at < p->count && !kept[at])
        at++;
    if (at < p->count)
        say_cycle(h, &p->pairs[at]);
    free(kept);
    return (at == p->count);
}
