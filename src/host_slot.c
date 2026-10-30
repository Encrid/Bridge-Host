/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "host.h"

file_t *find_file(host_t *h, const char *path)
{
    file_t *free_one = nullptr;

    for (uint32_t i = 0; i < h->d.files; i++) {
        if (h->files[i].path && !strcmp(h->files[i].path, path))
            return (&h->files[i]);
        if (!h->files[i].path && !free_one)
            free_one = &h->files[i];
    }
    return (free_one);
}

bool keep_path(file_t *f, const char *path)
{
    if (!f->path)
        f->path = malloc(strlen(path) + 1);
    if (f->path)
        strcpy(f->path, path);
    return (f->path != nullptr);
}
