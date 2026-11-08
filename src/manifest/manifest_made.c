/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdlib.h>

#include "manifest_inside.h"

static bool add_one(void ***list, uint32_t *count, void *one)
{
    void **more = realloc(*list, sizeof(*more) * (*count + 1));

    if (!more)
        return (false);
    *list = more;
    (*list)[(*count)++] = one;
    return (true);
}

void *manifest_keep(const manifest_module_t *m, void *block)
{
    manifest_made_t *made = m->made;

    if (block && !add_one(&made->blocks, &made->block_count, block)) {
        free(block);
        return (nullptr);
    }
    return (block);
}

manifest_value_t *manifest_keep_value(const manifest_module_t *m,
    manifest_value_t *v)
{
    manifest_made_t *made = m->made;

    if (v && !add_one((void ***)&made->values, &made->value_count, v)) {
        free(v);
        return (nullptr);
    }
    return (v);
}

void manifest_free_value(manifest_value_t *v)
{
    for (uint32_t i = 0; i < v->count; i++)
        manifest_free_value(&v->items[i]);
    free(v->items);
    free(v->text);
}

void manifest_free(manifest_made_t *made)
{
    for (uint32_t i = 0; i < made->block_count; i++)
        free(made->blocks[i]);
    for (uint32_t i = 0; i < made->value_count; i++) {
        manifest_free_value(made->values[i]);
        free(made->values[i]);
    }
    free(made->blocks);
    free(made->values);
}
