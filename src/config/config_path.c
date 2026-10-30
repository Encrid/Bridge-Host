/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "os.h"

static bool is_whole(const char *path)
{
    return (path[0] == '/' || path[0] == '\\'
        || (path[0] && path[1] == ':'));
}

char *config_path(const config_t *c, uint32_t file, const char *path)
{
    const char *folder = is_whole(path) ? "" : c->files[file].folder;
    size_t len = strlen(folder) + strlen(path) + 1;
    char *joined = malloc(len);

    if (joined)
        snprintf(joined, len, "%s%s", folder, path);
    return (joined);
}

size_t config_folder_size(const char *path)
{
    const char *slash = strrchr(path, '/');
    const char *back = strrchr(path, '\\');

    if (back && (!slash || back > slash))
        slash = back;
    return (slash ? (size_t)(slash - path) + 1 : 0);
}

const char *config_go_beside(char *name)
{
    size_t len = config_folder_size(name);

    if (!len)
        return (name);
    name[len - 1] = 0;
    if (os_folder_set(len == 1 ? "/" : name))
        return (nullptr);
    return (name + len);
}

char *config_get_folder(const char *path)
{
    size_t len = config_folder_size(path);
    char *folder = malloc(len + 1);

    if (folder)
        memcpy(folder, path, len);
    if (folder)
        folder[len] = 0;
    return (folder);
}
