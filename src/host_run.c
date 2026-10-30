/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "host.h"
#include "os.h"

static void say_record(void *ud, const bridge_record_t *r)
{
    const char *identity = r->identity ? r->identity : "-";
    const char *system = r->system ? r->system : "-";
    host_t *h = ud;

    if (r->level <= h->d.written)
        say("record[%d] %s/%s: %.*s %u %u\n", r->level, identity, system,
            (int)r->len, r->text, r->arg[0], r->arg[1]);
    h->counts.failed += r->level <= h->d.failing;
    h->counts.said++;
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
    h->w = bridge_create(&cfg, say_record, h);
    if (!h->w)
        say("bridge_create refused\n");
    return (h->w != nullptr);
}

static void say_ended(const host_t *h, int refused, int left)
{
    if (h->ended)
        say("ended at round %d: a module asked for the end\n", h->round);
    else if (os_signal_is_caught())
        say("ended at round %d: a signal stopped the run\n", h->round);
    else if (!refused && h->d.rounds && left < 0)
        say("ended after %u rounds, as many as it was given\n", h->d.rounds);
}

int run(host_t *h)
{
    int refused = 0;
    int left = (int)h->d.rounds;
    int did = 1;

    if (!make_world(h))
        return (1);
    while (!refused && !h->ended && !os_signal_is_caught()
        && (h->d.live == BRIDGE_TRUE || did || h->counts.said)
        && (!h->d.rounds || left--))
        refused = run_round(h, &did);
    say_ended(h, refused, left);
    say_end(h);
    say("destroyed: %d\n", bridge_destroy(h->w));
    return (refused || h->counts.failed ? 1 : 0);
}
