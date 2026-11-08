/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "host.h"

static void add_phase_names(const host_t *h, const char *table,
    names_t *into)
{
    const char *const names[] = { "module", table, "phases" };
    const keyed_t under = { names, 3 };
    names_t found = config_names(&h->config, &under);

    for (uint32_t i = 0; i < found.count; i++)
        names_add(into, found.names[i]);
    free(found.names);
}

uint32_t state_phases(void *ud, const char ***names)
{
    const stating_t *s = ud;
    names_t got = {};

    add_phase_names(s->h, s->of->who.identity, &got);
    add_phase_names(s->h, s->of->who.backend, &got);
    add_phase_names(s->h, "all", &got);
    *names = got.names;
    return (got.count);
}
