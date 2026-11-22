/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "host.h"

static void add_phase_names(const host_t *h, const char *table,
    manifest_names_t *into)
{
    const char *const names[] = { "module", table, "phases" };
    const keyed_t under = { names, 3 };
    manifest_names_t found = config_names(&h->config, &under);

    for (uint32_t i = 0; i < found.count; i++)
        manifest_add_name(into, found.names[i]);
    into->lost = into->lost || found.lost;
    free(found.names);
}

uint32_t state_phases(void *ud, const char ***names)
{
    const stating_t *s = ud;
    manifest_names_t got = {};

    add_phase_names(s->h, s->of->who.identity, &got);
    add_phase_names(s->h, s->of->who.backend, &got);
    add_phase_names(s->h, "all", &got);
    *names = got.lost ? nullptr : got.names;
    if (!got.lost)
        return (got.count);
    free(got.names);
    say("%sno memory is left for its phases\n", s->prefix);
    return (MANIFEST_NO_MEMORY);
}
