/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "host.h"

static manifest_module_t get_module(const stating_t *s, manifest_host_t *host)
{
    *host = (manifest_host_t){ (void *)s, state_parse, state_find,
        state_phases, state_undeclared, state_tell };
    return ((manifest_module_t){ host, &s->of->who,
        (const unsigned char *)s->of->bytes, s->of->len, &s->made->lib });
}

bool state_found(const stating_t *s, bridge_module_config_t *cfg)
{
    manifest_host_t host = {};
    const manifest_module_t m = get_module(s, &host);

    return (manifest_state(&m, cfg));
}

const value_t *state_find_key(const stating_t *s, const char *name)
{
    manifest_host_t host = {};
    const manifest_module_t m = get_module(s, &host);

    return (manifest_state_key(&m, name));
}
