/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdlib.h>

#include "manifest_inside.h"

static bool take_phase(const manifest_module_t *m, const char *name,
    bridge_phase_name_t *into)
{
    const char *const names[] = { "phases", name };
    const key_path_t path = { names, 2 };
    manifest_found_t found = {};
    int got = m->host->find(m->host->ud, names, 2, &found);

    if (!got)
        manifest_tell(m, MANIFEST_TOLD_NOT_STATED, &path, nullptr);
    if (got <= 0)
        return (false);
    *into = (bridge_phase_name_t){ name, found.value->text };
    return (true);
}

bool manifest_take_phases(const manifest_module_t *m,
    bridge_module_config_t *cfg)
{
    const char **names = nullptr;
    uint32_t count = m->host->phases(m->host->ud, &names);
    bridge_phase_name_t *map = nullptr;
    bool taken = true;

    if (count)
        map = manifest_keep(m, malloc(sizeof(*map) * count));
    for (uint32_t i = 0; map && i < count; i++)
        taken = take_phase(m, names[i], &map[i]) && taken;
    free(names);
    cfg->phases = map;
    cfg->phase_count = map ? count : 0;
    return (taken);
}
