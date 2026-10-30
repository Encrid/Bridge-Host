/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "host.h"

static void say_record(void *ud, const bridge_record_t *r)
{
    const char *identity = r->identity ? r->identity : "-";
    const char *system = r->system ? r->system : "-";
    counts_t *counts = ud;

    if (r->level <= BRIDGE_INFO)
        say("record[%d] %s/%s: %.*s %u %u\n", r->level, identity, system,
            (int)r->len, r->text, r->arg[0], r->arg[1]);
    counts->failed += r->level <= BRIDGE_ERR;
    counts->said++;
}

static void say_loaded(bridge_world_t *w)
{
    bridge_module_info_t loaded[FILES];
    int count = bridge_list_modules(w, loaded, FILES);

    say_part("still loaded:");
    for (int i = 0; i < count && i < FILES; i++)
        say_part("%s %s", i ? "," : "", loaded[i].identity);
    say("%s\n", count ? "" : " nothing");
}

static int run_round(host_t *h, int *did)
{
    int refused;
    int result = 0;

    *did = look(h, !h->looked);
    if (!h->looked && *did >= 0)
        h->counts.failed += (int)config_unfound(h);
    h->looked = true;
    h->counts.said = 0;
    refused = *did < 0;
    for (uint32_t p = 0; p < h->phase_count && !refused; p++) {
        result = bridge_run_phase(h->w, h->phases[p]);
        refused = result == 1;
        if (result == 2)
            h->ended = true;
    }
    h->round++;
    return (refused);
}

static bool make_world(host_t *h)
{
    bridge_config_t cfg = h->d.world;

    cfg.abi = BRIDGE_HOST_ABI;
    cfg.backends = h->backends;
    cfg.backend_count = h->backend_count;
    cfg.phases = h->phases;
    cfg.phase_count = h->phase_count;
    cfg.order = h->order;
    cfg.order_count = h->order_count;
    h->w = bridge_create(&cfg, say_record, &h->counts);
    if (!h->w)
        say("bridge_create refused\n");
    return (h->w != nullptr);
}

int run(host_t *h)
{
    int refused = 0;
    int left = (int)h->d.rounds;
    int did = 1;

    if (!make_world(h))
        return (1);
    while (!refused && !h->ended
        && (h->d.live == BRIDGE_TRUE || did || h->counts.said)
        && (!h->d.rounds || left--))
        refused = run_round(h, &did);
    if (h->ended)
        say("ended at round %d: a module asked for the end\n", h->round);
    else if (!refused && h->d.rounds && left < 0)
        say("ended after %u rounds, as many as it was given\n", h->d.rounds);
    say_loaded(h->w);
    say("destroyed: %d\n", bridge_destroy(h->w));
    return (refused || h->counts.failed ? 1 : 0);
}
