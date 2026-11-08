/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdlib.h>
#include <string.h>

#include "manifest_inside.h"

static char *join_key(const manifest_module_t *m, const key_path_t *path)
{
    size_t size = 1;
    char *key = nullptr;

    for (uint32_t i = 0; i < path->depth; i++)
        size += strlen(path->names[i]) + 1;
    key = manifest_keep(m, calloc(size, 1));
    for (uint32_t i = 0; key && i < path->depth; i++) {
        if (i)
            strcat(key, ".");
        strcat(key, path->names[i]);
    }
    return (key);
}

static bool read_own(const manifest_module_t *m, const key_path_t *path,
    manifest_value_t *v)
{
    const char *key = join_key(m, path);
    const char *raw = nullptr;
    uint32_t count = 0;

    if (key)
        count = manifest_get_value(m->bytes, m->len, key, &raw);
    if (count > 1)
        manifest_tell(m, MANIFEST_TOLD_TWICE, path, nullptr);
    return (count == 1 && m->host->parse(m->host->ud, raw, v));
}

const manifest_value_t *manifest_find_own(const manifest_module_t *m,
    const key_path_t *path, enum manifest_check check)
{
    manifest_value_t *own = manifest_keep_value(m, calloc(1, sizeof(*own)));
    manifest_told_t told = { MANIFEST_TOLD_WRONG, path->names, path->depth,
        nullptr, nullptr };

    if (!own || !read_own(m, path, own))
        return (nullptr);
    told.value = own;
    told.why = manifest_check_value(own, manifest_get_rule(check));
    if (told.why)
        m->host->tell(m->host->ud, &told);
    return (told.why ? nullptr : own);
}

void manifest_tell(const manifest_module_t *m, enum manifest_told_kind kind,
    const key_path_t *path, const manifest_value_t *v)
{
    const manifest_told_t told = { kind, path->names, path->depth, v,
        nullptr };

    m->host->tell(m->host->ud, &told);
}
