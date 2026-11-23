/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "host.h"
#include "os.h"

char *state_path(const char *name, const char *leaf)
{
    size_t len = strlen(name) + strlen(leaf) + sizeof(".d/");
    char *path = host_malloc(len);

    if (path)
        snprintf(path, len, "%s.d/%s", name, leaf);
    return (path);
}

bool take_lock(const char *name, os_lock_t **lock)
{
    char *path = state_path(name, "lock");
    size_t cut = strlen(name) + sizeof(".d") - 1;
    int got = -1;

    if (path)
        path[cut] = '\0';
    if (!path || os_folder_make(path)) {
        say("refused: %s.d is no folder\n", name);
        free(path);
        return (false);
    }
    path[cut] = '/';
    got = os_lock(path, lock);
    if (got > 0)
        say("refused: %s is being run\n", name);
    if (got < 0)
        say("cannot lock '%s'\n", path);
    free(path);
    return (!got);
}
