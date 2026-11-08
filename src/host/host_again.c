/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "host.h"

static void say_again(host_t *h, uint32_t p)
{
    if (BRIDGE_WARN <= h->d.written && h->again[p])
        say("phase %s asked again past its %u runs\n", h->phases[p],
            h->again[p]);
    else if (BRIDGE_WARN <= h->d.written)
        say("phase %s asked again, and again does not name it\n",
            h->phases[p]);
    h->counts.failed += BRIDGE_WARN <= h->d.failing;
}

int run_phase(host_t *h, uint32_t p)
{
    uint32_t cap = h->again[p] ? h->again[p] : 1;
    uint32_t runs = 0;
    int result = 3;

    while (result == 3 && runs < cap) {
        result = bridge_run_phase(h->w, h->phases[p]);
        if (result == 2)
            h->ended = true;
        runs++;
    }
    if (result == 3)
        say_again(h, p);
    return (result);
}
