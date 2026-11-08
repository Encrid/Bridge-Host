/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "host.h"

bool state_is_setting(const entry_t *e, const char *const *two)
{
    if (!e->value.kind || e->depth < 4 || e->depth > 5
        || strcmp(e->key[0], "module") || strcmp(e->key[1], two[0])
        || strcmp(e->key[2], "config"))
        return (false);
    if (two[1] && strcmp(e->key[3], two[1]))
        return (false);
    return (e->depth == 4 || !strcmp(e->key[4], "path"));
}

bool state_states_setting(const cfile_t *f, const void *asked, said_t *said)
{
    for (uint32_t i = 0; i < f->toml.count; i++)
        if (state_is_setting(&f->toml.entries[i], asked)) {
            *said = (said_t){ &f->toml.entries[i].value, 0,
                f->toml.entries[i].line };
            return (true);
        }
    return (false);
}

bool state_is_path(const config_t *c, const said_t *said)
{
    const cfile_t *f = &c->files[said->file];

    for (uint32_t i = 0; i < f->toml.count; i++)
        if (&f->toml.entries[i].value == said->value)
            return (f->toml.entries[i].depth == 5);
    return (false);
}
