/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "host.h"
#include "os.h"

static bool take_config(host_t *h, const char *name)
{
    bool taken = config_read(&h->config, name);

    taken = taken && !config_check(&h->config);
    if (!taken)
        return (false);
    taken = config_world(h);
    h->copies = state_path(name, "copies");
    taken = config_backends(h) && h->copies && taken;
    return (config_content(h) && taken);
}

int host_go(host_t *h, char *named, bool show)
{
    const char *name = config_go_beside(named);
    bool taken = name && take_config(h, name);
    os_lock_t *lock = nullptr;
    int status = 1;

    if (!name)
        say("cannot go into '%s'\n", named);
    if (show)
        return (show_config(h, taken));
    if (taken && take_lock(name, &lock))
        status = run(h);
    os_unlock(lock);
    return (status);
}
